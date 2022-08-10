//
// Created by IHEB on 06/07/2022.
//

#ifndef HZDR_DATA_ACQUISITION_LOGGER_H
#define HZDR_DATA_ACQUISITION_LOGGER_H
#include "../../Base/BaseProcess.h"
#include "../../DataStructures/Config.h"
#include "../../DataStructures/notification_queue.h"
#include "../../Engine/core.h"
#include "../../Engine/states.h"
#include "../../Tools/Timer.h"
#include <filesystem> //starting c++17
#include <iostream>
#include <fstream>
#include <ios>

namespace fs = std::filesystem;

static float cantor_pairing(int k1, int k2){
    return 0.5*(k1+k2)*(k1+k2+1)+k2;
}


/*** create a folder with name experiment_timestamp
 * log config and serial numbers and so on in one file
 * log raw and results in this structure: experiment_t->setup.txt
*                                                     ->channel_x->Raw.dat
 *                                                                 Results->results_wih_ref_n.dat]
 * each channels subscribes to the logger with a unique ID and is_ref bool.
 * upon subscription a new folder with the channel's id as name is created if doesn't exist
 * within it a raw file will be created if doesn't exist and if_ref a results folder will be created with
 * a specific file for each reference channels
 * after subscription the user can just call log raw or log res with it's specific id and provide the data to be
 * logged as an ENGINE::DataFrame, the reference ID ***/
namespace ENGINE{
    class LOGGERError : public std::exception {
        std::string what_;
    public:
        explicit LOGGERError(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

//a service used to create files and write in them
//template <typename inType>
    class LOGGER {//: public Process{
    public:
        const unsigned RAWTHREASHOLD = 1000;
        const unsigned RESTHREASHOLD = 10;
        LOGGER( Config& config_, ENGINE::LOGGING_MODE logging_mode = ENGINE::ASCII):
                cfg(config_), logging_mode_(logging_mode), base_path_(config_.app.logging_base_path) {
            //create an experiment folder with timestamp
            base_path_ /= "exp_" + get_timestamp(); //append new experiment directory to base path
            if(!std::filesystem::is_directory(base_path_))
                fs::create_directories(base_path_);
            this->write_meta_data();
        }
        ~LOGGER(){

            //write the rest of the buffer to the files, close them all and free their memory
            for(auto& f:raw_files){
                auto& buff = raw_buffer.at(f.first).second; //retrieve buffer
                std::cout << buff.str();
                this->write_buffer(f.second, buff);
                f.second.second->close();
                delete f.second.second;
            }
            for(auto& f:result_files){
                auto& buff = result_buffer.at(f.first).second; //retrieve buffer
                std::cout << buff.str();
                this->write_buffer(f.second, buff);
                f.second.second->close();
                delete f.second.second;
            }

        }

        void log_res(int ID, int ref_ID, const ENGINE::DataFrame<float, NM::COMPLEX>& dataFrame){
            /***this assumes the user already subscribed before logging***/
            //write to the respective results file
            //uses cantor_pairing to recreate the file key from both the channel ID and ref_ID
            auto key = cantor_pairing(ID, ref_ID);
            switch(logging_mode_) {
                case(BINARY):{
                    this->write_binary(result_files.at(key), dataFrame);
                    break;
                }
                case(ASCII):{
                    if(result_buffer.at(key).first >= RESTHREASHOLD){
//                        printf("LOGGING RES: %d\n", key);
                        this->write_buffer(result_files.at(key), result_buffer.at(key).second);
                        result_buffer[key] = std::pair<unsigned, std::ostringstream>(0, std::ostringstream()); //overwrite element to reset the,
                        raw_buffer[ID].second << std::setprecision(6) << std::fixed ;
                    }
                    else{
                        auto& buff_pair = result_buffer.at(key);
                        buff_pair.second << dataFrame.timestamp << ";\t" << dataFrame.data[0] << ";\t"
                        << dataFrame.data[1] << "\n";
                        buff_pair.first+=1; //increment line count
                    }

                    break;
                }
            }
        }

        void log_raw(int ID, const ENGINE::DataFrame<float, NM::COMPLEX>& dataFrame){
            /***this assumes the user already subscribed before logging***/
            switch(logging_mode_) {
                case(BINARY):{
                    this->write_binary(raw_files.at(ID), dataFrame);
                    break;
                }
                case(ASCII):{
                    if(raw_buffer.at(ID).first >= RAWTHREASHOLD){
                        this->write_buffer(raw_files.at(ID), raw_buffer.at(ID).second);
                        raw_buffer[ID]=  std::pair<unsigned, std::ostringstream>(0, std::ostringstream());
                        raw_buffer[ID].second << std::setprecision(6) << std::fixed ;
                                ; //overwrite element to reset the,
                    }
                    else{
                        auto& buff_pair = raw_buffer.at(ID);
                        buff_pair.second << dataFrame.timestamp << ";\t" << dataFrame.data[0] << ";\t"
                        << dataFrame.data[1] << "\n";
                        buff_pair.first+=1; //increment line count
                    }
                    break;
                }
            }
        }

        void subscribe(int ID, bool is_ref, std::vector<int> ref_ids= {}){
            //TODO: test subscribe and check if Users map is necessary
            //create channel folder
            auto ch_path = base_path_;
//            printf("%ls\n", base_path_.c_str());
            ch_path /= "ch_" + std::to_string(ID);
            if(!std::filesystem::is_directory(ch_path))
                fs::create_directories(ch_path);
            //create raw data file if does't exist
            auto raw_path = ch_path;
            raw_path /= "raw_data.dat";
            if (!fs::exists(raw_path)){
                //create file handler for raw_data logging
                std::lock_guard<std::mutex> lock(mutex_);
                raw_files.emplace(ID, std::pair<fs::path, std::ofstream*>{raw_path, new std::ofstream(raw_path,
                                                        std::ios_base::out|std::ios::binary)}); //pointer deleted in deconstructor
                raw_buffer.emplace(ID, std::pair<unsigned, std::ostringstream>(0, std::ostringstream()));
                if (!raw_files.at(ID).second->is_open())
                    raw_files.at(ID).second->open(raw_path, std::ios_base::out|std::ios::binary);
                *raw_files.at(ID).second << std::setprecision(6) << std::fixed ;
                raw_buffer.at(ID).second << std::setprecision(6) << std::fixed ;
                //write header
                this->write_header(raw_files.at(ID));
            }

            if (is_ref){
                return;
            }

            //create results folder
            auto res_path = ch_path;
            res_path /= "Results";
            if(!std::filesystem::is_directory(res_path))
                fs::create_directories(res_path);
            //create results for each ref_id if it doesn't exist
            for (auto& ref_id: ref_ids){
                auto new_path = res_path;
                new_path /= "res_"+std::to_string(ref_id)+".dat";
                int res_file_id = cantor_pairing(ID, ref_id);//use this as the new key to the map
                if (!fs::exists(new_path)){
                    std::lock_guard<std::mutex> lock(mutex_);
                    //create file handler for results-against-reference logging
                    result_files.emplace(res_file_id,
                                         std::pair<fs::path, std::ofstream*>{new_path,
                                         new std::ofstream(new_path, std::ios_base::out|std::ios::binary)}); //pointer deleted in destructor
                    result_buffer.emplace(res_file_id, std::pair<unsigned, std::ostringstream>(0, std::ostringstream()));

                    *result_files.at(res_file_id).second << std::setprecision(6) << std::fixed ;
                    result_buffer.at(res_file_id).second << std::setprecision(6) << std::fixed ;
                    //write header
                    this->write_header(result_files.at(res_file_id));
                }
            }
        }


    private:
        void write_binary(const std::pair<fs::path, std::ofstream*>& file_pair,
                        const ENGINE::DataFrame<float, NM::COMPLEX>& data){
            //copy data to avoid corruption
            float time = data.timestamp;
            double real = data.data[0];
            double im = data.data[1];

            auto* file = file_pair.second;
            if(!file->is_open())
                file->open(file_pair.first, std::ios_base::app|std::ios::binary);

            file->write(reinterpret_cast<char*>(&time), sizeof(double));
            file->write(";\t", sizeof(char));
            file->write(reinterpret_cast<char*>(&real), sizeof(double));
            file->write(";\t", sizeof(char));
            file->write(reinterpret_cast<char*>(&im), sizeof(double));
            file->write("\n", sizeof(char));
            file->flush();
        }

        void write_buffer(const std::pair<fs::path, std::ofstream*>& file_pair,
                          const std::ostringstream& buff){
            //only ascii mode needs to use a buffer
            auto* file = file_pair.second;
            if(!file->is_open())
                file->open(file_pair.first, std::ios_base::app|std::ios::binary);
            file->write(buff.str().c_str(), sizeof(char)*buff.str().size());
            file->flush();
        }


        void write_header(const std::pair<fs::path, std::ofstream*>& file_pair){
            auto* file = file_pair.second;
            if(!file->is_open())
                file->open(file_pair.first, std::ios_base::out|std::ios::binary);
            *file << "TIME;\tREAL;\tIMAGINARY\n";
            file->flush();
        }


        void write_meta_data(){
            /***uses base path to create a file where config and information about the experiment is logged***/
//TODO think of specs to write like serial numbers, sampling_frequency and so on..
// this writes to setup file in the respective experiment
            //only in ascii mode
            auto new_path = base_path_;
            new_path /= "SETTINGS.txt";
            std::ofstream settings_file(new_path, std::ios_base::out);
            settings_file << "#LOGGER STARTING TIME: " << get_timestamp() << "\n";
            settings_file << "#SAMPLING TIME: " << cfg.sampling.timebase << " ns\n";
            settings_file << "#DEVICES IN ORDER: ";
            for (auto& serial_number : cfg.devices_order)
                settings_file << serial_number << "; ";
            settings_file << "\n";
            settings_file.close();

        }
        Config& cfg;
        ENGINE::LOGGING_MODE logging_mode_;
        notification_queue worker_queue;
        fs::path base_path_; //path to the directory of the current experiment
        std::unordered_map<int, std::pair<fs::path, std::ofstream*>> raw_files;
        std::unordered_map<int, std::pair<unsigned, std::ostringstream>> raw_buffer;
        //the key to the results file is hashed from a pair of IDs: that of the demodulated channel and that of the
        //reference channel used in  the demodulation
        std::unordered_map<int, std::pair<fs::path, std::ofstream*>> result_files;
        std::unordered_map<int, std::pair<unsigned, std::ostringstream>> result_buffer; //first element of pairs used as a counter
        mutable std::mutex mutex_;
    };
}




void test_logger(){
    Config cg("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\tasler_config.json");
    ENGINE::LOGGER log(cg, ENGINE::ASCII);

    std::vector<int> ref_ids;

    for (auto& ch: cg.channels.analog_channels)
        if (ch.is_ref)
            ref_ids.push_back(ch.id);
    for (auto& ch: cg.channels.analog_channels)
        log.subscribe(ch.id, ch.is_ref, ref_ids);

//    log.run();
    Timer testing_timer_;
    testing_timer_.setInterval([&]{
        for (auto& ch: cg.channels.analog_channels){
            log.log_raw(ch.id, ENGINE::DataFrame<float, NM::COMPLEX>{0., NM::COMPLEX(69., 420.)});
            if (!ch.is_ref)
                log.log_res(ch.id, 15, ENGINE::DataFrame<float, NM::COMPLEX>{0., NM::COMPLEX(69., 420.)});
        }


    }, 1e3);
    std::cin.get();
//    log.stop();

//    log.add_target("OUT", );
}
#endif //HZDR_DATA_ACQUISITION_LOGGER_H

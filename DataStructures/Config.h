//
// Created by IHEB on 21/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_CONFIG_H
#define HZDR_DATA_ACQUISITION_CONFIG_H

#include <fstream>
#include <filesystem>
#include "../Libraries/JSON/json.h"
#include "../IOs/LTT24/ltt24_consts.h"

class CONFIG_ERROR : public std::exception {
    std::string what_;
public:
    explicit CONFIG_ERROR(std::string msg) { what_ = std::move(msg); }
    const char* what() const noexcept override  { return what_.c_str(); }
};

using json = nlohmann::json;
namespace fs = std::filesystem;
struct ANALOG_CHANNEL{
public:
//    ANALOG_CHANNEL(){}
    ANALOG_CHANNEL(const nlohmann::json& j){
        from_json(j);
    }

    int32_t id;
    uint32_t coup;
    int32_t analog_range;
    int32_t power_supply;
    uint32_t offset;
    std::string coil_type; ///TODO
    uint8_t is_ref; ///TODO

private:
    void from_json(const nlohmann::json& j) {
        j.at("id").get_to(this->id);
        this->coup = ltt_enums::ChannelCoupling.at(j.at("coup"));
        j.at("analog_range").get_to(this->analog_range);
        j.at("power_supply").get_to(this->power_supply);
        j.at("offset").get_to(this->offset);
        j.at("Coil_type").get_to(this->coil_type);
        j.at("is_ref").get_to(this->is_ref);
    }

//    void print(void){
//        std::cout << "\n\t ************ ANALOG CHANNEL_parameters ***************\n ";
//        std::cout << "id : " << id << "\n";
//        std::cout << "coup : " << coup << "\n";
//        std::cout << "analog_range : " << analog_range << " \n";
//        std::cout << "power_supply: " << power_supply << " \n";
//        std::cout << "offset: " << offset << " \n";
//    }
};


struct DIGITAL_CHANNEL{
public:
//    DIGITAL_CHANNEL(){}
    DIGITAL_CHANNEL(const nlohmann::json& j){
        from_json(j);
    }

    int32_t id;

private:
    void from_json(const nlohmann::json& j){
        j.at("id").get_to(this->id);
    }

//    void print(void){
//        std::cout << "\n\t ************ DIGITAL CHANNEL_parameters ***************\n ";
//        std::cout << "id : " << id << "\n";
//    }
};



struct Config{
    Config(){}

    Config(const fs::path& file_path){
        load(file_path);
    }
    struct AppConfig{
        unsigned buffer_size;
        unsigned block_size; //in terms of signal periods?
        unsigned sliding_window_size;
        unsigned server_port;
        std::string logging_base_path;
//        std::string logging_results_path;
        unsigned thread_count;
    } app;

    struct SamplingConfig{
        unsigned timebase;
        unsigned chunk_sequence_cnt;
    } sampling;

    struct TriggerConfig{
        unsigned state;
        unsigned slope;
        unsigned channel;
        unsigned chtype;
        unsigned trigtype;
        float f1;
        float f2;
    } trigger;

    struct PretriggerConfig{
        unsigned pre_triglen=0;					//amount of pre-tigger samples [ks]
        unsigned pre_sampletime=0; 				//sample time in [ns] before trigger has happened
    } pretrigger;
    std::vector<uint32_t> devices_order; //max size == 4
    struct ChannelsConfig{
//        std::vector<unsigned> reference_channels;
        std::vector<ANALOG_CHANNEL> analog_channels;
        std::vector<DIGITAL_CHANNEL> digital_channels;
    } channels;

    void load(const fs::path& config_file_path)try{
        this->import_as_string(config_file_path);
        json json_config = json::parse(this->string_format);
        //load app settings
        json_config.at("app").at("buffer_size").get_to(app.buffer_size);
        json_config.at("app").at("block_size").get_to(app.block_size);
        json_config.at("app").at("sliding_window_size").get_to(app.sliding_window_size);
        json_config.at("app").at("server_port").get_to(app.server_port);
        json_config.at("app").at("thread_count").get_to(app.thread_count);
        app.logging_base_path = std::string(json_config.at("app").at("logging_base_path"));
//        app.logging_results_path = std::string(json_config.at("app").at("logging_results_path"));
        //load time config
        sampling.timebase = json_config.at("sampling").at("timebase"); //TODO :investigate diff between THIS
//        sampling.sequence_cnt = json_config.at("sampling").at("sequence_cnt");
        sampling.chunk_sequence_cnt = json_config.at("sampling").at("chunk_sequence_cnt");
//        sampling.transfer_mode = ltt_enums::TransferMode.at(json_config.at("sampling").at("transfer_mode"));
        //load trigger config
        json_config.at("trigger").at("state").get_to(trigger.state); //TODO: and THIS
        json_config.at("trigger").at("channel").get_to(trigger.channel);
        json_config.at("trigger").at("f1").get_to(trigger.f1);
        json_config.at("trigger").at("f2").get_to(trigger.f2);
        trigger.slope = ltt_enums::TriggerSlope.at(json_config.at("trigger").at("slope"));
        trigger.chtype = ltt_enums::ChannelType.at(json_config.at("trigger").at("chtype"));
        trigger.trigtype = ltt_enums::TriggerMode.at(json_config.at("trigger").at("trigtype"));

        //load pretrigger config
        json_config.at("pretrigger").at("pre_triglen").get_to(pretrigger.pre_triglen);
        json_config.at("pretrigger").at("pre_sampletime").get_to(pretrigger.pre_sampletime);

        //load channels order
        devices_order = std::vector<uint32_t>(json_config.at("devices_order"));
        //load channel config
        auto& analog_ch = json_config.at("Analog_channels");
        channels.analog_channels.reserve(analog_ch.size());
        for (auto& ch: analog_ch){
            channels.analog_channels.emplace_back(ANALOG_CHANNEL(ch));
        }
        //redundancy to make sure the channels are sorted in the config
        std::sort( channels.analog_channels.begin( ), channels.analog_channels.end( ), [ ]( const auto& lhs, const auto& rhs )
        {
            return lhs.id < rhs.id;
        });

        auto& digital_ch = json_config.at("Digital_channels");
        channels.digital_channels.reserve(digital_ch.size());
        for (auto& ch: digital_ch){
            channels.digital_channels.emplace_back(DIGITAL_CHANNEL(ch));
        }
    }catch (const std::exception& ex){
        throw CONFIG_ERROR("ERROR IN CONFIG: couldn't load config from provided path: " + config_file_path.string() + ", "
                           + ex.what() + "\n");
    }

    std::string str(){
        return string_format;
    }
private:
    void import_as_string(const fs::path& config_file_path){
        std::ifstream t(config_file_path);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        string_format = str;
    }

    std::string string_format;
};

void test_config_loading(){
    auto cg = Config("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\tasler_config.json");
    printf("%d\n", cg.sampling.timebase);
    auto& ac = cg.channels.analog_channels;
    for (auto& ch: ac ){
        printf("ID: %d; is_ref: %d\n", ch.id, ch.is_ref);
    }
}

#endif //HZDR_DATA_ACQUISITION_CONFIG_H

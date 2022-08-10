//
// Created by iheb on 4/28/2020.
//

#ifndef HZDR_DEMO_SERVER_MESSAGES_H
#define HZDR_DEMO_SERVER_MESSAGES_H


#include "../../Libraries/JSON/json.h"
using json = nlohmann::json;
///defining API
///rcv : COMMANDS::HI + data_nature -> send : sampling_frequency + IDS
///rcv : COMMANDS::GET + data_nature + number_of_samples -> send : min(requested number_of_samples, samples_in_memory) in json format
///rcv : COMMANDS::end. -> send : MESSAGES::BYE; terminate connection

namespace MESSAGES{
    const std::string EMPTY = "buffer is empty!";
    const std::string INIT = "Howdy partner";
    const std::string BYE = "See ya partner";
    const std::string OK = "OK.";
    const std::string BAD_GET_REQUEST = "The request must be in the following form: command data_nature number_of_samples.";
    const std::string BAD_INIT_REQUEST = "The request must be in the following form: command data_nature.";

}

namespace COMMANDS{
    const std::string INIT = "*IDN?";
    const std::string GET = ":DATA?";
    const std::string BYE = "end.";

}

enum REQUEST_CODES{
    BAD_REQUEST=400,
    OK=200,


};
template <typename data_type=double>
struct MESSAGE {
    ///server message skeleton
///{
///    "Status": {"code": int,
///               "message:" string},
///    "Body": {"data": { "dataSize" : n,
///							"raw_timesteps":[..],
///                        {"channel_id": {
///                                     "real": [..],
///                                     "imag": [..]
///                                       },..}
///                     },
///             "metaData":{"comment": int,
///                         "dataNature" : string}
///             }
///}
    struct data_unit{
        std::vector<data_type> real;
        std::vector<data_type> imag;
//        std::vector<data_type> time;

        json to_json(){
            json j;
            j["real"] = real;
            j["imag"] = imag;
//            j["time"] = time;
            return j;
        }
    };
    int status_code=400;
    std::string status_message="BAD REQUEST, Request must be in this "
                               "Form: request_command + dataNature + number_of_samples.";
    struct{
		int data_size;
		std::vector<data_type> timesteps;
		std::unordered_map<std::string, data_unit> channels;

        json to_json(){
            json j;
            j["dataSize"] = data_size;
            j["timesteps"] = timesteps;
            json dummy;
			for (auto& ch:channels){
                dummy[ch.first] = ch.second.to_json();
			}
            j["channels"] = dummy;
			
            //j["channels"] = nlohmann::j_umap(channels).dump();
            return j;
        }
		
	}data; 

    struct{
        std::string comment ="";
        std::string data_nature;

        json to_json(){
            json j;
            j["comment"] = comment;
            j["dataNature"] = data_nature;
            //j["channels"] = channels;
            return j;
        }
		
    }metadata;

    std::string to_json(){
//        assert(data[0].real.size() == data[0].time.size());
        json msg;
        msg["Status"]["code"]= status_code;
        msg["Status"]["message"]= status_message;
        msg["Body"]["metaData"] = metadata.to_json();
        msg["Body"]["data"] = data.to_json();
       // for(auto& p:data)
         //   msg["Body"]["data"][p.first] = p.second.to_json();

////         = dummy;

      //  for(auto& e: msg["Body"]["data"])
        //    screen_logger.push_back(e.dump()+"\n");

//        if(print){
//            screen_logger.push_back(msg.dump()+"\n");
//        }
        return msg.dump();

    }

};



std::unordered_map<std::string, double> data_nature{{"raw", 0.}, {"demodulated", 0.}}; //data_nature, respective sampling rate

// a default value of 0 hz is set. the real value can only be determined in run time when the reference frequency is calculated
//this way the client can implicitly know that there's no data to send back.
void server_sampling_rate(const std::string& id, double Fs=0.){
    data_nature[id] = Fs;
}



#endif //HZDR_DEMO_SERVER_MESSAGES_H

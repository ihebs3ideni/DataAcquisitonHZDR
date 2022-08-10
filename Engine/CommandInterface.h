//
// Created by IHEB on 06/07/2022.
//

#ifndef HZDR_DATA_ACQUISITION_COMMANDINTERFACE_H
#define HZDR_DATA_ACQUISITION_COMMANDINTERFACE_H
#include <string>
#include <functional>
#include <iostream>
#include <memory>
#include "../IOs/TCP_IP/Server.h"

//TODO ADD DEVICES SERIAL NUMBER TO DATA LOGGING AND SERVER RESPONCES
namespace ENGINE{
    class CmdInterfaceError : public std::exception {
        std::string what_;
    public:
        explicit CmdInterfaceError(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

    struct CommandInterfaceArgs{
        const std::vector<std::string>& inputs;
        int src_fd = -696969;
        std::shared_ptr<Server> server = nullptr;
    };

    class CommandInterface{
    public:
        void add_command(const std::string& cmd, std::function<void(const CommandInterfaceArgs&)> callback){
            commands.emplace(cmd, std::move(callback));
        }

        void run_command(const std::string& cmd, const CommandInterfaceArgs& args){
            //this assumes the user made sure the command is already in the map
            commands.at(cmd)(args);
        }

        std::string echo_commands(){
            std::ostringstream streamObj;
            streamObj << "the following commands are in the map: ";
            for (auto& p: commands)
                streamObj << p.first << "; ";
            streamObj << "\n";
            return streamObj.str();
        }

        bool command_is_valid(const std::string& cmd){
            return commands.count(cmd); //0 false else true
        }

        std::vector<std::string> get_cmds(){
            std::vector<std::string> cmds(commands.size());
            for(auto& pcmd: commands)
                cmds.emplace_back(pcmd.first);
            return cmds;
        }

    private:
        std::unordered_map<std::string, std::function<void(const CommandInterfaceArgs&)>> commands;
    };
}

#endif //HZDR_DATA_ACQUISITION_COMMANDINTERFACE_H

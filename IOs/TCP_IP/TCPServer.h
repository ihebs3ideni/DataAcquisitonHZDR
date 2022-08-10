//
// Created by IHEB on 01/08/2022.
//

#ifndef HZDR_DATA_ACQUISITION_TCPSERVER_H
#define HZDR_DATA_ACQUISITION_TCPSERVER_H
#include "Server.h"
#include "../../Tools/Parser.h"
#include "../../Base/BaseProcess.h"
#include "../../Engine/CommandInterface.h"
#include "../../Engine/DataEngine.h"


class Broadcaster: public Process{
public:
    Broadcaster(unsigned id_, ENGINE::CommandInterface& ci_): Process(id_), CI(ci_){
    }

    void initialize(unsigned port, std::vector<std::function<void()>> cleanup_tasks={}){
        std::cout << "SERVER LISTENING ON PORT: " << port << "\n";
        this->server = std::make_shared<Server>(port);
        this->server->onConnect([](int fd){std::cout << "TCP SERVER: got connect from " << fd << std::endl;});
        this->server->onDisconnect([cleanup_tasks](int fd){
            std::cout << "TCP SERVER: got disconnect from " << fd << std::endl;
            for (auto& task:cleanup_tasks)
                task();
            });
        this->server->onInput([this](int fd, char *buffer){
            this->prs.parse(buffer);
            try{
                this->CI.echo_commands();
                if (this->CI.command_is_valid(this->prs.values.at(0)))
                    this->CI.run_command(this->prs.values.at(0), {prs.values, fd, std::shared_ptr<Server>(this->server)});
                else{
                    std::string s{"Invalid Command\n"};
                    printf("THE PROVIDED COMMAND ISN'T VALID: %s\n", this->prs.values.at(0).c_str());
                    this->server->sendMessage(fd, s.c_str(), s.size(),0);
                }
            }
            catch(const std::out_of_range& oor){
                //handle empty cmd
                printf("");
            }
            this->prs.values.clear();
        });
    }

    void run() override {
        this->shut_down_ = false;
        this->running = false;
        this->task_thread.push_back(std::make_shared<std::thread>([this]() {
            this->running = true;
            //we assume this->initialize is already called
            this->server->init();
            while(true){
                if(this->shut_down()){ // if shut_down is true raw data is an empty optional
                    this->running = false;
                    this->server->shutdown_();
                    return;
                }
                this->server->loop();
            }
        }));
        for (auto& t: this->task_thread)
            t->detach();
    }

    void stop() override{
        this->shut_down_ = true;
        this->server->snapOut();

    }

private:
    ENGINE::CommandInterface& CI;
    Parser prs;
    std::shared_ptr<Server> server;
};

void test_TCPServer(){
    Config cg("C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\tasler_config.json");
    std::cin.get();
    ENGINE::CommandInterface ci;
    Broadcaster bc(12345, ci);

    ci.add_command("HOWDY", [&](const ENGINE::CommandInterfaceArgs& args){
        if (args.server){
            std::string s = "Howdey Parter\n";
            args.server->sendMessage<const char*>(args.src_fd ,s.c_str(), s.size(), 0);
        }
    });

    ci.add_command("echo", [&](const ENGINE::CommandInterfaceArgs& args){
        if(args.server){
            std::string data = "";
            for(auto it = args.inputs.begin()+1; it != args.inputs.end(); ++it){
                std::cout << "EchoAction: attributes are : " << *it << std::endl;
                data.append(*it); data.append(" ");
            }
            args.server->sendMessage<const char*>(args.src_fd,data.c_str(), data.size(), 0);
        }
    });

    ci.add_command("kill", [&](const ENGINE::CommandInterfaceArgs& args){
        bc.stop();
    });
    bc.initialize(cg.app.server_port);
    bc.run();
    bc.stop();

}

#endif //HZDR_DATA_ACQUISITION_TCPSERVER_H

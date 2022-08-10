//
// Created by IHEB on 4/9/2020.
//

#ifndef HZDR_DEMO_SERVER_UI_H
#define HZDR_DEMO_SERVER_UI_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <thread>
#include "../../Tools/Parser.h"
#include "../../Engine/CommandInterface.h"
#include "../../Base/BaseProcess.h"
#include "../../DataStructures/TS_Queue.h"
using namespace std;

namespace UI_config{
    const std::vector<std::string> allowed_threads = {"server", "generator", "logging_raw", "logging_results", "demo", "importer"};
}


namespace UI{
    class UI_ERROR : public std::exception {
        std::string what_;
    public:
        explicit UI_ERROR(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

//******************************************************************************
// Console handler class
// This class reads the input from the console and parse it to a vector of strings.
// First element represents the command and further elements play different roles
// depending on the use case.
// In order to add a command, a callback function that takes a reference to a vector
// of strings needs to be defined and passed to addHandler alongside the command name.
// E.g: void test_callback(vector<string>& args) { std::cout << args[0] << "\n";}
//      ConsoleHandlerObject.addHandler("test", test_callback);
//******************************************************************************
    class ConsoleHandler: public Process{
    public:
        explicit ConsoleHandler(unsigned id_, ENGINE::CommandInterface& ci_);
        ENGINE::CommandInterface getCI();
        std::vector<std::string> valid_commands();
        void print(const std::string& txt);
        void run();
        void stop();

    private:
        Parser prs;
        ThreadsafeQueue<std::string> output_queue;
        ENGINE::CommandInterface& CI;
        void handle_input(const string& data);
        void get();
        string s;
    };

    ConsoleHandler::ConsoleHandler(unsigned id_, ENGINE::CommandInterface& ci_): Process(id_), CI(ci_){
    }


    ENGINE::CommandInterface ConsoleHandler::getCI(){
        return CI;
    }

    void ConsoleHandler::get() {
        getline(std::cin,s);
        std::transform(s.begin(), s.end(), s.begin(),
           [](unsigned char c){ return std::tolower(c); });
        try{
            handle_input(s);
        }
          catch(const ENGINE::CmdInterfaceError& err){
              std::cerr << err.what() << "\n";
          }
    }

    void ConsoleHandler::print(const std::string& txt){
        output_queue.push(txt);
    }

    void ConsoleHandler::handle_input(const string& data) {
        prs.parse(data);
        try{
            if (CI.command_is_valid(prs.values.at(0))){
                CI.run_command(prs.values.at(0), {prs.values});
            }

            else
                throw ENGINE::CmdInterfaceError("THE PROVIDED COMMAND ISN'T VALID: "+ prs.values.at(0) + "\n");
        }
        catch(const std::out_of_range& oor){
            //handle empty cmd
            print("");
        }
    }

    std::vector<std::string> ConsoleHandler::valid_commands(){
        return CI.get_cmds();
    }

    void ConsoleHandler::run() {
        this->shut_down_ = false;
        this->running = false;
        this->task_thread.push_back(std::make_shared<std::thread>([=, this]() {
            this->running = true;
            while(true){
                if(this->shut_down()){ // if shut_down is true raw data is an empty optional
                    this->running = false;
                    return;
                }
                try{
                    this->get(); //this interacts with the data base threads, look out for race conditions or queues being too slow
                }
                catch(const UI::UI_ERROR& err){
                    std::cerr << "ERROR IN UI Task: " << err.what();
                }
            }
        }));

        this->task_thread.push_back(std::make_shared<std::thread>([=, this]() {
            this->running = true;
            std::string msg;
            while(true){
                if(this->shut_down()){ // if shut_down is true raw data is an empty optional
                    this->running = false;
                    return;
                }
                try{
                    auto msg = this->output_queue.pop_blocking();
                    if (msg.has_value()){
                        std::cout << msg.value();
                        std::cout << "\n>> ";
                    }
                }
                catch(const UI::UI_ERROR& err){
                    std::cerr << "ERROR IN UI Task: " << err.what();
                }
            }
        }));
        this->task_thread[0]->detach();
        this->task_thread[1]->join();

//        for (auto& t: task_thread)
//            t->detach();
}

    void ConsoleHandler::stop() {
        this->shut_down_ = true;
        this->output_queue.snapOut();
    }
}

void test_UI(){
    ENGINE::CommandInterface ci;
    UI::ConsoleHandler console(456, ci);
    ci.add_command("aa", [&](const ENGINE::CommandInterfaceArgs&){console.print("3ASBA\n");});
    ci.add_command("shutup", [&](const ENGINE::CommandInterfaceArgs&){console.stop();});
    console.run();
}

#endif //HZDR_DEMO_SERVER_UI_H

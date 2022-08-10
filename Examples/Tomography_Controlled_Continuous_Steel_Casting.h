//
// Created by IHEB on 10/08/2022.
//

#ifndef HZDR_DATA_ACQUISITION_TOMOGRAPHY_CONTROLLED_CONTINUOUS_STEEL_CASTING_H
#define HZDR_DATA_ACQUISITION_TOMOGRAPHY_CONTROLLED_CONTINUOUS_STEEL_CASTING_H
#include "../DataStructures/Config.h"
#include "../Thread_Pool/Tasking_system.h"
#include "../Engine/CommandInterface.h"
#include "../Engine/DataEngine.h"
#include "../Engine/LTTEngine.h"
#include "../Engine/Tasks.h"
#include "../IOs/Files/Logger.h"
#include "../IOs/TCP_IP/TCPServer.h"
#include "../IOs/TCP_IP/API.h"
#include "../IOs/Console/UI.h"

#include <algorithm>

#define EMULATE 1
void tomography_controlled_continuous_steel_casting() {
    //TODO ADD SERVER REST API TO COMMAND INTERFACE
    ///read config from input, set default if enter is pressed
    std::string input;
    fs::path config_path;
    Config cg;

    program_start:
    std::cout << "Welcome to TCCSC-Experiment, please input the path to the config file: ";
    std::getline(std::cin, input);
    if (input.empty())
        config_path = "C:\\Users\\User\\Desktop\\HZDR\\hzdr_tasler_data_acquisition\\Examples\\tasler_config.json";
    else{
        config_path = input;
    }
    try{
        cg.load(config_path);
    }
    catch(const CONFIG_ERROR& err){
        std::cerr << err.what();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); //for formatting purposes
        goto program_start;
    }

    ///create static variables needed throughout the program's lifetime
    ENGINE::CommandInterface ci;
    UI::ConsoleHandler console(456, ci);
    Broadcaster bc(12345, ci);
    bc.initialize(cg.app.server_port, {[&]{testing_timer.stop();}});
    ///create placeholders for dynamically allocated variables during runtime
    std::shared_ptr<ENGINE::LOGGER> logger;
    std::shared_ptr<task_system> tp;
    std::shared_ptr<ENGINE::DataEngine> db;
    std::shared_ptr<ENGINE::LTTEngine> ltt_en;
    ///default operation parameters
    ENGINE::CHANNELS_OP_MODE opMode = ENGINE::SHARED;
    ENGINE::LOGGING_MODE logging_mode = ENGINE::ASCII;

    ///add commands to Command Interface
    ci.add_command("help", [&](const ENGINE::CommandInterfaceArgs &args){
        auto cmds = ci.echo_commands();
        if(args.server)
            args.server->sendMessage<const char*>(args.src_fd,
                                                  cmds.c_str(),
                                                  cmds.size(), 0);
        else
            console.print(cmds);
    });

    ci.add_command("start", [&](const ENGINE::CommandInterfaceArgs &args) {
        auto &values = args.inputs;
        if (std::find(values.begin(), values.end(), "-bin") != values.end()) {
           logging_mode = ENGINE::BINARY;
        }
        if (std::find(values.begin(), values.end(), "-l") != values.end()) {
            if (!logger)
                logger = std::make_shared<ENGINE::LOGGER>(cg, logging_mode);
        }

        if (std::find(values.begin(), values.end(), "-async") != values.end()) {
            if(!tp)
                tp = std::make_shared<task_system>(cg.app.thread_count);
            opMode = ENGINE::ASYNCHRONOUS;
        } else if (std::find(values.begin(), values.end(), "-sync") != values.end())
           opMode = ENGINE::SYNCHRONOUS;

        db = std::make_shared<ENGINE::DataEngine>(cg, opMode, logger, tp);
        db->initialize({ENGINE::clean_reference_signal, ENGINE::demodulate});
        db->add_filter();
        db->start_all();

        ltt_en = std::make_shared<ENGINE::LTTEngine>(-560, cg, db);
#if EMULATE
        ltt_en->test_mode();
#endif
        ltt_en->initialize();
        ltt_en->run();
        if (std::find(values.begin(), values.end(), "-tracker") != values.end()){
            if(args.server)
                ltt_en->start_tracker([&](const std::string& txt){args.server->sendMessage<const char*>(args.src_fd,
                                                                                                        txt.c_str(),txt.size(), 0);});
            else
                ltt_en->start_tracker([&](const std::string& txt){console.print(txt);});
        }
        }
    );

    ci.add_command("stop", [&](const ENGINE::CommandInterfaceArgs &args) {
        auto &values = args.inputs;
        if (values.size() == 1){
            if (ltt_en)
                ltt_en->stop();
            if(db)
                db->stop_all();
            testing_timer.stop();
            db->add_logger(std::shared_ptr<ENGINE::LOGGER>());
            logger = std::shared_ptr<ENGINE::LOGGER>();
        }
        else{
            if(ltt_en && std::find(values.begin(), values.end(), "-ltt") != values.end())
                ltt_en->stop();
            if(db && std::find(values.begin(), values.end(), "-db")!= values.end())
                db->stop_all();

            if (std::find(values.begin(), values.end(), "-tracker")!= values.end())
                testing_timer.stop();
            if (std::find(values.begin(), values.end(), "-l")!= values.end()){
                db->add_logger(std::shared_ptr<ENGINE::LOGGER>()); //override pointer to be empty to deactivate logging
                logger = std::shared_ptr<ENGINE::LOGGER>();
            }
        }

    });

    ci.add_command("kill", [&](const ENGINE::CommandInterfaceArgs &args) {
        if(ltt_en)
            ltt_en->stop();
        if(db)
            db->stop_all();
        testing_timer.stop();
        bc.stop();
        console.stop();

    });

    ci.add_command("log", [&](const ENGINE::CommandInterfaceArgs &args){
        if(db){
            auto &values = args.inputs;
            if (std::find(values.begin(), values.end(), "-bin") != values.end())
                logging_mode = ENGINE::BINARY;
            else
                logging_mode = ENGINE::ASCII;
            logger = std::make_shared<ENGINE::LOGGER>(cg, logging_mode);
            db->add_logger(logger);
        }
        else{
            std::string msg = "DATA ENGINE HAS TO BE CREATED BEFORE LOGGING CAN START, TYPE START COMMAND TO CREATE"
                              " DATA ENGINE";
            if(args.server)
                args.server->sendMessage<const char*>(args.src_fd, msg.c_str(),msg.size(), 0);
            else
                console.print(msg);
        }

    });

    ci.add_command("config", [&](const ENGINE::CommandInterfaceArgs &args){
        auto cfg_str = cg.str()+"\n";
        if(args.server)
            args.server->sendMessage<const char*>(args.src_fd,
                                                  cfg_str.c_str(),
                                                  cfg_str.size(), 0);
        else
            console.print(cfg_str);
    });

    ci.add_command("load", [&](const ENGINE::CommandInterfaceArgs &args){
        auto &values = args.inputs;
        //path string needs to be provided right after -path flag otherwise same path is reused
        auto p = std::find(values.begin(), values.end(), "-path");
        if (p != values.end()){
            p++; //increment pointer to the next element
            console.print("new config path: "+ *p);
            config_path = *p;
        }
        try{
            cg.load(config_path);
            if(args.server)
                args.server->sendMessage<const char*>(args.src_fd, "\n",1, 0);
            else
                console.print(" ");
        }
        catch(const CONFIG_ERROR& err){
            std::string msg(err.what());
            if(args.server)
                args.server->sendMessage<const char*>(args.src_fd, msg.c_str(),msg.size(), 0);
            else
                console.print(msg);
        }

    });

    ci.add_command("track", [&](const ENGINE::CommandInterfaceArgs &args){
        if (ltt_en){
            if(args.server)
                ltt_en->start_tracker([&](const std::string& txt){
                    args.server->sendMessage<const char*>(args.src_fd, txt.c_str(), txt.size(), 0);
                });
            else
                ltt_en->start_tracker([&](const std::string& txt){console.print(txt);});
        }
    });

    ///run Interfaces
    bc.run();
    console.run();//this is blocking
}

#endif //HZDR_DATA_ACQUISITION_TOMOGRAPHY_CONTROLLED_CONTINUOUS_STEEL_CASTING_H

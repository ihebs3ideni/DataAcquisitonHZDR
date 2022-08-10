//
// Created by iheb on 07.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_STATES_H
#define HZDR_DATA_ACQUISITION_STATES_H
namespace ENGINE{
    enum CHANNELS_OP_MODE
            {SYNCHRONOUS,
             ASYNCHRONOUS,
             SHARED};
    enum LOGGING_MODE
            {ASCII,
             BINARY};
//    struct SM_Inputs{
//        bool data_in;
//        bool data_to_process_available;
//        bool memory_is_free;
//    };
}
#endif //HZDR_DATA_ACQUISITION_STATES_H

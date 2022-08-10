//
// Created by IHEB on 14/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_LTT24_WHISPERER_H
#define HZDR_DATA_ACQUISITION_LTT24_WHISPERER_H

#include <vector>
#include <string>
#include "ltt2api.h"
#include "ltt2api_emulation.h"
#include "../../DataStructures/Config.h"
#include "../Numerical_Magic/Numerical_magic.h"

class LTTError : public std::exception {
        std::string what_;
    public:
        explicit LTTError(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

enum LTTOpMode {INDEPENDENT = 0x00,
    CASCADED_MSS=0x01, /// meas. start synchronised
    CASCADED_MSCS=0x02 /// meas. start and clock synchronised
};

class LTT24{
    //THIS ASSUMES configurations_ is up to date
public:
    LTT24(Config& conf): configurations_(conf){}
    unsigned numberOfDevices{0};
    unsigned numberOfChannels{0};
    unsigned sample_size{0};
    std::string id_{"howdy partner"};

    void emulate(unsigned number_of_devices = 1){
        /***For debuggin purposes***/
        LTTSetDeviceEmulation(number_of_devices + 0x00021821 -1);
    }

    void initialize(){
        ///returns the number of analog channels found
        int32_t dev_idx;
//        uint32_t an_cnt;
        // initialize LTT2API
        numberOfDevices = LTTInit( NULL, NULL, NULL, NULL );
        if( numberOfDevices <= 0 ) {
            throw LTTError( "INIT: no devices found !!\n" );
        }

        // check device type and get channel count
//        an_cnt = 0;
        auto dev_info = getDevicesParameter();
        for( dev_idx = 0; dev_idx < numberOfDevices; dev_idx++ ) {
            if (numberOfDevices != dev_info.size())
                throw LTTError("Something Wrong with the Initalization!!!");

            if( dev_info[ dev_idx ].lDeviceType != 24 ) {
                throw LTTError( "INIT: at least one of the devices is not a LTT24 device "
                                "!!Mixing different device types is still not possible!\n" );
            }

            if( dev_idx == 0 )
                sample_size = dev_info[ dev_idx ].sample_size;

            else if( sample_size != dev_info[ dev_idx ].sample_size ) {
                throw LTTError( "INIT: devices have different sample-sizes !!Currently not supported!\n" );
//                return -2;
            }

            printf( "INIT: devices %d --> LTT%d [%dBit] SN: %d\n", dev_idx, dev_info[ dev_idx ].lDeviceType,
                    sample_size * 8, dev_info[dev_idx].lSerialNumber );

            numberOfChannels += dev_info[ dev_idx ].lChannels;
        }
        if (numberOfChannels < configurations_.channels.analog_channels.size()){
            std::cout << numberOfChannels << ";" << configurations_.channels.analog_channels.size() << "\n";
            throw LTTError( "INIT: the number of channels set in Config is bigger than the number of available channels "
                            "at initialization!!\n" );
        }

    }

    void deinitialize(){
        LTTDeInit();
    }

    void correct_sync(unsigned mode){
        ///specifies wether multiple LTT devices are running cascadied or independent,
        ///but still with the same timebase
        ///0 - independent mode
        ///1 - cascading mode
        LTTSyncCorrection(mode);
    }

    void set_devices_order(){
        ///sets device order using a list of serial numbers ordered in the requested order
        /// !!! MUST BE CALLED BEFORE initialize
        for (auto& n: configurations_.devices_order)
            std::cout << "serial number: " << n << "\n";
        switch (LTTSetDeviceOrder((unsigned)configurations_.devices_order.size(),
                                  configurations_.devices_order.data())){
            case 0:
                printf( "Devices Order set Successful\n");return;
            case -1:
                throw LTTError("Invalid pointer to the list of Ordered Devices in \"SetDevicesOrder\"");
            case -2:
                throw LTTError("Too many/little devices in the list of Ordered devices in \"SetDevicesOrder\"");

        }
    }

//    void set_transfer_mode(){
//        switch(LTTSTREAMTransfer(0, 0)){
//            case 0:
//                printf( "BUFFER-Type set Successfully\n");break;
//            case -1:
//                throw LTTError( "BUFFER-Type:\n ERROR: RAM already set up !!\n RAM and STREAM are exclusive buffer settings.\n");
//        }
//
//    }

    void set_devices_op_mode(LTTOpMode opMode= CASCADED_MSCS){
        LTTCascadingMode((int32_t)opMode);
    }

    void upload_parameter(int32_t TimeFlag=LTT_SMPLTIME_FLAG_ABOVE){
        ///Uses the Conf struct to upload all parameters at once
        LTTResetParameter( 0 );
        for(auto& ch:configurations_.channels.analog_channels) {
            switch (LTTAnalogChannel(ch.id, 1, ch.coup, ch.analog_range, ch.power_supply, ch.offset)) {
                case 0 :
                    printf("Channel Configuration: status= 0\n Successful \n");
                    break;
                case -1 :
                    throw LTTError("Channel Configuration: status= -1\n channel doesn't exist \n");
                case -2 :
                    throw LTTError("Channel Configuration: status= -2\n invalid dms_coup \n");
                case -3 :
                    throw LTTError("Channel Configuration: status= -3\n dms_coup doesn't match LTT-Hardware \n");
                case -4 :
                    throw LTTError(
                            "Channel Configuration: status= -4\n channel dms_range doesn't match dms_coup settings \n");
                case -12 :
                    throw LTTError("Channel Configuration: status= -12\n device doesn't support DMS \n");
            }
        }
        for(auto& ch:configurations_.channels.digital_channels){
                switch (LTTDigitalChannel(ch.id, 1)){
                    case 0 : printf(  "Channel Configuration: status= 0\n Successful \n"); break;
                    case -1 : throw LTTError( "Channel Configuration: status= -1\n channel doesn't exist \n");
                }
        }


        switch (LTTTrigger(configurations_.trigger.state,configurations_.trigger.slope,configurations_.trigger.channel ,
                           configurations_.trigger.chtype,configurations_.trigger.trigtype,
                           configurations_.trigger.f1, configurations_.trigger.f2)){
            case 0 : printf(  "Trigger Configuration: status= 0\n  Successful\n"); break;
            case -1 : throw LTTError(  "Trigger Configuration: status= -1\n  channel doesn't exist.\n");
            case -2 : throw LTTError(  "Trigger Configuration: status= -2\n  level trigger: can't have negative hysterisis.\n");
            case -3 : throw LTTError(  "Trigger Configuration: status= -3\n  delta trigger: invalid range for f2 (0 < f2 < 6)\n");
            case -4 : throw LTTError(  "Trigger Configuration: status= -4\n  level+delta trigger: invalid range for f2 (0 < f2).\n");
            case -5 : throw LTTError(  "Trigger Configuration: status= -5\n  invalid digital trigger bit.\n");
            case -6 : throw LTTError(  "Trigger Configuration: status= -6\n  invalid trigger channel type\n");
            case -7 : throw LTTError(  "Trigger Configuration: status= -7\n  invalid trigger type\n");
        }
        if (configurations_.trigger.state)
            switch(LTTTrigger_SetPreTrigger( configurations_.pretrigger.pre_triglen, configurations_.pretrigger.pre_sampletime)){
                case 0 : printf(  "Pretrigger Configuration: status= 0\n  Successful\n"); break;
                case -1 : throw LTTError(  "Pretrigger Configuration: status= -1\n  No trigger active\n"); break;
                case -2 : throw LTTError(  "Pretrigger Configuration: status= -2\n  Wrong pre-trigger length\n"); break;
            }

        if(LTTSampleTime(configurations_.sampling.timebase, TimeFlag)==configurations_.sampling.timebase){
            printf(  "Timebase: %d ns\t Successful\n", configurations_.sampling.timebase);
        }
        else {
            throw LTTError( "Timebase: CONFIG: setting of timebase failed\n");
        }

        LTTFilter( LTT_FILTER_OFF, 0 );

        switch(LTTSTREAMTransfer(0, 0)){
            case 0:
                printf( "BUFFER-Type set Successfully\n");break;
            case -1:
                throw LTTError( "BUFFER-Type:\n ERROR: RAM already set up !!\n RAM and STREAM are exclusive buffer settings.\n");
        }

        switch (LTTUploadParameters()){
            case 0 :
                printf("UPLOAD PARAMETERS: status= %d \n Everything OK\n", 0);
                break;
            case -1 :
                throw LTTError("UPLOAD PARAMETERS: status= -1 \n CONFIG: parameter upload failed\n no devices present\n");
            case -2 :
                throw LTTError("UPLOAD PARAMETERS: status= -2 \n CONFIG: parameter upload failed\n "
                               "Wrong sample rate by current channel setting\n");
            case -4 :
                throw LTTError("UPLOAD PARAMETERS: status= -4 \n CONFIG: parameter upload failed\n"
                               " No channels active\n");
            case -8 :
                throw LTTError("UPLOAD PARAMETERS: status= -8 \n CONFIG: parameter upload failed\n transfer active\n");
            case -16 :
                throw LTTError("UPLOAD PARAMETERS: status= -16 \n CONFIG: parameter upload failed\n "
                        "Wrong channel combination!!!\n When backside channels used, "
                        "at least one front channel must be active also\n");
            case -32 :
                throw LTTError("UPLOAD PARAMETERS: status= -32 \n CONFIG: parameter upload failed\n "
                               "Trigger channel not active\n");
            case -64 :
                throw LTTError("UPLOAD PARAMETERS: status= -64 \n CONFIG: parameter upload failed\n "
                               "PRE-trigger settings wrong\n");
            case -256 :
                throw LTTError("UPLOAD PARAMETERS: status= -256 \n CONFIG: parameter upload failed\n "
                               "PRE-Trigger settings wrong\n");
            case -512 :
                throw LTTError("UPLOAD PARAMETERS: status= -512 \n CONFIG: parameter upload failed\n "
                               "No Buffer settings specified!!\n");
            case -1024 :
                throw LTTError("UPLOAD PARAMETERS: status= -1024  \n CONFIG: parameter upload failed\n "
                               "master is not active (internal error)\n");
            case -2048 :
                throw LTTError("UPLOAD PARAMETERS: status= -2048 \n CONFIG: parameter upload failed\n "
                               "filter cut-off frequency out fo range\n");
        }

    }

    void start_devices(){
        switch(LTTStart()){
            case 0:
                printf("START: Devices Started Succesfully\n"); break;
            case -1:
                throw LTTError("START: no devices present or no active channels\n");
        }
    }

    void stop_devices(){
        LTTStop();
    }

    int start_data_stream(unsigned char* buff_ptr){
        auto res = LTTStream2Buffer(buff_ptr, configurations_.sampling.chunk_sequence_cnt,
                                    LTT_TRANSFER_NONEBLOCKED | LTT_TRANSFER_CONTINUOUS);
///     Commented because throwing an error leads to an internal LTT TRANSFER ERROR that never gets handled
//        switch (status) {
//            case 0:
//                printf("MEAS: status= 0 \t transfer invoked\n");
//            case -1 :
//                throw LTTError("MEAS: status= -1 \n No LTT devices\n");
//            case -2 :
//                throw LTTError("MEAS: status= -2 \n STREAM transfer not active\n");
//            case -3 :
//                throw LTTError("MEAS: status= -3 \n transfer is active, so can`t invoke another transfer!!\n");
//            case -4 :
//                throw LTTError("MEAS: status= -4 \n sequence count is not valid\n");
//        }
        return res;
    }

    void check_trigger(uint32_t wait_time){
        if (configurations_.trigger.state)
            while(1)
                if ( LTTCheckTrigger(wait_time))
                    return;

    }

    int wait_for_transfer()
    {   /***                CAUTION:
                The amount of transferred samples must be
                checked against the requested data length.
                If the amount of transferred samples is
                less than the requested amount, an
                irrecoverable error has occurred during the
                transfer, which led to a break of the transfer.
                In the data buffer the correctly transferred
                data are stored up to the point where the error
                has occurred!!                                  ***/
        auto status = LTTDataTransferWait();
        if (status <= 0)
            switch (status){
                case -3: throw LTTError( "WAIT: status = -3 \t no transfer active\n");
                case -255: throw LTTError("WAIT: status = -255 \t error happened\n");
            }
        if (status < configurations_.sampling.chunk_sequence_cnt)
            throw LTTError("an irrecoverable error has occurred during the transfer, "
                           "which led to a break of the transfer\n");
        return status; // >0 transfer complete, number of transmitted samples
    }

    std::vector<LTTDeviceInfo> getDevicesParameter(){
        if (numberOfDevices == 0)
            throw LTTError("No devices found, checking in getDevicesIDs");
        std::vector<LTTDeviceInfo> l(numberOfDevices);
        for (unsigned dev_idx=0; dev_idx<numberOfDevices; dev_idx++){
            LTTGetDeviceInfoEx(dev_idx, &l[dev_idx]);
        }
        return l;
    }

    std::vector<double> getChannelsConversionFactors(){
        ///uses channel config to fill a conversion factor array where the index is the channel's id
        /// and the value is the conversion factor
        std::vector<double> conversion_factors(configurations_.channels.analog_channels.size());
        for (auto& ch:configurations_.channels.analog_channels){
            conversion_factors[ch.id] = LTTGetChannelConverter(ch.id);
        }
        return conversion_factors;
    }

    unsigned get_raw_buffer_size(){
        return LTTGetRAMBufferSettings(configurations_.sampling.chunk_sequence_cnt,
                                       configurations_.channels.analog_channels.size());
    }

    int32_t get_transfer_status(){
        auto status = LTTDataTransferStatus();
//        switch (status){
//            case 0: printf( "MEAS: status = 0 \t Nothing TODO\n"); break;
//            case -1: printf( "MEAS: status = -1 \t waiting for trigger\n"); break;
//            case -2: printf( "MEAS: status = -2 \t new block available\n"); break;
//            case -3: throw LTTError( "MEAS: status = -2 \t no transfer active\n");
//            case -255: throw LTTError( "MEAS: status = -255 \t error happened\n");
//        }

        return status;
    }

    float get_time_resolution(){
        return configurations_.sampling.timebase*1e-9;
    }

//    unsigned get_sample_size(){
//        return this->getDevicesParameter()[0].sample_size; //return sample_size of the first device
//
//    }

    /// fills containers with the ids needed to parse data according to ref channels first then other channels in
    ///ascending order..
    /// exp seq = [ch0, ch1, ch5, ch8], ref_ch = ch1, ch8
    /// => indices = [1,3,0,2], ids = ["ref_1", "ref_8", "channel_0", "channel_5"]
//    void index_magic(std::vector<unsigned int>& indices, std::vector<std::string>& ids,
//                  const std::vector<unsigned int>& ref_ids){
//        std::vector<uint32_t> tasler_ord(configurations_.channels.analog_channels.size());
//        LTTGetChannelIDSequence(tasler_ord.data());
////        std::vector<unsigned int> tasler_ord = {1,2,4,5,9};
//        if(indices.capacity() < tasler_ord.size()) indices.reserve(tasler_ord.size());
//        if(ids.capacity() < tasler_ord.size()) ids.reserve(tasler_ord.size());
//        for(auto& r: ref_ids){
//            auto res = NM::findInVector<unsigned int>(tasler_ord, r);
//            if(res.first){
//                indices.emplace_back(res.second);
//            }
//        }
//        for(unsigned int i =0; i< tasler_ord.size(); i++){
//            if(std::count(ref_ids.begin(), ref_ids.end(), tasler_ord[i])){
//
//                ids.emplace_back("ref_"+to_string(tasler_ord[i]));
//
//            }
//            else{
//                indices.emplace_back(i);
//                ids.emplace_back("channel_"+to_string(tasler_ord[i]));
//            }
//        }
//
//    }

private:
    Config& configurations_;

};

#endif //HZDR_DATA_ACQUISITION_LTT24_WHISPERER_H

////
//// Created by iheb on 07.05.2022.
////
//
#ifndef HZDR_DATA_ACQUISITION_CORE_H
#define HZDR_DATA_ACQUISITION_CORE_H
#include <unordered_map>
#include <string>
#include <sstream>
#include <memory>
#include <functional>
#include <atomic>
#include <vector>
#include "../Libraries/Numerical_Magic/Numerical_magic.h"
#include "../Models/Coils.h"

namespace ENGINE{
    class CHANNEL_ERROR : public std::exception {
        std::string what_;
    public:
        explicit CHANNEL_ERROR(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

    class default_logger{
    public:
        void log(const std::string& msg){
            std::cout << msg << std::endl;
        }
    };

    struct rInput{
        //raw input container
        float timestamp;
        int raw_data;
    };

    template<typename T=float, typename D=NM::COMPLEX>
    struct DataFrame{
        //processed data container
        T timestamp;
        D data;
    };

    template<typename T=ENGINE::DataFrame<float, NM::COMPLEX>>
    struct JOB{
        int ID; //or ENUM/Int?
//        std::function<void()> func;
        std::vector<T> data;
        std::vector<T> reference_data;
        std::function<void(const T&)> return_result;
        COIL_FACTORS<double> coil_factors;
        COIL_FACTORS<double> ref_coil_factors;
        double timebase;
        float ref_frequency;
    };
}

#endif //HZDR_DATA_ACQUISITION_CORE_H

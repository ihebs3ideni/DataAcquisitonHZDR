//
// Created by IHEB on 30/06/2022.
//

#ifndef HZDR_DATA_ACQUISITION_CUSTOM_ALGORITHMS_H
#define HZDR_DATA_ACQUISITION_CUSTOM_ALGORITHMS_H
#include <vector>
#include "../Libraries/Numerical_Magic/Numerical_magic.h"
#include "../Engine/core.h"
namespace ALGORITHMS{
    class COMPUTATION_ERROR : public std::exception {
        std::string what_;
    public:
        explicit COMPUTATION_ERROR(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

    void centre_signal(const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& sig){
        std::vector<double> signal(sig.size());
        for (unsigned i=0; i<sig.size(); i++){
            signal[i] = sig[i].data[0];
        }
        signal = NM::centre_data(signal);
        for (unsigned i=0; i<sig.size(); i++){
            (double&)sig[i].data[0] = signal[i];
        }
    }

    int get_period_block_size(float ref_frequency_hz, float timebase_s){//T/dt
        return std::rint((1/ref_frequency_hz)/(timebase_s));
    }

    /// based on the work of Matthias Ratajczak with python
    int find_frequency(std::vector<ENGINE::DataFrame<float, NM::COMPLEX>> sig, float timebase){
        float sampling_frequency = 1/timebase;

        std::vector<double> signal(sig.size());
        for (unsigned i=0; i<sig.size(); i++){
            signal[i] = sig[i].data[0];
        }
        double av = NM::mean(signal);

        signal = NM::apply_mask<double, double, double>(signal, [&](const double& e) -> double{return e - av; });
        std::vector<unsigned> zero_idxs = NM::argwhere_index(NM::diff(NM::sign(signal)), [](const double& e) -> bool{return e != 0.; });
        std::vector<unsigned> diff_zero_idxs_raw = NM::diff(zero_idxs);

        std::vector<unsigned> close_root_indices = NM::argwhere_index(diff_zero_idxs_raw,
                                                     [&](const double& e) -> bool{ return e >= (sampling_frequency / 50);});

        std::vector<double> diff_zero_idxs_;
        diff_zero_idxs_.reserve(close_root_indices.size());
        for(auto& i: close_root_indices) {
            diff_zero_idxs_.emplace_back(diff_zero_idxs_raw[i]);
        }
        double max_diff;
        if(diff_zero_idxs_.size() != 0)
//            std::cout << "1..\n";
            max_diff = *std::max_element(diff_zero_idxs_.begin(), diff_zero_idxs_.end());
        else if (diff_zero_idxs_raw.size() != 0)
//            std::cout << "2..\n";
            max_diff = *std::max_element(diff_zero_idxs_raw.begin(), diff_zero_idxs_raw.end());
        else  //THROW EXCEPTION MAYBE?
            throw COMPUTATION_ERROR("FREQUEUCNY ALGORITHM DOESN'T SEEM TO WORK ON THE PROVIDED SAMPLE\n");
        auto mean_diff = NM::mean(diff_zero_idxs_);

        if (max_diff > 1.3 * mean_diff){ //INTERLEAVED excitation

            std::vector<double> diff_zero_idxs;
            std::vector<unsigned> indices_(NM::argwhere_index(diff_zero_idxs_,
                                                               [&](const double& e) -> bool{return e <= (max_diff / 2); }));

            for(auto& i: indices_){
                diff_zero_idxs.push_back(diff_zero_idxs_[i]);
            }


            auto occurrences = NM::occurrences(diff_zero_idxs);
            std::vector<double> distances;
            std::vector<double> count;
            for(auto& p: occurrences){
                distances.push_back(p.first);
                count.push_back(p.second);
            }
            auto max_index = NM::arg_max(count);
            return std::rint(sampling_frequency / (2*distances[max_index]));
        }
        else{
            return std::rint(sampling_frequency / (2*mean_diff));
        }
    }

    ///phase shift needs to be relative to pi
    ///f is signal frequency
    ///timebase is the reverse of the sampling frequency: 1/FS
    ///only works on n signal periods, where n is an integer
    std::vector<double> shift_phase(const std::vector<double>& signal, float phase_shift, float f_hz, float timebase){
        try {
//            std::cout << "integral= " << NM::sum(signal) << "\n";
            int signal_period_size = std::rint(((1 / f_hz) / (timebase ))); //* 1e-9
            int block_size = std::rint(signal_period_size * phase_shift / 2); //signal period is 2*pi therefore /2
//            std::cout << "block_size: " << block_size << "\n";


            std::vector<double> res;
            res.resize(signal.size());
            unsigned j = 0;

            if (phase_shift > 0) {
                //take the first block and put it in the end,
                for (unsigned i = block_size; i < signal.size(); i++) {
                    res[j] = signal[i];
                    j++;
                }
                for (int i = 0; i < block_size; i++) {
                    res[j] = signal[i];
                    j++;
                }
                return res;

            } else if (phase_shift < 0) {
                //take the last block and put it in the beginning
                for (unsigned i = -1*block_size; i < signal.size(); i++) {
                    res[i] = signal[j];
                    j++;
                }
                for (int i = 0; i < -1*block_size; i++) {
                    res[i] = signal[j];
                    j++;
                }
                return res;
            } else {

                return signal;
            }
        }
        catch (const std::exception &exc){
            std::cout <<exc.what();
            return signal;
        }
    }
    NM::COMPLEX LOMB_SCARGLE(const std::vector<double>& signal,
                             const std::vector<double>& ref_signal,
                             float ref_frequency, float timebase,
                             const double& phase=0., int timestamp=0
    ){
        float duration = timebase*signal.size();
        auto& ref_cos = ref_signal;
        auto ref_sin = ALGORITHMS::shift_phase(ref_signal, -0.5, ref_frequency, timebase);
//        std::cout << "timebase in lombscargle: " << timebase << "\n";

        //double w = 2*NM::PI*ref_frequency;
        double tau = atan2(NM::sum(ref_sin), NM::sum(ref_cos))/2;

        std::vector<double> t_cos = NM::cos_(duration, 1.,ref_frequency,((-1.*tau)/NM::PI), timebase);
        std::vector<double> t_sin = NM::sin_(duration, 1.,ref_frequency,((-1.*tau)/NM::PI), timebase);
        std::vector<double> t_cos_squared = NM::multiply(t_cos, t_cos, 1.);
        std::vector<double> t_sin_squared = NM::multiply(t_sin, t_sin, 1.);

//        std::cout << "size 1: " << signal.size() << "size 2: " << t_cos.size() << "\n";
        double Ai = NM::sum(NM::multiply(signal, t_cos,1.))/NM::sum(t_cos_squared);
        double Bi = NM::sum(NM::multiply(signal, t_sin,1.))/NM::sum(t_sin_squared);
        double A = sqrt(Ai*Ai+Bi*Bi);
        double PHI= -(atan2(Bi, Ai)+ tau + phase);
//        NM::COMPLEX res(A*cos(PHI), A*sin(PHI), timestamp);
        return NM::COMPLEX(A*cos(PHI), A*sin(PHI), timestamp);

    }

    NM::COMPLEX LOMB_SCARGLE(const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& sig,
                             const std::vector<double>& ref_sig,
                             float ref_frequency, float timebase,
                             const double& phase=0., int timestamp=0
                             ){
//        screen_logger.push_back("ch_len: "+to_string(ch_signal.size())+"; ref_len: "+to_string(ref_signal.size())+"\n");
        std::vector<double> signal; signal.resize(sig.size());
//        std::vector<double> ref_signal; ref_signal.resize(ref_sig.size());
        for (unsigned i=0; i<sig.size(); i++){
            signal[i] = sig[i].data[0];
//            ref_signal[i] = ref_sig[i].data[0];
        }
        return ALGORITHMS::LOMB_SCARGLE(signal, ref_sig, ref_frequency, timebase, phase, timestamp);
    }

    NM::COMPLEX LOMB_SCARGLE(const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& sig,
                             const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& ref_sig,
                             float ref_frequency, float timebase,
                             const double& phase=0., int timestamp=0
    ){
//        screen_logger.push_back("ch_len: "+to_string(ch_signal.size())+"; ref_len: "+to_string(ref_signal.size())+"\n");
        std::vector<double> signal; signal.resize(sig.size());
        std::vector<double> ref_signal; ref_signal.resize(ref_sig.size());
        for (unsigned i=0; i<sig.size(); i++){
            signal[i] = sig[i].data[0];
            ref_signal[i] = ref_sig[i].data[0];
        }
        return ALGORITHMS::LOMB_SCARGLE(signal, ref_signal, ref_frequency, timebase, phase, timestamp);
    }


    NM::COMPLEX Qdt(const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& sig,
                    const std::vector<ENGINE::DataFrame<float, NM::COMPLEX>>& ref_sig,
                    float ref_frequency_, float timebase){
//        auto sig_integral = NM::sum(ref_signal);
//        if(sig_integral > integration_threashold){
//            std::cout << "ERROR: QDT only works for perfect periods.\nIntegral of this signal = " << sig_integral <<"\n";
//            return {0.,0.};
//        }

        std::vector<double> signal; signal.resize(sig.size());
        std::vector<double> ref_signal; ref_signal.resize(ref_sig.size());
//        std::vector<double> ref_signal(ref_sig.size());
        for (unsigned i=0; i<sig.size(); i++){
            signal[i] = sig[i].data[0];
            ref_signal[i] = ref_sig[i].data[0];
        }
        auto& ref1 = ref_signal;
        auto ref2 = ALGORITHMS::shift_phase(ref_signal, -0.5, ref_frequency_, timebase);

        NM::COMPLEX res(NM::mean(NM::multiply(signal, ref1, 2.0)),
                        NM::mean(NM::multiply(signal, ref2, 2.0))*-1    ,
                        0.0);
        return res;

    }




}




#endif //HZDR_DATA_ACQUISITION_CUSTOM_ALGORITHMS_H

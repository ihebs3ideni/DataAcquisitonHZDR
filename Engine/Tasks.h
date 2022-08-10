//
// Created by IHEB on 31/07/2022.
//

#ifndef HZDR_DATA_ACQUISITION_TASKS_H
#define HZDR_DATA_ACQUISITION_TASKS_H

#include "core.h"
#include "../Tools/custom_algorithms.h"
#include "../Models/DspFilters/Butterworth.h"
#include "../Models/DspFilters/SmoothedFilter.h"
#include "../Models/DspFilters/Params.h"

namespace ENGINE{
    auto clean_reference_signal = [](const ENGINE::JOB<ENGINE::DataFrame<float, NM::COMPLEX>>& j){
        try{
            //centre reference signals
             ALGORITHMS::centre_signal(j.reference_data);

            auto duration = j.reference_data.size()*j.timebase;
            auto refs_ref = NM::cos_(duration, 1., j.ref_frequency, 0, j.timebase);

            auto z =  ALGORITHMS::LOMB_SCARGLE(j.reference_data, refs_ref, j.ref_frequency, j.timebase);
            auto& R = z[0]; auto& I= z[1];

            // apply phase correction
            double phi= z.arg_() + j.ref_coil_factors.phase_correction;

            //recreate clean reference signals
            std::vector<double> clean_ref = NM::cos_(duration, 1, j.ref_frequency, (float)phi/NM::PI, j.timebase);
            //update ref data in job
            for (unsigned i=0; i< j.reference_data.size(); i++){
                (double&)(j.reference_data[i].data[0]) = clean_ref[i];
            }
        }
        catch(const std::exception& ex){
            std::cerr << "ERROR IN clean_reference_signal CALLBACK CH" << j.ID << " :" << ex.what() << "\n";
        }
    };

    auto demodulate = [](const ENGINE::JOB<ENGINE::DataFrame<float, NM::COMPLEX>>& j) {
        try {
            //centre channel data
            ALGORITHMS::centre_signal(j.data);
//            for (auto t: j.data)
//                printf("%f; ", t.timestamp);
//            std::cout << std::endl;
            auto res = ALGORITHMS::LOMB_SCARGLE(j.data, j.reference_data, j.ref_frequency, j.timebase);
            //correct for coils
            res.vals[0] *= j.coil_factors.amplitude_correction;
            res.vals[1] *= j.coil_factors.amplitude_correction;
            //extract B
            res.vals[0] /= j.coil_factors.vToB;
            res.vals[1] /= j.coil_factors.vToB;

            float t_ = j.data[j.data.size() - 1].timestamp; //timestamp of last element in the block
            if (j.return_result)
                j.return_result({t_, res});

        }
        catch (const std::exception &ex) {
            std::cerr << "ERROR IN demodulate CALLBACK CH" << j.ID << " :" << ex.what() << "\n";
        }
    };
}

#endif //HZDR_DATA_ACQUISITION_TASKS_H

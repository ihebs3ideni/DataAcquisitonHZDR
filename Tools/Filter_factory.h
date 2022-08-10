//
// Created by IHEB on 31/07/2022.
//

#ifndef HZDR_DATA_ACQUISITION_FILTER_FACTORY_H
#define HZDR_DATA_ACQUISITION_FILTER_FACTORY_H
#include "../Models/DspFilters/Butterworth.h"
#include "../Models/DspFilters/SmoothedFilter.h"
#include "../Models/DspFilters/Params.h"

Dsp::Filter* create_default_filter(){
    /*** THIS FUNCTION DOES NOT OWN THE POINTER AND THE USER HAS TO MANAGE IT***/
    Dsp::Filter* filter = new Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::LowPass <8>, 1, Dsp::DirectFormII>(1024);
    Dsp::Params params;
    params[0] = 5000; // sample rate
    params[1] = 5; // order
    params[2] = 24; // cutoff frequency
    filter->setParams (params);
    return filter;
}



#endif //HZDR_DATA_ACQUISITION_FILTER_FACTORY_H

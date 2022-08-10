//
// Created by iheb on 4/19/2021.
//

#ifndef HZDR_TASLER_TP_NUMERICAL_MAGIC_H
#define HZDR_TASLER_TP_NUMERICAL_MAGIC_H

#include <cmath>
#include <math.h>
#include <cassert>
#include <map>

namespace NM{

    class NMError : public std::exception {
        std::string what_;
    public:
        explicit NMError(std::string msg) { what_ = std::move(msg); }
        const char* what() const noexcept override  { return what_.c_str(); }
    };

    const double PI = 3.1415926535897932384626433832795028841971;

    struct COMPLEX{
    public:
        COMPLEX(){
            vals[0] = 0.; vals[1] = 0.; vals[2] = 0.;
        }
        COMPLEX(double r, double i=0., double t_=0.)
        {
            vals[0] = r;  vals[1] = i;  vals[2] = t_;
        }

        COMPLEX(const COMPLEX &other) {
            vals[0] = other.vals[0];
            vals[1] = other.vals[1];
            vals[2] = other.vals[2];
        }

//        const double& operator[] (unsigned i) { if (i > 2) exit(-1); return vals[i]; }
        double& operator[] (unsigned i) { if (i > 2) exit(-1); return vals[i]; }
        const double& operator[] (unsigned i) const { if (i > 2) exit(-1); return vals[i]; }

        double* get_pointer(){
            return vals;
        }

        const double abs_(){
            auto& R = vals[0];
            auto& I = vals[1];
            return sqrt(R*R+I*I);
        }

        const double arg_(){
            auto& R = vals[0];
            auto& I = vals[1];
            return atan2(I, R);
        }

        std::string as_string(){
            return std::string("{"+std::to_string(vals[0])+";"+std::to_string(vals[1])+";"+std::to_string(vals[2])+"}"); //{real;imag;time}
        }

        std::string as_string() const{
            return std::string("{"+std::to_string(vals[0])+";"+std::to_string(vals[1])+";"+std::to_string(vals[2])+"}"); //{real;imag;time}
        }


        // Thomas Wondrak
        // DON't DO THIS!!!!!
        // If you are not writing a copy constructor, then after copying the object in memory,
        // the adresses are wrong!!!
        // READ Effective C++ by Scotty Meyers. He explains in more detail, why this is not a good idea
        // Especially, if you use std::map and similar things, where you do not know,
        // how often these containers are resized and probably some copying is done
        //double* real = &vals[0];
        //double* imag = &vals[1];
        //double* t = &vals[2];



//        private:
        double vals[3];

    };


    const COMPLEX j(0.,1.,0.);

    COMPLEX conjugate(const COMPLEX& z);

    std::vector<double> sin_(float duration, float amplitude, float frequency, float phase, float resolution);

    std::vector<double> cos_(float duration, float amplitude, float frequency, float phase, float resolution);

    template<typename data_type>
    auto random_(int size, data_type min_value, data_type max_value);
    void plant_a_seed();


    template<typename data_type>
    auto diff(const std::vector<data_type>& v1, const std::vector<data_type>& v2);
    template<typename data_type>
    auto diff(const std::vector<data_type>& v1);

    template<typename data_type>
    std::vector<int> sign(const std::vector<data_type>& data_);


    template<typename data_type>
    std::vector<bool> nonzero_bool(const std::vector<data_type>& data_);

    template<typename data_type>
    std::vector<unsigned int> nonzero_index(const std::vector<data_type>& data_);

    template<typename data_type>
    std::vector<unsigned int> zero_index(const std::vector<data_type>& data_);


    template<typename data_type , typename func_type>
    std::vector<bool> argwhere_bool(const std::vector<data_type>& data_, func_type&& f_);

    template<typename data_type, typename func_type>
    std::vector<unsigned int> argwhere_index(const std::vector<data_type>& data_, func_type&& f_);

    template<typename data_type, typename func_ret, typename func_param, typename func_type>
    std::vector<func_ret> apply_mask(const std::vector<data_type>& data_, func_type&& f_);

    template<typename data_type, typename func_ret, typename func_param, typename func_type>
    std::vector<func_ret> apply_mask(data_type* data_, unsigned size, func_type&& f_);

    template<typename data_type>
    data_type mean(const std::vector<data_type>& data_);

    template<typename data_type>
    data_type mean(data_type* data_, unsigned size);

    template<typename data_type>
    data_type mm_mean(const std::vector<data_type>& data_);

    template<typename data_type>
    unsigned int arg_max(const std::vector<data_type>& data_);

    ///makes a copy of data to not change the original data
    template<typename data_type>
    std::vector<data_type> unique(const std::vector<data_type>& data_, const std::string& criteria );


    template<typename data_type>
    std::map<data_type, unsigned int> occurrences(const std::vector<data_type>& data_);

    template<typename data_type>
    std::vector<data_type> multiply(const std::vector<data_type>& V1, const std::vector<data_type>& V2, const data_type& coef);

    template<typename data_type>
    std::vector<data_type> multiply(const std::vector<data_type>& V1, const data_type& coef);

    template<typename data_type>
    std::vector<data_type> add(const std::vector<data_type>& V1, const std::vector<data_type>& V2);

    template<typename data_type>
    std::vector<data_type> add(const std::vector<data_type>& V1, data_type offset);

    template<typename data_type>
    data_type dot(const std::vector<data_type>& V1, const std::vector<data_type>& V2);

    template<typename data_type>
    data_type sum(const std::vector<data_type>& data_);

    template<typename data_type>
    data_type sum(data_type* data_, int size);



    template <typename data_type>
    std::pair<bool, int > findInVector(const std::vector<data_type>  & vecOfElements, const data_type  & element);

    template <typename data_type>
    std::vector<data_type> centre_data(const std::vector<data_type>& data_);

    template <typename data_type>
    std::vector<data_type> centre_data(data_type* data_, unsigned size);

    template <typename data_type>
    std::vector<data_type> normalize(const std::vector<data_type>& data_);
    template <typename data_type>
    std::vector<data_type> normalize(data_type* data_, unsigned size);

    template <typename data_type>
    std::vector<data_type> abs_(const std::vector<data_type>& data_);
    template <typename data_type>
    std::vector<data_type> abs_(data_type* data_, unsigned size);


//    COMPLEX complex_multiply(COMPLEX z1, COMPLEX z2);
    COMPLEX conjugate(const COMPLEX& z);

    bool is_number(const std::string& s);

}

bool NM::is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

///THIS PART ALLOWS FOR COMPLEX NUMBER MANIPULATION USING STANDARD OPERATORS :BEGIN

template<typename T>
NM::COMPLEX operator* (const NM::COMPLEX& z, const T& x)
{
    return NM::COMPLEX(z[0]*x, z[1]*x);
}

template<typename T>
NM::COMPLEX operator* (const T& x, const NM::COMPLEX& z)
{
    return NM::COMPLEX(z[0]*x, z[1]*x);
}

NM::COMPLEX operator*( const NM::COMPLEX& z1, const NM::COMPLEX& z2)
{
    return NM::COMPLEX(z1[0]*z2[0]-z1[1]*z2[1],
                   z1[0]*z2[1]+z1[1]*z2[0]);
}

template<typename T>
NM::COMPLEX operator/(const NM::COMPLEX& z, const T& x)
{
    return NM::COMPLEX(z[0]/x, z[1]/x);
}

NM::COMPLEX operator/(const NM::COMPLEX& z1, const NM::COMPLEX& z2)
{
   return (z1*NM::conjugate(z2))/(z2*NM::conjugate(z2))[0];
}


template<typename T>
NM::COMPLEX operator/(const T& x, const NM::COMPLEX& z)
{
    return x*(NM::conjugate(z)/(z*NM::conjugate(z))[0]);
}

NM::COMPLEX operator+(const NM::COMPLEX& z1, const NM::COMPLEX& z2){
    return NM::COMPLEX(z1[0]+z2[0], z1[1]+z2[1]);
}

template<typename T>
NM::COMPLEX operator+(const T& x, const NM::COMPLEX& z){
    return NM::COMPLEX(z[0]+x, z[1]);
}

template<typename T>
NM::COMPLEX operator+(const NM::COMPLEX& z, const T& x){
    return NM::COMPLEX(z[0]+x, z[1]);
}

NM::COMPLEX operator-(const NM::COMPLEX& z1, const NM::COMPLEX& z2){
    return NM::COMPLEX(z1[0]-z2[0], z1[1]-z2[1]);
}

template<typename T>
NM::COMPLEX operator-(const T& x, const NM::COMPLEX& z){
    return NM::COMPLEX(x-z[0], 0.-z[1]);
}

template<typename T>
NM::COMPLEX operator-(const NM::COMPLEX& z, const T& x){
    return NM::COMPLEX(z[0]-x, z[1]);
}

NM::COMPLEX NM::conjugate(const NM::COMPLEX& z){
    return NM::COMPLEX(z[0], -1*z[1]);
}

///THIS PART ALLOWS FOR COMPLEX NUMBER MANIPULATION USING STANDARD OPERATORS: END


template <typename data_type=double>
std::vector<data_type> NM::abs_(const std::vector<data_type>& data_){
    std::vector<data_type> res; res.reserve(data_.size());
    for(auto& d_:data_){
        res.emplace_back(abs(d_));
    }
    return res;
}

template <typename data_type=double>
std::vector<data_type> NM::abs_(data_type* data_, unsigned size){
    std::vector<data_type> res; res.reserve(size);
    for(int i=0; i<size; i++){
        res.emplace_back(abs(data_[i]));
    }
    return res;
}

template <typename data_type=double>
std::vector<data_type> NM::centre_data(const std::vector<data_type>& data_){
    double av = NM::mean(data_);
    return NM::apply_mask<data_type, data_type, data_type>(data_, [&](const data_type& e) -> data_type{return e - av; });
}

template <typename data_type=double>
std::vector<data_type> NM::centre_data(data_type* data_, unsigned size){
    double av = NM::mean(data_, size);
    return NM::apply_mask<data_type, data_type, data_type>(data_, size, [&](const data_type& e) -> data_type{return e - av; });
}

template <typename data_type=double>
std::vector<data_type> NM::normalize(const std::vector<data_type>& data_){
    data_type ampl = (*std::max_element(data_.begin(), data_.end()) - *std::min_element(data_.begin(), data_.end()))/2;
    return NM::apply_mask<data_type, data_type, data_type>(data_, [&](const data_type& e) -> data_type{return e/ampl; });
}

template <typename data_type=double>
std::vector<data_type> NM::normalize(data_type* data_, unsigned size){
    data_type ampl = (*std::max_element(data_, data_+size+1) - *std::min_element(data_, data_+size+1))/2;
    return NM::apply_mask<data_type, data_type, data_type>(data_, size, [&](const data_type& e) -> data_type{return e/ampl; });
}

/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.
bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
*/
template <typename data_type=double>
std::pair<bool, int > NM::findInVector(const std::vector<data_type>  & vecOfElements, const data_type  & element)
{
    std::pair<bool, int > result;
    // Find given element in vector
    auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
    if (it != vecOfElements.end())
    {
        result.second = distance(vecOfElements.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}

template<typename data_type=double>
data_type NM::sum(const std::vector<data_type>& data_){
    data_type res = 0.;
    for(auto& n:data_){
        res += n;
    }
    return res;
}

///takes last pointer instead of first to facilitate the use in the ring buffer
template<typename data_type=double>
data_type sum(data_type* data_, int size){
    data_type res = 0.;
    double* data_arr = data_ - size + 1;
    for(unsigned i=0; i<size; i++){
        res+= data_arr[i];
    }
    return res;
}

template<typename data_type=double>
data_type NM::dot(const std::vector<data_type>& V1, const std::vector<data_type>& V2){
    assert(V1.size() == V2.size());
    data_type res = 0.;
    for(int i=0; i<res.size(); i++){
        res += V1[i]*V2[i];
    }
    return res;
}

template<typename data_type=double>
std::vector<data_type> NM::multiply(const std::vector<data_type>& V1, const std::vector<data_type>& V2, const data_type& coef){
    assert(V1.size() == V2.size());
    std::vector<data_type> res; res.resize(V1.size());
    for(int i=0; i<res.size(); i++){
        res[i] = V1[i]*V2[i]*coef;
    }
    return res;
}

template<typename data_type=double>
std::vector<data_type> NM::multiply(const std::vector<data_type>& V1, const data_type& coef){
    std::vector<data_type> res; res.resize(V1.size());
    for(int i=0; i<res.size(); i++){
        res[i] = V1[i]*coef;
    }
    return res;
}

template<typename data_type=double>
std::vector<data_type> NM::add(const std::vector<data_type>& V1, const std::vector<data_type>& V2){
    assert(V1.size() == V2.size());
    std::vector<data_type> res; res.resize(V1.size());
    for(int i=0; i<res.size(); i++){
        res[i] = V1[i]+V2[i];
    }
    return res;
}

template<typename data_type=double>
std::vector<data_type> NM::add(const std::vector<data_type>& V1, data_type offset){
    std::vector<data_type> res; res.resize(V1.size());
    for(int i=0; i<res.size(); i++){
        res[i] = V1[i]+offset;
    }
    return res;
}



template<typename data_type=double>
unsigned int NM::arg_max(const std::vector<data_type>& data_){
    return std::distance(data_.begin(), std::max_element(data_.begin(), data_.end()));
}



template<typename data_type = double>
std::map<data_type, unsigned int> NM::occurrences(const std::vector<data_type>& data_){
    std::map<data_type,unsigned int> oc;
    for(auto& e:NM::unique(data_, "<"))
        oc[e] = 0;

    for(auto& e:data_)
        oc[e]+=1;

    return oc;

}



template<typename data_type=double>
std::vector<data_type> NM::unique(const std::vector<data_type>& data_, const std::string& criteria ){
    std::vector<data_type> data = data_;
    if(criteria == ">") std::sort(data.begin(), data.end(), std::greater<>());
    else if(criteria == "<") std::sort(data.begin(), data.end(), std::less<>());
    else if(criteria == ">=") std::sort(data.begin(), data.end(), std::greater_equal<>());
    else if(criteria == "<") std::sort(data.begin(), data.end(), std::less_equal<>());
    else if(criteria == "==") std::sort(data.begin(), data.end(), std::equal_to<>());
    else throw NM::NMError("the criteria provided to NM::Unique isn't valid\n");
    return data;
}


template<typename data_type = double, typename func_ret=double, typename func_param=double,
        typename func_type=std::function<func_ret(func_param&)>>
std::vector<func_ret> NM::apply_mask(const std::vector<data_type>& data_, func_type&& f_){
    std::vector<func_ret> ret; ret.resize(data_.size());
    for(int i = 0; i<data_.size(); i++){
        ret[i] = std::forward<func_type>(f_)(data_[i]);
    }
    return ret;
}

template<typename data_type = double, typename func_ret=double, typename func_param=double,
        typename func_type=std::function<func_ret(const func_param&)>>
std::vector<func_ret> NM::apply_mask(data_type* data_, unsigned size, func_type&& f_){
    std::vector<func_ret> ret; ret.resize(size);
    for(int i = 0; i<size; i++){
        ret[i] = std::forward<func_type>(f_)(data_[i]);
    }
    return ret;
}


/// according to numpy np.argwhere() is equivalent to np.transpose(np.nonzero()), in this case f_ is more generic
///only works with 1D arrays
template<typename data_type = double, typename func_type=std::function<bool(const data_type&)>>
std::vector<unsigned int> NM::argwhere_index(const std::vector<data_type>& data_, func_type&& f_){
    std::vector<unsigned int> ret;
    for (int i = 0; i < data_.size(); i++) {
        if(f_(data_[i])) ret.push_back(i);
    }
    return ret;
}

/// f_ must be a funtion returning a boolean value.. aka a mask
///only works with 1D arrays and return a bool vector
template<typename data_type = double, typename func_type=std::function<bool(const data_type&)>>
std::vector<bool> NM::argwhere_bool(const std::vector<data_type>& data_, func_type&& f_){
    std::vector<bool> ret; ret.resize(data_.size());
    for(int i = 0; i<data_.size(); i++){
        ret[i] = std::forward<func_type>(f_)(data_[i]);
    }
    return ret;
}

template<typename data_type = double>
std::vector<bool> NM::nonzero_bool(const std::vector<data_type>& data_) {
    std::vector<bool> ret;
    ret.resize(data_.size());
    for (int i = 0; i < data_.size(); i++) {
        if (data_[i]){
            ret[i] = true;
        }
        else ret[i] = false;
    }
    return ret;
}

template<typename data_type= double>
std::vector<unsigned int> NM::nonzero_index(const std::vector<data_type>& data_){
    std::vector<unsigned int> ret;
    for (unsigned int i = 0; i < data_.size(); i++) {
        if(data_[i]) ret.push_back(i);
    }
    return ret;
}

template<typename data_type= double>
std::vector<unsigned int> NM::zero_index(const std::vector<data_type>& data_){
    std::vector<unsigned int> ret;
    for (unsigned int i = 0; i < data_.size(); i++) {
        if(!data_[i]) ret.push_back(i);
    }
    return ret;
}

template<typename data_type = double>
std::vector<int> NM::sign(const std::vector<data_type>& data_){
    std::vector<int> ret; ret.resize(data_.size());
    for(int i=0; i< data_.size(); i++){
        if(data_[i] > 0) ret[i] = 1;
        else if(data_[i]  < 0) ret[i] = -1;
        else ret[i] = 0;
    }
    return ret;
}


template<typename data_type = double>
auto NM::diff(const std::vector<data_type>& data_){
    std::vector<data_type> ret;
    if (data_.size() > 0){
        ret.resize(data_.size() -1);
    for(int i = 0; i<ret.size(); i++)
        ret[i] = data_[i+1] - data_[i];
    }

    return ret;

}

template<typename data_type = double>
auto NM::diff(const std::vector<data_type>& data_1, const std::vector<data_type>& data_2){
    assert(data_1.size() == data_2.size());
    std::vector<data_type> ret; ret.resize(data_1.size());
    for(int i = 0; i<ret.size(); i++){
        ret[i] = data_1[i] - data_2[i];
    }
    return ret;
}


template<typename data_type= double>
data_type NM::mean(const std::vector<data_type>& data_){
    data_type m = 0.0;
    for(auto& e:data_){
        m = m+e; //no += for compatibility with NM::COMPLEX
    }
    m = m/data_.size(); //no /= for compatibility with NM::COMPLEX
    return m;
}

template<typename data_type= double>
data_type NM::mean(data_type* data_, unsigned size){
    data_type m = 0.0;
    for(int i =0; i<size; i++){
        m+= data_[i];
    }
    m/= size;
    return m;
}


template<typename data_type= double>
data_type NM::mm_mean(const std::vector<data_type>& data_){
    return (*std::max_element(data_.begin(), data_.end()) + *std::min_element(data_.begin(), data_.end()))/2;
}

std::vector<double> NM::sin_(float duration, float amplitude, float frequency, float phase, float resolution){
    /// f(t) = amplitude*sin(2*pi*frequency*t + phase)
    if (duration==0.)
        return {};
    double t = 0.;
    int size = std::rint(duration/resolution);
    std::vector<double> _array; _array.resize(size);
    for(auto i=0; i<size; i++){
        _array[i] = amplitude * sin(2* NM::PI * frequency *t + phase*NM::PI);
        t+= resolution;
        }
    return _array;
}


std::vector<double> NM::cos_(float duration, float amplitude, float frequency, float phase, float resolution){
    /// f(t) = amplitude*sin(2*pi*frequency*t + phase)
    if (duration==0.)
        return {};
    double t = 0.;
    int size = std::rint(duration/resolution);
    std::vector<double> _array; _array.resize(size);
    for(auto i=0; i<size; i++){
        _array[i] = amplitude * cos(2* NM::PI * frequency *t + phase*NM::PI);
        t += resolution;
    }
    return _array;
}


/// a seed needs to be planted once before using this function
template<typename data_type = double>
auto NM::random_(int size, data_type min_value, data_type max_value){
    std::vector<data_type> _array; _array.resize(size);
    for(auto& e:_array){
        e = (data_type) min_value + static_cast <data_type> (rand()) /( static_cast <data_type> (RAND_MAX/(max_value-min_value)));
    }
    return _array;
}

//void NM::plant_a_seed(){
//    srand (static_cast <unsigned> (std::time(0)));
//}

#endif //HZDR_TASLER_TP_NUMERICAL_MAGIC_H

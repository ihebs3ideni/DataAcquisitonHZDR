//
// Created by iheb on 5/20/2021.
//

#ifndef HZDR_TASLER_TP_COILS_H
#define HZDR_TASLER_TP_COILS_H

#include "../Libraries/Numerical_Magic/Numerical_magic.h"
#include "spline.h"

class PhaseCorrection{
public:
    PhaseCorrection(){}
    PhaseCorrection(const std::vector<double>& f, const std::vector<double>& delta_phi){
    spline.set_points(f, delta_phi);
    }
    PhaseCorrection(const double& f, const double& delta_phi){
        spline.set_points(std::vector<double>{f}, std::vector<double>{delta_phi});
    }

    std::vector<double> correct(const std::vector<double>& f){
        std::vector<double> res; res.reserve(f.size());
        for(auto& f_:f){
            res.emplace_back(spline(f_));
        }
        return res;
    }

    double correct(double f){
        return spline(f);
    }
private:
    tk::spline spline;
};


class HallSensor{
public:
    HallSensor(){}

    std::vector<double> calculateAmplitudeCorrection(int size){
        return std::vector<double>(size, 1.0);
    }

    std::vector<double> calculatePhaseCorrection(int size){
        return std::vector<double>(size, NM::PI/2.);
    }
};


class Coil{
public:
    Coil(const double& vToB_): vToB(vToB_){    }
    Coil(double&& vToB_) : vToB(std::forward<double>(vToB_)){}
    //todo::check if emplaceabale, take name out
//    Coil(Coil<T>&& coil_factors) noexcept : vToB(std::move(coil_factors.vToB))
//    {
//    }

//    virtual vector<NM::COMPLEX> _transferFunction(const vector<double>& w) = 0;
    virtual NM::COMPLEX _transferFunction(double w) = 0;

//    vector<double> calculateAmplitudeCorrection(const std::vector<double>& f){
//        vector<double> w = NM::apply_mask<double, double, double>(f, [&](double& f_) -> double{return 2*NM::PI*f_; });
//        return NM::apply_mask<NM::COMPLEX, double,NM::COMPLEX>(this->_transferFunction(w), [&](NM::COMPLEX& e) -> double{return 1 / e.abs_(); });
//    }

    double calculateAmplitudeCorrection(double f){
//        double w = 2*NM::PI*f;
        return 1/_transferFunction(2*NM::PI*f).abs_();

    }

//    vector<double> calculatePhaseCorrection(const std::vector<double>& f){
//        auto w = NM::apply_mask<double, double, double>(f, [&](double& f_) -> double{return 2*NM::PI*f_; });
//        return NM::apply_mask<NM::COMPLEX, double,NM::COMPLEX>(this->_transferFunction(w), [&](NM::COMPLEX& e) -> double{return e.arg_(); });
//    }

    double calculatePhaseCorrection(double f){
//        double w = 2*NM::PI*f;
        return _transferFunction(2*NM::PI*f).arg_();

    }

    double& get_vToB(){

        return vToB;
    }


protected:
    double vToB;
};

class PerfectCoil: public Coil{
//    vector<NM::COMPLEX> _transferFunction(const vector<double>& w){
//        return vector<NM::COMPLEX>(w.size(), NM::COMPLEX(1.0));
//    }

    NM::COMPLEX _transferFunction(double w){
        return NM::COMPLEX(1.0);
    }
};

class SimpleCoil: public Coil{
public:
    SimpleCoil(const double& vToB_,const  double& R_,const  double& L_,const  double& C_):
    Coil(vToB_), R(R_), L(L_), C(C_)
    {}

    SimpleCoil(double&& vToB_,double&& R_,double&& L_,double&& C_):
    Coil(vToB_), R(std::forward<double>(R_)), L(std::forward<double>(L_)), C(std::forward<double>(C_))
    {}
    //todo::make emplaceabale

//    vector<NM::COMPLEX> _transferFunction(const vector<double>& w){
//        std::vector<NM::COMPLEX> c_w; c_w.reserve(w.size());
//        for(auto& w_:w){
//            c_w.emplace_back(NM::COMPLEX(w_));
//        }
//        auto numerator = NM::apply_mask<NM::COMPLEX,NM::COMPLEX, NM::COMPLEX>(c_w, [&](NM::COMPLEX& _w) -> NM::COMPLEX{
//                        return 1/(NM::j*_w*C);});
//        auto denomerator = NM::apply_mask<NM::COMPLEX, NM::COMPLEX, NM::COMPLEX>(c_w, [&](NM::COMPLEX& _w) -> NM::COMPLEX{
//            return (1/(NM::j*_w*C)) + (NM::j*_w*L)+R;});
//
//        std::vector<NM::COMPLEX> res; res.reserve(w.size());
//        for(int i=0; i<w.size(); i++){
//            res.emplace_back(numerator[i]/denomerator[i]);
//        }
//        return res;
//    }

    NM::COMPLEX _transferFunction(double w){
        NM::COMPLEX numerator = 1./(NM::j*w*C);
        NM::COMPLEX denominator = R+ (NM::j*w*L) + (1./(NM::j*w*C));
        return numerator/denominator;
    }


    double R,L,C;
};

class AbsoluteCoil: public SimpleCoil{
public:
    AbsoluteCoil(const double& vToB_,const  double& Rs,const  double& Ls,const  double& Cs,
                 const  double& Rc_,const  double& Cp_,const  double& Rin_):
            SimpleCoil(vToB_, Rs, Ls, Cs), Rc(Rc_), Cp(Cp_), Rin(Rin_){}


    AbsoluteCoil(const double& vToB_,const  double& Rs,const  double& Ls,const  double& Cs,
                 const  double& Rc_,const  double& Cp_,const  double& Rin_,
                 const std::vector<double>& f_, const std::vector<double>& delta_phi):
    SimpleCoil(vToB_, Rs, Ls, Cs), Rc(Rc_), Cp(Cp_), Rin(Rin_)
    {
            phase_correction = new PhaseCorrection(f_, delta_phi);

    }


    AbsoluteCoil(const double& vToB_,const  double& Rs,const  double& Ls,const  double& Cs,
                 const  double& Rc_,const  double& Cp_,const  double& Rin_,
                 double f_, double delta_phi_):
            SimpleCoil(vToB_, Rs, Ls, Cs), Rc(Rc_), Cp(Cp_), Rin(Rin_)
    {
            phase_correction = new PhaseCorrection(f_, delta_phi_);

    }


    AbsoluteCoil(double&& vToB_,double&& Rs,double&& Ls,double&& Cs,
                 double&& Rc_,double&& Cp_,double&& Rin_,
                 std::vector<double>&& f_, std::vector<double>&& delta_phi):
            SimpleCoil(vToB_, Rs, Ls, Cs), Rc(std::forward<double>(Rc_)), Cp(std::forward<double>(Cp_)),
            Rin(std::forward<double>(Rin_)){
        if(f_.size()>0)
            phase_correction = new PhaseCorrection(f_, delta_phi);
    }

    AbsoluteCoil(double&& vToB_,double&& Rs,double&& Ls,double&& Cs,
                 double&& Rc_,double&& Cp_,double&& Rin_,
                 double&& f_, double&& delta_phi_):
            SimpleCoil(vToB_, Rs, Ls, Cs), Rc(std::forward<double>(Rc_)), Cp(std::forward<double>(Cp_)),
            Rin(std::forward<double>(Rin_))
    {
        if(f_>0)
            phase_correction = new PhaseCorrection(f_, delta_phi_);

    }

    //todo::make emplaceabale


    ~AbsoluteCoil(){
        delete phase_correction;
    }

//    vector<NM::COMPLEX> _transferFunction(const vector<double>& w){
//        std::vector<NM::COMPLEX> c_w; c_w.reserve(w.size());
//        for(auto& w_:w){
//            c_w.emplace_back(NM::COMPLEX(w_));
//        }
//        std::vector<NM::COMPLEX> numerator  = NM::apply_mask<NM::COMPLEX, NM::COMPLEX, NM::COMPLEX>(c_w, [&](const NM::COMPLEX& _w) -> NM::COMPLEX{
//            return 1./(NM::j*_w*(C+Cp)+1./Rin);});
//        auto v1 = NM::apply_mask<NM::COMPLEX, NM::COMPLEX, NM::COMPLEX>(c_w, [&](const NM::COMPLEX& _w) -> NM::COMPLEX{
//            return R + (NM::j*_w*L);});
//        std::vector<NM::COMPLEX> denominator = NM::add<NM::COMPLEX>( v1, numerator);
//        std::vector<NM::COMPLEX> res; res.reserve(w.size());
//        for(int i=0; i<w.size(); i++){
//            res.emplace_back(numerator[i]/denominator[i]);
//        }
//        return res;
//    }

    NM::COMPLEX _transferFunction(double w){
        NM::COMPLEX numerator = 1./(NM::j*w*(C+Cp)+1./Rin);
        NM::COMPLEX denominator = R+NM::j*w*L+numerator;
        return numerator/denominator;
    }

//    std::vector<double> calculatePhaseCorrection(const std::vector<double>& f){
//        if (phase_correction)
//            return NM::add(SimpleCoil::calculatePhaseCorrection(f), phase_correction->correct(f));
//        return SimpleCoil::calculatePhaseCorrection(f);
//    }

    double calculatePhaseCorrection(double f){
        if (phase_correction)
            return SimpleCoil::calculatePhaseCorrection(f) + phase_correction->correct(f);
        return SimpleCoil::calculatePhaseCorrection(f);
    }

private:
    double Rc, Cp, Rin;
    PhaseCorrection* phase_correction = nullptr;
};


class GradiometricCoil: public SimpleCoil{
public:
    GradiometricCoil(const double& vToB_,const  double& R_,const  double& L_,const  double& C_,
                 const  double& km_,const  double& Rin_):
            SimpleCoil(vToB_, R_, L_, C_), km(km_), Rin(Rin_)
    {
        M = km*L;

    }
    //todo: make emplaceabale

    NM::COMPLEX _transferFunction(double w){
        auto impedance = _calculate_impedances(w);
        NM::COMPLEX tf_own =_calculate_own(impedance["Zs"], impedance["Za"], impedance["Zg"]);
        NM::COMPLEX tf_other =_calculate_other(impedance["Zs"], impedance["Za"], impedance["Zg"], impedance["Z"]);
        return tf_own+tf_other;
    }


    std::unordered_map<std::string, NM::COMPLEX> _calculate_impedances(double w){
        std::unordered_map<std::string, NM::COMPLEX> res;
        res["Zm"] = NM::j*w*M;
        res["Za"] = _parallel(Rin, (1./(NM::j*w*C)));
        res["Zs"] = res["Za"]+R+NM::j*w*(L-M);
        res["Zg"] = _parallel(res["Zs"], res["Zm"]);
        res["Z"] = res["Zs"]+res["Zg"];
        return res;
    }


    NM::COMPLEX _parallel(const NM::COMPLEX& z1, const NM::COMPLEX& z2){
        return (z1*z2)/(z1+z2);
    }

    NM::COMPLEX _calculate_own(const NM::COMPLEX& Zs, const NM::COMPLEX& Za, const NM::COMPLEX& Zg){
        return Za/(Zs+Zg);
    }

    NM::COMPLEX _calculate_other(const NM::COMPLEX& Zs, const NM::COMPLEX& Za, const NM::COMPLEX& Zg,
            const NM::COMPLEX& Z)
    {
        return (Zg*Za)/(Z*Zs);
    }

private:
    double Rin, km;
    double M;
};



#include <memory>
const std::unordered_map<std::string, std::shared_ptr<Coil>> Coils_MAP = {
        {"340K_low_impedance", std::shared_ptr<Coil>(new AbsoluteCoil(510.,600e3,463.417, 412.41e-12, 1e12, 50e-12, 1e6))},
        {"340K_high_impedance", std::shared_ptr<Coil>(new AbsoluteCoil(510.,600e3,463.417, 412.41e-12, 1e12, 50e-12, 1e12))},
        {"34K_high_impedance", std::shared_ptr<Coil>(new AbsoluteCoil(531.,25030,38.5210, 377.55e-12, 1e12, 50e-12, 1e12))},
        {"160K_high_impedance", std::shared_ptr<Coil>(new AbsoluteCoil(240.,273e3,122.79, 341.75e-12, 1e12, 0e-12, 1e12))},
        {"2x160K_low_impedance", std::shared_ptr<Coil>(new GradiometricCoil(240.,274.9e3,71.690, 505.49e-12, -0.549, 1e6))},
        {"2x160K_high_impedance", std::shared_ptr<Coil>(new GradiometricCoil(240.,274.9e3,71.690, 505.49e-12, -0.549, 1e12))}
};

//void clean_up_coil_map(){
//    for(auto& coil:Coils_MAP)
//        delete coil.second;
//}

//data_Structure used for each channel to store the correction factors
template<typename data_type=double>
struct COIL_FACTORS{
    data_type amplitude_correction;
    data_type phase_correction;
    data_type vToB; //initial vToB*frequency
};

#endif //HZDR_TASLER_TP_COILS_H

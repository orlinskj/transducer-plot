#include "solver.h"

//#include <boost/math/constants/constants.hpp>

bool BVDSolver::solve(const Transducer& t, boost::optional<SolverType> input_capacity)
{
    // getting frequency set
    auto freq_unit = Unit::from_symbol("f");
    auto freq_set = t.get_set(freq_unit);
    if (!freq_set)
        throw std::runtime_error("Transducer does not contain frequency data set.");

    // getting impedance set
    auto impedance_unit = Unit::from_symbol("|Z|");
    auto impedance_set = t.get_set(impedance_unit);
    if (!impedance_set)
        throw std::runtime_error("Transducer does not contain impedance magnitude data set.");

    // getting impedance imaginary set
    auto imaginary_unit = Unit::from_symbol("Im");
    auto imaginary_set = t.get_set(imaginary_unit);
    if (!imaginary_set)
        throw std::runtime_error("Transducer does not contain impedance imaginary fraction data set.");

    // calculating resonance frequencies
    // looking for min and max impedance - min is for series, max is for parallel resonance
    SolverType fs_index = std::get<1>(impedance_set->max());
    SolverType fp_index = std::get<1>(impedance_set->min());

    //SolverType pi = boost::math::constants::pi<SolverType>();
    SolverType pi = 3.14159;

    SolverType fs = freq_set->values().at(fs_index);
    SolverType fp = freq_set->values().at(fp_index);
    SolverType ws = 2 * pi * fs;
    SolverType wp = 2 * pi * fp;

    // calculating transducer input capacity
    if (input_capacity)
        Cop_ = *input_capacity;
    else
    {
        SolverType min_f = freq_set->values().at(0);
        SolverType min_f_Im = imaginary_set->values().at(0);
        SolverType min_w = 2.0 * M_PI * min_f;

        Cop_ = 1.0 / (min_w * min_f_Im);
    }

    SolverType f_ratio = fs_ / fp_;
    Cos_ = f_ratio*f_ratio * Cop_;

    // calculating R
    Rs_ = impedance_set->values().at(fs_index);
    Rp_ = impedance_set->values().at(fp_index);

    // calculating C
    Cs_ = Cos_ * (fp_*fp_-fs_*fs_)/(fs_*fs_);
    Cp_ = Cop_ * (fp_*fp_)/(fp_*fp_-fs_*fs_);

    // calculating L
    Ls_ = 1.0 / (ws*ws*Cs_);
    Lp_ = 1.0 / (wp*wp*Cp_);

    // calculating Q
    Qs_ = 1.0 / Rs_ * std::sqrt(Ls_/Cs_);
    Qp_ = Rp_ * std::sqrt(Cp_/Lp_);
    QQ_ = std::sqrt(Qs_*Qp_);

    // calculating coefficients
    //SolverType half_pi = boost::math::constants::half_pi<SolverType>();
    SolverType half_pi = 1.570796;
    k33_ = std::sqrt(half_pi*fs_*fp_*std::tan((fp_-fs_)/fp_*half_pi));
    keff_ = std::sqrt((fp_*fp_-fs_*fs_)/(fs_*fs_));
    k_ = (keff_/k33_)*(keff_/k33_);

    return true;
}

SolverType BVDSolver::Co(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return Cos_;
    else
        return Cop_;
}

SolverType BVDSolver::R(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return Rs_;
    else
        return Rp_;
}

SolverType BVDSolver::L(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return Ls_;
    else
        return Lp_;
}

SolverType BVDSolver::C(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return Cs_;
    else
        return Cp_;
}

SolverType BVDSolver::f(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return fs_;
    else
        return fp_;
}

SolverType BVDSolver::Q(BVDSolver::Type type) const
{
    if (type == BVDSolver::Type::Series)
        return Qs_;
    else
        return Qp_;
}

SolverType BVDSolver::QQ() const { return QQ_; }
SolverType BVDSolver::keff() const { return keff_; }
SolverType BVDSolver::k() const { return k_; }
SolverType BVDSolver::k33() const { return k33_; }

#ifndef SOLVER_H
#define SOLVER_H

#define _USE_MATH_DEFINES // msvc2015 needs this
#include <cmath>
#include <utility>
#include "transducer.h"

using SolverType = double;

class BVDSolver
{
public:
    enum class Type{
        Series,
        Parallel
    };

    bool solve(const Transducer& t, const std::pair<bool, SolverType> &input_capacity);

    SolverType Co(Type type) const;
    SolverType R(Type type) const;
    SolverType L(Type type) const;
    SolverType C(Type type) const;
    SolverType Q(Type type) const;
    SolverType f(Type type) const;
    SolverType Z(Type type) const;

    SolverType QQ() const;
    SolverType k33() const;
    SolverType keff() const;
    SolverType k() const;

protected:
    SolverType Cos_, Cop_;
    SolverType Rs_, Rp_;
    SolverType Ls_, Lp_;
    SolverType Cs_, Cp_;
    SolverType fs_, fp_;
    SolverType Zs_, Zp_;
    SolverType Qs_, Qp_, QQ_;
    SolverType keff_, k33_, k_;

};

#endif // SOLVER_H

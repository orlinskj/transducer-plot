#ifndef SOLVER_H
#define SOLVER_H

#include <cmath>
#include <boost/optional.hpp>
#include "transducer.h"

using SolverType = double;

class Solver
{
public:
    virtual bool solve(const Transducer& t) = 0;
};

class BVDSolver : public Solver
{
public:
    bool solve(const Transducer& t);
    bool solve(const Transducer& t, boost::optional<SolverType> Co);

    SolverType Co() const;
    SolverType R() const;
    SolverType L() const;
    SolverType C() const;
protected:
    SolverType Co_, R_, L_, C_;


};

#endif // SOLVER_H

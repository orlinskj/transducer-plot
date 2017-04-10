#include "solver.h"

SolverType BVDSolver::Co() const
{
    return Co_;
}

SolverType BVDSolver::R() const
{
    return R_;
}

SolverType BVDSolver::L() const
{
    return L_;
}

SolverType BVDSolver::C() const
{
    return C_;
}

BVDSeriesSolver::BVDSeriesSolver(const Transducer& t)
{
    this->solve(t);
}

bool BVDSeriesSolver::solve(const Transducer &t)
{
    SolverType Co, R, L, C;
    return false;
}

BVDParallelSolver::BVDParallelSolver(const Transducer& t)
{
    this->solve(t);
}

bool BVDParallelSolver::solve(const Transducer &t)
{
    SolverType Co, R, L, C;
    return false;
}

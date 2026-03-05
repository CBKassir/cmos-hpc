#pragma once
#include "Circuit.hpp"
#include <Eigen/SparseLU>
#include <iostream>

class NewtonSolver {
public:
    NewtonSolver(double tol = 1e-9, int maxIter = 50)
        : tol_(tol), maxIter_(maxIter) {}

    bool solve(const Circuit& circuit,
               Eigen::VectorXd& x) const
    {
        for (int k = 0; k < maxIter_; ++k) {
            SparseMatrix J;
            Eigen::VectorXd F(x.size());

            circuit.assemble(x, J, F);

            if (F.norm() < tol_) {
                std::cout << "Converged in "
                          << k << " iterations\n";
                return true;
            }

            Eigen::SparseLU<SparseMatrix> solver;
            solver.compute(J);

            if (solver.info() != Eigen::Success)
                return false;

            Eigen::VectorXd dx = solver.solve(-F);
            x += dx;

            if (dx.norm() < tol_)
                return true;
        }
        return false;
    }

private:
    double tol_;
    int maxIter_;
};
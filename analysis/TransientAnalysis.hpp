#pragma once
#include "../core/Solver.hpp"

class DCAnalysis {
public:
    DCAnalysis(const Circuit& circuit)
        : circuit_(circuit) {}

    bool run(Eigen::VectorXd& x) {
        return solver_.solve(circuit_, x);
    }

private:
    const Circuit& circuit_;
    NewtonSolver solver_;
};
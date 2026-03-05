#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

using SparseMatrix = Eigen::SparseMatrix<double>;
using Triplet = Eigen::Triplet<double>;

class Device {
public:
    virtual ~Device() = default;

    virtual void stamp(
        const Eigen::VectorXd& x,
        std::vector<Triplet>& triplets,
        Eigen::VectorXd& F
    ) const = 0;
};
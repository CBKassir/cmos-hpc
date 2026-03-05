#pragma once
#include "Device.hpp"
#include <memory>

class Circuit {
public:
    Circuit(int nodes) : nodes_(nodes) {}

    void addDevice(std::unique_ptr<Device> device) {
        devices_.push_back(std::move(device));
    }

    int nodeCount() const { return nodes_; }

    void assemble(const Eigen::VectorXd& x,
                  SparseMatrix& J,
                  Eigen::VectorXd& F) const
    {
        std::vector<Triplet> T;
        F.setZero();

        for (const auto& dev : devices_)
            dev->stamp(x, T, F);

        J.resize(nodes_, nodes_);
        J.setFromTriplets(T.begin(), T.end());
    }

private:
    int nodes_;
    std::vector<std::unique_ptr<Device>> devices_;
};
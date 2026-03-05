#pragma once
#include "../core/Device.hpp"

class Resistor : public Device {
public:
    Resistor(int n1, int n2, double value)
        : n1_(n1), n2_(n2), value_(value) {}

    void stamp(const Eigen::VectorXd&,
               std::vector<Triplet>& T,
               Eigen::VectorXd&) const override
    {
        double g = 1.0 / value_;

        T.emplace_back(n1_, n1_, g);
        T.emplace_back(n2_, n2_, g);
        T.emplace_back(n1_, n2_, -g);
        T.emplace_back(n2_, n1_, -g);
    }

private:
    int n1_, n2_;
    double value_;
};
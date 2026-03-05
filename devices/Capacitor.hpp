#pragma once
#include "../core/Device.hpp"

class Capacitor : public Device {
public:
    Capacitor(int n1, int n2, double value)
        : n1_(n1), n2_(n2), value_(value) {}

    void stampTransient(
        const Eigen::VectorXd& x,
        const Eigen::VectorXd& x_prev,
        double dt,
        std::vector<Triplet>& T,
        Eigen::VectorXd& F) const
    {
        double g = value_ / dt;

        T.emplace_back(n1_, n1_, g);
        T.emplace_back(n2_, n2_, g);
        T.emplace_back(n1_, n2_, -g);
        T.emplace_back(n2_, n1_, -g);

        double Ieq = g * (x_prev[n1_] - x_prev[n2_]);

        F[n1_] += Ieq;
        F[n2_] -= Ieq;
    }

private:
    int n1_, n2_;
    double value_;
};
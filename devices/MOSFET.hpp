#pragma once
#include "../core/Device.hpp"
#include <cmath>

class MOSFET : public Device {
public:
    MOSFET(int d, int g, int s,
           double beta,
           double vth,
           double lambda,
           double n,
           double I0)
        : d_(d), g_(g), s_(s),
          beta_(beta), vth_(vth),
          lambda_(lambda), n_(n), I0_(I0) {}

    void stamp(const Eigen::VectorXd& x,
               std::vector<Triplet>& T,
               Eigen::VectorXd& F) const override
    {
        double Vd = x[d_];
        double Vg = x[g_];
        double Vs = x[s_];

        double Vgs = Vg - Vs;
        double Vds = Vd - Vs;

        const double Vt = 0.02585;

        double Id, gm, gds;

        if (Vgs > vth_) {
            Id = 0.5 * beta_ * pow(Vgs - vth_,2) * (1 + lambda_ * Vds);
            gm = beta_ * (Vgs - vth_) * (1 + lambda_ * Vds);
            gds = 0.5 * beta_ * pow(Vgs - vth_,2) * lambda_;
        } else {
            Id = I0_ * exp((Vgs - vth_) / (n_ * Vt))
                 * (1 - exp(-Vds / Vt));

            gm = Id / (n_ * Vt);
            gds = Id / Vt;
        }

        F[d_] += Id;
        F[s_] -= Id;

        T.emplace_back(d_, d_, gds);
        T.emplace_back(d_, g_, gm);
        T.emplace_back(d_, s_, -gm - gds);

        T.emplace_back(s_, d_, -gds);
        T.emplace_back(s_, g_, -gm);
        T.emplace_back(s_, s_, gm + gds);
    }

private:
    int d_, g_, s_;
    double beta_, vth_, lambda_, n_, I0_;
};
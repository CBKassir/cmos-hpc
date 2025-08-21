#include "nmos.h"
#include <iostream>
#include <cmath>

NMOS::NMOS(double v_G_, double v_D_, double v_S_, double v_B_, double i_D_,
           double V_TH0_,
           double gamma_, double phi_f_,
           double lambda_,
           double mu_n_, double C_ox_, double W_, double L_,
           bool v_GS_update_ = 1, bool v_DS_update_ = 1, bool i_D_update_ = 1
    )
    : v_G(v_G_), v_D(v_D_), v_S(v_S_), v_B(v_B_), i_D(i_D_),
      V_TH0(V_TH0_), gamma(gamma_), phi_f(phi_f_), lambda(lambda_),
      mu_n(mu_n_), C_ox(C_ox_), W(W_), L(L_),
      v_GS_update(v_GS_update_), v_DS_update(v_DS_update_), i_D_update(i_D_update_)
{
    V_TH = V_TH0 + gamma * (std::sqrt(std::abs((v_S-v_B) + 2 * phi_f)) - std::sqrt(2 * phi_f));
    K = mu_n * C_ox * W/L;

    int code = (v_GS_update_ << 2) | (v_DS_update << 1) | i_D_update;
    VoidFn make_update_iD_triode();
    VoidFn make_update_vDS_triode();
    VoidFn make_update_vGS_triode();




    switch (code) {
        case 0b000:
        update_cutoff = [this]() {
            std::cout << "Nothing updates.\n";
        };
        case 0b001:
        case 0b010:
        case 0b011:
        case 0b100:
        case 0b101:
        case 0b110:
        case 0b111:
    }
}

extern "C" double lambertw(double k);


NMOS::VoidFn NMOS::combine(VoidFn a, VoidFn b) {
    if (!a) return b;
    if (!b) return a;
    return [a,b]() {
        a();
        b();
    };
}


NMOS::VoidFn NMOS::update_i_D_cutoff() {
    return [this]() {
        double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult;
        double I_0 = K * ((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS /
                    std::exp(V_min_assumed / (zeta * V_TH));
        i_D = I_0 * std::exp(v_GS / (zeta * V_TH));
    };
}
NMOS::VoidFn NMOS::update_v_DS_cutoff() {
    return [this]() {
        double dummy = i_D * std::exp(V_TH_toler_mult / 2) /
            (2 * zeta * V_TH * V_TH_toler_mult * K *
            std::exp(v_GS / (zeta * V_TH)) * V_TH * (V_TH_toler_mult - 1));
        v_DS = -lambertw(-dummy) * 2 * zeta * V_TH;
    };
}
NMOS::VoidFn NMOS::update_v_GS_cutoff() {
    return [this]() {
        double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult;
        double I_0 = K * ((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS /
            std::exp(V_min_assumed / (zeta * V_TH));
        v_GS = std::log(i_D / I_0) * (zeta * V_TH);
    };
}
void NMOS::make_update_cutoff() {
    update_cutoff = []() {};

    if (i_D_update) update_cutoff = combine(update_cutoff, update_i_D_triode());
    if (v_DS_update) update_cutoff = combine(update_cutoff, update_v_DS_triode());
    if (v_GS_update) update_cutoff = combine(update_cutoff, update_v_GS_triode());
}

NMOS::VoidFn NMOS::update_i_D_triode() {
    return [this]() {
        i_D = K * ((v_GS - V_TH) - 0.5 * v_DS) * v_DS;
    };
}
NMOS::VoidFn NMOS::update_v_DS_triode() {
    return [this]() {
        v_DS = (v_GS - V_TH) - std::sqrt(v_GS - V_TH);
    };
}
NMOS::VoidFn NMOS::update_v_GS_triode() {
    return [this]() {
        v_GS = (i_D / v_DS + v_DS / 2 + V_TH) / K;
    };
}
void NMOS::make_update_triode() {
    update_triode = []() {};

    if (i_D_update) update_triode = combine(update_triode, update_i_D_triode());
    if (v_DS_update) update_triode = combine(update_triode, update_v_DS_triode());
    if (v_GS_update) update_triode = combine(update_triode, update_v_GS_triode());
}

NMOS::VoidFn NMOS::update_i_D_saturation() {
    return [this]() {
        i_D = K * 0.5 * (v_GS - V_TH)*(v_GS - V_TH) * (1 + lambda * v_DS); 
    };
}
NMOS::VoidFn NMOS::update_v_DS_saturation() {
    return [this]() {
        v_DS = 2 * i_D / (K * (v_GS - V_TH)*(v_GS - V_TH) * lambda) - 1/lambda;
    };
}
NMOS::VoidFn NMOS::update_v_GS_saturation() {
    return [this]() {
        v_GS = V_TH + std::sqrt(2*i_D / (K * (1 + lambda * v_DS)));
    };
}
void NMOS::make_update_saturation() {
    update_saturation = []() {};

    if (i_D_update) update_saturation = combine(update_saturation, update_i_D_saturation());
    if (v_DS_update) update_saturation = combine(update_saturation, update_v_DS_saturation());
    if (v_GS_update) update_saturation = combine(update_saturation, update_v_GS_saturation());
}


void NMOS::update_state_vars() {
    double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult;

    if (v_GS < V_min_assumed) {
        update_cutoff();
    } else if (v_DS < (v_GS-V_TH)) {
        update_triode();
    } else {
        update_saturation();
    }
}

void NMOS::solve_state_vars() {
    for (int i=0; i<MAX_FORWARD_ITERATIONS; i++) {
        update_state_vars();
    }
}


/*
--- New plan ---

New algorithm (*) for determining node voltages:
Full forward-pass, then CHECK WHICH NODES ARE KNOWN. And start from there.
(Maybe document the failed attempt, check in Git of course)
THIS WILL BE IN ANOTHER .CPP, since it's scope is the entire circuit

Also: there will be two uses of multithreading:
- Monte Carlo with starting node voltages
- More niche: do that new algo (*) in parallel with solve_state_vars()

*/





/// VERY OUTDATED
// also why'd it auto-add "@brief"
void NMOS::print_device_params() const {
    std::cout << "NMOS Device Parameters:\n";
    std::cout << "-------------------\n";
    std::cout << "V_TH0 = " << V_TH0 << " V\n";
    std::cout << "gamma = " << gamma << "\n";
    std::cout << "phi_f = " << phi_f << " V\n";
    std::cout << "lambda = " << lambda << "\n";
    std::cout << "mu_n = " << mu_n << " cm^2/V·s\n";
    std::cout << "C_ox = " << C_ox << " F/cm^2\n";
    std::cout << "W = " << W << " cm, L = " << L << " cm\n";
    std::cout << "Computed K = " << K << " A/V^2\n";
    std::cout << "Computed V_TH = " << V_TH << " V\n";
}

void NMOS::print_state_vars() const {
    std::cout << "NMOS State Variables:\n";
    std::cout << "-------------------\n";
    std::cout << "v_G = " << v_G << " V\n";
    std::cout << "v_D = " << v_D << " V\n";
    std::cout << "v_S = " << v_S << " V\n";
    std::cout << "v_B = " << v_B << " V\n";
    std::cout << "V_GS = " << v_G - v_S << " V\n";
    std::cout << "V_DS = " << v_D - v_S << " V\n";
    std::cout << "I_D = " << i_D << " A\n";
}
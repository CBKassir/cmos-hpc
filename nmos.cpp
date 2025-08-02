#include "nmos.h"
#include <iostream>
#include <cmath>

NMOS::NMOS(double v_G_, double v_D_, double v_S_, double v_B_,
           double i_D_,
           double V_TH0_,
           double gamma_, double phi_f_,
           double lambda_,
           double mu_n_, double C_ox_, double W_, double L_)
    : v_G(v_G_), v_D(v_D_), v_S(v_S_), v_B(v_B_), i_D(i_D_),
      V_TH0(V_TH0_), gamma(gamma_), phi_f(phi_f_), lambda(lambda_),
      mu_n(mu_n_), C_ox(C_ox_), W(W_), L(L_)
{
    V_TH = V_TH0 + gamma * (std::sqrt(std::abs((v_S-v_B) + 2 * phi_f)) - std::sqrt(2 * phi_f));
    K = mu_n * C_ox * W/L;
}

extern "C" double lambertw(double k);

NMOS::state_vars NMOS::update_return_state_vars() {
    /*
    Forward pass of state vars.
    Update order: i_D, v_DS, v_GS.
    Intra-update: static operating region.
    */

    double v_GS = v_G - v_S;
    double v_DS = v_D - v_S;

    double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult; // i.e. where the exponential subthreshold current meets the triode current

    double I_0, dummy;
    if (v_GS < V_min_assumed) { // Cutoff
        I_0 = K*((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS / 
                    std::exp(V_min_assumed / (zeta * V_TH));
        i_D = I_0 * std::exp(v_GS / (zeta * V_TH));
        double dummy = i_D * std::exp(V_TH_toler_mult/2) /
                        (2*zeta*V_TH * V_TH_toler_mult*K * std::exp(v_GS/(zeta*V_TH)) * V_TH*(V_TH_toler_mult-1));
        v_DS = -lambertw(-dummy) * 2*zeta*V_TH;
        v_GS = std::log(i_D/I_0) * (zeta * V_TH);
    } else if (v_DS < (v_GS - V_TH)) { // Triode
        i_D = K * ((v_GS - V_TH) - 0.5 * v_DS) * v_DS;
        v_DS = (v_GS - V_TH) - std::sqrt((v_GS - V_TH)); // take - of the +- quadratic since v_DS < (v_GS - V_TH)
        v_GS = (i_D/v_DS + v_DS/2 + V_TH)/K;
    } else { // Saturation
        i_D = 0.5 * K * (v_GS - V_TH)*(v_GS - V_TH) * (1 + lambda * v_DS);
        v_DS = 2 * i_D / (K * (v_GS - V_TH)*(v_GS - V_TH) * lambda) - 1/lambda;
        v_GS = V_TH + std::sqrt(2*i_D / (K * (1 + lambda * v_DS)));

    }
    
    return NMOS::state_vars{i_D, v_DS, v_GS};
}





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
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
      mu_n(mu_n_), C_ox(C_ox_), W(W_), L(L_) {}

double NMOS::compute_V_TH() const {
    double v_SB = v_S - v_B;
    return V_TH0 + gamma * (std::sqrt(std::abs(v_SB + 2 * phi_f)) - std::sqrt(2 * phi_f));
}

double NMOS::compute_K() const {
    return mu_n * C_ox * (W / L);
}



double NMOS::update_return_i_D() {
    double v_GS = v_G - v_S;
    double v_DS = v_D - v_S;
    double V_TH = compute_V_TH();
    double K = compute_K();
    
    double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult; // i.e. where the exponential subthreshold current meets the triode current

    if (v_GS < V_min_assumed) { // Cutoff
        double I_0 = K*((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS / 
                    std::exp(V_min_assumed / (zeta * V_TH));
        i_D = I_0 * std::exp(v_GS / (zeta * V_TH));
    } else if (v_DS < (v_GS - V_TH)) { // Triode
        i_D = K * ((v_GS - V_TH) - 0.5 * v_DS) * v_DS;
    } else { // Saturation
        i_D = 0.5 * K * (v_GS - V_TH)*(v_GS - V_TH) * (1 + lambda * v_DS);
    }
    return i_D;
}


double NMOS::update_return_v_DS() {
    double v_DS = v_D - v_S;
    double V_TH = compute_V_TH();
    double K = compute_K();
    double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult;
    if (v_GS < V_min_assumed) {
        v_DS = solve_cutoff_v_DS(3); // lambert W thing;
    } else if (v_DS < (v_GS - V_TH)) {
        v_DS = (v_GS - V_TH) - std::sqrt((v_GS - V_TH)); // take - of the +- quadratic since v_DS < (v_GS - V_TH)
    } else {
        v_DS = 2 * i_D / (K * (v_GS - V_TH)*(v_GS - V_TH) * lambda) - 1/lambda;
    }
    return v_DS;
}


double NMOS::update_return_v_GS() {
    double v_DS = v_D - v_S;
    double V_TH = compute_V_TH();
    double K = compute_K();
    
    double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult;
    if (v_GS < V_min_assumed) {
        double I_0 = K*((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS / 
                    std::exp(V_min_assumed / (zeta * V_TH));
        v_GS = std::log(i_D/I_0) * (zeta * V_TH);
    } else if (v_DS < (v_GS - V_TH)) {
        v_GS = i_D/v_DS + v_DS/2 + V_TH;
    } else {
        v_GS = V_TH + std::sqrt(2*i_D / (K * (1 + lambda * v_DS)));
    }
    return v_GS;
}



// double NMOS::update_return_i_D_v_DS_v_GS() {
//     double v_GS = v_G - v_S;
//     double v_DS = v_D - v_S;
//     double V_TH = compute_V_TH();
//     double K = compute_K();
    
//     double V_min_assumed = v_DS/2 + V_TH*V_TH_toler_mult; // i.e. where the exponential subthreshold current meets the triode current

//     if (v_GS < V_min_assumed) { // Cutoff
//         double I_0 = K*((V_min_assumed - V_TH) - 0.5 * v_DS) * v_DS / 
//                     std::exp(V_min_assumed / (zeta * V_TH));
//         i_D = I_0 * std::exp(v_GS / (zeta * V_TH));
//     } else if (v_DS < (v_GS - V_TH)) { // Triode
//         i_D = K * ((v_GS - V_TH) - 0.5 * v_DS) * v_DS;
//     } else { // Saturation
//         i_D = 0.5 * K * (v_GS - V_TH)*(v_GS - V_TH) * (1 + lambda * v_DS);
//     }
//     return i_D;
// }




double NMOS::node_current(int node) const {
    if (node == D_NODE) {
        return compute_i_D();
    } else if (node == S_NODE) {
        return compute_i_D();
    } else if (node == G_NODE) {
        return 0;
    } else if (node == B_NODE) {
        return compute_i_D();
    }
}

double NMOS::node_voltage(int node) const {
    if (node == D_NODE) {
        return v_D;
    } else if (node == S_NODE) {
        return compute_i_D();
    } else if (node == G_NODE) {
        return 0;
    } else if (node == B_NODE) {
        return compute_i_D();
    }
}


void NMOS::print_device_paramters() const {
    std::cout << "NMOS Device Parameters:\n";
    std::cout << "-------------------\n";
    std::cout << "V_TH0 = " << V_TH0 << " V\n";
    std::cout << "gamma = " << gamma << "\n";
    std::cout << "phi_f = " << phi_f << " V\n";
    std::cout << "lambda = " << lambda << "\n";
    std::cout << "mu_n = " << mu_n << " cm^2/V·s\n";
    std::cout << "C_ox = " << C_ox << " F/cm^2\n";
    std::cout << "W = " << W << " cm, L = " << L << " cm\n";
    std::cout << "Computed K = " << compute_K() << " A/V^2\n";
    std::cout << "Computed V_TH = " << compute_V_TH() << " V\n";
}

void NMOS::print_state_variables() const {
    std::cout << "NMOS State Variables:\n";
    std::cout << "-------------------\n";
    std::cout << "v_G = " << v_G << " V\n";
    std::cout << "v_D = " << v_D << " V\n";
    std::cout << "v_S = " << v_S << " V\n";
    std::cout << "v_B = " << v_B << " V\n";
    std::cout << "V_GS = " << compute_v_GS() << " V\n";
    std::cout << "V_DS = " << compute_v_DS() << " V\n";
    std::cout << "I_D = " << compute_i_D() << " A\n";
}
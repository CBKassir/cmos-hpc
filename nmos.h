#ifndef NMOS_H
#define NMOS_H

#include "node.h"

#include <functional>

class NMOS {
    private:
        double v_G, v_D, v_S, v_B, i_D;
        double v_GS = 0, v_DS = 0;
        double mu_n, C_ox, W, L;
        double V_TH0, lambda, gamma, phi_f;
        bool v_GS_update = 1, v_DS_update = 1, i_D_update = 1;
        
        double V_TH, K;

        Node *G, *D, *S;

        NMOS(double v_G_ = 0.0, double v_D_ = 0.0, double v_S_ = 0.0, double v_B_ = 0.0,
            double i_D_ = 0.0,
            double V_TH0_ = 0.7,
            double gamma_ = 0.4, double phi_f_ = 0.3,
            double lambda_ = 0.02,
            double mu_n_ = 450e-4, double C_ox_ = 3.45e-8,
            double W_ = 1e-4, double L_ = 1e-4,
            bool v_GS_update_ = 1, bool v_DS_update_ = 1, bool i_D_update_ = 1
            );


        using VoidFn = std::function<void()>;
        static VoidFn combine(VoidFn a, VoidFn b);

        VoidFn update_i_D_cutoff();
        VoidFn update_v_DS_cutoff();
        VoidFn update_v_GS_cutoff();
        VoidFn update_cutoff;
        void make_update_cutoff();
        
        VoidFn update_i_D_triode();
        VoidFn update_v_DS_triode();
        VoidFn update_v_GS_triode();
        VoidFn update_triode;
        void make_update_triode();

        VoidFn update_i_D_saturation();
        VoidFn update_v_DS_saturation();
        VoidFn update_v_GS_saturation();
        VoidFn update_saturation;
        void make_update_saturation();


        static constexpr double zeta = 1.1;
        static constexpr double V_TH_toler_mult = 1.1; 
        void update_state_vars();
        static constexpr int MAX_FORWARD_ITERATIONS = 5;
        void solve_state_vars();

        
        void print_state_vars() const;
        void print_device_params() const;

    public:
    
};

#endif

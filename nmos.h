#ifndef NMOS_H
#define NMOS_H

#include <array>

class NMOS {
    private:
        double v_G, v_D, v_S, v_B, i_D;
        double v_GS = 0, v_DS = 0;
        double mu_n, C_ox, W, L;
        double V_TH0, lambda, gamma, phi_f;
        
        double V_TH, K;

        NMOS(double v_G_ = 0.0, double v_D_ = 0.0, double v_S_ = 0.0, double v_B_ = 0.0,
            double i_D_ = 0.0,
            double V_TH0_ = 0.7,
            double gamma_ = 0.4, double phi_f_ = 0.3,
            double lambda_ = 0.02,
            double mu_n_ = 450e-4, double C_ox_ = 3.45e-8,
            double W_ = 1e-4, double L_ = 1e-4);

        static constexpr double zeta = 1.1;
        static constexpr double V_TH_toler_mult = 1.1; 
        bool v_GS_update = 1, v_DS_update = 1, i_D_update = 1; 
        void update_state_vars();

        void update_output_vars();

        static constexpr int MAX_FORWARD_ITERATIONS = 5;
        std::array<double, MAX_FORWARD_ITERATIONS> v_GS_track = {};
        std::array<double, MAX_FORWARD_ITERATIONS> v_DS_track = {};
        void forward_pass_iterations();

        void print_state_vars() const;
        void print_device_params() const;

    public:
    
};

#endif

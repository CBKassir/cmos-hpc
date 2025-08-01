#ifndef NMOS_H
#define NMOS_H

class NMOS {
    private:
        double v_G, v_D, v_S, v_B, i_D;
        double V_TH0, lambda, gamma, phi_f;
        double mu_n, C_ox, W, L;

        // not necessary
        enum node_index {
            D_NODE = 0,
            S_NODE = 1,
            G_NODE = 2,
            B_NODE = 3
        };

        static constexpr double zeta = 1.1;
        static constexpr double V_TH_toler_mult = 1.1; 

    public:
        NMOS(double v_G_ = 0.0, double v_D_ = 0.0, double v_S_ = 0.0, double v_B_ = 0.0,
            double i_D_ = 0.0,
            double V_TH0_ = 0.7,
            double gamma_ = 0.4, double phi_f_ = 0.3,
            double lambda_ = 0.02,
            double mu_n_ = 450e-4, double C_ox_ = 3.45e-8,
            double W_ = 1e-4, double L_ = 1e-4);

        double compute_V_TH() const;
        double compute_K() const; 

        double update_return_i_D();
        double update_return_v_GS();
        double update_return_v_DS();

        double node_current(int node) const;
        double node_voltage(int node) const;

        void print_state_variables() const;
        void print_device_paramters() const;

};

#endif

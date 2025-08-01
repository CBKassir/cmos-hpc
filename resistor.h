#ifndef RESISTOR_H
#define RESISTOR_H

class resistor {
private:

    double R;

public:
    resistor(R);

    double compute_V_TH() const;
    double compute_K() const; 

    double compute_v_GS() const;
    double compute_v_DS() const;
    double compute_i_D() const;

    void set_voltages_GDSB(double v_G_, double v_D_, double v_S_, double v_B_);

    void print_state_variables() const;
    void print_device_paramters() const;
};

#endif

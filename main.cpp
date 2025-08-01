#include "nmos.h"

// * brag about : no fragmentation! because structs
// use the saved 'C++ optimiatoina' video tools + more for the above

int main() {
    // NOTE: in future, user specifies nodes not voltages
    // cut/tri/sat are condition variable names
    // constexpr everything
    // change "v_DS" to "V_DS" if it's just large signal.
    // change variable declaration order
    // compute K earlier // but perhaps not V_TH
    // design choiceS in "update_return_all" -- (a) which order for i_D v_GS v_DS (b) dynamically consider operating region for each (numerical stability here?)
    const double VAR_INDICATOR = -1;

    NMOS transistor1;
    transistor1.set_voltages_GDSB(2.0, 1.0, 0.0, 0.0);
    
    NMOS transistor2;
    transistor2.set_voltages_GDSB(2.0, 1.0, 0.0, 0.0);


    return 0;
}

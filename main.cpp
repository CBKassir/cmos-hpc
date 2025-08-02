#include "nmos.h"

// * brag about : no fragmentation! because structs
// use the saved 'C++ optimiatoina' video tools + more for the above --- also VALGRIND
// in README brag about how subthreshold calcs were tuned to be continuous with triode + desmos
// rename V_TH_toler_mult
// brag about namespace

int main() {
    // struct for NMOS initialization paramms (double double double .... :[)
    // NOTE: in future, user specifies nodes not voltages
    // cut/tri/sat are condition variable names
    // constexpr everything
    // change "v_DS" to "V_DS" if it's just large signal.
    // change variable declaration order
    // compute K earlier // but perhaps not V_TH
    // design choiceS in "update_return_all" -- (a) which order for i_D v_GS v_DS (b) dynamically consider operating region for each (numerical stability here?)

    return 0;
}

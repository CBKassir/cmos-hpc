
// Why have subthreshold modeling in large-signal? For better operation at the "border"!

// autogen script for nmos_helper?
// make 'double I_0, dummy;' 'V_min_assumed' one-time compile
// showcase  : cache hits and branch predictiblity
// *** Find a way to showcase NO FRAGMENTATION (structs?)
// use the saved 'C++ optimiatoina' video tools + more for the above --- also VALGRIND
// in README showcase  how subthreshold calcs were tuned to be continuous with triode + desmos
// document equations derivation in readme
// fix function names
// replace for loops with cool c++ alterantive?
// switch to clever initial guess for v_GS, v_DS 
    // struct for NMOS initialization paramms (double double double .... :[)
    // NOTE: in future, user specifies nodes not voltages
    // cut/tri/sat are condition variable names
    // constexpr everything
    // change "v_DS" to "V_DS" if it's just large signal.
    // change variable declaration order
    // compute K earlier // but perhaps not V_TH
// clean up -- add destructors and all that.

// LOW PRIORITY:
// autogen, but for order of state var update (i.e. i_D then v_DS then v_GS)??

#include "nmos.h"

int main() {

 
    return 0;
}

#include <iostream>
#include <memory>

#include "core/Circuit.hpp"
#include "devices/Resistor.hpp"
#include "devices/MOSFET.hpp"
#include "analysis/DCAnalysis.hpp"

int main() {

    Circuit circuit(3);

    circuit.addDevice(
        std::make_unique<Resistor>(1,0,1000)
    );

    circuit.addDevice(
        std::make_unique<MOSFET>(
            1,2,0,
            1e-4,
            0.7,
            0.02,
            1.5,
            1e-12
        )
    );

    Eigen::VectorXd x(3);
    x.setZero();

    DCAnalysis dc(circuit);

    if (!dc.run(x)) {
        std::cout << "DC failed\n";
        return 1;
    }

    std::cout << "DC Solution:\n" << x << "\n";
}
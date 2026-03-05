# CMOS-HPC Simulator

## Overview

Minimal high-performance CMOS circuit simulator written in modern C++17, supporting DC and transient analyses for resistive, capacitive, and MOS devices. The simulator uses sparse Modified Nodal Analysis (MNA), Newton–Raphson nonlinear solving, and subthreshold MOSFET modeling. It is designed with OOP best practices for extensibility, maintainability, and HPC scaling.

---

## 1. Architecture

### Object-Oriented Design

- **Device Base Class (`Device`)** — polymorphic interface for all devices  
- **Derived Devices:** Resistor, Capacitor, MOSFET  
- **Circuit Container (`Circuit`)** — stores devices and assembles sparse MNA  
- **Solver (`NewtonSolver`)** — handles nonlinear Newton–Raphson solve  

**Analysis Layer:**
- `DCAnalysis` — DC operating point  
- `TransientAnalysis` — implicit Backward Euler transient  

Sparse MNA is implemented using Eigen::SparseMatrix for memory efficiency and HPC scaling.

### Call Flow

```
main.cpp
   └── Circuit
          ├── Devices (Resistors, MOS, Capacitors)
          └── assemble()
               └── Sparse MNA + residual
                       └── NewtonSolver
                             └── Solve DC or Transient
```

---

## 2. Circuit Modeling

Supports:
- Resistors  
- Capacitors (for transient simulation)  
- MOSFETs (NMOS, with subthreshold model)

---

## 3. Mathematical Foundations

### Modified Nodal Analysis (MNA)

MNA represents a circuit as a system of equations:

```
G(x) x - I(x) = 0
```

Where:
- `x` is the vector of node voltages  
- `G(x)` is the conductance/admittance matrix  
- `I(x)` is the current vector from devices  

For nonlinear devices (MOSFETs), Newton–Raphson iterations are applied:

```
J(x_k) Δx = -F(x_k),   x_{k+1} = x_k + Δx
```

Where `J(x)` is the Jacobian:

```
J(x) = ∂F/∂x
```

### MOSFET Modeling

**Above Threshold (Vgs > Vth):**

```
ID = (1/2) * β * (Vgs - Vth)^2 * (1 + λ Vds)
gm = ∂ID/∂Vgs
gds = ∂ID/∂Vds
```

**Subthreshold (Vgs < Vth):**

```
ID = I0 * exp((Vgs - Vth)/(n VT)) * (1 - exp(-Vds/VT))
```

Where:
- `VT = kT/q` — thermal voltage  
- `n` — subthreshold slope factor  
- `I0` — process-dependent constant  

### Transient Analysis

Transient (time-domain) uses Backward Euler:

```
IC = C * (V_{n+1} - V_n) / Δt
```

Stamped into MNA as:
- Conductance: `G = C / Δt`  
- Current source: `Ieq = (C / Δt) * Vn`  

---

## 4. C++ Specifications

- Language: C++17  
- Dependencies: Eigen3 (for sparse matrices)  
- Memory management: `std::unique_ptr` for devices  
- Polymorphism: Device base class + virtual `stamp()`  
- Solver: Sparse LU (`Eigen::SparseLU`), extendable to iterative solvers  

**Modules:**

```
core/: Device.hpp, Circuit.hpp, Solver.hpp
devices/: Resistor.hpp, Capacitor.hpp, MOSFET.hpp
analysis/: DCAnalysis.hpp, TransientAnalysis.hpp
```

---

## 5. Features

- DC Analysis — nonlinear steady-state solutions  
- Transient Analysis — implicit Backward Euler for capacitors  
- Subthreshold MOS modeling  
- Sparse MNA for HPC-scale circuits  
- Fully object-oriented architecture  
- Extensible — add PMOS, voltage sources, or iterative solvers  
- C++17 modern features — `unique_ptr`, smart memory management, modular headers  

---

## 6. Example Usage

```cpp
#include <memory>
#include "core/Circuit.hpp"
#include "devices/Resistor.hpp"
#include "devices/MOSFET.hpp"
#include "analysis/DCAnalysis.hpp"

int main() {
    Circuit circuit(3);

    circuit.addDevice(std::make_unique<Resistor>(1,0,1000));
    circuit.addDevice(std::make_unique<MOSFET>(1,2,0,1e-4,0.7,0.02,1.5,1e-12));

    Eigen::VectorXd x(3); x.setZero();
    DCAnalysis dc(circuit);
    dc.run(x);

    std::cout << "DC voltages:\n" << x << "\n";
}
```


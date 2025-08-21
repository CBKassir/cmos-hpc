#include <circuit.h>
#include <nmos.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>


/// Represents a CircuitNode in the circuit
class CircuitNode {
public:
    int id;
    explicit CircuitNode(int id) : id(id) {}
};


/// Represents the whole circuit
class Circuit {
private:
    std::unordered_map<int, CircuitNode*> nodes;
    std::vector<NMOS> nmosList;

    CircuitNode* getOrCreateNode(int id) {
        if (nodes.find(id) == nodes.end()) {
            nodes[id] = new CircuitNode(id);
        }
        return nodes[id];
    }

public:
    ~Circuit() {
        for (auto& kv : nodes) delete kv.second;
    }

    void addNMOS(const std::string& name, int g, int d, int s) {
        getOrCreateNode(g);
        getOrCreateNode(d);
        getOrCreateNode(s);
        nmosList.emplace_back(name, g, d, s);
    }

    void parseNetlistLine(const std::string& line) {
        std::istringstream iss(line);
        std::string name;
        int g, d, s;

        iss >> name >> g >> d >> s;
        if (name.empty() || iss.fail()) {
            std::cerr << "Invalid line: " << line << "\n";
            return;
        }
        addNMOS(name, g, d, s);
    }

    void print() const {
        std::cout << "Circuit NMOS devices:\n";
        for (const auto& m : nmosList) {
            m.print();
        }
    }
};

int main() {
    Circuit circuit;
    std::string line;

    std::cout << "Enter netlist (NMOS: name gate drain source), type 'end' to finish:\n";
    while (true) {
        std::getline(std::cin, line);
        if (line == "end") break;
        if (!line.empty()) {
            circuit.parseNetlistLine(line);
        }
    }

    circuit.print();
    return 0;
}

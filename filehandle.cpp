#include "filehandle.hh"
#include <iostream>
#include <fstream>
#include <vector>


bool read_file(Agent& agent, std::string filename) {
    std::ifstream file(filename, std::ios::binary);

    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(agent.q_table->data()), agent.q_table->size() * sizeof(float));
        file.close();
        return true;
    } else {
        return false;
    }
}


bool write_file(Agent& agent, std::string filename) {
    std::ofstream file(filename, std::ios::binary);

    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(agent.q_table->data()), agent.q_table->size() * sizeof(float));
        file.close();
        return true;
    } else {
        return false;
    }
}

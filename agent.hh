#ifndef AGENT_H
#define AGENT_H

#include <array>
#include <vector>
#include "state.hh"


class Agent {
public:
    Agent();
    Agent(float custom_epsilon);
    int choose_action(Side side, State* state, bool training);
    void reduce_epsilon(float amount);

    std::array<float, 61261200>* q_table;


private:
    float cumulated_reward;
    float epsilon;
    std::pair<int, int> prev_score = {0, 0};
    int prev_hits = 0;
    std::vector<unsigned long long> prev_states;
    std::vector<int> prev_actions;
};


#endif // AGENT_H

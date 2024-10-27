#include "agent.hh"
#include <algorithm>
#include <iostream>
#include <limits>


Agent::Agent() {
    q_table = new std::array<float, 61261200>;
    cumulated_reward = 0;
    prev_score = {0, 0};
    prev_hits = 0;
    epsilon = 0.01;
}


Agent::Agent(float custom_epsilon) {
    q_table = new std::array<float, 61261200>;
    cumulated_reward = 0;
    prev_score = {0, 0};
    prev_hits = 0;
    epsilon = custom_epsilon;
}


int action_from_index(int index) {
    switch(index) {
    case 0:
        return -10;
    case 1:
        return 0;
    case 2:
        return 10;
    }
}


int Agent::choose_action(Side side, State* state, bool training) {

    unsigned long long state_index = state->state_index(side);
    int action_index = 0;
    float best_reward = std::numeric_limits<float>::lowest();

    for (int i = 0; i < 3; ++i) {
        float reward = q_table->at(state_index*3+i);
        if (reward > best_reward) {
            best_reward = reward;
            action_index = i;
        }
    }

    if (!training) {
        return action_from_index(action_index);
    }

    /* Q-learning algorithm is adjusted to our game's case.
     * The actions made by the agent from the start of the serve
     * or the hit of the paddle to a point scored or another hit of
     * the paddle are thought of as a single "action" of the algorithm.
    */

    // Learning rate
    float alpha = 0.1;

    // This algorithm parameter should stay as 1.0 as in this case making the correct move
    // later on is equally valuable as making it earlier.
    float gamma = 1.0;

    switch(side) {
    case Left:
        cumulated_reward += state->left_reward;
        break;
    case Right:
        cumulated_reward += state->right_reward;
        break;
    }

    // Epsilon hungry = make a random choice instead of the best one with the likelihood of epsilon
    if (rand() % 100 < epsilon*100) {
        action_index = rand() % 3;
        best_reward = q_table->at(state_index*3+action_index);
    }

    std::pair<int, int> score = {state->left_points, state->right_points};
    int hits = state->ball_hits;

    // This is the end of the action period
    if (hits != prev_hits || score != prev_score) {
        auto iter1 = prev_states.rbegin();
        auto iter2 = prev_actions.rbegin();

        // The Q-table is adjusted for every state and action made in this action period
        while (iter1 != prev_states.rend() && iter2 != prev_actions.rend()) {
            unsigned long long prev_state = *iter1;
            int prev_action = *iter2;

            q_table->at(prev_state*3 + prev_action) +=
                alpha * (cumulated_reward + gamma*best_reward - q_table->at(prev_state*3 + prev_action));

            // The earlier moves should not be judged as hard because the coarse direction approximation
            if (alpha > 0.01) {
                alpha -= 0.01;
            }

            ++iter1;
            ++iter2;

        }

        prev_states.clear();
        prev_actions.clear();
        cumulated_reward = 0;
    }

    prev_states.push_back(state_index);
    prev_actions.push_back(action_index);
    prev_score = score;
    prev_hits = hits;

    return action_from_index(action_index);
}


void Agent::reduce_epsilon(float amount){
    if (epsilon-amount > 0.01) {
        epsilon -= amount;
    } else {
        epsilon = 0.01;
    }
}

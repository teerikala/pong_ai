#include "training.hh"
#include "state.hh"
#include <iostream>
#include <algorithm>

void evaluate(int episodes, Agent& agent1, Agent& agent2) {
    State* game_state = new State();
    std::vector<int> ball_hits_by_episode;
    bool saved = false;

    int episode = 1;

    while (episode < episodes) {
        if (game_state->winner != None || game_state->ball_hits >= 5000) {
            ball_hits_by_episode.push_back(game_state->ball_hits);

            delete game_state;
            game_state = new State();

            ++episode;
            saved = false;
        }

        game_state->left_input = agent1.choose_action(Left, game_state, false);
        game_state->right_input = agent2.choose_action(Right, game_state, false);
        game_state->tick();
    }

    delete game_state;

    float average = 0;
    auto iter = ball_hits_by_episode.begin();
    while (iter != ball_hits_by_episode.end()) {
        average += *iter;
        ++iter;
    }

    average = average/ball_hits_by_episode.size();
    std::cout << "Average ball hits: " << average << std::endl;
    std::cout << "Best: " << *(std::max_element(
        ball_hits_by_episode.begin(), ball_hits_by_episode.end())) << std::endl;
    std::cout << "Worst: " << *(std::min_element(
        ball_hits_by_episode.begin(), ball_hits_by_episode.end())) << std::endl;
    ball_hits_by_episode.clear();
}


void train(int episodes, Agent& agent1, Agent& agent2) {
    State* game_state = new State();
    game_state->ball_velocity = 1.0;
    int episode = 1;

    while (episode < episodes) {
        if (game_state->winner != None || game_state->ball_hits >= 5000) {
            delete game_state;
            game_state = new State();

            ++episode;
        }

        game_state->left_input = agent1.choose_action(Left, game_state, true);
        game_state->right_input = agent2.choose_action(Right, game_state, true);
        game_state->tick();
    }

    delete game_state;
}

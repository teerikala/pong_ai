#ifndef STATE_HH
#define STATE_HH
#include <cmath>


constexpr double PI = 3.14159265358979323846;


enum Side {
    None = 0,
    Left = 1,
    Right = 2,
};


struct StateSize {
    size_t BX = 286;
    size_t BY = 175;
    size_t BD = 8;
    size_t PL = 51;
    size_t total = BX*BY*BD*PL;
};


class State {
public:
    State();
    unsigned long long state_index(Side side);
    void tick();

    int grid_x_size;
    int grid_y_size;
    int ball_size;
    int paddle_x_size;
    int paddle_y_size;

    int left_input;
    int right_input;

    float ball_x;
    float ball_y;
    float ball_velocity;

    int left_paddle_y;
    int right_paddle_y;

    int left_points;
    int right_points;

    int left_reward;
    int right_reward;

    Side winner;
    int ball_hits;

private:
    void serve(Side side);
    float ball_direction;

    int paddle1_location;
    int paddle2_location;
};


#endif // STATE_HH

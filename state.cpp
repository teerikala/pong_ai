#include "state.hh"
#include <ctime>
#include <iostream>


State::State() {

    left_input = 0;
    right_input = 0;

    left_points = 0;
    right_points = 0;

    left_reward = 0;
    right_reward = 0;

    winner = None;
    ball_hits = 0;

    // Default grid size 858 x 525
    grid_x_size = 858;
    grid_y_size = 525;
    // Default ball size 6 x 6
    ball_size = 6;
    // Default paddle size 6 x 25
    paddle_x_size = 6;
    paddle_y_size = 25;

    int middle_y = (grid_y_size-paddle_y_size)/2;
    left_paddle_y = middle_y;
    right_paddle_y = middle_y;

    paddle1_location = 25;
    paddle2_location = 25;

    ball_velocity = 6.0;

    State::serve(Left);
}


void State::tick() {
    left_reward = 0;
    right_reward = 0;

    // Small punishment from moving
    if (left_input != 0) {
        --left_reward;
    }
    if (right_input != 0) {
        --right_reward;
    }

    int y_upper_limit = grid_y_size-paddle_y_size-1;

    // Move paddles (if legal move)
    int left_move = left_paddle_y + left_input;

    if (left_move < 0) {
        left_reward -= 10;
        left_paddle_y = 0;
        paddle1_location = 0;
    } else if (left_move > y_upper_limit) {
        left_reward -= 10;
        left_paddle_y = y_upper_limit;
        paddle1_location = 50;
    } else {
        left_paddle_y = left_move;
        if (left_input > 0) {
            ++paddle1_location;
        } else if (left_input < 0) {
            --paddle1_location;
        }
    }
    left_input = 0;

    int right_move = right_paddle_y + right_input;

    if (right_move < 0) {
        right_reward -= 10;
        right_paddle_y = 0;
        paddle2_location = 0;
    } else if (right_move > y_upper_limit) {
        right_reward -= 10;
        right_paddle_y = y_upper_limit;
        paddle2_location = 50;
    } else {
        right_paddle_y = right_move;
        if (right_input > 0) {
            ++paddle2_location;
        } else if (right_input < 0) {
            --paddle2_location;
        }
    }
    right_input = 0;

    float prev_ball_y = ball_y;

    // Move ball
    ball_x += ball_velocity*std::cos(ball_direction);
    ball_y += ball_velocity*std::sin(ball_direction);

    // Collision with the paddles
    int x_limit1 = paddle_x_size+ball_velocity-1;
    int x_limit2 = grid_x_size-paddle_x_size-ball_velocity-ball_size-1;

    if (ball_x < x_limit1 && left_paddle_y <= ball_y && ball_y+ball_size <= left_paddle_y+paddle_y_size) {
        ball_x = x_limit1;
        ball_direction = PI - ball_direction;
        left_reward += 500;
        ++ball_hits;

    } else if (ball_x > x_limit2 && right_paddle_y <= ball_y && ball_y+ball_size <= right_paddle_y+paddle_y_size) {
        ball_x = x_limit2;
        ball_direction = PI - ball_direction;
        right_reward += 500;
        ++ball_hits;
    }

    // Collision with the walls
    if (ball_y < ball_size-1 || ball_y > grid_y_size-ball_size-1) {
        ball_direction = -ball_direction;
    }

    // Keep angles between 0 and 2*PI
    if (ball_direction < 0) {
        ball_direction += 2*PI;
    } else if (ball_direction > 2*PI) {
        ball_direction -= 2*PI;
    }

    // Check for points
    if (ball_x < 0) {
        if (prev_ball_y+ball_size < left_paddle_y) {
            left_reward -= (left_paddle_y-prev_ball_y+ball_size)*10;
        } else {
            left_reward -= (prev_ball_y-left_paddle_y+paddle_y_size)*10;
        }

        ++right_points;
        if (right_points == 11) {
            winner = Right;
        } else {
            State::serve(Right);
        }

    } else if (ball_x+ball_size > grid_x_size-1) {
        if (prev_ball_y+ball_size < right_paddle_y) {
            right_reward -= (right_paddle_y-prev_ball_y+ball_size)*10;
        } else {
            right_reward -= (prev_ball_y-right_paddle_y+paddle_y_size)*10;
        }

        ++left_points;
        if (left_points == 11) {
            winner = Left;
        } else {
            State::serve(Left);
        }
    }
}

int ball_direction_index(float ball_direction) {
    if (ball_direction >= 0 && ball_direction < PI/4) {
        return 0;
    } else if (ball_direction >= PI/4 && ball_direction < PI/2) {
        return 1;
    } else if (ball_direction >= PI/2 && ball_direction < 3*PI/4) {
        return 2;
    } else if (ball_direction >= 3*PI/4 && ball_direction < PI) {
        return 3;
    } else if (ball_direction >= PI && ball_direction < 5*PI/4) {
        return 4;
    } else if (ball_direction >= 5*PI/4 && ball_direction < 3*PI/2) {
        return 5;
    } else if (ball_direction >= 3*PI/2 && ball_direction < 7*PI/4) {
        return 6;
    } else if (ball_direction >= 7*PI/4 && ball_direction < 2*PI) {
        return 7;
    }
}


unsigned long long State::state_index(Side side) {
    StateSize state_size;

    size_t BY = state_size.BY;
    size_t BD = state_size.BD;
    size_t PL = state_size.PL;

    int bx = ball_x/3;
    int by = ball_y/3;
    int bd = ball_direction_index(ball_direction);

    int pl;
    switch(side) {
    case Left:
        pl = paddle1_location;
        break;
    case Right:
        pl = paddle2_location;
        break;
    }

    return bx*BY*BD*PL + by*BD*PL + bd*PL + pl;
}


void State::serve(Side side) {
    ball_x = (grid_x_size-ball_size-1)/2;
    ball_y = grid_y_size/2+grid_y_size/4-(rand() % grid_y_size/2);

    int random = rand();

    switch (side) {
    case Left:
        if (random % 2) {
            ball_direction = std::fmod(random, PI/4) + PI/12;
        } else {
            ball_direction = (2*PI) - std::fmod(random, PI/4) - PI/12;
        }
        break;
    case Right:
        if (random % 2) {
            ball_direction = PI + std::fmod(random, PI/4) + PI/12;
        } else {
            ball_direction = PI - std::fmod(random, PI/4) - PI/12;
        }
        break;
    }
}

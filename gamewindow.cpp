#include "gamewindow.hh"
#include "filehandle.hh"
#include <QPainter>
#include <QKeyEvent>
#include <iostream>


GameWindow::GameWindow(GameMode mode, QWidget* parent) : QWidget(parent) {
    this->setStyleSheet("background-color: black;");

    game_mode = mode;
    game_state = new State();
    connect(&timer, &QTimer::timeout, this, &GameWindow::game_tick);
    timer.start(16);
}


void GameWindow::game_tick() {
    if (game_state->winner != None || game_state->ball_hits >= 100) {
        delete game_state;
        game_state = new State();
    }

    switch(game_mode) {
    case SinglePlayerLeft:
        game_state->right_input = agent2.choose_action(Right, game_state, false);
        break;
    case SinglePlayerRight:
        game_state->left_input = agent1.choose_action(Left, game_state, false);
        break;
    case Multiplayer:
        break;
    case AIvsAI:
        game_state->left_input = agent1.choose_action(Left, game_state, false);
        game_state->right_input = agent2.choose_action(Right, game_state, false);
        break;
    }

    game_state->tick();
    update();
}


void GameWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(Qt::white);
    // Drawn from upper left corner
    painter.drawRect(0, 3*game_state->left_paddle_y/2, 3*5/2, 3*25/2);
    painter.setBrush(Qt::white);
    painter.drawRect(3*853/2, 3*game_state->right_paddle_y/2, 3*5/2, 3*25/2);

    painter.setBrush(Qt::white);
    painter.drawRect(3*game_state->ball_x/2, 3*game_state->ball_y/2, 3*5/2, 3*5/2);

    // Draw scores and episode number
    painter.setPen(Qt::white);
    painter.setFont(QFont("OCR A", 60));
    painter.drawText(3*200/2, 100, QString("%1").arg(game_state->left_points));
    painter.drawText(3*600/2, 100, QString("%1").arg(game_state->right_points));
}


void GameWindow::set_agents(Agent& set_agent1, Agent& set_agent2) {
    agent1 = set_agent1;
    agent2 = set_agent2;
}

void GameWindow::terminate()
{
    delete game_state;
    this->close();
}


void GameWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        delete game_state;
        this->close();
    }

    switch(game_mode) {
    case SinglePlayerLeft:
        if (event->key() == Qt::Key_Up) {
            game_state->left_input = -10;
        } else if (event->key() == Qt::Key_Down) {
            game_state->left_input = 10;
        }
        break;
    case SinglePlayerRight:
        if (event->key() == Qt::Key_Up) {
            game_state->right_input = -10;
        } else if (event->key() == Qt::Key_Down) {
            game_state->right_input = 10;
        };
        break;
    case Multiplayer:
        if (event->key() == Qt::Key_Up) {
            game_state->left_input = -10;
        } else if (event->key() == Qt::Key_Down) {
            game_state->left_input = 10;
        } else if (event->key() == Qt::Key_W) {
            game_state->right_input = -10;
        } else if (event->key() == Qt::Key_S) {
            game_state->right_input = 10;
        }
        break;
    case AIvsAI:
        break;
    }
}

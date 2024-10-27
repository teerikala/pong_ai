#ifndef GAMEWINDOW_HH
#define GAMEWINDOW_HH

#include <QWidget>
#include <QTimer>
#include "state.hh"
#include "agent.hh"


enum GameMode {
    SinglePlayerLeft = 0,
    SinglePlayerRight = 1,
    Multiplayer = 2,
    AIvsAI = 3,
};


class GameWindow : public QWidget {
public:
    GameWindow(GameMode mode = AIvsAI, QWidget* parent = nullptr);
    void set_agents(Agent& set_agent1, Agent& set_agent2);
    void terminate();


protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;


private slots:
    void game_tick();


private:
    Agent agent1;
    Agent agent2;
    State* game_state;
    QTimer timer;
    GameMode game_mode;

    int left_input;
    int right_input;
};


#endif // GAMEWINDOW_HH

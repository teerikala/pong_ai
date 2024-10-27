#include "gamewindow.hh"
#include "training.hh"
#include "filehandle.hh"
#include <iostream>
#include <QApplication>


int main(int argc, char* argv[]) {
    srand(time(0));

    Agent agent1 = Agent(0.2);
    Agent agent2 = Agent(0.2);

    if (!read_file(agent1, "agent1.bin")) {
        auto iter = agent1.q_table->begin();
        while (iter != agent1.q_table->end()) {
            *iter = 0;
            ++iter;
        }
    }

    if (!read_file(agent2, "agent2.bin")) {
        auto iter = agent2.q_table->begin();
        while (iter != agent2.q_table->end()) {
            *iter = 0;
            ++iter;
        }
    }

//    for (int i = 0; i < 5; ++i) {
//        std::cout << "Unknown actions: " << std::count(agent1.q_table->begin(), agent1.q_table->end(), 0)
//                  << " and " << std::count(agent2.q_table->begin(), agent2.q_table->end(), 0) << std::endl;

//        train(10000, agent1, agent2);
//        if (write_file(agent1, "agent1.bin") && write_file(agent2, "agent2.bin")) {
//            std::cout << "Saving to file successful." << std::endl;
//        } else {
//            std::cout << "Saving to file failed." << std::endl;
//        }
//        evaluate(1000, agent1, agent2);
//    }

    QApplication app(argc, argv);
    GameWindow window = GameWindow(SinglePlayerLeft);
//    GameWindow window = GameWindow(SinglePlayerRight);
//    GameWindow window = GameWindow(Multiplayer);
//    GameWindow window = GameWindow(AIvsAI);
    window.set_agents(agent1, agent2);
    window.setFixedSize(3*858/2, 3*525/2);
    window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    window.show();

    return app.exec();
}

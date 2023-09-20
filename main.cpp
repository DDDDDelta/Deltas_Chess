#include <iostream>

#include "chess_game.h"
#include "GUI.h"
#include "controller.h"

int main(int argc, char* argv[]) {
    DDDelta::ChessGame chess_game { DDDelta::Player { "Bob", "BobZheng00", "Zheng" , 2000, "Dev" }, DDDelta::Player { "Steven", "DDDelta", "Shi", 2300, "GM" } };
    BobZheng00::GUI GUI = BobZheng00::GUI(&chess_game);
    BobZheng00::Controller controller = BobZheng00::Controller(&GUI, &chess_game);
    controller.run();

    return 0;
}

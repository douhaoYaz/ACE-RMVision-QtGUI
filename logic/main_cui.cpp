///*******************************************************************************************///
///               ##                     ########   #####           #######################   ///
///              ####                  ##########   #######         #######################   ///
///             ######               ########         ######        #######################   ///
///              ######             #######             #####                                 ///
///           #   ######            ######                                                    ///
///          ###   ######           #####                           ####  #################   ///
///         #####   ######          #####                           ####  #################   ///
///        ######    ######         #####                           ####  #################   ///
///       ######      ######        ######                                                    ///
///      ######   ###########       #######             #####                                 ///
///     ######   #############       ########         ######        #######################   ///
///    ######            ######         #########   #######         #######################   ///
///   ######               #####          #######   #####           #######################   ///
///*******************************************************************************************///

#include "ui.h"
#include "thread_task.h"

/**
* @brief main
* argv[1] CAPTURE(0) or VIDEO(1) mode
* argv[2] first capture(CAPTURE mode, it will be only, main, left or short capture)
*         or video path(VIDEO mode)
* argv[3] second capture(CAPTURE mode, it will be assiant, right or long capture)
* argv[4] robot type(CAPTURE mode, 0 for INFANTRY, 1 for GUARD..)
* @return 1 for input error, 2 for run error
*/
int main(int argc, char* argv[]) {

    UserInterfaceFactory::getFactory().initialize();

    Setting setting;
    int mode = 0;
    std::string param1;
    std::string param2;
    int type = 0;
    if (argc >= 2) {
        mode = argv[1][0] - '0';
        if (argc > 3)
            param1 = argv[2];
        if (argc > 4)
            param2 = argv[3];
        if (argc > 5)
            type = argv[4][0] - '0';
    }
    if (argc == 1) {
        std::cout << "选择模式（0:摄像头 1:录像）：";
        std::cin >> mode;
        std::cin.clear();
        if (mode != 1 && mode != 0)
            return 2;
        std::cout << "参数1（唯一/主/左目/短焦摄像头 / 录像路径）：";
        std::cin >> param1;
        std::cin.clear();
        if (mode == 0) {
            std::cout << "参数2（空/副/右目/长焦摄像头）：";
            std::cin >> param2;
            std::cin.clear();
            std::cout << "兵种（0:步兵 1:哨兵）：";
            std::cin >> type;
            std::cin.clear();
        }
    }

    if (mode == 0) {
        std::string param1(argv[2]);
        std::string param2(argv[3]);
        switch (type) {
        default: case ROBOT_INFANTRY:
            if (param2 != "")
                setting.setInfantryParameter("../../ACERMVision/parameter/capture/"+param1+".xml");
            else
                setting.setInfantryParameter("../../ACERMVision/parameter/capture/"+param1+".xml",
                        "../../ACERMVision/parameter/capture/"+param2+".xml");
            break;
        case ROBOT_GUARD:
            if (param2 != "")
                setting.setGuardParameter("../../ACERMVision/parameter/capture/"+param1+".xml");
            break;
        }
    }
    else if (mode == 1)
        setting.setVideoParameter("../../ACERMVision/video/"+param1+".avi");

    ThreadRunner task(setting);

    task.run();
    task.join();


    return 1;
}

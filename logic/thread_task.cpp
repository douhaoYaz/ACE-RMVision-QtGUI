#include "thread_task.h"
#include "data.h"
#include "parameter.h"
#include "serial.h"
#include "kalman.hpp"
#include "armor_detector.h"
#include "buff_detector.h"
#include "ui.h"
#include <QDebug>

ThreadRunner::ThreadRunner(Setting& settings): ThreadRunnerBase(),
        port("/dev/ttyUSB0"), setting(settings),
        data_cap(2), flag_end_thread(false) {
    registerUserInterfaceData();
}

ThreadRunner::~ThreadRunner() {
    exit();
}

void ThreadRunner::run() {

    ImageProcessTask task1(flag_end_thread);
    ImageProduceTask task2(flag_end_thread);
    addTask(task1, std::ref(data_cap), std::ref(port), std::ref(setting));
    addTask(task2, std::ref(data_cap), std::ref(setting));
#ifndef GUI
//    control();
#endif  //GUI
}

void ThreadRunner::join() {
    for (auto& t: threads)
        t.join();
    threads.clear();
}

void ThreadRunner::exit() {
    flag_end_thread = true;
    join();
}

void ThreadRunner::registerUserInterfaceData() {
    auto ui = UserInterfaceFactory::getFactory().get();
    auto& data = Data::getData();
//    auto param = Parameter::getTempParameter();
    ui->registerImage("main", &data.img_show, UserInterface::UI_8U|UserInterface::UI_BGR);
    ui->registerImage("id", &data.img_id, UserInterface::UI_8U|UserInterface::UI_GRAY);
    ui->registerImage("gray", &data.img_gray, UserInterface::UI_8U|UserInterface::UI_GRAY);
    ui->registerImage("color", &data.img_color, UserInterface::UI_8U|UserInterface::UI_GRAY);
    ui->registerImage("roi", &data.img_roi, UserInterface::UI_8U|UserInterface::UI_BGR);

    ui->registerDataTable("armor",
            { "lost count", "distance", "angle x", "angle y", "fps" },
            {  &data.cnt_lost, &data.dist,
                    &data.angle_x, &data.angle_y, &data.fps });

    ui->registerDataTable("buff",
            { "lost count", "buff angle", "pre angle", "buff quadrant",
                    "last angle", "sample count", "origin time", "real speed",
                    "distance", "angle_x", "angle_y", "fps" },
            {  &data.cnt_lost, &data.angle_buff, &data.angle_pre,
                    &data.quadrant_buff, &data.angle_buff_last, &data.sample_count,
                    &data.orig_t, &data.real_spd, &data.dist, &data.angle_x,
                    &data.angle_y, &data.fps });

    ui->setCurrentTable("armor");

    auto& param_tmp = Parameter::getTempParameter();
    ui->createEditItemBar("armor");
    ui->createEditItemBar("buff");
    ui->createEditItemBar("angle");

    ui->registerTrackBar("armor", "th gray", &param_tmp.armor.th_gray, 0, 255);
    ui->registerTrackBar("armor", "th color", &param_tmp.armor.th_color, 0, 255);
    ui->registerTrackBar("armor", "offset x", &param_tmp.armor.pt_offset_cap.x, -100, 100);
    ui->registerTrackBar("armor", "offset y", &param_tmp.armor.pt_offset_cap.y, -100, 100);
    ui->registerTrackBar("buff", "th gray", &param_tmp.buff.th_gray, 0, 255);
    ui->registerTrackBar("buff", "th color", &param_tmp.buff.th_color, 0, 255);
    ui->registerTrackBar("buff", "offset x", &param_tmp.buff.th_color, 0, 200);
    ui->registerTrackBar("buff", "offset y", &param_tmp.buff.pt_offset_cap.x, 0, 200);
    ui->registerTrackBar("buff", "world offset x", &param_tmp.buff.x_pt_offset_world, 0, 200);
    ui->registerTrackBar("buff", "delay x", &param_tmp.buff.pt_offset_follow_delay.x, 0, 200);
    ui->registerTrackBar("buff", "delay y", &param_tmp.buff.pt_offset_follow_delay.y, 0, 200);
    ui->registerTrackBar("angle", "th gray", &param_tmp.angle.gravity_offset, 0, 200);

}
//void ThreadRunner::control() {

////#ifdef CUI
////    Parameter& param_tmp = Parameter::getTempParameter();
////    auto onChange = [](int, void*) {
////        Data::getData().flag_update_param = true;
////    };

////    namedWindow("control");
////    cv::createTrackbar("armor_gray_th", "control", &param_tmp.armor.th_gray, 255, onChange);
////    cv::createTrackbar("armor_color_th", "control", &param_tmp.armor.th_color, 255, onChange);


////    cv::createTrackbar("armor_aim_offset_x", "control", &param_tmp.armor.pt_offset_cap.x, 200, onChange);
////    cv::createTrackbar("armor_aim_offset_y", "control", &param_tmp.armor.pt_offset_cap.y, 200, onChange);

////    cv::createTrackbar("buff_gray_th", "control", &param_tmp.buff.th_gray, 255, onChange);
////    cv::createTrackbar("buff_color_th", "control", &param_tmp.buff.th_color, 255, onChange);
////    cv::createTrackbar("buff_offset_cap_x", "control", &param_tmp.buff.pt_offset_cap.x, 200, onChange);
////    cv::createTrackbar("buff_offset_cap_y", "control", &param_tmp.buff.pt_offset_cap.y, 200, onChange);
////    cv::createTrackbar("buff_world_offset_x", "control", &param_tmp.buff.x_pt_offset_world, 1000, onChange);
////    cv::createTrackbar("buff_offset_follow_x", "control", &param_tmp.buff.pt_offset_follow_delay.x, 200, onChange);
////    cv::createTrackbar("buff_offset_follow_y", "control", &param_tmp.buff.pt_offset_follow_delay.y, 200, onChange);
////    cv::createTrackbar("gimbal_offset_gravity", "control", &param_tmp.angle.gravity_offset, 10, onChange);

////    while (1) {
////        std::string str;
////        WindowPainter::painter >> str;
////        std::regex reg("-[a-zA-Z]+(=(\\S)+)?");

////        std::smatch sm;
////        while (std::regex_search(str, sm, reg)) {
////            std::string str_param = sm.str();

////            std::string str_key;
////            std::string str_value;

////            auto pos = str_param.find("=", 0);
////            if (pos != std::string::npos) {
////                str_key = str_param.substr(0, pos);
////                str_value = str_param.substr(pos+1, str_param.size());
////            }
////            else
////                str_key = str_param;

////            if (str_key == "-quit" || str_key == "-exit") {
////                flag_end_thread = true;
////                break;
////            }
////            else if (str_key == "-save")
////                param_tmp.other.flag_save = !Parameter::getParameter().other.flag_save;
////            else if (str_key == "mode") {
////                if (str_value == "armor" && param_tmp.other.mode_detect != DETECT_ARMOR) {
////#ifdef GUI
////                    Tool::log("Change to ARMOR mode!");
////#else
////                    std::cout<<"Change to AutoAim mode!"<<std::endl;
////#endif
////                    param_tmp.other.mode_detect = DETECT_ARMOR;
////                }
////                else if (str_value == "buff" && param_tmp.other.mode_detect != DETECT_BUFF) {
////#ifdef GUI
////                    Tool::log("Change to BUFF mode!");
////#else
////                    std::cout<<"Change to AttackBuff mode!"<<std::endl;
////#endif
////                    param_tmp.other.mode_detect = DETECT_ARMOR;
////                }
////            }

////            str = sm.suffix();

////        }

////        if (flag_end_thread)
////            break;

////        Data::getData().flag_update_param = true;
////    }
////#endif
//}

//void ThreadRunner::getSTM32Order() {

//    while (1) {
//        if (flag_end_thread)
//            return;
////        port.receiveData(Parameter::getTempParameter());

//        if (flag_end_thread)
//            return;
//    }
//}

//void ImageProduceTask::operator()() {
void ImageProduceTask::operator()(CaptureData& data_cap, Setting& setting) {
    if (flag_end)
        return;

    std::shared_ptr<Capture> cap_short;		//短焦/单摄
    if (setting.is_video)	//视频模式
        cap_short.reset(new Capture(setting.path_video, setting.sz_short));
    else {
        cap_short.reset(new Capture(setting.sz_short, setting.type_driver_short));
    }

    std::shared_ptr<Capture> cap_long;
    if (setting.is_short_only == false)
        cap_long.reset(new Capture(setting.sz_long, setting.type_driver_long));

//    CameraChooser chooser_cap(setting);
    std::shared_ptr<Capture> cap_prime = cap_short;

    while(1) {
//        if (param.other.mode_detect == DETECT_ARMOR) {
//            if (setting.is_switch_cap) {
//                if (setting.is_short_only == false && chooser_cap.chooseCamera()) {
//                    if (data.mode_cap == Capture::CAPTURE_SHORT)
//                        cap_prime = cap_short;
//                    else
//                        cap_prime = cap_long;
//                    data_cap.reset();
//                }
//            }
//            else if (cap_prime == cap_long) {
//                cap_prime = cap_short;
//                data_cap.reset();
//            }
//        }

//        else if (param.other.mode_detect == DETECT_BUFF) {
//            if (setting.is_short_only == false && cap_prime != cap_long) {
//                cap_prime = cap_long;
//                data_cap.reset();
//            }
//        }

        while (data_cap.isFull())
            if (flag_end)
                return;
        //图像获取
        *cap_prime >> data_cap;

        if (flag_end)
            return;
    }
}


//void ImageProcessTask::operator()() {
void ImageProcessTask::operator()(CaptureData& data_cap,
        RMSerialPort& port, Setting& setting) {
    Data& data = Data::getData();
    Parameter& param = Parameter::getParameter();

    if (flag_end)
        return;

    cv::Mat image;

    double t;
    Timer timer;
    cv::VideoWriter writer;
    bool is_last_video_open = false;
    std::shared_ptr<ArmorDetector> detector_armor(new ArmorDetector(image, setting));
    std::shared_ptr<BuffDetector> detector_buff;
    if (setting.is_buff)
        detector_buff.reset(new BuffDetector(image, setting));
    std::shared_ptr<Detector> detector = detector_armor;

    //短焦角度解算
    std::shared_ptr<AngleSolver> solver_short;
    if (setting.is_video == false)
        solver_short.reset(new AngleSolver(setting.mat_camera_short,
                setting.mat_coeffs_distortion_short, setting));
    //长焦角度解算
    std::shared_ptr<AngleSolver> solver_long;
    if (setting.is_short_only == false)
        solver_long.reset(new AngleSolver(setting.mat_camera_long,
                setting.mat_coeffs_distortion_long, setting));
    std::shared_ptr<AngleSolver> solver = solver_short;

    while (1) {
        while (data_cap.isEmpty())
            if (flag_end)
                return;

        if (data.flag_update_param) {
            Parameter::update();
            data.flag_update_param = false;
        }
        data_cap >> image;

        if (param.other.flag_save == true && is_last_video_open == false) {
            std::string str = "../../ACERMVision/video/"+Tool::getCurentTimeString();

            writer.open(str, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 300,
                    cv::Size(image.cols, image.rows));
            if (writer.isOpened())
                ;
            else
                param.other.flag_save = false;

        }

        if (param.other.flag_save == true) {
            writer << image;
            is_last_video_open = true;
        }

        if (param.other.mode_detect == DETECT_ARMOR && detector != detector_armor) {
            detector = detector_armor;
            data.dist = 0;
            data.cnt_lost = 0;
        }
        else if (param.other.mode_detect == DETECT_BUFF && detector != detector_buff) {
            detector = detector_buff;
            data.dist = 0;
            data.cnt_lost = 0;
        }

        timer.start();
        bool flag_find = false;
        if (image.empty() == false)
            flag_find = detector->run();

        if (setting.is_video == false) {
            if(flag_find) {
                std::cout << "flag_find" << std:: endl;
                data.cnt_lost = 0;
                if (data.mode_cap == CAPTURE_SHORT)
                    solver = solver_short;
                else if (data.mode_cap == CAPTURE_LONG) {
                    solver = solver_long;
                    if(param.other.mode_detect == DETECT_BUFF)
                        solver->predictOffset3D();
                }
                solver->getAngle(data.angle_x, data.angle_y);
#ifdef USE_KALMANFILTER
                kf.angles_kalman = kf.run(angle_x + vec_gimbal_yaw,angle_y);
                angle_x = kf.angles_kalman.at<float>(0) - vec_gimbal_yaw;
#endif  //USE_KALMANFILTER

            }
            else {
                if (param.other.mode_detect == DETECT_BUFF && data.quadrant_buff && data.cnt_lost > 3) {
                    if (data.quadrant_buff && data.cnt_lost > 3) {
                        data.cnt_lost = 0;
                        solver->barrelReturnCenter(data.angle_x, data.angle_y);
                    }
                    else {			//这里是没有检测到就给角度为0
                        data.angle_x = 0;
                        data.angle_y = 0;
                        data.dist = 0;
                    }
                }
            }
            port.sendXYZ(data.angle_x*100*param.other.factor_x,
                    data.angle_y*100*param.other.factor_y, 0);
        }

        t = timer.end();
        data.fps = 1000./t;

        if (param.other.flag_save == false && is_last_video_open == true) {
            writer.release();
            is_last_video_open = false;
        }


        if (flag_end)
            return;
    }
}

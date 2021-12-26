#include "include/setting.h"
#include <iomanip>

Setting::Setting() {
    path_classifier_arm_svm = "../../ACERMVision/parameter/other/small_armor.xml";
    path_classifier_arm_caffe_net = "../../ACERMVision/parameter/other/lenet_predict.protext";
    path_classifier_arm_caffe_model = "../../ACERMVision/parameter/other/lenet.caffemodel";
}

void Setting::set() {

    /**
     *@比赛大风车参数：
     *@大能量机关最底部装甲板(中心)与桥面高度 暂未知
     *@能量机关到桥头水平距离	7100mm
     *@大风车半径				100cm
     *@大风车中心离地高度		2283mm
     */

    /**
     *@ACE大风车参数：
     *@大能量机关最底部装甲板(中心)与桥面高度 35cm
     *@能量机关到桥头水平距离	4米多
     *@大风车半径				70cm
     *@大风车中心离地高度		2283mm
     */
    /**
     *@步兵车参数：
     *@步兵枪口距离桥面高度		41cm
     *@大风车半径				70cm
     *@相机到云台水平距离		17cm
     */

    dist_buff = 5200;
    h_buff = 350;
    h_car = 410;
    r_buff = 700;

    speed_bullet = 17;
    path_classifier_arm_svm = "../../ACERMVision/parameter/other/small_armor.xml";
    path_classifier_arm_caffe_net = "../../ACERMVision/parameter/other/lenet_predict.protext";
    path_classifier_arm_caffe_model = "../../ACERMVision/parameter/other/lenet.caffemodel";

}

void Setting::setInfantryParameter(const std::string& path_prime) {

    is_video = false;
    is_short_only = true;
    is_switch_cap = false;
    path_param_short = path_prime;

    is_buff = true;
    h_buff = 350;
    r_buff = 700;
    rate_buff = 0.1;
    dist_buff = 5200;

    cv::FileStorage fs("../../ACERMVision/parameter/infantry.xml", cv::FileStorage::READ);

    fs["speed_bullet"] >> speed_bullet;
    fs["h_barrel2ptz"] >> h_barrel2ptz;
    fs["h_car"] >> h_car;
    fs["pt3_ptz2camera"] >> pt3_ptz2camera;
    fs["path_param_short"] >> path_param_short;

    fs.open(path_prime, cv::FileStorage::READ);
    fs["sz"] >> sz_short;
    fs["type_driver"] >> type_driver_short;
    fs["pt3_offset"] >> pt3_offset_short;
    fs["mat_camera"] >> mat_camera_short;
    fs["mat_coeffs_distortion"] >> mat_coeffs_distortion_short;
    fs.release();
    set();
}

void Setting::setInfantryParameter(const std::string& path_short,
        const std::string& path_long, bool flag_switch_cap) {

    is_video = false;
    is_short_only = false;
    is_switch_cap = flag_switch_cap;
    path_param_short = path_short;
    path_param_long = path_long;

    cv::FileStorage fs("../../ACERMVision/parameter/infantry.xml", cv::FileStorage::READ);

    fs["speed_bullet"] >> speed_bullet;
    fs["h_barrel2ptz"] >> h_barrel2ptz;
    fs["h_car"] >> h_car;
    fs["pt3_ptz2camera"] >> pt3_ptz2camera;
    fs["path_param_short"] >> path_param_short;

    is_buff = true;
    fs["h_buff"] >> h_buff;
    fs["r_buff"] >> r_buff;
    fs["rate_buff"] >> rate_buff;
    fs["dist_buff"] >> dist_buff;

    fs.open(path_short, cv::FileStorage::READ);
    fs["sz"] >> sz_short;
    fs["type_driver"] >> type_driver_short;
    fs["pt3_offset"] >> pt3_offset_short;
    fs["mat_camera"] >> mat_camera_short;
    fs["mat_coeffs_distortion"] >> mat_coeffs_distortion_short;

    fs.open(path_long, cv::FileStorage::READ);
    fs["length_f"] >> length_f_long;
    fs["sz"] >> sz_long;
    fs["type_driver"] >> type_driver_long;
    fs["pt3_offset"] >> pt3_offset_long;
    fs["mat_camera"] >> mat_camera_long;
    fs["mat_coeffs_distortion"] >> mat_coeffs_distortion_long;
    fs.release();

    th_lost_target = 200;
    rate_switch_cap = 0.5;
    th_short = 1300;
    th_long = 1600;

    set();
}

void Setting::setGuardParameter(const std::string& path_prime) {

    is_video = false;
    is_short_only = true;
    is_switch_cap = false;
    path_param_short = path_prime;

    cv::FileStorage fs("../../ACERMVision/parameter/guard.xml", cv::FileStorage::READ);

    fs["speed_bullet"] >> speed_bullet;
    fs["h_barrel2ptz"] >> h_barrel2ptz;
    fs["h_car"] >> h_car;
    fs["pt3_ptz2camera"] >> pt3_ptz2camera;
    fs["path_param_short"] >> path_param_short;

    fs.open(path_prime, cv::FileStorage::READ);

    fs["sz"] >> sz_short;
    fs["type_driver"] >> type_driver_short;
    fs["pt3_offset"] >> pt3_offset_short;
    fs["mat_camera"] >> mat_camera_short;
    fs["mat_coeffs_distortion"] >> mat_coeffs_distortion_short;
    fs.release();

    set();
}

void Setting::setVideoParameter(const std::string& path_video) {

    is_video = false;
    is_short_only = true;
    is_switch_cap = false;
    is_video = true;
    this->path_video = path_video;

    is_buff = true;
    h_buff = 350;
    r_buff = 700;
    rate_buff = 0.1;
    dist_buff = 5200;

    set();
}

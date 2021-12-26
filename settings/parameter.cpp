#include "include/parameter.h"

Parameter& Parameter::getParameter() {
    static Parameter param;
    return param;
}

Parameter& Parameter::getTempParameter() {
    static Parameter param_tmp;
    return param_tmp;
}

void Parameter::update() {
    getParameter() = getTempParameter();
}

Parameter::Parameter() {
    other.factor_x = 100;
    other.factor_y = 100;
    other.factor_z = 100;
    other.color_enemy = DETECT_BLUE;
    other.mode_detect = DETECT_ARMOR;
    other.type_buff = SMALL_RUNE;
    other.direction = BUFF_NONE;
    other.flag_save = false;

    armor.th_color = 16;
    armor.th_gray = 60;
    buff.th_color = 96;
    buff.th_gray = 78;

    //4mm 的装甲板补偿
    armor.pt_offset_cap = cv::Point(100, 100);
    //8mm 的装甲板补偿
    //armor.pt_offset_cap = cv::Point(0, 0);

    buff.pt_offset_cap = cv::Point(45, 133);

    buff.pt_offset_follow_delay = cv::Point2f(0, -20);

    angle.gravity_offset = 1;
}

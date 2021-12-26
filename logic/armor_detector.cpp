#include "armor_detector.h"
#include "parameter.h"
#include "data.h"

bool ArmorDetector::run() {

    Data& data = Data::getData();
    Parameter& param = Parameter::getParameter();
    data.img_show = img.clone();


    setROI(); //roi区域矩形
    data.img_roi = img_roi;

    cv::Mat img_gray;
    cv::cvtColor(img_roi, img_gray, cv::COLOR_BGR2GRAY);
    cv::threshold(img_gray, img_gray, param.armor.th_gray, 255, cv::THRESH_BINARY);
    data.img_gray = img_gray.clone();

    cv::Mat img_color;
    std::vector<cv::Mat> channels;
    cv::split(img_roi, channels);

    if (DETECT_RED == param.other.color_enemy)
        cv::threshold(channels[2]-channels[0], img_color, param.armor.th_color, 255, cv::THRESH_BINARY);
    else
        cv::threshold(channels[0]-channels[2], img_color, param.armor.th_color, 255, cv::THRESH_BINARY);
    data.img_color = img_color.clone();

    LightBarSet bars = detector_lb.findLightBar(img_gray, img_color);
    std::vector<Armor> armors = matcher_armor.matchArmor(img_gray, bars);    //目标装甲
    for (auto& armor: armors)
        if (is_classifier && armor.getROIImage(img_roi))
            classifier.getResult(armor);
        else
            armor.getTypeBySize();

    Armor target;
    bool flag = chooseArmor(target, armors);

    if (flag) {
        history.update(target, pt_roi);
        getPoints(target);

    }
    else
        history.updateNoFound();
    return flag;

}

void ArmorDetector::setROI(double ratio_w, double ratio_h) {

    cv::Rect2f rect_roi;  //roi矩形框

    Armor arm_last = history.arm_last;

    if (is_useroi && history.flag && (is_ignore2 == false || arm_last.id != 2)) {

        rect_roi = arm_last.rect;
        rect_roi.x -= rect_roi.width * ((ratio_w-1)/2);
        rect_roi.width = static_cast<int>(rect_roi.width * ratio_w);
        rect_roi.y -= rect_roi.height * ((ratio_h-1)/2);
        rect_roi.height *= ratio_h;

        if (rect_roi.x < 0)
            rect_roi.x = 0;
        if (rect_roi.y < 0)
            rect_roi.y = 0;
        if (rect_roi.x + rect_roi.width >= img.cols)
            rect_roi.width = img.cols - rect_roi.x - 1;
        if (rect_roi.y + rect_roi.height >= img.rows)
            rect_roi.height = img.rows - rect_roi.y - 1;
        if (rect_roi.width <= 0 || rect_roi.height <= 0)
            rect_roi = cv::Rect(0, 0, img.cols, img.rows);
    }
    else
        rect_roi = cv::Rect(0, 0, img.cols, img.rows);
    img_roi = img(rect_roi);

    pt_roi = cv::Point2f(rect_roi.x, rect_roi.y);
}

bool ArmorDetector::chooseArmor(Armor& target, std::vector<Armor>& armors) {

    cv::Point pt_aim = Parameter::getParameter().armor.pt_offset_cap - cv::Point(100, 100)
            + cv::Point(img.cols, img.rows) / 2;
    if (armors.empty() == true)
        return false;
    if (armors.size() == 1) {
        target = armors[0];
        return true;
    }

    Armor arm_last;
    if (history.flag)
        arm_last = history.arm_last;

    if (rule_choose == ARMOR_LAST_ID_FIRST && history.flag && (!is_ignore2 || arm_last.id != 2)) {
        auto iter = std::find_if(armors.begin(), armors.end(),
                [&arm_last](const Armor& armor) {
                    return armor.id == arm_last.id;
                });

        if (iter != armors.end())
            return true;
    }

    auto iter = std::min_element(armors.begin(), armors.end(),
            [this, pt_aim](const Armor& armor1, const Armor& armor2)->bool {
                if (is_ignore2)
                    if (armor1.id == 2 && armor2.id != 2)
                        return false;
                    if (armor1.id != 2 && armor2.id == 2)
                        return true;

                return Tool::calculateDistance(convertSourcePoint(armor1.pt_cen), pt_aim) <
                        Tool::calculateDistance(convertSourcePoint(armor2.pt_cen), pt_aim);
            });
    target = *iter;

    return true;

}

void ArmorDetector::getPoints(Armor& arm_target) {
    Data& data = Data::getData();
    Parameter& param = Parameter::getParameter();
    cv::Point2f pts_tmp[4];
    cv::Point2f pts_2d[4];
    cv::Point pt_offset = cv::Point(100, 100) - static_cast<cv::Point>(param.armor.pt_offset_cap);

    arm_target.pair[0].rrect.points(pts_tmp);
    pts_2d[0] = convertSourcePoint(static_cast<cv::Point>(pts_tmp[1]) + pt_offset);
    pts_2d[3] = convertSourcePoint(static_cast<cv::Point>(pts_tmp[0]) + pt_offset);
    arm_target.pair[1].rrect.points(pts_tmp);
    pts_2d[1] = convertSourcePoint(static_cast<cv::Point>(pts_tmp[2]) + pt_offset);
    pts_2d[2] = convertSourcePoint(static_cast<cv::Point>(pts_tmp[3]) + pt_offset);


    data.pts_2d.clear();
    for (int i = 0; i < 4; i++)
        data.pts_2d.push_back(pts_2d[i]);

#ifdef DEBUG
    //瞄准点
    cv::Point2f pt_aim = cv::Point((pts_2d[0].x + pts_2d[2].x)/2,(pts_2d[0].y + pts_2d[2].y)/2);
    cv::circle(data.img_show, pt_aim, 3, cv::Scalar(255, 255, 255), 2);

    //装甲
    for(int i = 0; i < 4; i++)
        cv::line(data.img_show, pts_2d[i%4], pts_2d[(i+1)%4], cv::Scalar(128, 0, 128), 3);
#endif
}

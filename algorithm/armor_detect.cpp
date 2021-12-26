#include "include/armor_detect.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

LightBar::LightBar(const cv::RotatedRect& r) {
    rrect = r;
    length = rrect.boundingRect2f().size().height;
}

std::vector<cv::Point2f> Armor::points = {
	cv::Point(0, 64), cv::Point(64, 64),
	cv::Point(64, 0), cv::Point(0, 0)
};

Armor::Armor(const LightBar& l1, const LightBar& l2, const cv::Size& size_image):
        is_empty(false) {
    pair[0] = l1;
    pair[1] = l2;

    rect.width = static_cast<int>(pair[1].rrect.center.x -
            pair[0].rrect.center.x);
    double height_led = static_cast<double>((pair[0].rrect.size.height +
            pair[0].rrect.size.height) / 2);
    pt_cen = (pair[0].rrect.center + pair[1].rrect.center) / 2;

    //灯条大约6cm，装甲高大约12cm，小装甲宽大约12cm，大装甲宽大约24cm
    rect.height = static_cast<int>(height_led * 2); //装甲高约等于2倍灯条

    rect.x = pt_cen.x - rect.width / 2;
    rect.y = pt_cen.y - rect.height / 2;
    if (rect.y < 0)
        rect.y = 0;
    if (rect.height + rect.y > size_image.height)
        rect.height = size_image.height - rect.y;


    pts[0] = cv::Point(rect.x, rect.y + rect.height);
    pts[1] = cv::Point(rect.x + rect.width, rect.y + rect.height);
    pts[2] = cv::Point(rect.x + rect.width, rect.y);
    pts[3] = cv::Point(rect.x, rect.y);
}

bool Armor::getROIImage(const cv::Mat& img_src) {
	std::vector<cv::Point2f> points(pts, pts + 4);							//装甲矩形的四个点，放入透视变换
    cv::Mat img_gray = cv::Mat::zeros(64, 64, CV_8UC3);						//透视变换后的图像
	cv::Mat mat_trans = cv::getPerspectiveTransform(points, Armor::points); //透视变换矩阵
    cv::warpPerspective(img_src, img_gray, mat_trans, img_gray.size());
    cv::cvtColor(img_gray, img_gray, cv::COLOR_BGR2GRAY);
	//填充两侧灯条，防止影响
    cv::rectangle(img_gray, cv::Point(0, 0), cv::Point(9, 63), cv::Scalar::all(0), -1);
    cv::rectangle(img_gray, cv::Point(54, 0), cv::Point(63, 63), cv::Scalar::all(0), -1);
//	cv::equalizeHist(img_gray, img_gray);		//直方图均匀

    cv::Mat img_bin;        //二值图像
    double th;              //大津得出来的阈值
    int area_light;         //亮点面积
    int area_all;           //总面积
    double ratio;           //亮点面积比例

    th = cv::threshold(img_gray, img_bin, 5, 255, cv::THRESH_OTSU);
    area_light = cv::countNonZero(img_bin);
    area_all = 64*64;
    ratio = 1.0 * area_light / area_all;
//    cv::imshow("gray", img_gray);

    const double ratio_min = 0.1;
    const double ratio_max = 0.5;
    static int th_const = 12;
    bool res = false;

    if ((ratio < ratio_min && th > th_const) || (ratio > ratio_max && th < th_const)) {
        //大津无效，改为固定阈值
        res = false;
        cv::threshold(img_gray, img_bin, th_const, 255, cv::THRESH_BINARY);

        area_light = cv::countNonZero(img_bin);
        ratio = 1.0 * area_light / area_all;
        if (ratio < ratio_min)
            th_const = th_const - 1 == 0 ? 1 : th_const - 1;
        else if (ratio > ratio_max)
            th_const = th_const + 1 == 255 ? 254 : th_const + 1;
        else {
            img = img_bin;
            res = true;
        }

    }
    else if (ratio > ratio_min && ratio < ratio_max) {
        res = true;
    }
    else {
        img = cv::Mat();
        res = false;
    }

    return res;
}

void Armor::getTypeBySize() {

    std::cout << "getTypeBySize" << std::endl;
    double height_led = static_cast<double>((pair[0].rrect.size.height +
            pair[1].rrect.size.height) / 2);
    const double ratio_wh = rect.width / height_led;	//灯条长和装甲宽度之比
    if (ratio_wh > 3. && ratio_wh < 5.)
        is_big = true;
    else if (ratio_wh > 1.2 && ratio_wh <= 3.)
        is_big = false;
}

void ArmorClassifier::getResult(Armor& armor) {
//    cv::Mat input_blob = cv::dnn::blobFromImage(armor.img, 1, cv::Size(64, 64));

//    net.setInput(input_blob, "data");

//    cv::Mat pred = net.forward("prob");

//    cv::Point pt;
//    cv::minMaxLoc(pred, NULL, NULL, NULL, &pt);
//    int res = 3;
//    armor.is_big = res < 8;
//    armor.id = res % 8 + 1;

    armor.is_big = false;
    armor.id = 4;

}

void ArmorHistory::update(const Armor& arm, const cv::Point& pt_offset) {
    arm_last = arm;
    arm_last.rect.x += pt_offset.x;
    arm_last.rect.y += pt_offset.y;
    cnt_update = 0;
    flag = true;

}

void ArmorHistory::updateNoFound() {
    if (++cnt_update >= cnt_max) {
        arm_last = Armor();
        flag = false;
    }
}



LightBarSet LightBarDetector::findLightBar(const cv::Mat& img_gray, const cv::Mat& img_color) {

    LightBarSet bars;                              //直接返回这个并不会有损性能（copy elision）
    std::vector<std::vector<cv::Point>> contours_color;	//颜色通道相减图像（外部）轮廓
    std::vector<std::vector<cv::Point>> contours_gray;	//灰度图像（外部）轮廓
    cv::findContours(img_gray, contours_gray, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::findContours(img_color, contours_color, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    size_t sz_gray = contours_gray.size();		//灰度轮廓数量
    for (size_t i = 0; i < sz_gray; i++) {
        double area = cv::contourArea(contours_gray[i]);	//灰度单条轮廓面积
        if (area < range_s.start || range_s.end < area)
            continue;
        for (size_t j = 0; j < contours_color.size(); j++) {
            if (cv::pointPolygonTest(contours_color[j], contours_gray[i][0], false) >= 0.0) {
                double length = cv::arcLength(cv::Mat(contours_gray[i]), true);	//灰度轮廓周长
                if (length < range_len.start || length > range_len.end)//排除周长不合理
                    continue;
                cv::RotatedRect rrect = cv::fitEllipse(cv::Mat(contours_gray[i]));	//拟合的旋转矩阵
                if (rrect.angle > 90.0f)	//标准化
                    rrect.angle = rrect.angle - 180.0f;

                if (abs(rrect.angle) >= 30)		//筛选倾斜过度的灯条
                    continue;
                bars.emplace_back(rrect);
            }
        }
    }
    return bars;
}

std::vector<Armor> ArmorMatcher::matchArmor(const cv::Mat& img_gray, LightBarSet& pairs) {
    std::vector<Armor> armors;
//    size_t sz_leds = pairs.size();	//灯条数量

    for (auto iter1=pairs.begin(); iter1!=pairs.end(); ++iter1)
        for (auto iter2=iter1+1; iter2!=pairs.end(); ++iter2) {
            LightBar l, r;
            if (iter1->rrect.center.x < iter2->rrect.center.x) {
                l = *iter1;
                r = *iter2;
            }
            else {
                l = *iter2;
                r = *iter1;
            }

            float angle_err = fabs(l.rrect.angle - r.rrect.angle);
            float rate_len = fabs(l.length / r.length);
            cv::Point pt_cen = (l.rrect.center + r.rrect.center) / 2;
            cv::Rect rect;
            rect.width = abs(static_cast<int>(l.rrect.center.x - r.rrect.center.x));
            rect.height = static_cast<int>((l.rrect.size.height + r.rrect.size.height) / 2);
            rect.x = pt_cen.x - rect.width / 3;
            rect.y = pt_cen.y - rect.height / 3;
            rect.width *= (2.0/3);
            rect.height *= (2.0/3);

            if(l.rrect.size.height*0.7f > r.rrect.size.height
                    || l.rrect.size.height*1.3f < r.rrect.size.height)
                break;

            cv::Mat roi = img_gray(cv::Range(rect.y, rect.y + rect.height),
                    cv::Range(rect.x, rect.x + rect.width));

            float h_avg = (l.rrect.size.height + r.rrect.size.height)/2.0f;
//            if (rect.x < 0 || rect.y < 0 || rect.width == 0 || rect.height == 0
//                    || rect.x+rect.width < img_gray.size().width
//                    || rect.y+rect.height < img_gray.size().height
//                    || fabs(l.rrect.center.y - r.rrect.center.y) > 0.8f* h_avg
//                    || h_avg*2.7f < rect.width || h_avg > 2.0f* rect.width
//                    || angle_err > this->th_angle_err
//                    || rate_len > 1.5f
//                    || static_cast<int>(cv::mean(roi).val[0]) > intensity_avg)
//                continue;
            armors.emplace_back(l, r, img_gray.size());
        }

    std::cout << "armors.size: " << armors.size() << std::endl;
    return armors;

}



#ifndef ARMOR_DETECTOR_H
#define ARMOR_DETECTOR_H

#define DEBUG
#include "utils.h"
#include "armor_detect.h"
#include "setting.h"
#include "interface.h"

/**
 *@brief 装甲选择规则
 *@Armor_ +:
 *	@LAST_ID_FIRST  0，优先选择上次检测到的ID，再根据距离选择
 *	@NEAREST_FIRST  1，在图像中距补偿中心最近的装甲优先
 *                    (最靠近炮台方向而不是靠近实际距离最近)
 */
enum ArmorChooseRule {
    ARMOR_LAST_ID_FIRST = 0,
    ARMOR_NEAREST_FIRST = 1
};


/**
 *@brief 装甲识别类
 *@获取ROI->图像预处理->寻找灯条->灯条配对拟合装甲->选择目标->获取点
 */
class ArmorDetector: public Detector {
public:
    /**
     *@brief 初始化
     *@param img_src   要读入的相机画面
     *@param setting   读入的固定参数
     *@param data_tmp  读入的交互参数
     */
    ArmorDetector(cv::Mat& img_src, Setting& setting): Detector(img_src),
            history(10), classifier(setting.path_classifier_arm_caffe_net,
            setting.path_classifier_arm_caffe_model),
            detector_lb({ 30, 400 }, { 20, 10000 }, 30),
            matcher_armor(5.5, 100), is_useroi(setting.is_armor_useroi),
            is_classifier(setting.is_armor_useclassifier) { }


    /**
     *@brief  装甲识别的主要函数
     *@return 是否识别到装甲
     */
    bool run() override;


private:

    cv::Mat img_roi;                    //图像roi区域
    cv::Point pt_roi;                   //roi图像偏移，即roi左上角点在源图像的位置
    ArmorHistory history;               //上次装甲历史
    ArmorClassifier classifier;         //装甲分类器
    LightBarDetector detector_lb;     //灯条器
    ArmorMatcher matcher_armor;           //装甲检测器
    bool is_useroi;                   //是否使用roi
    bool is_classifier;                 //是否使用分类器
    int rule_choose;                    //选择装甲的规则
    bool is_ignore2;                    //是否忽略工程

    /**
     *@brief 根据上次装甲设置ROI
     */
    void setROI(double ratio_w=3, double ratio_h=2);

    bool chooseArmor(Armor& target, std::vector<Armor>& armors);

    /**
     *@brief 从目标中获取点，用于角度解算
     *@param arm_target 目标装甲
     */
    void getPoints(Armor& arm_target);

    /**
     *@brief 得到roi点在原图的位置
     *@param pt roi上要转换的点
     *@return 原图上的点
     */
    template<class _Pt>
    inline _Pt convertSourcePoint(const _Pt& pt) {
        return pt + static_cast<_Pt>(pt_roi);
    }

    /**
     *@brief 将相对roi的矩形转换到原图上
     *@param rect roi上要转换的矩形
     *@return 原图上的矩形
     */
    template<class _Rect>
    inline _Rect convertSourceRectange(const _Rect& rect) {
        _Rect tmp = rect;
        rect.x += pt_roi.x;
        rect.y += pt_roi.y;
        return tmp;
    }

    /**
     *@brief 将相对roi的旋转矩形转换到原图上
     *@param rrect roi上要转换的旋转矩形
     *@return 原图上的旋转矩形
     */
    cv::RotatedRect convertSourceRotatedRectange(const cv::RotatedRect& rrect) {
        cv::RotatedRect tmp = rrect;
        tmp.center = convertSourcePoint(rrect.center);
        return tmp;
    }
};

#endif // ARMORDETECTOR_H

#ifndef ARMOR_DETECT_H_INCLUDE
#define ARMOR_DETECT_H_INCLUDE

#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

namespace dnn = cv::dnn;

/**
 *@brief 灯条
 */
class LightBar {
public:

    cv::RotatedRect rrect;	//灯条的矩形区域
    float length;           //灯条长度

    LightBar() { }

    /**
     *@brief 用拟合出来的旋转矩阵构造结构体
     *@param r 灯条轮廓拟合出来的旋转矩阵
     */
    LightBar(const cv::RotatedRect& r);
};
typedef std::vector<LightBar> LightBarSet;    //灯条集

/**
 *@brief 装甲类
 *@主要为两个灯条之间的装甲区域
 */
class Armor {
public:

    // 3           2
    //  II-------II
    //  II   5   II
    //  II-------II
    // 0           1
    cv::Size size_img;  //图像的尺寸
    LightBar pair[2];
    cv::Point pts[4];   //装甲矩形的四个点
    bool is_big;        //装甲类型
    cv::Mat img;		//装甲区域对应部分二值
    int id;             //装甲号码牌，0为未知，正数为对应号码
    bool is_empty;      //是否为空
    cv::Point pt_cen;
    cv::Rect rect;      //装甲区域

    /**
     *@brief 使用两灯条来拟合装甲
     *@param pair       要拟合的灯条对
     *@param size_image 图像的大小
     */
    Armor(const LightBar& l1, const LightBar& l2, const cv::Size& size_image);

    Armor(): is_empty(true) { }

    /**
     *@brief 获取roi图像
     *@param img 装甲板所处背景
     *@return 得出图像是否可用
     */
    bool getROIImage(const cv::Mat& img);

    /**
     *@brief 根据尺寸获取装甲板类型
     */
    void getTypeBySize();


private:
    static std::vector<cv::Point2f> points; //透视变换后的四个点
};

/**
 *@brief 装甲板id和size分类器
 *@有SVM（太原理工开源）和cv::dnn+caffe两种方法
 */
class ArmorClassifier {
public:

//    dnn::Net net;	//caffe网络

    /**
     *@brief 初始化
     *@param path_caffe_net   caffe网络文件路径(.protext/.prototxt)
     *@param path_caffe_model caffe权重文件(.caffemodel)
     */
    ArmorClassifier(const std::string& path_dnn_net,
            const std::string& path_dnn_model)/*:
            net(dnn::readNetFromCaffe(path_dnn_net, path_dnn_model))*/ { }

    /**
     *@brief 分类
     *@输入：
     *	@param img     待分类图像
     *@输出：
     *	@param type_sz 装甲大小类型
     *	@param id      得到的id
     */
    void getResult(Armor& armor);
};

/**
 * @brief 上次检测的装甲的历史数据
 */
class ArmorHistory {
public:
    Armor arm_last;
    bool flag;
    int cnt_update;     //更新的计数器
    int cnt_max;        //计数器最大值

    /**
     *@brief 唯一构造
     *@param data_tmp   交互数据
     */
    ArmorHistory(int count_max): cnt_max(count_max) { }

    /**
     *@brief 根据装甲更新
     *@param arm    要更新的装甲数据
     *@param pt_roi roi图像偏移，即roi左上角点在源图像位置
     */
    void update(const Armor& arm, const cv::Point& pt_offset);

    /**
     *@brief 没有检测到的情况，根据计数器决定是否重置
	 *@param sz_img 图像尺寸
     */
    void updateNoFound();
};

class LightBarDetector {
public:
    LightBarDetector(const cv::Range& range_length,
            const cv::Range& range_area, float threshold_angle):
            range_len(range_length), range_s(range_area),
            th_angle(threshold_angle) { }

    LightBarSet findLightBar(const cv::Mat& img_gray, const cv::Mat& img_color);

private:
    cv::Range range_len;
    cv::Range range_s;
    float th_angle;
};

class ArmorMatcher {
public:
    ArmorMatcher(float angle_error, int intensity_avge): th_angle_err(angle_error),
            intensity_avg(intensity_avge) { }

    std::vector<Armor> matchArmor(const cv::Mat& img_gray, LightBarSet& pairs);

private:
    float th_angle_err;
    int intensity_avg;
};

#endif	//ARMOR_DETECT_H_INCLUDE

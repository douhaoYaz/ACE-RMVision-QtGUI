#ifndef SETTING_H
#define SETTING_H

#include <opencv2/core.hpp>

/**
 *@brief 设置类，程序初始的配置，所有线程只读
 */
class Setting {
public:
    cv::Point3f pt3_ptz2camera;	//相机到云台补偿(mm)
    float speed_bullet;			//子弹速度(m/s)
    float h_barrel2ptz;			//枪管与云台的相对高度(mm)
    float h_car;				//车（枪管）距地面高度(mm)

    std::string path_classifier_arm_svm;			//svm分类器路径
    std::string path_classifier_arm_caffe_net;		//caffe网络
    std::string path_classifier_arm_caffe_model;	//caffe权重文件

    std::string path_param_short;			//短焦相机相关参数路径
    cv::Size sz_short;						//图像尺寸(px)
    int type_driver_short;					//驱动类型(v4l2/OpenCV/galaxy)
    cv::Point3f pt3_offset_short;			//图像补偿点
    cv::Mat mat_camera_short;				//内参矩阵
    cv::Mat mat_coeffs_distortion_short;	//畸变系数

    bool is_short_only;             //是否只有短焦相机
    std::string path_param_long;    //长焦相机相关参数路径
    int length_f_long;
    cv::Size sz_long;
    int type_driver_long;
    cv::Point3f pt3_offset_long;
    cv::Mat mat_camera_long;
    cv::Mat mat_coeffs_distortion_long;

    bool is_buff;		//是否开启大符检测
    int h_buff;			//大风车（底部装甲）高度(mm)
    float r_buff;		//大风车扇叶半径(mm)
    float rate_buff;    //用于判断旋转方向的刷新频率
    int dist_buff;		//大风车距离(mm)

    bool is_switch_cap;		//是否开启切换长短焦相机模式
    int th_lost_target;		//丢失目标帧阈值，到达后短->长
    float rate_switch_cap;	//刷新频率
    int th_short;			//长->短距离阈值(mm)
    int th_long;			//短->长距离阈值(mm)

    bool is_video;			//是否为视频模式
    std::string path_video;	//视频路径


    bool is_armor_useroi;
    bool is_armor_useclassifier;

    Setting();

    /**
     *@brief 在读取xml之后，临时调参写在这，可以不用改xml文件
     **/
    void set();

    void setInfantryParameter(const std::string& path_prime);

    void setInfantryParameter(const std::string& path_short,
            const std::string& path_long, bool flag_switch_cap=false);

    void setGuardParameter(const std::string& path_prime);

    void setVideoParameter(const std::string& path_video);

};


#endif // SETTING_H

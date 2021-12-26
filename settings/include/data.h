#ifndef DATA_H
#define DATA_H

#include "defines.h"
#include <vector>
#include <opencv2/core.hpp>

/**
 *@brief 程序各部分的中间交互数据，控制线程只读，其他线程可读可写
 */
struct Data {
public:
    TargetMode type_target;
    int mode_cap;
    int cnt_lost;
    bool is_short;                      //当前是否为短焦
    std::vector<cv::Point2f> pts_2d;	//检测出的目标点

    float angle_buff;					//能量机关装甲板角度
    float angle_pre;	                //能量机关预测角，根据旋转方向和暴力推算得出
    int quadrant_buff;			    //能量机关象限位置

//    Timer buff_timer;
    float angle_buff_last;      //上一次的能量机关装甲板角度
    int sample_count;             //大符起始时间求取的采样数目标志位
    float orig_t;               //打击大符程序启动执行初始时间
    float real_spd;             //大符的实时转速
    //拟合斜率的vector
    std::vector<int> temp_x;
    std::vector<float> temp_y;

    float dist;                       //自瞄角度解算出的距离
    cv::Point2f pt_final_world_offset;  //最终暴力推算得到的世界坐标补偿,即弹丸运动的延时补偿
    cv::Point pt_offset_world_armor;
    cv::Point pt_offset_world_buff;     //能量机关初始世界坐标补偿
                                        //x轴(yaw轴)方向补偿值,y轴(pitch轴)由计算得出

    cv::Mat img_gray;
    cv::Mat img_color;
    cv::Mat img_roi;
    cv::Mat img_id;
    cv::Mat img_show;

    bool flag_update_param;

    float angle_x;
    float angle_y;
    double fps;

    static Data& getData();

private:
    Data();
};

#endif // DATA_H

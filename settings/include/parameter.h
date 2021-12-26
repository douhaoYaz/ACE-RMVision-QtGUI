#ifndef PARAMETER_H
#define PARAMETER_H

#include "defines.h"
#include <opencv2/core.hpp>

/**
 *@brief 参数类，由控制线程输入，其他线程只读，控制线程可读可写
 */
class Parameter {
public:

    struct ArmorParameter {
        int th_color;				//自瞄检测算法的颜色阈值
        int th_gray;                //自瞄检测算法的灰度阈值
        cv::Point pt_offset_cap;    //自瞄短焦相机成像偏移坐标补偿，(100, 100)为图像中心，等尺度（即装甲板的位置补偿）
    } armor;

    struct BuffParameter {
        int th_color;                  //能量机关检测算法的颜色阈值
        int th_gray;                   //能量机关检测算法的灰度阈值
        cv::Point pt_offset_cap;       //能量机关长焦相机成像偏移坐标补偿
        cv::Point pt_offset_follow_delay;//云台识别跟随延时的补偿(由旋转方向最终确定补偿的极性)
        int x_pt_offset_world;         //打小符云台枪管弹丸执行延时的世界坐标补偿的x值
    } buff;

    struct AngleParameter {
        int gravity_offset;     //云台pitch轴重力补偿角度
    } angle;

    struct OtherParameter {
        double factor_x;
        double factor_y;
        double factor_z;
        TargetColor color_enemy;//敌方颜色，从串口接收或手动设置调试
        DetectMode mode_detect;	//目标类型的模式，从串口接收或手动设置调试
        BuffType type_buff;      //能量机关的大小符类型
        BuffDirection direction;    //能量机关旋转方向，1顺时针，-1逆时针
        bool flag_save;		//保存视频标志
    } other;

    static Parameter& getParameter();

    static Parameter& getTempParameter();

    static void update();

private:
    /**
     *@brief 动态参数的默认设定
     */
    Parameter();

};


#endif // PARAMETER_H

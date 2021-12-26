#include "include/data.h"

Data& Data::getData() {
    static Data data;
    return data;
}

Data::Data() {
    type_target = DETECT_SMALL_ARMOR;
    cnt_lost = 0;
    pts_2d = std::vector<cv::Point2f>(4);	//检测出的目标点

    angle_buff = 0;					//能量机关装甲板角度
    angle_pre = 0;	                //能量机关预测角，根据旋转方向和暴力推算得出
    quadrant_buff = 0;			    //能量机关象限位置

//    Timer buff_timer;
    angle_buff_last = 0;      //上一次的能量机关装甲板角度
    sample_count = 0;             //大符起始时间求取的采样数目标志位
    orig_t = 0;               //打击大符程序启动执行初始时间
    real_spd = 0;             //大符的实时转速

    dist = 0;                       //自瞄角度解算出的距离
    pt_final_world_offset = { 0, 0 };  //最终暴力推算得到的世界坐标补偿,即弹丸运动的延时补偿
    pt_offset_world_armor = { 0, 0 };
    pt_offset_world_buff = { 650, 500 };     //能量机关初始世界坐标补偿
                                        //x轴(yaw轴)方向补偿值,y轴(pitch轴)由计算得

    flag_update_param = false;

    angle_x = 0;
    angle_y = 0;
    fps = 0;

    img_id = cv::Mat();
    img_roi = cv::Mat();
    img_gray = cv::Mat();
    img_show = cv::Mat();
    img_color = cv::Mat();
}

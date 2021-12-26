#ifndef KALMAN_HPP_INCLUDE
#define KALMAN_HPP_INCLUDE

#include <opencv2/video/tracking.hpp>

/**
 *@brief 通用卡尔曼预测器
 *@加了带步长的预测
 */
class KalmanFilterPredictor {
public:

    /**
     *@brief 初始化，参数和opencv的一致
     *@param cnt_state   状态参数数量
     *@param cnt_measure 测量参数数量
     *@param cnt_ctrl    控制参数数量
     *@param dtype       数据类型
     */
    KalmanFilterPredictor(int  cnt_state, int cnt_measure, int cnt_ctrl, int dtype=CV_32F):
            cvkf_cur(cnt_state, cnt_measure, cnt_ctrl, dtype),
            cvkf_pre(cnt_state, cnt_measure, cnt_ctrl, dtype) { }

    /**
     *@brief 带步长的预测，实际参数不会更新
     *@输入:
     *  @stride 预测步长，即当前数据预测n次的结果
     *@输出:
     *  @return 得到的状态向量
     */
    cv::Mat predict(int stride=1);

    /**
     *@brief 更新，实际即原opencv卡尔曼的KalmanFilter.predict()
     *@输入:
     *  @mat_ctrl 控制矩阵
     *@输出:
     *  @return 更新后的状态向量
     */
    cv::Mat update(const cv::Mat& mat_ctrl=cv::Mat());

    /**
     *@brief 修正，实际即原OpenCV卡尔曼的KalmanFilter::correct()
     *@输入:
     *  @mat_measurement 测量矩阵
     *@输出:
     *  @return 修正后的状态值
     */
    cv::Mat correct(const cv::Mat& mat_measurement);

protected:
    cv::KalmanFilter cvkf_cur;  //opencv的卡尔曼滤波器
    cv::KalmanFilter cvkf_pre;  //用于步长预测的opencv卡尔曼滤波器

    /**
     * @brief 同步预测用的滤波器
     */
    void setPredictFilter();
};

#endif // KALMAN_HPP_INCLUDE


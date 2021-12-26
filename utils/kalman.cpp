#include "include/kalman.hpp"

cv::Mat KalmanFilterPredictor::predict(int stride) {
    if (stride <= 0)
        return cv::Mat();
    this->setPredictFilter();

    cv::Mat mat_predict;
    for (int i = 0; i < stride; i++)
        mat_predict = cvkf_pre.predict();
    return mat_predict;
}

cv::Mat KalmanFilterPredictor::update(const cv::Mat& mat_ctrl) {
    return cvkf_cur.predict(mat_ctrl);
}

cv::Mat KalmanFilterPredictor::correct(const cv::Mat& mat_measurement) {
    return cvkf_cur.correct(mat_measurement);
}

void KalmanFilterPredictor::setPredictFilter() {
    cvkf_pre.transitionMatrix = cvkf_cur.transitionMatrix.clone();
    cvkf_pre.measurementMatrix = cvkf_cur.measurementMatrix.clone();
    cvkf_pre.processNoiseCov = cvkf_cur.processNoiseCov.clone();
    cvkf_pre.errorCovPost = cvkf_cur.errorCovPost.clone();
    cvkf_pre.statePost = cvkf_cur.statePost.clone();
    cvkf_pre.controlMatrix = cvkf_cur.controlMatrix.clone();
}

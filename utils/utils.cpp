#include "include/utils.h"
#include <iomanip>

void Timer::start(){
    t_begin = cv::getTickCount();
}

double Timer::calculateAverageTime() const {
    return tol / q.size();
}

double Timer::end() {
    double t_end = cv::getTickCount();
    double time = (t_end - t_begin) / cv::getTickFrequency() * 1000;
    q.push(time);
    tol += time;
    while (q.size() > static_cast<uint>(cnt)) {
        tol -= q.front();
        q.pop();
    }
    return time;
}

void Tool::drawRotatedRectangle(cv::Mat& img, const cv::RotatedRect& rrect,
        const cv::Scalar& color, int thickness, int type_line, int shift) {
    cv::Point2f pts[4];
    rrect.points(pts);
    for (int i = 0; i < 4; i++)
        cv::line(img, pts[i], pts[(i+1)%4], color, thickness, type_line, shift);
}

double Tool::calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2)*(x1 - x2)+((y1 - y2)*(y1 - y2)));
}

int Tool::calculateRoiAverageIntensity(const cv::Mat& img, const cv::Rect& rect) {
    if (rect.width < 1 || rect.height < 1 || rect.x < 1 || rect.y < 1
			|| rect.width + rect.x > img.cols || rect.height + rect.y > img.rows)
        return 255;
    cv::Mat roi = img(cv::Range(rect.y, rect.y + rect.height), cv::Range(rect.x, rect.x + rect.width));
    int average_intensity = static_cast<int>(cv::mean(roi).val[0]);
    return average_intensity;
}

int Tool::calculateRoiAverageIntensity(const cv::Mat& img) {
	int average_intensity = static_cast<int>(cv::mean(img).val[0]);
	return average_intensity;
}

float Tool::limitAngle(float angle, float angle_max) {
    if (angle > 0)
        return angle > angle_max ? angle_max : angle;
    else
        return angle < -angle_max ? -angle_max : angle;
}


float Tool::smoothAngleChange(float cur_angle, float factor, float th_angle) {
    //next=k*cur
    if(cur_angle >= 0)
		return cur_angle > th_angle ? cur_angle * factor : 0;
	else 
        return cur_angle < -th_angle ? cur_angle * factor : 0;
}

cv::Point2f Tool::LineFitting(const std::vector<int>& x, const std::vector<float>& y, int size) {
    float x_mean = 0.0f;
    float y_mean = 0.0f;
    for(int i = 0; i < size; i++) {
        x_mean += x[i];
        y_mean += y[i];
    }
    x_mean /= size;
    y_mean /= size;

    float sumx2 = 0.0f;
    float sumxy = 0.0f;
    for(int i = 0; i < size; i++) {
        sumx2 += (x[i] - x_mean) * (x[i] - x_mean);
        sumxy += (y[i] - y_mean) * (x[i] - x_mean);
    }

    float k = sumxy / sumx2;
    float b = y_mean - k*x_mean;

    cv::Point2f point_coefficient;
    point_coefficient.x = k;
    point_coefficient.y = b;

    return point_coefficient;
}

std::string Tool::getCurentTimeString(const char* format) {
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d_%H:%M:%S");
    return ss.str();
}

#ifndef UTILS_H_INCLUDE
#define UTILS_H_INCLUDE

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <cxxabi.h>
#include <queue>
#include <map>

//#define registerReflector(name_class, name_member, type) {                      \
//    size_t offset = reinterpret_cast<size_t>(&((name_class*)0)->name_member);   \
//    Reflector<name_class>::registerMember(#name_member, type, offset);          \
//}

/**
 *@brief 基于opencv的简易计时器
 */
class Timer {
public:

    Timer(int count=100): cnt(count) { }

    /**
     *@brief 开始计时
     */
    void start();

    /**
     *@brief 计算平均时间
     *@return
     */
    double calculateAverageTime() const;

    /**
     *@brief 结束本次计时
     *@return 时间，单位ms
     */
    double end();

private:
    double t_begin;     //开始的时间
    int cnt;
    double tol;
    std::queue<double> q;
};

//template <typename _Type>
//class Reflector {
//public:

//    enum Type {
//        CHAR   = 0,
//        BOOL   = 1,
//        INT    = 2,
//        FLOAT  = 3,
//        DOUBLE = 4,
//        STRING = 5,
//        OTHER  = 6
//    };

//    struct ClassInfomation {

//        std::string name;
//        int type;
//        size_t offset;

//        ClassInfomation(const std::string& name_member, int type_member, size_t offset_member):
//                name(name_member), type(type_member), offset(offset_member) { }

//        bool operator<(const ClassInfomation& info) const {
//            return offset < info.offset;
//        }

//        bool operator==(const ClassInfomation& info) const {
//            return name == info.name;
//        }

//        bool operator==(const std::string& name_info) const {
//            return name == name_info;
//        }

//    };

//public:

//    static void registerMember(const std::string& name_member, int type, size_t offset) {
//        auto& infos = getInfomations();
//        ClassInfomation info(name_member, type, offset);
//        infos.emplace(name_member, info);
//    }

//    static std::vector<ClassInfomation> getVector() {
//        auto& infos = getInfomations();
//        std::vector<ClassInfomation> v;
//        std::map<int, int> m;

//        for (auto var: infos)
//            v.push_back(var.second);

//        return std::move(v);
//    }

//    static void* get(_Type* p, const std::string& name) {
//        auto& infos = getInfomations();
//        auto iter = infos.find(name);
//        if (iter == infos.end())
//            return nullptr;
//        size_t offset = iter->second.offset;
//        return p+offset;
//    }

//    template <typename _Data>
//    static void set(_Type* p, const std::string& name, _Data data) {
//        auto& infos = getInfomations();
//        auto iter = infos.find(name);
//        if (iter == infos.end())
//            return;
//        std::map<int, int> m;
//        size_t offset = iter->second.offset;
//        int type = iter->second.type;
//        switch (type) {
//        case INT:
//            *(int*)(p+offset) = data;
//            break;
//        case FLOAT:
//            *(float*)(p+offset) = data;
//            break;
//        case CHAR:
//            *(char*)(p+offset) = data;
//            break;
//        case BOOL:
//            *(bool*)(p+offset) = data;
//            break;
//        case STRING:
//            *(std::string*)(p+offset) = data;
//            break;
//        case OTHER:
//        default:
//            *(_Data*)(p+offset) = data;
//            break;
//        }
//    }

//    static std::string getName() {
//        static std::string name;
//        if (name.empty())
//            name = abi::__cxa_demangle(typeid(_Type).name(), nullptr, nullptr, nullptr);
//        return name;
//    }


//protected:
//    static std::map<std::string, ClassInfomation>& getInfomations() {
//        static std::map<std::string, ClassInfomation> infos;
//        return infos;
//    }

//    Reflector<_Type>()=delete;

//};

/**
 *@brief 散装的工具函数，静态方法形式
 */
class Tool {
public:
    /**
     *@brief 绘制opencv旋转矩形
     *@param img   要绘制的原图
     *@param rrect 要绘制的旋转矩形
     *@param ..    其余与cv::line基本一致
     */
    static void drawRotatedRectangle(cv::Mat& img, const cv::RotatedRect& rrect,
            const cv::Scalar& color, int thickness=1, int type_line=cv::LINE_8, int shift=0);

    /**
     *@brief 在点处绘制十字标记
     *@param img       要绘制的原图
     *@param pt        十字原点
     *@param len_cross 十字一侧的线长
     *@param ...       其余与cv::line基本一致
     */
    static void drawCross(cv::Mat& img, cv::Point& pt, int len_cross, const cv::Scalar& color,
            int thickness=1, int type_line=cv::LINE_8, int shift=0) {
        cv::line(img, cv::Point2f(pt.x, pt.y-len_cross),  \
                cv::Point2f(pt.x, pt.y+len_cross),
                color, thickness, type_line, shift);
        cv::line(img, cv::Point2f(pt.x-len_cross, pt.y),  \
                cv::Point2f(pt.x+len_cross, pt.y),
                color, thickness, type_line, shift);
    }

    /**
     *@brief 计算直线距离
     *@param pt1, pt2 要计算的两个点
     *@return 距离
     */
    template<typename _T1, typename _T2>
    static double calculateDistance(const cv::Point_<_T1>& pt1, const cv::Point_<_T2>& pt2) {
        return sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x)+((pt1.y - pt2.y)*(pt1.y - pt2.y)));
    }

    static double calculateDistance(double x1, double y1, double x2, double y2);

    /**
     *@brief 把弧度转化为角度
     *@param rad 要转化的弧度
     *@return 转化出的角度
     */
    static constexpr double radian2Angle(double rad) {
        return rad * 180 / CV_PI;
    }

    /**
     *@brief 把角度转化为弧度
     *@param angle 要转化的角度
     *@return 转化出的弧度
     */
    static constexpr double angle2Radian(double angle) {
        return angle / 180. * CV_PI;
    }

    /**
     *@brief 计算图像ROI区域强度
     *@param img      要计算的原图
     *@param rect_roi 要计算的矩形区域
     *@return 区域强度
     */
    static int calculateRoiAverageIntensity(const cv::Mat& img, const cv::Rect& rect_roi);

	/**
	 *@brief 计算图像区域强度
	 *@param img  要计算的图像
	 *@return 区域强度
	 */
    static int calculateRoiAverageIntensity(const cv::Mat& img);

    /**
         *@brief 限制角度
         *@param angle      要限制的角度
         *@param angle_max  最大角度
         *@return 角度值
         */
	static float limitAngle(float angle, float angle_max);


	/**
	 *@brief 平滑角度变化(防止角度数据跳动过大)
	 *@param angle     要限制的角度
	 *@param angle_max 最大角度
	 *@return 角度值
	 */
	static float smoothAngleChange(float cur_angle, float factor, float th_angle);

    /**
     *@brief 最小二乘法线性拟合
     *@param angle     要限制的角度
     *@param angle_max 最大角度
     *@param angle_max 最大角度
     *@return 角度值
     */
    static cv::Point2f LineFitting(const std::vector<int>& x,
            const std::vector<float>& y, int size);

    template <typename _Type>
    static std::string toString(_Type val) {
        std::stringstream ss;
        ss << val;
        std::string str;
        ss >> str;
        return str;
    }

    static std::string getCurentTimeString(const char* format="%Y-%m-%d_%H:%M:%S");
};

template <>
inline std::string Tool::toString<const char*>(const char* val) {
    return std::string(val);
}

template <>
inline std::string Tool::toString<const std::string&>(const std::string& val) {
    return val;
}

#endif // UTILS_H_INCLUDE

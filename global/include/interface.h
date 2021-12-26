#ifndef INTERFACE_H
#define INTERFACE_H

#include "defines.h"
#include <opencv2/core.hpp>
#include <vector>
#include <thread>
#include <atomic>
#include <type_traits>

template <typename _Task>
class ThreadTask {
public:

    ThreadTask(std::atomic_bool& flag_exit): flag_end(flag_exit) { }

    void operator()() {
        static_cast<_Task*>(this)->operator()();
    }


    template <typename... _Args>
    void operator()(_Args... args) {
        static_cast<_Task*>(this)->operator()(args...);
    }

protected:
    std::atomic_bool& flag_end;
};

class ThreadRunnerBase {
public:

    ThreadRunnerBase() { }

    virtual ~ThreadRunnerBase() { }

    template <typename _Task, typename... _Args>
    inline void addTask(ThreadTask<_Task>& task, _Args... args) {
        threads.push_back(std::thread(task, args...));
    }

//    template <typename _Func, typename... _Args>
//    inline void addTask(_Func& func, _Args... args) {
//        threads.push_back(std::thread(func, args...));
//    }

//    template <typename _Task>
//    void addTask(ThreadTask<_Task>& task) {
//        threads.push_back(std::thread(std::ref(task)));
//    }

//    template <typename _Task, typename... _Args>
//    inline void addTask(ThreadTask<_Task>& task, _Args... args) {
//        __addTask(task, args...);
//    }

    virtual void run()=0;

    virtual void join()=0;

    virtual void exit()=0;

protected:
    std::vector<std::thread> threads;

//    template <typename _Func, typename _Var, typename... _Args>
//    inline void __addTask(_Func& func, _Var var, _Args... args) {
//        std::is_reference<_Var>::value == 1 ?
//            __addTask(std::bind(func, std::ref(var)), args...)
//        :
//            __addTask(std::bind(func, var), args...);
//    }
//    template <typename _Func>
//    inline void __addTask(_Func& func) {
//        threads.push_back(func);
//    }

};

class UserInterface {
public:

    enum ImageType {
        UI_8U  = 0x00,
        UI_16U = 0x01,
        UI_16S = 0x02,
        UI_32S = 0x03,

        UI_BGR  = 0x00,
        UI_RBG  = 0x10,
        UI_GRAY = 0x20,
        UI_HSV  = 0x30
    };

    virtual void log(const char* str)=0;

    virtual void registerImage(const char* name_title, cv::Mat* img, int type)=0;

    virtual void registerDataTable(const char* name_table, const std::vector<const char*>& keys,
            const std::vector<Pointer>& ptrs)=0;

    virtual void setCurrentTable(const char* name)=0;
    
    virtual void createEditItemBar(const char* name)=0;

    virtual void registerTrackBar(const char* name_bar, const char* name, int* data,
            int min, int max)=0;

protected:
    UserInterface() { }
};

/**
 *@brief 检测器纯虚类
 */
class Detector {
public:


    /**
     *@brief 初始化
     *@param img_src   要读入的相机画面
     *@param setting   读入的固定参数
     */
    Detector(cv::Mat& img_src): img(img_src) { }

    virtual ~Detector() { }

    /**
     *@brief 运行，纯虚模板
     *@return 是否识别到
     */
    virtual bool run()=0;
protected:
    cv::Mat& img;			//输入的原图

};

#endif // INTERFACE_H

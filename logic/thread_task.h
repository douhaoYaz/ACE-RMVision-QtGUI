#ifndef THREAD_TASK_H_INCLUDE
#define THREAD_TASK_H_INCLUDE

#include "interface.h"
#include "capture_video.h"
#include "solve_angle.h"
#include "port.h"
#include "setting.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <atomic>

class ImageProduceTask: public ThreadTask<ImageProduceTask> {
public:
//    ImageProduceTask(CaptureData& data, Setting& settings,
//            std::atomic_bool& flag): ThreadTask(flag), data_cap(data),
//            setting(settings) { }

    ImageProduceTask(std::atomic_bool& flag): ThreadTask(flag) { }

    void operator()(CaptureData& data_cap, Setting& setting);

//    CaptureData& data_cap;
//    Setting& setting;
};

class ImageProcessTask: public ThreadTask<ImageProcessTask> {
public:
//    ImageProcessTask(CaptureData& data, RMSerialPort& rmport,
//            Setting& settings, std::atomic_bool& flag):
//            ThreadTask(flag), data_cap(data), port(rmport),
//            setting(settings) { }

    ImageProcessTask(std::atomic_bool& flag):
            ThreadTask(flag) { }

//    void operator()();
    void operator()(CaptureData& data_cap, RMSerialPort& port, Setting& setting);

//    CaptureData& data_cap;
//    RMSerialPort& port;
//    Setting& setting;
};

class ThreadRunner: public ThreadRunnerBase {

public:
    ThreadRunner(Setting& setting);
    ~ThreadRunner();

    void run() override;

    void join() override;

    void exit() override;

    void registerUserInterfaceData();

private:
    RMSerialPort port;
    Setting& setting;
    CaptureData data_cap;
    std::atomic_bool flag_end_thread; //设置结束线程标志位，负责结束所有线程

};

#endif //THREAD_TASK_H_INCLUDE

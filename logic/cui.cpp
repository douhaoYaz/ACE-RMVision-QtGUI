#include "cui.h"
#include "data.h"
#include <iostream>
#include <opencv2/highgui.hpp>

void CommandUserInterface::log(const char* str) {
    std::cout << str << std::endl;
}

void CommandUserInterface::registerImage(const char* name_title, cv::Mat* img, int type) {

}

void CommandUserInterface::registerDataTable(const char* name_table,
        const std::vector<const char*>& keys, const std::vector<Pointer>& ptrs) {

}

void CommandUserInterface::setCurrentTable(const char* name) {

}

void CommandUserInterface::createEditItemBar(const char* name) {
    cv::namedWindow(name);
}

void CommandUserInterface::registerTrackBar(const char* name_bar,
        const char* name, int* data, int min, int max) {
    bool* ptr_flag = &(Data::getData().flag_update_param);
    cv::TrackbarCallback callback = [](int, void* data) {
        *static_cast<bool*>(data) = true;
    };
    cv::createTrackbar(name, name_bar, data, max-min,
            callback, ptr_flag);
}

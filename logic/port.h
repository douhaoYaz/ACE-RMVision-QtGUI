#ifndef PORT_H
#define PORT_H

#include "serial.h"

class RMSerialPort {
public:
    RMSerialPort(const char* path_device);

    bool isOpened() const;

    /**
     *@brief 发送角度数据
     *@param x, y, z 要发送的角度数据
     *@return 发送成功:失败
     */
    bool sendXYZ(double x, double y, double z);

    /**
     *@brief 获取数据
     */
    bool receiveData(int* color_target, int* mode, int* type_buff);

private:
    SerialPort port;
};

#endif // PORT_H

#include "port.h"

RMSerialPort::RMSerialPort(const char* path_device): port(path_device) {
    port.open();
}

bool RMSerialPort::isOpened() const {
    return port.isOpened();
}

bool RMSerialPort::sendXYZ(double x, double y, double z) {
//    Parameter& param = Parameter::getParameter();
    unsigned char bytes_send[] = { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE };
    if (x > 0 || y > 0 || z > 0) {
        if (port.sendData(bytes_send, 8))
            return true;
        return false;
    }

    short* data = (short*)(bytes_send+1);
    data[0] = x;
    data[1] = y;
    data[2] = z;

    if (port.sendData(bytes_send, 8))
        return true;
    return false;
}

bool RMSerialPort::receiveData(int* color_target, int* mode, int* type_buff) {
    unsigned char data[5];

    unsigned char head = static_cast<unsigned char>(0xFF);
    unsigned char end = static_cast<unsigned char>(0xFE);

    if (port.receiveData(data, 5) == false) {
        port.restart();
        return false;
    }

    if(data[0] == head && data[4] == end) {
        *color_target = static_cast<int>(data[1]);
        *mode = static_cast<int>(data[2]);
        *type_buff = static_cast<int>(data[3]);

        return true;
    }
    return false;
}

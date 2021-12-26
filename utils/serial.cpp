#include "include/serial.h"

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <cstring>
#include <cerrno>
#include <string>
#include <iterator>

SerialPort::SerialPort(const char* path_device):
    path_dev(path_device), is_open(false) { }

SerialPort::~SerialPort() {
    close();
}

bool SerialPort::open() {
    fd = ::open(path_dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd != -1) {
        fcntl(fd, F_SETFL, 0);
        configure();
        is_open = true;
    }
    else
        is_open = false;

    return is_open;
}

void SerialPort::close() {
    ::close(fd);
    is_open = false;
}

bool SerialPort::isOpened() const {
    return is_open;
}

int SerialPort::configure() {
    struct termios port_settings;               // structure to store the port settings in

    cfsetispeed(&port_settings, B115200);       // set baud rates
    cfsetospeed(&port_settings, B115200);

    port_settings.c_cflag = (port_settings.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    port_settings.c_iflag &= ~IGNBRK;         // disable break processing
    port_settings.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    port_settings.c_oflag = 0;                // no remapping, no delays
    port_settings.c_cc[VMIN]  = 0;            // read doesn't block
    port_settings.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    port_settings.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    port_settings.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    port_settings.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    port_settings.c_cflag |= 0;
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CRTSCTS;
    port_settings.c_iflag = ICANON;
    port_settings.c_cc[VMIN] = 10;           // read doesn't block
    port_settings.c_cc[VTIME] = 5;          // 0.5 seconds read timeout

    tcsetattr(fd, TCSANOW, &port_settings);             // apply the settings to the port

    return fd;
}

void SerialPort::restart() {
    close();
    open();
    configure();
}

bool SerialPort::sendData(unsigned char* data, int size) {
    if (size != write(fd, data, size))
        return false;
    return true;
}

bool SerialPort::receiveData(unsigned char* dst, int size) {
    tcflush(fd, TCIFLUSH);   // Discards old data in the rx buffer

    auto read_status = read(fd, dst, size);

    if(read_status == -1 || read_status == 0)
        return false;

    return true;
}

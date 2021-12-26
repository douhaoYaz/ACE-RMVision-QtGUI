#ifndef SERIAL_H_INCLUDE
#define SERIAL_H_INCLUDE

#include <iostream>
#include <set>

/**
 *@param 串口类
 */
class SerialPort {
public:

	/**
     *brief 构造
	 *@param path_device 串口路径
	 *@param parameter   调控参数
	 */
    SerialPort(const char* path_device);
    SerialPort(const std::string& path_device):
            SerialPort(path_device.c_str()) { }

    ~SerialPort();
	/**
	 *@brief 打开串口，产生fd
     *@param  path_dev 串口路径
     *@return 打开成功:失败
	 */
    bool open();

    void close();

    bool isOpened() const;

    /**
	 *@brief 配置串口
	 */
    int configure();

	/**
	 *@brief 重启串口
	 */
    void restart();

	/**
     *@brief 发送任意数据
	 *@param data 要发送的数组
	 *@param size 数组大小
	 *@return     发送成功:失败
	 */
    bool sendData(unsigned char* data, int size);

    bool receiveData(unsigned char* dst, int size);
    
protected:
    int fd;					//串口描述配置文件
    std::string path_dev;   //串口路径
    bool is_open;			//是否已经打开
};

#endif //SERIAL_H_INCLUDE

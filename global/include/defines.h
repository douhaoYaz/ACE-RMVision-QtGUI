//*******************
//全局的定义
//********************

#ifndef DEFINES_H
#define DEFINES_H

#include <memory>

/**
 *@brief 机器人类型
 *@ROBOT_+:
 *  @INFANTRY 0，步兵
 *  @GUARD    1，哨兵
 *  @HERO     2，英雄
 */
enum RobotType {
    ROBOT_INFANTRY = 0,
    ROBOT_GUARD    = 1,
    ROBOT_HERO     = 2
};

/**
 *@brief 敌人颜色
 *@TARGET_ +:
 *  @RED  0，红色
 *  @BLUE 1，蓝色
 */
enum TargetColor {
    DETECT_RED  = 0,
    DETECT_BLUE = 1
};

/**
 *@brief 待检测目标类型
 *@DETECT_ +:
 *  @ARMOR 0，小装甲
 *  @BUFF  1，大装甲
 *  @DART  2，风车
 */
enum TargetMode {
    DETECT_SMALL_ARMOR = 0,
    DETECT_BIG_ARMOR   = 1,
    DETECT_BUFF_ARMOR  = 2,
};

/**
 *@brief 检测模式
 *@DETECT_ +:
 *	@ARMOR 0,装甲模式
 *	@BUFF  1,能量机关模式
 *  @TOP   2,陀螺模式
 */
enum DetectMode {
    DETECT_ARMOR = 0,
    DETECT_BUFF  = 1,
    DETECT_TOP   = 2
};

/**
 *@brief 能量机关类型
 *@TARGET_ +:
 *  @SMALL_RUNE  0，小符
 *  @BIG_RUNE    1，大符
 */
enum BuffType {
    SMALL_RUNE  = 0,
    BIG_RUNE    = 1
};

/**
 *@brief 大风车旋转方向
 *@BUFF_ +:
 *	@CLOCKWISE	    1，顺时针
 *	@ANTICLOCKWISE	-1，逆时针
 *	@NONE           0，未知状态
 */
enum BuffDirection {
    BUFF_CLOCKWISE	    = 1,
    BUFF_ANTICLOCKWISE	= -1,
    BUFF_NONE	        = 0
};

/**
 *@brief 能量机关扇叶所在象限
 *@BUFF_ +:
 *	@BUFF_ORIGIN	0，中心原点
 *	@BUFF_QUADRANT1	1，第一象限
 *	@BUFF_QUADRANT2	2，第二象限
 *	@BUFF_QUADRANT3	3，第三象限
 *	@BUFF_QUADRANT4	4，第四象限
 */
enum BuffQuadrant {
    BUFF_ORIGIN    = 0,
    BUFF_QUADRANT1 = 1,
    BUFF_QUADRANT2 = 2,
    BUFF_QUADRANT3 = 3,
    BUFF_QUADRANT4 = 4
};

enum CaptureType {
    CAPTURE_SHORT = 0,
    CAPTURE_LONG  = 1
};

class Pointer {
public:

    enum Flag {
        NULL_POINTER = 0,
        BOOLEAN      = 1,
        SHORT        = 2,
        INTEGER      = 3,
        FLOAT        = 4,
        DOUBLE       = 5,
        STRING       = 6
    };

public:

    Pointer(std::nullptr_t n=nullptr): flag(NULL_POINTER) {
        data.b = n;
    }

    Pointer(bool* b): flag(BOOLEAN) {
        data.b = b;
    }

    Pointer(int* i): flag(INTEGER) {
        data.i = i;
    }

    Pointer(short* s): flag(SHORT) {
        data.s = s;
    }

    Pointer(float* f): flag(FLOAT) {
        data.f = f;
    }

    Pointer(double* d): flag(DOUBLE) {
        data.d = d;
    }

    Pointer(char* str): flag(STRING) {
        data.str = str;
    }

    Pointer(const Pointer& ptr): data(ptr.data), flag(ptr.flag) { }
    Flag getFlag() const {
        return flag;
    }

    void* get() {
        return (void*)(*this);
    }

    operator void*() {
        switch (flag) {
        case NULL_POINTER: default:
            return nullptr;
            break;
        case BOOLEAN:
            return data.b;
            break;
        case SHORT:
            return data.s;
            break;
        case INTEGER:
            return data.i;
            break;
        case FLOAT:
            return data.f;
            break;
        case DOUBLE:
            return data.d;
            break;
        case STRING:
            return data.str;
            break;
        };
    }

    Pointer& operator=(const Pointer& ptr) {
        data = ptr.data;
        flag = ptr.flag;
        return *this;
    }
private:
    union Data {
        bool* b;
        short* s;
        int* i;
        float* f;
        double* d;
        char* str;
    } data;

    Flag flag;
};

#endif // DEFINES_H

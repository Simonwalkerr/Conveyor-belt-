/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 17:43:46
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 13:49:12
 * @Description: 
 * 数据结构体定义
 */


#ifndef _DATA_H_
#define _DATA_H_

#include <ctime>
#include <chrono>
#include <string>
#include "antenna.hpp"

#define DATA_NAME_LEN (40)

typedef std::chrono::milliseconds msType;


typedef struct data_s
{
    /* data */

    char name[DATA_NAME_LEN];
    std::string name_str;
    msType time;
    float phase;
    float rssi;
    antenna* ant;

} dataS;




#endif
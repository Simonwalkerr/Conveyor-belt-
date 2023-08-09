/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 17:14:57
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-12 20:17:37
 * @Description: 
 * 标签类定义
 * 
 */


#ifndef _TAGS_H_
#define _TAGS_H_

#include <iostream>
#include <string>
#include <ctime>
#include <mutex>

#include "data.hpp"

#define DATA_LENGTH_MAX (400)


class tag{
    public:

    std::string name;

    msType initialTime;

    msType recentTime;

    //lock
    std::mutex lock;

    float pha[DATA_LENGTH_MAX]; //解缠结果

    long long time[DATA_LENGTH_MAX];

    int state;//tag消除标志
    
    dataS* dataSet[DATA_LENGTH_MAX]; //存储数据指针

    int datasetPointer = 0;


    private:

    float phaseAdjPiJump,phaseAdjReverse;


    public:

    //tag();

    tag(dataS*);

    ~tag();

    int addData(dataS* data);
    
    int phaNum();

};





#endif
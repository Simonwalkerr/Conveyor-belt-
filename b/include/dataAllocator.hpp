/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 22:48:31
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 12:49:34
 * @Description: 
 * 支持多线程的数据分配器
 * 从前级fifo中获取数据并分配或创建对应的tag对象，tag对象储存在vector中
 * 由于添加数据的方法调用，会发生解缠，所以可以通过增加allocator（实例、线程）来并行提升速度
 */

#ifndef _DATAALLOCATOR_H_
#define _DATAALLOCATOR_H_

#include <queue>
#include <list>
#include <mutex>
#include "data.hpp"
#include "tag.hpp"

#define DATA_ACCESS_MAX (30)


class dataAllocator{
    std::queue<dataS*>& dataFifo;
    std::mutex* fifoMutex;

    std::list<tag*>& tagList;

    public:

    dataAllocator(std::queue<dataS*>& Fifo, std::mutex *mutex, std::list<tag*>& List);

    void dataAllocateTask(void);

};



#endif
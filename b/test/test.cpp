/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-24 22:05:40
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 15:21:57
 * @Description: 
 */
#include <iostream>
#include "dataAllocator.hpp"
#include "tag.hpp"
#include "tagChecker.hpp"
#include "dataReader.hpp"
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>



using namespace std;

int main(void){


    queue<dataS*> dataFifo;
    mutex dataFifoMutex;
    list<tag*> tagList;
    //reader
    // cout<<"1"<<endl;

    auto reader = dataReader(dataFifo);
    thread readerThread(&dataReader::dataReadTask,reader);
    readerThread.detach();
    //allocator
    // cout<<"2"<<endl;
    auto dataAllo1 = dataAllocator(dataFifo,&dataFifoMutex,tagList);
    auto dataAllo2 = dataAllocator(dataFifo,&dataFifoMutex,tagList);

    thread dataAlloTask1(&dataAllocator::dataAllocateTask,dataAllo1);
    dataAlloTask1.detach();
    thread dataAlloTask2(&dataAllocator::dataAllocateTask,dataAllo2);
    dataAlloTask2.detach();
    
    //checker
    
    auto tagChe = tagChecker(tagList,100);
    thread tagCheTask(&tagChecker::tagCheckTask,tagChe);
    tagCheTask.detach();
    while(true){
        sleep(3);
    }


}
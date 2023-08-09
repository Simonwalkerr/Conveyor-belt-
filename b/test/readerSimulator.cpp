/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-24 22:18:26
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 16:25:43
 * @Description: 
 */


#include "readerSimulator.hpp"
#include "antenna.hpp"
#include <malloc.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>


using namespace std;



dataReaderSim::dataReaderSim(queue<dataS*>& fifo):dataFifo(fifo){
}


void dataReaderSim::dataReadSimTask(){
    //模拟reader
    int times = 0;
    int tagNum = 5;

    auto ant = antenna();
    

    while(true){

        dataS* dataPtr = (dataS*)malloc(sizeof(dataS));

        if(dataPtr == nullptr){
            cerr << "[!]dataReaderSim: UNable to malloc for dataS" << endl;
            continue;
        }

        //编辑data
        dataPtr->ant = &ant;
        strcpy(dataPtr->name, "simulatingTag");
        dataPtr->name[13] = '0'+(times%tagNum);
        dataPtr->name[14] = 0;
        dataPtr->phase = 1.2f;
        dataPtr->rssi = -65;
        //dataPtr->time = chrono::duration_cast<msType>(chrono::system_clock::now().time_since_epoch());
        // cout << dataPtr->time.count() << ' ';
        //装入fifo

        dataFifo.push(dataPtr);

        times++;

        this_thread::sleep_for(chrono::milliseconds(50));

        if(times == 200){
            return;
        }


    }
}


/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-24 22:22:05
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-24 23:23:53
 * @Description: 
 */


#ifndef _DATAREADER_SIM_H_
#define _DATAREADER_SIM_H_


#include "data.hpp"
#include <queue>
#include <string>
#include <mutex>


class dataReaderSim{

    std::queue<dataS*>& dataFifo;    

    public:
    
    dataReaderSim(std::queue<dataS*>&);

    void dataReadSimTask(void);

};





#endif

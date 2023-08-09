/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 22:37:03
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-24 23:52:27
 * @Description: 
 */


#ifndef _DATAREADER_H_
#define _DATAREADER_H_

#include "data.hpp"
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <mutex>

#define READER_HOST "192.168.1.20"
#define READER_PORT (3001)

class dataReader{

    std::queue<dataS*>& dataFifo;

    int s_server;
    struct sockaddr_in server_addr;

    int strToData(std::string str, dataS* dataToLoad);

   

    public:
    


    dataReader(std::queue<dataS*>& fifo);

    void dataReadTask(void);

};





#endif
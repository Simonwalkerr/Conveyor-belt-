/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-14 10:33:07
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 12:50:16
 * @Description: 
 */


#ifndef _TAG_CHECKER_H_
#define _TAG_CHECKER_H_

#include <list>
#include "tag.hpp"


// #define FLOATING_WINDOW_LENGTH (8)
// #define FLOATING_WINDOW_STEP (4)
// #define CHECK_DATA_NUM_MIN (FLOATING_WINDOW_LENGTH+2*FLOATING_WINDOW_STEP)
#define CHECK_DATA_NUM_MIN (30)

typedef struct checkOutput_s{

    std::string tagName;

    // msType initialTime; //第一个data的时间
    // msType passTime;    //经过天线处时间，谷底
    // msType checkTime;   //检查时间

    long long min_time;

    int posX,posY,posZ;


}checkOutput;

class tagChecker{

    std::list<tag*>& tagList;

    int checkIntervalMS;


    int checkAlgorithm(tag&, checkOutput* );


    public:

    tagChecker(std::list<tag*>&, int intervalMS);

    void tagCheckTask();


};








#endif

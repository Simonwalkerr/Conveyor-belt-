/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 17:15:59
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 14:13:01
 * @Description: 
 * tag类实现
 */


#include "tag.hpp"
#include <math.h>
#include <iostream>
using namespace std;

#define LOWER_BOUND (0.5)
#define UPPER_BOUND (1.5)

#define PI (3.1415)


// tag::tag(){
//     //无首个数据

// }

tag::tag(dataS* firData){
    //首个数据初始化
   
    name = firData->name_str;
    initialTime = firData->time;
    recentTime = initialTime;
    datasetPointer = 0;
    dataSet[datasetPointer] = firData;
    datasetPointer++;
    state = 0;

}

int tag::addData(dataS* data){
    //添加数据  j
    if(datasetPointer > (DATA_LENGTH_MAX-1)){
        //数据已满
        cerr << "[!]tag:" << name << "dataSet overload. Data discarded." << endl;
        return -1;
    }
    dataSet[datasetPointer] = data;
    datasetPointer++;
    recentTime = data->time;
    // cout<<"时间:"<<recentTime.count()<<endl;
    // cout<<name<<" "<<datasetPointer<<endl;
    //本次相位解缠
    if(datasetPointer == 2){
        //第二个data,特殊处理，补全0的adj
        phaseAdjPiJump = dataSet[0]->phase;
        pha[0] = 2*PI - phaseAdjPiJump;
        time[0] = initialTime.count();
        // cout<<"转化前  "<<initialTime.count()<<endl<<"转化后  "<<time[0]<<endl;
    }

    float phaseDiff = data->phase - phaseAdjPiJump;

    if(phaseDiff > (PI*UPPER_BOUND) && phaseDiff < (PI*LOWER_BOUND)){
        phaseAdjPiJump = data->phase - PI;
    }else if((0-phaseDiff) > (PI*UPPER_BOUND) && (0-phaseDiff) < (PI*LOWER_BOUND)){
        phaseAdjPiJump = data->phase + PI;
    }else{
        phaseAdjPiJump = data->phase;
    }
    
    phaseAdjReverse = 2*PI - phaseAdjPiJump;
    
    int k = round((phaseAdjReverse - pha[datasetPointer - 2]) / PI);
    pha[datasetPointer - 1] = phaseAdjReverse - PI*k;
    time[datasetPointer - 1] = recentTime.count();
    
    return 0;
}


tag::~tag(){
    //清除所有动态资源

    //释放data
    for(int i = 0; i<datasetPointer; i++){
        free(dataSet[i]);
    }
}


int tag::phaNum(){
    return datasetPointer;
}

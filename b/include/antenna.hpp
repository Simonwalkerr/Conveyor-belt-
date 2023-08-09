/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 17:48:29
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-12 21:46:05
 * @Description: 
 * 天线类定义
 */


#ifndef _ANTENNA_H_
#define _ANTENNA_H_


class antenna{
    public:

    float freqInMHz;
    float wavelengthInCM;

    int posX;
    int posY;
    int posZ;


    public:

    antenna();

    antenna(int, int, int);

};




#endif
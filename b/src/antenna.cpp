/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 21:39:49
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-12 21:53:00
 * @Description: 
 */

#include "antenna.hpp"


antenna::antenna(){
    freqInMHz = 920.625;
    wavelengthInCM = 30000 / freqInMHz;

    posX = 0;
    posY = 0;
    posZ = 0;
}

antenna::antenna(int x, int y, int z){
    freqInMHz = 920.625;
    wavelengthInCM = 30000 / freqInMHz;
    posX = x;
    posY = y;
    posZ = z;
}
/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 21:35:35
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-27 15:55:47
 * @Description: 
 */


#include "tagChecker.hpp"
#include <thread>
#include <chrono>
#include <cmath>
#include <vector>
#include <iostream>
#include <numeric>
#include <time.h>

#define TAG_REMOVE_LIMIT_CHECKED (20)
#define TAG_REMOVE_LIMIT_TIMEOUT (-20)
#define TAG_TIMEOUT_MS (5000)

using namespace std;

tagChecker::tagChecker(std::list<tag*>& list, int intervalMS):tagList(list){
    checkIntervalMS = intervalMS;
}
float Sum(float arr[],int len,int begin)  //求和
{
    float add = 0;//总和
	for (int i=begin;i<begin+len;i++) add += arr[i];	
	return add;
    
}
long long  AverageSum(long long arr[],int len,int begin)  //求和再求平均数
{
    // cout<<"下面是一组求和求平均"<<"先输出len"<<len<<endl<<"求和";
    long long add = 0;//总和
	for (int i=begin;i<begin+len;i++) 
    {
        // add += dataSet[i]->time;	
        add += arr[i];
        // cout << arr[i] << ' ';
        // cout<<arr[i]<<'+';
    }
    // cout<<endl;
    long long ave=add/len;
    // cout<<"我输出的ave"<<ave<<endl;
	return ave;
}


void tagChecker::tagCheckTask(){
    // int sc=100;
    // cout<<1231241241414<<endl;
    while(true){
        this_thread::sleep_for(chrono::milliseconds(checkIntervalMS));
        if(tagList.empty())
        {
            // sc--;
            // cout<<sc<< endl;
            continue;
        }
        for(list<tag*>::const_iterator iter = tagList.begin(); iter != tagList.end(); iter++){
            tag& candidate = **iter;

            if(false == candidate.lock.try_lock()){
                //tag busy
                continue;
            }

            //取得tag lock

            //检查是否要移除
            if(candidate.state != 0){
                if(candidate.state > 0){
                    //检出状态
                    candidate.state++;
                    if(candidate.state == TAG_REMOVE_LIMIT_CHECKED){
                        tagList.erase(iter--); //移除tag
                        delete &candidate; //回收资源
                        continue;
                    }
                }else{
                    //超时状态
                    candidate.state--;
                    if(candidate.state == TAG_REMOVE_LIMIT_TIMEOUT){
                        tagList.erase(iter--); //移除tag
                        cout<<"移除"<<candidate.name<<endl;
                        delete &candidate; //回收资源  
                        continue;    
                    }
                }
                candidate.lock.unlock();
                continue;
            }

            do{
                checkOutput* outputPtr = new checkOutput;
                if(outputPtr == nullptr)break;

                //算法检测
                int result = checkAlgorithm(candidate,outputPtr);

                if(0 == result){
                    //未检出，时间正常
                    

                }else if(1 == result){
                    //检出位置
                    candidate.state = 1; //检出结果，预备移除列表

                }else if(-1 == result){
                    //数据过时

                    candidate.state = -1; //超时， 预备删除状态

                }else{
                    
                }


            }while(false);

            candidate.lock.unlock();

        }
    }
}


int tagChecker::checkAlgorithm(tag& candidate, checkOutput* output){
    //解缠后相位判定，滚动窗

    float* phaSet = candidate.pha;
    int dataNum = candidate.phaNum();
    // cout<<candidate.name<<"的数量是"<<dataNum<<endl;
    // for(int i=0;i<dataNum;i++)
    // {
    //     cout<<candidate.time[i]<<endl;
    // }

    // cout<<candidate.name<<"的数量是"<<dataNum<<endl;

    //超时检查
    msType period = chrono::duration_cast<msType>(chrono::system_clock::now().time_since_epoch()) - candidate.recentTime;

    // cout<<period.count()<<endl;
    //int period = chrono::duration_cast<msType>(chrono::system_clock::now().time_since_epoch()).count()/1000 - candidate.recentTime.count();
    if(period.count() > TAG_TIMEOUT_MS){
        //timeout<
        // cout<<"被减数"<<chrono::duration_cast<msType>(chrono::system_clock::now().time_since_epoch()).count()<<endl;
        // cout<<"减数"<<candidate.recentTime.count()<<endl;
        // cout<<"period:"<<period.count()<<endl;
        cout<<candidate.name<<"超时"<<endl;
        return -1;
    }

    if(dataNum < CHECK_DATA_NUM_MIN){
        //数据量太少
        return 0;
    }

    if(candidate.name=="e200001a041101881050a542")
    {
        cout<<"e200001a041101881050a542的全部相位输出,共计"<<candidate.phaNum() << "个"<<endl;
        for(int i=0;i<=candidate.phaNum()-1;i++)
        {
            cout<<candidate.pha[i]<<endl;
        }
        cout<<endl<<endl;
    }
    int data_range = round(0.05 * dataNum); 
    float min_sum = Sum(candidate.pha,data_range,0);
	float sum=min_sum;
    long long min_time = AverageSum(candidate.time,data_range,0);
    // cout<<"min time:"<<min_time<<endl;

	for(int i=0;i<dataNum-data_range;i++)    
	{
        sum=sum-candidate.pha[i]+candidate.pha[i+data_range];
        // cout<<min_sum<<endl;
		if(sum<min_sum)
		{
            
			min_time=AverageSum(candidate.time,data_range,i);

            // cout<<"candidate:"<<candidate.time[0]<<' '<<candidate.time[1]<<endl;
            // cout<<endl;
            // cout<<"min time:222    "<<min_time<<endl;
			min_sum=sum;
            
            int biggerNum = 0;
            for(int j=0; j<dataNum; j++){
                if(candidate.pha[j] > min_sum/data_range && candidate.time[j] >= min_time)biggerNum++;

            }
            if(biggerNum > 0.2*dataNum){
                //检出
                // cout<<"biggerNum  "<<biggerNum<<"   dataNum   "<<dataNum<<endl;
                output->min_time = min_time;
                output->tagName = candidate.name;
                cout <<" 我是输出！！！" << candidate.name <<' '<< min_time <<endl;
                return 1;
            }
		}
	}
    
    return 0;

}

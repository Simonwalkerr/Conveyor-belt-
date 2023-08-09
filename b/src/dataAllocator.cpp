/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 21:35:28
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-25 12:52:02
 * @Description: 
 */


#include "dataAllocator.hpp"
#include <thread>
#include <chrono>
#include <cstring>

using namespace std;


dataAllocator::dataAllocator(std::queue<dataS*>& Fifo, std::mutex *mutex, std::list<tag*>& List):dataFifo(Fifo),tagList(List){

    fifoMutex = mutex;

}


void dataAllocator::dataAllocateTask(){

    
    dataS* dataPtrArray[DATA_ACCESS_MAX];
    
    while(true){
        
        fifoMutex->lock();
        //cout <<"allocator:" << dataFifo.size() << endl;
        if(dataFifo.empty()){
           
            fifoMutex->unlock();
            this_thread::sleep_for(chrono::milliseconds(50));
            continue;
        }
        int dataNum = 0;
        for(int i=0; i < DATA_ACCESS_MAX; i++){
            dataPtrArray[i] = dataFifo.front();
            dataFifo.pop();
            if(!dataFifo.empty() && 0 == strcmp(dataFifo.front()->name,dataPtrArray[i]->name))continue;
            else {
                dataNum = i+1;
                break;
            }
        }

        fifoMutex->unlock();

        string tagName = dataPtrArray[0]->name_str;
        tag* tagInList = nullptr;

        //遍历tagList寻找
        for(list<tag*>::const_iterator iter = tagList.begin(); iter != tagList.end(); iter++){
            // cout<<"iter:"<<(*iter)->name<<"tagName:"<<tagName<<endl;
            
            if((*iter)->name == tagName){
                //已有匹配
                // cout<< "已有匹配:" <<tagName <<endl;
                tagInList = (*iter);
            }
        }

        if(tagInList == nullptr){
            //新建tag
            
            tagInList = new tag(dataPtrArray[0]);
            // cout<<"原来："<<dataPtrArray[0]->time<<"后来"<<tagInList->initialTime.count()<<endl;
            tagInList->lock.lock();

            //新tag入队列尾部
            tagList.push_back(tagInList);
            cout<<"新建tag:"<<tagInList->name<<endl;
            // cout<<tagInList->time<<endl;
            for(int i=1; i<dataNum; i++){//存在重复新建隐患
                tagInList->addData(dataPtrArray[i]);
            }

            tagInList->lock.unlock();

            
        }else{
            //已有tag

            if(tagInList->state != 0){
                //要被清除的tag，丢弃结果
                for(int i=0; i<dataNum; i++){
                    if(dataPtrArray[i] != nullptr)free(dataPtrArray[i]);
                }
                continue;
            }

            if(false == tagInList->lock.try_lock()){
                //未能获取tag锁
                this_thread::sleep_for(chrono::milliseconds(50));
                if(false == tagInList->lock.try_lock()){
                    //丢弃数据
                    continue;
                }
            }
            for(int i=0; i<dataNum; i++){
                tagInList->addData(dataPtrArray[i]);
            }
            tagInList->lock.unlock();
            
        }



    }

}
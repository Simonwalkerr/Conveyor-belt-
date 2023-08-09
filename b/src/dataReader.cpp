/*
 * @Author: huxioan hu7006@outlook.com
 * @Date: 2023-07-12 21:35:19
 * @LastEditors: huxioan hu7006@outlook.com
 * @LastEditTime: 2023-07-24 23:52:35
 * @Description: 
 */

#include "dataReader.hpp"
#include<iostream>
#include <thread>
#include<stdio.h>
#include<unistd.h>
#include<array>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iomanip>
#include<string.h>
#include<string>
#include<sstream>
#include<queue>
#include<ctime>
#include <chrono>

using namespace std;
string change(char a[])
{
    int num;
	std::stringstream s;
	string output;
    for(int i=0;i<=11;i++)
    {
        num = (unsigned int)(unsigned char)a[i];
		s << std::hex << std::setw(2) << std::setfill('0') << num;
		num = 0;
    }
	output = s.str();
    return output;
}

dataReader::dataReader(std::queue<dataS*>& fifo):dataFifo(fifo){
    
    //初始化socket
    if((s_server = socket(AF_INET,SOCK_STREAM,0))<0){
        cerr << "[!]Error creating socket" << endl;
        return;
    }
	//指定IP地址和端口号
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(READER_PORT);

	if(inet_pton(AF_INET, READER_HOST, &server_addr.sin_addr) <= 0){
        cerr << "[!]Invalid reader address" << endl;
        return;
	}
    //s_server = socket(AF_INET, SOCK_STREAM, 0);
	

	if (connect(s_server, (struct sockaddr*)&server_addr, sizeof(sockaddr)) != 0) {
		cout << "连接失败" << endl;
		close(s_server); 
        exit(0); 
	}
	else {
		cout << "连接成功" << endl;
	}

}


void dataReader::dataReadTask(){
    //读取读写器硬件 data的产生地
	
	int times = 0;
	//dataS* dataPtr = (dataS*)malloc(sizeof(dataS));
	//dataS datarecv;
	char medi[40];
	char mediphase[4];
	int num1, num2, num3, num4, num5, num6;
	//设置发送和接收长度
	int send_len1 = 0;
	int recv_len1 = 0;
	int send_len2 = 0;
	int recv_len2 = 0;
	int send_len3 = 0;
	int recv_len3 = 0;
	int send_len4 = 0;
	int recv_len4 = 0;
	int send_len5 = 0;
	int recv_len5 = 0;
	int send_len6 = 0;
	int recv_len6 = 0;
    //设置发送和接收缓冲区和所发送的指令
	char recv_buf1[100];
	char recv_buf2[100] = {0};
	char recv_buf3[100];
	unsigned char Array1[5] = { 0xff,0x00,0x03,0x1d,0x0c };
	unsigned char Array2[6] = { 0xff,0x01,0x61,0x05,0xbd,0xb8 };
	unsigned char Array3[6] = { 0xff,0x01,0x61,0x03,0xbd,0xbe };
	unsigned char Array4[8] = { 0xff,0x03,0x91,0x02,0x01,0x01,0x42,0xc5 };
	unsigned char Array5[21] = { 0xff,0x10,0x2f,0x00,0x00,0x01,0x22,0x00,0x00,0x05,0x07,0x22,0x10,0x00,0x1b,0x03,0xe8,0x01,0xff,0xdd,0x2b };
	unsigned char Array6[11] = { 0xff,0x06,0x91,0x03,0x01,0x08,0x98,0x09,0x60,0x1e,0x90 };//设置功率
	unsigned char Array7[8] = { 0xff,0x01,0x2f,0x00,0x00,0x02,0x30,0xe6 };//关闭
    //发送接收信息
	bool flag1=0,flag2=0;
	msType dec_init_time;   //读写器读到的第一个的时间
	msType beginTime;   //距离1970年1月1日的时间
	while (1) {

		send_len1 = send(s_server, (char*)Array1, sizeof(Array1), 0);
		if (send_len1 < 0) {
			cout << "发送失败" << endl;	
		}
		else{
			cout<<"发送成功"<<endl;
		}
		recv_len1 = recv(s_server, recv_buf1, 100, 0);
		if (recv_len1 < 0) {
			cout << "接收失败" << endl;
			break;
		}


		else
		{
			if (recv_buf1[3] == 0 && recv_buf1[4] == 0)
			{
				cout << "读写器链路正常" << endl;
				send_len2 = send(s_server, (char*)Array2, sizeof(Array2), 0);
				if (send_len2 < 0) {
					cout << "发送失败" << endl;
					break;
				}
				recv_len2 = recv(s_server, recv_buf1, 100, 0);
				if (recv_len2 < 0) {
					cout << "接收失败" << endl;
				}
				else
				{
					if (recv_buf1[7] == 01)
					{
						cout << "天线1已设置" << endl;
					}
					else if (recv_buf1[9] == 01)
					{
						cout << "天线2已设置" << endl;
					}
					else if (recv_buf1[11] == 01)
					{
						cout << "天线3已设置" << endl;
					}
					else if (recv_buf1[13] == 01)
					{
						cout << "天线4已设置" << endl;
					}
					else
					{
						cout << " 无天线已设置" << endl;
					}
					send_len3 = send(s_server, (char*)Array3, sizeof(Array3), 0);
					if (send_len3 < 0)
					{
						cout << "发送失败" << endl;
						break;
					}
					recv_len3 = recv(s_server, recv_buf1, 100, 0);
					if (recv_len3 < 0)
					{
						cout << "接收失败" << endl;
					}
					else
					{
						cout << "天线功率已接收" << endl;//后需完善
						send_len6 = send(s_server, (char*)Array6, sizeof(Array6), 0);
						if(send_len6 < 0)
						{
							cout<<"设置天线功率失败"<<endl;
							break;
						}
						recv_len6 = recv(s_server, recv_buf1, 100, 0);	
						if (recv_len4 < 0) {
							cout << "接收失败" << endl;
							break;
						}
						else
						{
							cout<<"已设置天线功率"<<endl;
						
							send_len4 = send(s_server, (char*)Array4, sizeof(Array4), 0);
							if (send_len4 < 0)
							{
								cout << "发送失败" << endl;
								break;
							}
							recv_len4 = recv(s_server, recv_buf1, 100, 0);
							if (recv_len4 < 0) {
								cout << "接收失败" << endl;
								break;
							}
							else
							{
								if (recv_buf1[3] == 0 && recv_buf1[4] == 0)
								{
									cout << "工作天线已设置" << endl;
									send_len5 = send(s_server, (char*)Array5, sizeof(Array5), 0);
									if (send_len5 < 0)
									{
										cout << "发送失败" << endl;
										break;
									}
									while (true)
									{
										dataS* dataPtr = (dataS*)malloc(sizeof(dataS));
										recv_len5 = recv(s_server, recv_buf2, 100, 0);
										if (recv_len5 < 0) {
											cout << "接收失败" << endl;
											break;
										}
										else
										{
										// for (int j = 0; j < recv_len5; j++) 
										// {
										// 	cout << hex << (unsigned int)(unsigned char)recv_buf2[j] << " ";
										// }
										// cout << endl;
										// cout<<recv_len5<<endl;
											if (recv_len5 > 40)
											{
												if(flag1==0)
												{
													beginTime = chrono::duration_cast<msType>(chrono::system_clock::now().time_since_epoch());
													flag1=1;
												}

											//将天线EPC编码存入datarecv.name,char转化char
												medi[0] = recv_buf2[31];
												for (int i = 32; i < 43; i++)
												{
													medi[i - 31] = recv_buf2[i];
												}
												for (int i = 0; i < 12; i++)
												{
													dataPtr->name[i] = medi[i];
												}
											// dataPtr->name_str = dataPtr->name;
											// cout<<dataPtr->name_str;

												dataPtr->name_str = change(dataPtr->name);
											//cout<<dataPtr->name_str<<endl;
												num1 = (unsigned int)(unsigned char)recv_buf2[21];
												num2 = (unsigned int)(unsigned char)recv_buf2[22];
												std::stringstream ss;
												ss << std::hex << std::setw(2) << std::setfill('0') << num1
													<< std::hex << std::setw(2) << std::setfill('0') << num2;

												std::string result = ss.str();

											// std::cout << result << std::endl;
											// 16进制字符串转化整型
												unsigned int intValuephase;
												std::stringstream sss;
												sss << std::hex << result;
												sss >> intValuephase;
											// std::cout << intValuephase << std::endl;
											// 整型转化浮点数
												float floatValue = static_cast<float>(intValuephase);
												floatValue = floatValue*3.1415*2/128;
												dataPtr->phase = floatValue;
											// std::cout << result << std::endl;
											// std::cout << intValuephase << std::endl;
											// std::cout << "Dec phase: " << floatValue << std::endl;
											
												for (int i = 0; i < 2; i++)
												{
													mediphase[i] = 0;
												}
											//将标签读取的相对时间存入datarecv.time 16进制char转化浮点数（单位ms）
												num3 = (unsigned int)(unsigned char)recv_buf2[17];
												num4 = (unsigned int)(unsigned char)recv_buf2[18];
												num5 = (unsigned int)(unsigned char)recv_buf2[19];
												num6 = (unsigned int)(unsigned char)recv_buf2[20];
												std::stringstream ssss;
												ssss << std::hex << std::setw(2) << std::setfill('0') << num3
													<< std::hex << std::setw(2) << std::setfill('0') << num4
													<< std::hex << std::setw(2) << std::setfill('0') << num5
													<< std::hex << std::setw(2) << std::setfill('0') << num6;

												std::string timestring = ssss.str();

											// std::cout << timestring << std::endl;
											// 16进制字符串转化整型
												unsigned int intValuetime;
												std::stringstream stime;
												stime << std::hex << timestring;
												stime >> intValuetime;

											// 整型转化浮点数
												float floatValuetime = static_cast<float>(intValuetime);
											// 浮点型转化msType
												msType mstypevaluuetime = std::chrono::milliseconds(int(floatValuetime));
												if(flag2 == 0)
												{
													dec_init_time = mstypevaluuetime;
													// std::cout << "Dec time: " << dataPtr->time << std::endl;
													flag2=1;
												}
												dataPtr->time = mstypevaluuetime-dec_init_time+beginTime;
												// std::cout << "Hex time: " << timestring << std::endl;
												
												dataFifo.push(dataPtr);
											//cout <<"reader:" << dataFifo.size() << endl;
											// times++;

        									//this_thread::sleep_for(chrono::milliseconds(50));

        									// if(times == 200){
           									// 	 cout<<200<<endl;
        									// }
											}
										
										}
									}

								}	

							}
						}	
					}

				}
			}
		}
		
		break;

	}
	//关闭套接字
	close(s_server);
	return;

}



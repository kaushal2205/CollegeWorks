#include<iostream>
#include <time.h>
#include <cstdlib>
#include<ctime>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/shm.h> 
#include <unistd.h>
#include<bits/stdc++.h>
#include "helper.h"
using namespace std;
int main()
{
    key_t key,key1,key2; 
    int msgid,msgid1; 
    key = ftok("notes", 65); 
    key1=ftok("data",65);
    key2=ftok("shmfile",65);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgid1 = msgget(key1, 0666 | IPC_CREAT); 
    int shmid = shmget(key2,sizeof(int),0666|IPC_CREAT);      
    msgctl(msgid, IPC_RMID, NULL);
    msgctl(msgid1, IPC_RMID, NULL);
    msgid= msgget(key, 0666 | IPC_CREAT);
    msgid1=msgget(key1, 0666 | IPC_CREAT); 
    mesg_buffer message;
    int *str = (int *) shmat(shmid,(void*)0,0);
    *str=1;
    shmdt(str);
     while(true){
        msgrcv(msgid,&message,sizeof(message),1,0);
        cout<<"Data Received is : "; 
        for(int i=0;i<32;i++){
            cout<<message.Frame.data[i];
        }
        for(int i=0;i<4;i++){
            cout<<message.Frame.crc[i];
        }
        cout<<endl;
        int *str = (int *) shmat(shmid,(void*)0,0);
        *str=0;
        cout<<"channel busy"<<endl;
        //cout<<*str<<endl;
        sleep(2);
        msgsnd(msgid1,&message,sizeof(message),0);
        *str=1;
        shmdt(str);
     }
    return 0;
}
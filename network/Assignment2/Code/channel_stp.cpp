#include<iostream>
#include <time.h>
#include <cstdlib>
#include<ctime>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <unistd.h>
#include<bits/stdc++.h>
using namespace std;

struct frame{
    int address;
    int sqno;
    char data[32];
    char crc[4];
};
struct mesg_buffer{
    long mes_type;
    frame Frame;
};
struct ackframe{
    int ack;
    int address;
};
struct ack{
    long mesg_type;
    ackframe frame;
} ackno;
void inserterror(char data[]){
    cout<<"recieved message :"<<endl;
    for(int i=0;i<32;i++)
    {
        cout<<data[i];
    }
    cout<<endl;
    srand(time(NULL));
    int x =rand()%5;
    if(x==2){
        int y= rand()%5;
        cout<<"Waiting....."<<endl;
        sleep(y);
    }else if(x==4){
        cout<<"error inserted"<<endl;
        int y=rand()%3;
        for(int i=0;i<y;i++){
            int idx=rand()%32;
            if(data[idx]=='1'){
                data[idx]='0';
            }else{
                data[idx]='1';
            }
        }
    }
}
int main()
{
    key_t key1,key2;
    int msgid1,msgid2;
    key1=ftok("notes",65);
    msgid1= msgget(key1, 0666 | IPC_CREAT); //from sender
    key2=ftok("data",65);
    msgid2=msgget(key2, 0666 | IPC_CREAT); 
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    msgid1= msgget(key1, 0666 | IPC_CREAT);
    msgid2=msgget(key2, 0666 | IPC_CREAT); 
    mesg_buffer message;
    ack ackno;
    while(true){
        msgrcv(msgid1,&message,sizeof(message),1,0);
        inserterror(message.Frame.data);
        msgsnd(msgid2,&message,sizeof(message),0);
        msgrcv(msgid2,&ackno,sizeof(ackno),5,0);
        srand(time(NULL));
        int x= rand()%10;
        if(x==5){
        int y= rand()%5;
        cout<<"Waiting....."<<endl;
        sleep(y);
        }
        msgsnd(msgid1,&ackno,sizeof(ackno),0);
    }

}
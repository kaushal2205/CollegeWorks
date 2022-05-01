#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <time.h>
#include <cstdlib>
#include<ctime>
#include<bits/stdc++.h>
#include<unistd.h>
// structure for message queue 
using namespace std;

class timer {
    private:
     unsigned long begTime;
    public:
     void start() {
      begTime = clock();
     }
   double elapsedTime() {
      return (double) (clock() - begTime) / CLOCKS_PER_SEC;
    }
   bool isTimeout(unsigned long seconds) {
      return elapsedTime()>=seconds;
     }
};

struct frame{
    int address;
    int sqno;
    char data[32];
    char crc[4];
};
struct mesg_buffer{
    long mes_type;
    frame Frame;
}message;
struct ackframe{
    int ack;
    int address;
};
struct ack{
    long mesg_type;
    ackframe frame;
} ackno;


void division(int temp[],int gen[],int n,int r)
{
 for(int i=0;i<n;i++)
 {
     if (gen[0]==temp[i])
     {
         for(int j=0,k=i;j<r+1;j++,k++)
             if(!(temp[k]^gen[j]))
                 temp[k]=0;
             else
                 temp[k]=1;
     } }}
int checkcrc(char data[],char crc[],int r)
{
    int gen[]={1,0,0,1,1};
    int n=32;
    int message[n+r],temp[n+r];
    for(int i=0;i<n;i++)
    {
         if(data[i]=='1')
           {
               message[i]=1;
               temp[i]=1;
           }
         else
         {
             message[i]=0;
             temp[i]=0;
         }
     }
     for(int i=0;i<r;i++)
    {
         if(crc[i]=='1')
           {
               message[n+i]=1;
               temp[n+i]=1;
           }
         else
         {
             message[n+i]=0;
             temp[n+i]=0;
         }
     }


     division(temp,gen,n,r);
     for(int i=0;i<r;i++)
    {
     if(temp[n+i])
     {
         cout<<"Error detected in received message."<<endl;
         return -1;
     } }
   cout<<"No error in received Message.\nReceived Message : ";
    for(int i=0;i<n;i++)
     cout<<message[i];
    cout<<endl;
    return 0;
}
int main() 
{ 
    int sender,reciever;
    cout<<"Enter your address"<<endl;
    cin>>reciever;
    cout<<"Enter the senders address"<<endl;
    cin>>sender;
    key_t key1,key2; 
    int msgid1,msgid2; 
  
   
    key1 = ftok("data", 65); 
    key2=ftok("notes",65);
  
    msgid1 = msgget(key1, 0666 | IPC_CREAT); 
    msgid2=msgget(key2,0666|IPC_CREAT);
    
    while(true){
        int curr=0;
        int ack=0;
        bool error=false;
        for(int i=0;i<4;i++){
            msgrcv(msgid1,&message,sizeof(message),1,0);
            if(message.Frame.address==reciever){
                if(error){
                    continue;
                }
                cout<<"Data Received is : "; 
                for(int i=0;i<32;i++){
                    cout<<message.Frame.data[i];
                }
                for(int i=0;i<4;i++){
                    cout<<message.Frame.crc[i];
                }
                    cout<<endl;   
                int check=checkcrc(message.Frame.data,message.Frame.crc,4);
                if(check==-1){
                    cout<<"error in packet no. "<<message.Frame.sqno<<endl;
                    error=true;
                    ack=curr+1;
                }else{
                    cout<<"Data packet "<<message.Frame.sqno<<" recieved"<<endl;
                    curr=curr+1;
                }
            }else{
                msgsnd(msgid1,&message,sizeof(mesg_buffer),0);
                i--;
            }
        }
        if(error==false){
        ackno.mesg_type=5;
        ackno.frame.address=sender;
        ackno.frame.ack=4;
        cout<<"Sending ackno "<<ackno.frame.ack<<endl;
        msgsnd(msgid1,&ackno,sizeof(ackno),0);
        }else{

                    ackno.mesg_type=5;
                    ackno.frame.address=sender;
                    ackno.frame.ack=ack;
                    cout<<"Sending ackno "<<ackno.frame.ack<<endl;
                    msgsnd(msgid1,&ackno,sizeof(ackno),0);
        }
    }
    
    msgctl(msgid1, IPC_RMID, NULL);
     msgctl(msgid2, IPC_RMID, NULL);
    return 0; 
} 
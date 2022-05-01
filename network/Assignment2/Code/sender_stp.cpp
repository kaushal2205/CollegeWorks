#include<iostream>
#include <time.h>
#include <cstdlib>
#include<ctime>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <unistd.h>
#include<bits/stdc++.h>
using namespace std;

class timer {
    private:
     unsigned long begTime;
    public:
     void start() {
      begTime = clock();
     }
    double elapsedTime() {
      return (double) (clock() - begTime)/CLOCKS_PER_SEC;
    }
   bool isTimeout(double seconds) {
      return elapsedTime()>=seconds;
     }
};

//need to do framming 
//frame me fixed use krenge no need to preamble and destination address
//just ek data part and one crc code;

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

vector<mesg_buffer> frames;
void truncdata(string filename,int size)
{
    fstream fout,fin;
    fin.open(filename,ios::in);
    string line;
    getline(fin,line);
    int len= line.size();
    if(len%size!=0)
    {
        int i=len%size;
        i=size-i;
           while(i>0)
           {
               line='0'+line;
               i--;
           }
    }
    fin.close();
    cout<<"The orginal data is: "<<line<<endl;
    fout.open(filename,ios::out);
    fout<<line<<endl;
    fout.close();
}

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
void checkcrc(char data[],char crc[],int r)
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
         return;
     } }
   cout<<"No error in received Message.\nReceived Message : ";
    for(int i=0;i<n;i++)
     cout<<message[i]<<" ";
    cout<<endl;
}
void crc(char data[],int n,char rem[])
{
    int r=5;
    int gen[r]={1,0,0,1,1};
    int message[n+r],temp[n+r];
    r--;
    for(int i=0;i<n;i++)
    {
         if(data[i]=='1')
           message[i]=1;
         else
         message[i]=0;
     }
    for(int i=0;i<r;i++)
     message[n+i] = 0;
    for(int i=0;i<n+r;i++)
     temp[i] = message[i];
    division(temp,gen,n,r);

    for(int i=0;i<r;i++)
    {
     if(temp[n+i]==1)
     {
         rem[i]='1';
     }
     else
     {
         rem[i]='0';
     }
     
     message[n+i] = temp[n+i];
    }
}

void createframe(int address){
    truncdata("data.txt",32);
        fstream fin;
        fin.open("data.txt",ios::in|ios::out);
        char data[32];
        char rem[4];
        while(fin.read((char *)&data,sizeof(data)))
        {
            mesg_buffer message;
            message.mes_type=1;
            message.Frame.sqno=0;
            strcpy(message.Frame.data,data);
            message.Frame.address=address;
            crc(data,32,rem);
            for(int i=0;i<4;i++)
            {
                message.Frame.crc[i]=rem[i];
            }
            frames.push_back(message);
        }
}

void send(int msgid,mesg_buffer itr){
    
    msgsnd(msgid, &itr, sizeof(itr), 0);  
    cout<<"Data sent is :- ";
    for(int i=0;i<32;i++){
        cout<<itr.Frame.data[i];
    } 
    cout<<endl;
}
int main(){
    timer t;
    int sender,reciever;
    cout<<"Enter your address"<<endl;
    cin>>sender;
    cout<<"Enter reciever's address"<<endl;
    cin>>reciever;
    createframe(reciever);
    key_t key,key1; 
    int msgid,msgid1; 
  
    key = ftok("notes", 65); 
    key1=ftok("data",65);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgid1 = msgget(key1, 0666 | IPC_CREAT); 
    
    double timeout=0.0003;
    bool curr=0;
    bool sucess;
    double avg=0.0;
    int count=0;

    for(auto itr:frames){
            
        sucess=false;
        itr.Frame.sqno=curr;
        t.start();
        while(sucess!=true){
            send(msgid,itr);
            sleep(5);
            if(t.isTimeout(timeout)){
                cout<<"Timeout"<<endl;
                //cout<<t.elapsedTime();
                t.start();
                continue;
            }
                    if(!t.isTimeout(timeout)){
                    msgrcv(msgid,&ackno,sizeof(ackno),5,0);
                    if(ackno.frame.address==sender&&ackno.frame.ack==!curr){
                        cout<<"Ackno recieved "<<ackno.frame.ack<<endl;
                        cout<<"rtt "<<t.elapsedTime()<<endl;
                        avg+=t.elapsedTime();
                        count++;
                        sucess=true;
                        curr=!curr;
                        cout<<"Data sent successfully"<<endl;
                        break;
                    }
                    else if(ackno.frame.address==sender&&ackno.frame.ack==curr) {
                        continue;
                    }
                    else{
                        msgsnd(msgid,&ackno,sizeof(ackno),0);
                    }
                    }else{
                        cout<<"Timeout"<<endl;
                        //cout<<t.elapsedTime();
                        t.start();
                        continue;
                    }
                }
            
            }
    cout<<"Average RTT:- "<<avg/count<<endl;
 return 0;
}
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

struct frame{
    int address;
    char data[32];
    char crc[4];
};
struct mesg_buffer{
    long mes_type;
    frame Frame;
};
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


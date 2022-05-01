/*all the scheme will take thefilename where data is present and convert it and send to channel.txt file
the inserterror function will insert error in the data and then send the data to the reciever.txt file 
the checker will the check the data and output the result if pass/ fail.
the truncdata will introduced redundant bits in case the length in not a multiple of size.
*/
#include<bits/stdc++.h>
using namespace std;
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

void inserterror(){
    cout<<"Enter the number of errors introduced by channel medium"<<endl;
    int nofe;
    cin>>nofe;
    fstream fin;
    fstream fout;
    fin.open("channel.txt",ios::in);
    fout.open("reciever.txt",ios::out);
    string line;
    while(!fin.eof())
    {
        getline(fin,line);
        int len=line.size();
        cout<<"The data after conversion is :- "<<line<<endl;
        for(int i=0;i<nofe;i++)
      {

         int pos=rand()%len;
         if(line[pos]=='1')
         {
             line[pos]='0';
         }else{
             line[pos]='1';
         }
        
      }
      cout<<"After the error by the medium :- "<<line<<endl;
      fout<<line;
    }
    fin.close();
    fout.close();
}

void checkvrc(){
    fstream fin;
    fin.open("reciever.txt",ios::in);
    char word[9];
    while(fin.read((char*)&word,sizeof(word)))
    {
        int count=0;
        for(int i=0;i<9;i++)
        {
            if(word[i]=='1')
            {
                count++;
            }
        }
        if(count%2!=0)
        {
            cout<<"found error by vertical redundancy checker"<<endl;
            return;
        }
    }
    cout<<"The data is correct and passed the vrc check"<<endl;
}
void vrc(string filename)
{
    //checks error detection by parity check
    truncdata(filename,8);
    cout<<"In the following scheme the data is checked using VRC :-"<<endl;
    fstream fin,fout;
    fin.open(filename,ios::in);
    fout.open("channel.txt",ios::out);
    char word[8];
    while(fin.read((char*)&word,sizeof(word)))
    {
        string wordout;
        int count=0;
        for(int i=0;i<8;i++)
        {
            if(word[i]=='1')
              count++;

           wordout+=word[i];
        }
        if(count%2==0)
        {
            wordout+='0';
        }
        else
        {
            wordout+='1';
        }
        fout<<wordout;
    }
    fin.close();
    fout.close();
    inserterror();
    checkvrc();
}

void checklrc()
{
    fstream fin;
    fin.open("reciever.txt",ios::in);
    char word[40];
    while(fin.read((char*)&word,sizeof(word)))
    {
        int count=0;
        for(int i=0;i<8;i++)
        {
            int count =0;
           for(int j=i;j<40;j=j+8)
           {
               if(word[j]=='1')
               {
                   count++;
               }
           }
        
         if(count%2!=0)
         {
            cout<<"found error by longitudnal redundancy checker"<<endl;
            return;
          }

        }
    }
    cout<<"The data is correct and passed the lrc check"<<endl;
}

void lrc(string filename)
{
truncdata(filename,32);
cout<<"In the following scheme the data is checked using LRC :-"<<endl;
fstream fin,fout;
fin.open(filename,ios::in);
fout.open("channel.txt",ios::out);
char word[32];
char wordout[40];
while(fin.read((char*)&word,sizeof(word)))
    {
        for(int i=0;i<8;i++)
        {
            int count=0;
            for(int j=i;j<32;j=j+8)
            {
                wordout[j]=word[j];
               if(word[j]=='1')
               count++;

             }
          if(count%2==0)
         {
            wordout[i+32]='0';
         }
        else
        {
            wordout[i+32]='1';
         }
      }
        fout.write((char*)&wordout,sizeof(wordout));
    }
    fin.close();
    fout.close();
    inserterror();
    checklrc();
}

void addbits(char sum[],char word2[])
{
    
    char carry='0';
        for(int i=7;i>=0;i--)
        {
            char b1=sum[i];
            char b2=word2[i];
            if(b1=='0'&&b2=='0'&&carry=='0')
            {
                sum[i]='0';
                carry='0';
            }
            else if(b1=='0'&&b2=='0'&&carry=='1')
            {
                sum[i]='1';
                carry='0';
            }
            else if(b1=='1'&&b2=='0'&&carry=='0')
            {
                sum[i]='1';
                carry='0';
            }
            else if(b1=='1'&&b2=='0'&&carry=='1')
            {
                sum[i]='0';
                carry='1';
            }else if(b1=='0'&&b2=='1'&&carry=='0')
            {
                sum[i]='1';
                carry='0';
            }
            else if(b1=='0'&&b2=='1'&&carry=='1')
            {
                sum[i]='0';
                carry='1';
            }
            else if(b1=='1'&&b2=='1'&&carry=='0')
            {
                sum[i]='0';
                carry='1';
            }
            else if(b1=='1'&&b2=='1'&&carry=='1')
            {
                sum[i]='1';
                carry='1';
            }
            else
            {

            }
        }
      if(carry=='1')
      {
          char word[]={'0','0','0','0','0','0','0','1'};
          addbits(sum,word);
      }  
}

void checksumChecker()
{
    fstream fin;
    fin.open("reciever.txt",ios::in);
    char sum[]={'0','0','0','0','0','0','0','0','0'};
    char word2[8];
    while(fin.read((char*)&word2,sizeof(word2)))
    {
        addbits(sum,word2);
    }
    for(int i=0;i<8;i++)
    {
        if(sum[i]!='1')
        {
           cout<<"There is a error present by checksum technique \n data not accepted"<<endl;
            return;
        }
    }
    cout<<"Data accepted by checksum "<<endl;
   fin.close();
}
void checksum(string filename)
{
    truncdata(filename,8);
    cout<<"In the following scheme the data is checked using Checksum :-"<<endl;
    fstream fin,fout;
    fin.open(filename,ios::in);
    fout.open("channel.txt",ios::out);
    char sum[]={'0','0','0','0','0','0','0','0','0'};
    char word2[8];
    while(fin.read((char*)&word2,sizeof(word2)))
    {
        addbits(sum,word2);
        for(int i=0;i<8;i++)
        fout<<word2[i];
    }
    for(int i=0;i<8;i++)
    {
        if(sum[i]=='1')
        {
            sum[i]='0';
        }else{
            sum[i]='1';
        }
    }
     cout<<"The checksum obtained is : ";
    for(int i=0;i<8;i++)
    {
        cout<<sum[i];
    }
    cout<<endl;
    for(int i=0;i<8;i++)
    fout<<sum[i];
    fin.close();
    fout.close();
    inserterror();
    checksumChecker();    
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

void checkcrc(int gen[],int r)
{
    fstream fin;
    fin.open("reciever.txt",ios::in);
    string line;
    getline(fin,line);
    cout<<"The recieved data is "<<line<<endl;
    int n=line.size()-r;
    int message[n+r],temp[n+r];
    for(int i=0;i<n+r;i++)
    {
         if(line[i]=='1')
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
void crc(string filename)
{
    cout<<"In the following scheme the data is checked using CRC :-"<<endl;
    fstream fin,fout;
    fin.open(filename,ios::in);
    fout.open("channel.txt",ios::out);
    string line;
    getline(fin,line);
    int n=line.size();
    cout<<"The entered data is "<<line<<endl;
    int r;
    cout<<"Enter the size of generetor"<<endl;
    cin>>r;
    int gen[r];
    int message[n+r],temp[n+r];
    cout<<"Enter the generator : "<<endl;
    for(int i=0;i<r;i++)
     cin>>gen[i];
    r--;
    for(int i=0;i<n;i++)
    {
         if(line[i]=='1')
           message[i]=1;
         else
         message[i]=0;
     }
    for(int i=0;i<r;i++)
     message[n+i] = 0;
    for(int i=0;i<n+r;i++)
     temp[i] = message[i];
    division(temp,gen,n,r);
    cout<<"The remained of CRC is :-  ";
    for(int i=0;i<r;i++)
    {
     cout<<temp[n+i]<<" ";
     message[n+i] = temp[n+i];
    }
    cout<<endl<<"Transmitted Message : ";
     for(int i=0;i<n+r;i++)
     cout<<message[i]<<" ";
     cout<<endl;
    for(int i=0;i<n+r;i++)
    {
        fout<<message[i];
    }
   fin.close();
   fout.close();
  inserterror();
   checkcrc(gen,r);
}
int main()
{
    string filename;
    cout<<"Enter the file name : ";
    cin>>filename;
    
    cout<<"Enter 1 to for only checksum technique"<<endl;
    cout<<"Enter 2 for only VRC technique"<<endl;
    cout<<"Enter 3 to check by all the techniques"<<endl;
    int choice;
    cin>>choice;
    if(choice==1)
    {
        checksum(filename);
    }
    else if(choice== 2)
    {
        vrc(filename);
    }else if(choice==3)
    {
        vrc(filename);
        checksum(filename);
        crc(filename);
        lrc(filename);
    }
    else
    {
        cout<<"wrong choice ";
    }
    return 0;
}
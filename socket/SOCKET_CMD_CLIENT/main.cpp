#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#define PORT 15001
#define BUFFER_SIZE 1024
using namespace std;
void Rec()
{
  SOCKET sock2;
  WSAData WSAData;
  SOCKADDR_IN ServerAddr;
  SOCKET msgsock;
  if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
  {
    //printf("Socket initialize fail!\n");
    exit(1);
  }
  if((sock2=socket(AF_INET,SOCK_STREAM,0))==ERROR)
  {
    //printf("Socket create fail!\n");
    WSACleanup();
    exit(1);
  }
  ServerAddr.sin_family=AF_INET;
  ServerAddr.sin_port=htons(8800);
  ServerAddr.sin_addr.s_addr=INADDR_ANY;
  if(bind(sock2,(LPSOCKADDR)&ServerAddr,sizeof(ServerAddr))==SOCKET_ERROR)
  {
    //printf("Bind fail!\n");
    closesocket(sock2);
    WSACleanup();
    exit(1);
  }
  //printf("Server Socket Port:%d\n",ntohs(ServerAddr.sin_port));
  if(listen(sock2,10)==SOCKET_ERROR)
  {
    //printf("Listen fail!\n");
    closesocket(sock2);
    WSACleanup();
    exit(1);
  }
  char buf[BUFFER_SIZE];
  if((msgsock=accept(sock2,(LPSOCKADDR)0,(int *)0))==INVALID_SOCKET)
    {
      //printf("Accept fail!\n");
      exit(1);
    }
    memset(buf,0,sizeof(buf));
    while(recv(msgsock,buf,BUFFER_SIZE,0))
    {
        cout<<buf<<endl;
        memset(buf,0,BUFFER_SIZE);
    }
    closesocket(sock2);
    WSACleanup();
}
SOCKET sen(char IP[])
{
        SOCKET sock;
        WSADATA WSAData;
        SOCKADDR_IN ClientAddr;
        if(WSAStartup(MAKEWORD(2,2),&WSAData)==SOCKET_ERROR)
        {
            cout<<"Socket initialize fail!"<<endl;
            exit(1);
        }
        if((sock=socket(AF_INET,SOCK_STREAM,0))==SOCKET_ERROR)
        {
            cout<<"Socket create fail!"<<endl;
            WSACleanup();
            system("pause");
            exit(1);
        }
        ClientAddr.sin_family=AF_INET;
        ClientAddr.sin_port=htons(PORT);
        ClientAddr.sin_addr.s_addr=inet_addr(IP);
        if(connect(sock,(LPSOCKADDR)&ClientAddr,sizeof(ClientAddr))==SOCKET_ERROR)
        {
            cout<<"Connect fail!"<<endl;
            closesocket(sock);
            WSACleanup();
            system("pause");
            exit(1);
        }
        return sock;
}
int main()
{
    char buf[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];
    int inputLen;
    char IP[100];
    SOCKET sock;
    char Point;
    cout<<"Would you get the serve IP?(Y or N)";
    Point=getchar();
    if(Point=='Y')
    {
        cout<<"Waiting for the conection..."<<endl;
        Rec();
    }
    cout<<"input IP:";
    cin>>IP;
    getchar();
    while(1)
    {
        inputLen=0;
        memset(buf,0,sizeof(buf));
        cout<<"Socket\\Client>";
        while((buf[inputLen]=getchar())!='\n')
        {
            inputLen++;
        }
        buf[inputLen]='\0';
        if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
        {
            cout<<"The End"<<endl;
        }
        sock=sen(IP);
        if(buf[0]=='s' && buf[1]=='e' && buf[2]=='n' && buf[3]=='d' )//send E:\\chain.txt
        {
            char a[50];
            int i=0;
            int by=0;
            while(buf[i]!='\0')
            {
                a[i]=buf[i];
                i++;
            }
            a[i]='\0';
            send(sock,buf,BUFFER_SIZE,0);
            FILE *f=fopen(&buf[5],"rb");
            int length=BUFFER_SIZE;
            memset(buf,0,BUFFER_SIZE);
            while(1)
            {
                length=fread(buf,sizeof(char),BUFFER_SIZE,f);
                by=by+length;
                if(length==0) break;
                cout<<"send:"<<length<<"B/s"<<endl;
                send(sock,buf,length,0);
                memset(buf,0,BUFFER_SIZE);
            }
            fclose(f);
            cout<<endl<<"send finished"<<endl<<"bytes:"<<by<<"B      "<<"file name: "<<&a[5]<<endl;
        }
        if(buf[0]=='r'&&buf[1]=='e'&&buf[2]=='c'&&buf[3]=='v')
        {
            int by=0;
            char a[20]="E:\\";
            int i=0,j=0;
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
            send(sock,buf,BUFFER_SIZE,0);
            FILE *f=fopen(a,"wb");
            int length=0;
            memset(buf,0,BUFFER_SIZE);
            while(1)
            {
                length=recv(sock,buf,BUFFER_SIZE,0);
                by=by+length;
                if(length<BUFFER_SIZE) break;
                fwrite(buf,sizeof(char),length,f);
                memset(buf,0,BUFFER_SIZE);
                cout<<"recvice:"<<length<<"B/s"<<endl;
                length=0;
            }
            fclose(f);
            cout<<endl<<"recive finished"<<endl<<"bytes:"<<by<<"B      "<<"file name: "<<a<<endl;
        }
        else
        {
            send(sock,buf,BUFFER_SIZE,0);
            char ch;
            if(buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t')
            {
                closesocket(sock);
                WSACleanup();
                return 0;
            }
            cout<<"Would want to get the order information?(Y or N):";
            ch=getchar();
            if(ch=='Y')
            {
                memset(buf2,0,sizeof(buf2));
                recv(sock,buf2,BUFFER_SIZE,0);
                cout<<buf2<<endl;
            }
            getchar();
        }
        closesocket(sock);
        WSACleanup();
    }
    return 0;
}

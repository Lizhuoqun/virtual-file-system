//Name: Li,Zhuoqun
//Project: PA-2 (Programming)
//Instructor: Feng Chen
//Class: cs7103-au21
//Login: cs710316

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"
#include <iostream>
#include <string>


using std::cout; using std::cin;
using std::endl; using std::string;

void sendfile(FILE *fp, int sockfd);
void sendpara(char *para,int sockfd);
char * recieve_para(int connfd);
void writefile(int sockfd, FILE *fp);

int main(int argc, char* argv[])
{

    //CREATE TCP SOCKET
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Can't allocate sockfd");
        exit(1);
    }

    //SET addr
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, argv[1], &serveraddr.sin_addr) < 0)
    {
        perror("IPaddress Convert Error");
        exit(1);
    }

    //connect
    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Connect Error");
        exit(1);
    }

    char *owner;//owner
    string onr;
    cout<<"input the user"<<endl;
    cin>>onr;//filename
    owner=(char *)onr.c_str();
    sendpara(owner,sockfd);

    char *feedback= recieve_para(sockfd);
    string feed=feedback;

    if (feed=="True") {}
    else{
        perror("wrong user!");
        exit(1);
    }
        while (true) {
            string type;
            cout << "input the operation type" << endl;
            cin >> type;
            if (type == "end") {
                //close

                close(sockfd);

                return 0;
            }
            if (type == "send") {

                char *type_name = (char *) type.c_str();//operation type

                char *long_name;
                string ln;
                cout << "input the addr and name of file you want send " << endl;
                cin >> ln;//filename
                long_name = (char *) ln.c_str();
                char *filename = basename(long_name);


                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of file you want add in the directory " << endl;
                cin >> va;//filename
                virtual_addr = (char *) va.c_str();


                string zipfile;
                zipfile = filename;
                string s = "zip temp.zip " + zipfile;
                const char *sys = s.c_str();
                system(sys);


                sendpara(type_name, sockfd);

                sendpara(virtual_addr, sockfd);
                sendpara(filename, sockfd);

                //open
                FILE *fp = fopen("temp.zip", "rb");
                if (fp == NULL) {
                    perror("Can't open file");
                    exit(1);
                }

                //read and send and delete compress
                sendfile(fp, sockfd);
                puts("Send Success");
                system("rm -f temp.zip");

                fclose(fp);
                sleep(3);


            }
            else if (type=="list_by_owner"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);
                char * result= recieve_para(sockfd);
                string s=result;
                cout<<s<<endl;
            }
            else if (type=="list_by_directory"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);

                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of directory you want to list " << endl;
                cin >> va;
                virtual_addr = (char *) va.c_str();
                sendpara(virtual_addr, sockfd);

                char * result= recieve_para(sockfd);
                string s=result;
                cout<<s<<endl;
            }
            else if (type=="list_info"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);

                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of file you want to list " << endl;
                cin >> va;
                virtual_addr = (char *) va.c_str();
                sendpara(virtual_addr, sockfd);

                char *name;//virtual_addr
                string fn;
                cout << "input the name of file you want to list " << endl;
                cin >> fn;
                name = (char *) fn.c_str();
                sendpara(name, sockfd);

                char * result= recieve_para(sockfd);
                string s=result;
                cout<<s<<endl;
            }

            else if (type=="remove"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);

                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of file or directory you want to remove " << endl;
                cin >> va;
                virtual_addr = (char *) va.c_str();
                sendpara(virtual_addr, sockfd);

                char *name;//virtual_addr
                string fn;
                cout << "input the name of file or directory you want to remove " << endl;
                cin >> fn;
                name = (char *) fn.c_str();
                sendpara(name, sockfd);
            }
            else if (type=="create"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);

                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of  directory you want to create " << endl;
                cin >> va;
                virtual_addr = (char *) va.c_str();
                sendpara(virtual_addr, sockfd);

                char *name;//virtual_addr
                string fn;
                cout << "input the name of  directory you want to create " << endl;
                cin >> fn;
                name = (char *) fn.c_str();
                sendpara(name, sockfd);
            }

            else if (type=="download"){
                char *type_name = (char *) type.c_str();//operation type
                sendpara(type_name, sockfd);

                char *virtual_addr;//virtual_addr
                string va;
                cout << "input the addr of  directory you want to download " << endl;
                cin >> va;
                virtual_addr = (char *) va.c_str();
                sendpara(virtual_addr, sockfd);

                char *name;//name
                string fn;
                cout << "input the name of  directory you want to download " << endl;
                cin >> fn;
                name = (char *) fn.c_str();
                sendpara(name, sockfd);


                FILE *fp = fopen(name, "wb");
                if (fp == NULL)
                {
                    perror("Can't open file");
                    exit(1);
                }


                char addr[INET_ADDRSTRLEN];
                //printf("Start receive file: %s from %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));

                writefile(sockfd, fp);

                puts("Receive Success");

                fclose(fp);
            }

            else {
                perror("wrong operation type");
                //exit(1);
            }
        }



}

void sendfile(FILE *fp, int sockfd)
{
    int n;
    char sendline[MAX_LINE] = {0};
    if ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0)
    {
        if (n != MAX_LINE && ferror(fp))
        {
            perror("Read File Error");
            exit(1);
        }


        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("Can't send file");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE);
    }
}

void sendpara(char *para,int sockfd){
    if (para == NULL)
    {
        perror("Can't get parameters");
        exit(1);
    }
    char buff[BUFFSIZE] = {0};
    strncpy(buff, para, strlen(para));
    if (send(sockfd, buff, BUFFSIZE, 0) == -1)
    {
        perror("Can't send parameters");
        exit(1);
    }

}

char *recieve_para(int connfd){
    char para[BUFFSIZE] ;
    if (recv(connfd, para, BUFFSIZE, 0) == -1)
    {
        perror("Can't receive");
        exit(1);
    }
    char *result;
    result=para;
    return result;
}

void writefile(int sockfd, FILE *fp)
{
    ssize_t n;
    char buff[MAX_LINE] = {0};
    if ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0)
    {
        if (n == -1)
        {
            perror("Receive File Error");
            exit(1);
        }


        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
    }
}
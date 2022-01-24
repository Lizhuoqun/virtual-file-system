//
// Created by lizhuoqun on 11/15/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"
#include "tree.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;


void sendfile(FILE *fp, int sockfd);
void writefile(int sockfd, FILE *fp);
char * recieve_para(int connfd);
bool check_user(string user );
void sendpara(char *para,int sockfd);
vector<string> split(const string &str, const string &pattern);

int main(int argc, char *argv[])
{
    //create root
    MNode node;
    MTree tree;
    node.Parent= nullptr;
    node.name ="root";
    //node.owner="aaabbb";
    node.if_directory= true;
    tree.init(&node);
    MNode node2,node3,node4,node5,node6,node7,node8,node9,node10,node11;
    MNode node1[10]={node2,node3,node4,node5,node6,node7,node8,node9,node10,node11};

    //create tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Can't allocate sockfd");
        exit(1);
    }


    struct sockaddr_in clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);


    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Bind Error");
        exit(1);
    }


    if (listen(sockfd, LINSTENPORT) == -1)
    {
        perror("Listen Error");
        exit(1);
    }


    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen); //已连接套接字
    if (connfd == -1)
    {
        perror("Connect Error");
        exit(1);
    }
    close(sockfd);


    //check owner name
    char *ownername;
    ownername= recieve_para(connfd);
    string on;
    on=ownername;
    bool user_flag;
    user_flag=check_user(on);
    if(user_flag){
        string t="True";
        char* feedback=(char *)t.c_str();
        sendpara(feedback,connfd);
    }
    if (!user_flag)
    {
        string f="False";
        char * feedback=(char *)f.c_str();
        sendpara(feedback,connfd);
        close(sockfd);
        perror("Wrong password");
        exit(1);

    }
    int count=0;
    while (true){
    char *type_name;
    type_name=recieve_para(connfd);
    string tn;
    tn=type_name;

    if (tn=="send"){

        // get vitrual address
        char *v_addr;
        v_addr= recieve_para(connfd);
        string va;
        va=v_addr;

        //get filename
        char *filename;
        filename=recieve_para(connfd);
        string f=filename;
        string tmp=f+".zip";
        char *after_filename;
        after_filename=const_cast<char*>(tmp.c_str());

        //add to the tree

        MNode temp_node;
        temp_node.name =f;
        temp_node.owner=on,
        temp_node.true_addr=after_filename;
        temp_node.if_directory= false;

        node1[count]=temp_node;

        vector<string> split_s= split(va,"/");//format: root/directory1/
        tree.add_node(&(node1[count]),split_s);

        //create file
        FILE *fp = fopen(after_filename, "wb");
        if (fp == NULL)
        {
            perror("Can't open file");
            exit(1);
        }


        //cout<<"add to the tree"<<endl;
        //write to the file
        char addr[INET_ADDRSTRLEN];
        printf("Start receive file: %s from %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));

        writefile(connfd, fp);

        puts("Receive Success");
//        string notice="recieve success";
//        cout<<notice;
//
        fclose(fp);

        count=count+1;
        }
    else if(tn=="list_by_owner"){
        string lo_result=tree.all_file_for_owner(on);
        //cout<<lo_result;
        char *lor=(char *)lo_result.c_str();

        sendpara(lor,connfd);
    }
    else if(tn=="list_by_directory"){
        char * rec= recieve_para(connfd);
        string dir=rec;
        vector<string> split_s= split(rec,"/");
        string result=tree.list_directory(split_s);


        char *lor=(char *)result.c_str();
        sendpara(lor,connfd);
    }

    else if(tn=="list_info"){
        char * rec= recieve_para(connfd);
        string dir=rec;
        vector<string> split_s= split(rec,"/");

        char *name= recieve_para(connfd);
        string n=name;

        string result=tree.find_file_info(n,split_s);

        char *lor=(char *)result.c_str();
        sendpara(lor,connfd);
    }

    else if(tn=="remove"){
        char * rec= recieve_para(connfd);
        string dir=rec;
        vector<string> split_s= split(rec,"/");

        char *name= recieve_para(connfd);
        string n=name;

        tree.del_node(n,split_s);

    }

    else if(tn=="create"){
        char * rec= recieve_para(connfd);
        string dir=rec;
        vector<string> split_s= split(rec,"/");

        char *name= recieve_para(connfd);
        string n=name;

        MNode temp_node;
        temp_node.name =n;
        temp_node.owner=on,

        temp_node.if_directory= true;

        node1[count]=temp_node;

        tree.add_node(&(node1[count]),split_s);
        count=count+1;

    }

    else if(tn=="download"){
        char * rec= recieve_para(connfd);
        string dir=rec;
        vector<string> split_s= split(rec,"/");

        char *name= recieve_para(connfd);
        string n=name;

        string true_addr;
        true_addr=tree.find_true_addr(n,split_s);



        string s = "unzip " + true_addr;
        const char *sys = s.c_str();
        system(sys);

        //open
        FILE *fp = fopen(name, "rb");
        if (fp == NULL) {
            perror("Can't open file");
            exit(1);
        }

        //read and send and delete compress
        sendfile(fp, connfd);
        puts("Send Success");
        fclose(fp);

        string q=name;
        string s2="rm -f "+q;
        const char *sys2 = s2.c_str();
        system(sys2);



    }

    else{
        perror("No such operation type");
        exit(1);
    }

    }





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

bool check_user(string user){

  string data;
  ifstream  infile;
  infile.open("users.txt");
  //infile>>data;

//  if (data==user){
//      return true;
//  }
  while(getline(infile,data)){
      if (data==user){
	  infile.close();
          return true;
      }
  }
   infile.close();
   return false;

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

vector<string> split(const string &str, const string &pattern)
{
    vector<string> res;
    string strs = str ;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);

        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }

    return res;
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


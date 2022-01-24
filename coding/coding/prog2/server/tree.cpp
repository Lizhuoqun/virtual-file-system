//
// Created by lizhuoqun on 11/27/21.
//

#include <fstream>
#include "tree.h"
void MTree::init(MNode *root) { this->root = root; }

void MTree::putChild(MNode *node, MNode *parent) {
    parent->children.push_back(node);
    node->Parent = parent;
}

void MTree::del_child(MNode* parent,string del_name){
    for(int i=0;i<parent->children.size();i++){
        if(parent->children[i]->name==del_name){
            if(parent->children.size()> 1){
                auto iter=parent->children.erase(parent->children.begin()+i);
            }
            else{
                parent->children.clear();
            }
        }
    }
}

void MTree::putChildren(vector<MNode *> nodes, MNode *parent) {
    for (int i = 0; i < nodes.size(); ++i) {
        putChild(nodes[i], parent);
    }
}

string MTree::tranversal() {
    string s;
    s=this->tranversal(this->root);
    return s;
}



string MTree::tranversal(MNode *root) {
    string s="";
    vector<MNode *> nodes = root->children;
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i]->children.size() > 0)
            s=s+tranversal(nodes[i])+",";
        else
//            cout << nodes[i]->name << ",";
            s=s+nodes[i]->name+",";
    }
//    cout << root->name << ",";
    s=s+root->name;
    return s;
}



void MTree::add_node( MNode *node,vector<string> s){
    if (s[0]!="root"){
            perror("Not from root");
            exit(1);
    }
    MNode *now_node;
    now_node=this->root;
    for(int i=1;i<s.size();i++){
        for(int j=0;j<now_node->children.size();j++){
            if(now_node->children[j]->name==s[i]){
                now_node=now_node->children[j];
                break;
            }
        }
    }
    this->putChild(node,now_node);

}

void MTree::del_node(string node_name,vector<string> s){
    if (s[0]!="root"){
        perror("Not from root");
        exit(1);
    }
    MNode *now_node;
    now_node= this->root;
    for(int i=1;i<s.size();i++){
        for(int j=0;j<now_node->children.size();j++){
            if(now_node->children[j]->name==s[i]){
                now_node=now_node->children[j];
                break;
            }
        }
    }
    this->del_child(now_node,node_name);
}

string MTree::list_directory(vector<string> s){
    if (s[0]!="root"){
        perror("Not from root");
        exit(1);
    }
    MNode *now_node;
    now_node= this->root;
    for(int i=1;i<s.size();i++){
        for(int j=0;j<now_node->children.size();j++){
            if(now_node->children[j]->name==s[i]){
                now_node=now_node->children[j];
                break;
            }
        }
    }
    string r=this->tranversal(now_node);
    return r;
}

string MTree::find_file_info(string node_name,vector<string> s){
    if (s[0]!="root"){
        perror("Not from root");
        exit(1);
    }
    MNode *now_node;
    now_node= this->root;
    for(int i=1;i<s.size();i++){
        for(int j=0;j<now_node->children.size();j++){
            if(now_node->children[j]->name==s[i]){
                now_node=now_node->children[j];
                break;
            }
        }
    }
    string rs="";
    vector<MNode *> nodes = now_node->children;
    for (int i = 0; i < nodes.size(); ++i) {

        if (nodes[i]->name==node_name){
            if(!nodes[i]->if_directory){
            rs="name:"+nodes[i]->name+",owner:"+nodes[i]->owner;}
            else{
                perror("not a file");
                //exit(1);
            }
        }

    }
    return rs;

}

string MTree::find_true_addr(string node_name,vector<string> s){
    if (s[0]!="root"){
        perror("Not from root");
        exit(1);
    }
    MNode *now_node;
    now_node= this->root;
    for(int i=1;i<s.size();i++){
        for(int j=0;j<now_node->children.size();j++){
            if(now_node->children[j]->name==s[i]){
                now_node=now_node->children[j];
                break;
            }
        }
    }
    string rs="";
    vector<MNode *> nodes = now_node->children;
    for (int i = 0; i < nodes.size(); ++i) {

        if (nodes[i]->name==node_name){
            if(!nodes[i]->if_directory){
                rs=nodes[i]->true_addr;
            }
            else{
                perror("not a file");
                //exit(1);
            }
        }

    }
    return rs;

}

string MTree::all_file_for_owner(string owner_name){
    string s=owner_name+":"+this->tranversal();
    return s;
}

int MTree::getMaxDepth(MNode *root,vector<MNode*> nodes) {
    auto iResult = 0;

    return iResult;
}


//vector<string> split(const string &str, const string &pattern)
//{
//    vector<string> res;
//    string strs = str ;
//    size_t pos = strs.find(pattern);
//
//    while(pos != strs.npos)
//    {
//        string temp = strs.substr(0, pos);
//        res.push_back(temp);
//
//        strs = strs.substr(pos+1, strs.size());
//        pos = strs.find(pattern);
//    }
//
//    return res;
//}
//
//
//int main(){
//    MNode node;
//    MTree tree;
//    node.Parent= nullptr;
//    node.name ="root";
//    node.owner="aaabbb";
//    node.if_directory= true;
//    tree.init(&node);
//
//    MNode node2,node3,node4;
//    node2.name ="directory1",
//    node2.owner="aaabbb";
//    node2.if_directory= true;
//    node3.name ="a.txt",
//    node3.owner="kkk",
//    node3.if_directory= false;
//    node3.size="10";
//    node4.name ="directory2";
////    tree.putChild(&node2,&node);
////
////
////    tree.putChild(&node3,&node);
////    tree.del_child(&node,"a.txt");
//
//
//
//
//
//string s="root/";
//vector<string> split_s= split(s,"/");
//tree.add_node(&node3,split_s);
//tree.add_node(&node4,split_s);
//tree.del_node("a.txt",split_s);
//string rs=tree.tranversal();
////tree.del_node("a.txt",split_s);
////string list_info=tree.list_directory(split_s);
////string rs=tree.find_file_info("a.txt",split_s);
//
////cout<<list_info;
//cout<<rs;
////string r=tree.tranversal();
////cout<<r;
//
//
//}
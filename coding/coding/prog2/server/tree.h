//
// Created by lizhuoqun on 11/27/21.
//

#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include <iostream>
#include <vector>
using namespace std;


typedef struct MNode {
    string name;
    vector<MNode*> children;
    MNode *Parent;
    //other feature of directory tree
    bool if_directory;
    string owner;
    string size;
    string true_addr;

} MNode;

class MTree {
private:
    MNode *root;
public:
    void init(MNode *root);
    void putChild(MNode* node,MNode* parent);
    void del_child(MNode* parent,string del_name);
    void putChildren(vector<MNode*> nodes, MNode *parent);
    string tranversal(MNode *root);
    string tranversal();
    int getMaxDepth(MNode *root,vector<MNode*> nodes);
    void add_node(MNode *node,vector<string> s);
    void del_node(string node_name,vector<string> s);
    string list_directory(vector<string> s);
    string find_file_info(string node_name,vector<string> s);
    string find_true_addr(string node_name,vector<string> s);
    string all_file_for_owner(string owner_name);
};


#endif //UNTITLED1_TREE_H

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

enum NodeType { DIR, FILE_NODE };

struct Node {
    std::string name;
    NodeType type;
    std::string content;
    Node *parent, *child, *sibling;

    Node(std::string n, NodeType t, Node* p) 
        : name(n), type(t), parent(p), child(nullptr), sibling(nullptr) {}
};

class FileSystem {
public:
    FileSystem();
    ~FileSystem();

    // Navigation and Information
    void ls();
    void cd(std::string name);
    std::string get_path();
    void find(std::string name);

    // Manipulation
    void mkdir(std::string name);
    void touch(std::string name, std::string content);
    void rm(std::string name);
    void cp(std::string src, std::string dest);
    void mv(std::string src, std::string dest);

    // Persistence
    void save(std::string filename);
    void load(std::string filename);

private:
    Node* root;
    Node* pwd;

    // Internal Helpers
    Node* find_in_node(Node* parent_node, std::string name);
    void detach_node(Node* target);
    Node* clone_node(Node* source, Node* new_parent);
    void delete_recursive(Node* node);
    void save_recursive(Node* curr, int depth, std::ofstream& out);
    void search_recursive(Node* current, std::string target, std::string path);
};

#endif
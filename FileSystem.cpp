#include "FileSystem.h"

FileSystem::FileSystem() {
    root = new Node("/", DIR, nullptr);
    pwd = root;
}

FileSystem::~FileSystem() {
    delete_recursive(root);
}

void FileSystem::delete_recursive(Node* node) {
    if (!node) return;
    delete_recursive(node->child);
    delete_recursive(node->sibling);
    delete node;
}

Node* FileSystem::find_in_node(Node* parent_node, std::string name) {
    Node* temp = parent_node->child;
    while (temp) {
        if (temp->name == name) return temp;
        temp = temp->sibling;
    }
    return nullptr;
}

void FileSystem::mkdir(std::string name) {
    if (find_in_node(pwd, name)) return;
    Node* newNode = new Node(name, DIR, pwd);
    if (!pwd->child) pwd->child = newNode;
    else {
        Node* temp = pwd->child;
        while (temp->sibling) temp = temp->sibling;
        temp->sibling = newNode;
    }
}

void FileSystem::touch(std::string name, std::string content) {
    Node* newNode = new Node(name, FILE_NODE, pwd);
    newNode->content = content;
    if (!pwd->child) pwd->child = newNode;
    else {
        Node* temp = pwd->child;
        while (temp->sibling) temp = temp->sibling;
        temp->sibling = newNode;
    }
}

void FileSystem::ls() {
    Node* temp = pwd->child;
    while (temp) {
        std::cout << (temp->type == DIR ? "[DIR]  " : "[FILE] ") << temp->name << "\n";
        temp = temp->sibling;
    }
}

void FileSystem::cd(std::string name) {
    if (name == "..") {
        if (pwd->parent) pwd = pwd->parent;
    } else {
        Node* target = find_in_node(pwd, name);
        if (target && target->type == DIR) pwd = target;
        else std::cout << "Directory not found.\n";
    }
}

void FileSystem::detach_node(Node* target) {
    if (!target || !target->parent) return;
    Node* p = target->parent;
    if (p->child == target) p->child = target->sibling;
    else {
        Node* prev = p->child;
        while (prev && prev->sibling != target) prev = prev->sibling;
        if (prev) prev->sibling = target->sibling;
    }
    target->sibling = nullptr;
}

void FileSystem::rm(std::string name) {
    Node* target = find_in_node(pwd, name);
    if (!target) return;
    detach_node(target);
    target->sibling = nullptr; // Isolate from sibling chain
    delete_recursive(target->child);
    delete target;
}

void FileSystem::save(std::string filename) {
    std::ofstream out(filename);
    if (out.is_open()) {
        save_recursive(root->child, 0, out);
        out.close();
        std::cout << "Saved to " << filename << "\n";
    }
}

void FileSystem::save_recursive(Node* curr, int depth, std::ofstream& out) {
    if (!curr) return;
    out << depth << " " << curr->type << " " << curr->name << " " 
        << curr->content.length() << " " << curr->content << "\n";
    save_recursive(curr->child, depth + 1, out);
    save_recursive(curr->sibling, depth, out);
}

std::string FileSystem::get_path() {
    std::string path = "";
    Node* temp = pwd;
    while (temp != root) {
        path = "/" + temp->name + path;
        temp = temp->parent;
    }
    return path == "" ? "/" : path;
}
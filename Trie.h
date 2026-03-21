#include <unordered_map>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
};

class Autocomplete {
    TrieNode* root;
public:
    Autocomplete() { root = new TrieNode(); }

    void insert(std::string word) {
        TrieNode* curr = root;
        for (char ch : word) {
            if (!curr->children.count(ch)) curr->children[ch] = new TrieNode();
            curr = curr->children[ch];
        }
        curr->isEndOfWord = true;
    }

    // Returns a suggestion if a unique prefix matches
    std::string getSuggestion(std::string prefix) {
        TrieNode* curr = root;
        for (char ch : prefix) {
            if (!curr->children.count(ch)) return "";
            curr = curr->children[ch];
        }
        // Simplified: find the first available completion
        std::string res = prefix;
        while (curr && curr->children.size() == 1 && !curr->isEndOfWord) {
            auto it = curr->children.begin();
            res += it->first;
            curr = it->second;
        }
        return (res == prefix) ? "" : res;
    }
};
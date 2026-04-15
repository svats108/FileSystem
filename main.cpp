#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include "FileSystem.h"
#include "Trie.h"

// Helper to read raw characters from terminal
char getch() {
    char buf = 0;
    struct termios old = {0};
    tcgetattr(0, &old);
    struct termios current = old;
    current.c_lflag &= ~ICANON; // Disable line buffering
    current.c_lflag &= ~ECHO;   // Disable local echo
    tcsetattr(0, TCSANOW, &current);
    read(0, &buf, 1);
    tcsetattr(0, TCSANOW, &old);
    return buf;
}

int main() {
    FileSystem gh;
    Autocomplete ac;
    
    // Seed with commands
    std::vector<std::string> commands = {"mkdir", "touch", "ls", "cd", "rm", "save", "exit", "help"};
    for (auto& c : commands) ac.insert(c);

    std::string input = "";

    while (true) {
        // \r moves cursor to start of line, std::flush ensures immediate print
        std::cout << "\r\033[Kgh-lite:" << gh.get_path() << "$ " << input << std::flush;
        
        char c = getch();

        if (c == '\n') { // ENTER
            std::cout << "\n";
            if (input == "exit") break;
            
            input = ""; 
        } 
        else if (c == 9) { // TAB
            std::string suggestion = ac.getSuggestion(input);
            if (!suggestion.empty()) input = suggestion;
        } 
        else if (c == 127) { // BACKSPACE (Mac/Linux)
            if (!input.empty()) input.pop_back();
        } 
        else {
            input += c;
        }
    }
    return 0;
}

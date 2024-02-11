#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <sstream>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>

bool exit_program = 0;
std::string usr_name;
const char *help_list = "Help List:\n  ls                  View files/folders on directory\n  cd                  Changes the current directory\n  clear               Clear terminal\n  title               Set the title of window\n  help                Show this list\n  print               Print a message on terminal\n  exit                Close terminal\n  extern_             Execute an external function\n";

void ls_cmd();                                          //
void cd_cmd(std::string path);                          //
void help_cmd();                                        //
void clear_cmd();                                       //
void exit_cmd();                                        //
void title_cmd(std::vector<std::string> tokens);        //
void print_cmd(std::vector<std::string> tokens);        //
void extern_function(std::vector<std::string> command); //
void breakln();

bool manager(std::string command) {
    // Criar um stringstream a partir da string de entrada
    std::stringstream iss(command);

    std::vector<std::string> tokens;
    std::string token;

    // Extrair cada palavra da string
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens[0] == "ls")         ls_cmd();
    else if (tokens[0] == "cd")    cd_cmd(tokens[1]);
    else if (tokens[0] == "help")  help_cmd();
    else if (tokens[0] == "clear") clear_cmd();
    else if (tokens[0] == "exit")  exit_cmd();
    else if (tokens[0] == "title") title_cmd(tokens);
    else if (tokens[0] == "print") print_cmd(tokens);
    else if (tokens[0] == "extern_") {
        //elimina o primeiro token da lista
        tokens.erase(tokens.begin());
        extern_function(tokens);
    } else {
        std::cerr << "Could not find '" << tokens[0] << "' in the command list\n";
    }
    breakln();

    return false;
}

int main() {
    // Aqui eh obtido o nome do usuario que esta executando o programa
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    char *root_name = pw->pw_name;

    // Cria um usuario host do terminal
    std::cout << "Enter your user name: ";
    std::getline(std::cin, usr_name);

    while (!exit_program) {
        char path[(unsigned short)2000];

        if (getcwd(path, sizeof(path)) == NULL) {
            std::cerr << "Error getting the working directory\n";
        }

        // troca o nome do usuario que executa o programa para o nome do usuario host do programa (faz sentido?)
        char* new_path = std::strstr(path, root_name);
        std::string str(new_path);
        
        std::string bar = "/";
        std::string usr_path = usr_name + bar;
        if (new_path != nullptr) {
            std::memcpy(new_path, usr_path.c_str(), usr_path.size() - 1);
        }

        std::cout << usr_name << "@root [" << path << "]\n$ ";
        std::string command;
        std::getline(std::cin, command);

        bool exit = manager(command);

        if (exit) return 0;
    }

    return 0;
}
void ls_cmd() {
    std::system("ls");
}
void cd_cmd(std::string pathL) {
    // Isso funciona apenas para sistemas Unix-like, como linux ou macOS
    const char* path = pathL.c_str();

    if (chdir(path) == -1) {
        std::cerr << "Error changing working directory\n";
    }
}
void help_cmd() {
    std::cout << help_list;
}
void clear_cmd() {
    std::system("clear");
}
void exit_cmd() {
    std::cout << "Shutdown...";
    exit(0);
}
void title_cmd(std::vector<std::string> tokens) {
    try {
        std::string title = "printf '\033]2;";
        for (const auto& i : tokens) {
            title += i + " ";
        }
        title += "\033\\'";

        std::system(title.c_str());
    } catch (const std::exception e) {
        std::cerr << e.what();
    }

    std::cout << "new title\n";
}
void print_cmd(std::vector<std::string> tokens) {
    int start_from = 1;
    bool special   = false;
    std::string out;
    std::istringstream iss(tokens[1]);
    int number;

    if (iss >> number) {
        start_from = 2;
        special    = true;
    }
    
    for (int i = start_from; i < (int)tokens.size(); i++) {
        out += tokens[i] + " ";
    }

    if (special) {
        std::string code = std::to_string(number);
        char* prefix    = (char*)"echo '\e[";
        char* char_code = (char*)code.c_str();
        char* text      = (char*)out.c_str();
        char* sufix     = (char*)"\e[0m'";

        int total_size = strlen(prefix) + strlen(char_code) + strlen(text) + strlen(sufix) + 1;

        char* end_command = new char[total_size];
        strcat(char_code, "m");
        strcpy(end_command, prefix);
        strcat(end_command, char_code);
        strcat(end_command, text);
        strcat(end_command, sufix);
        std::system(end_command);
    } else {
        std::cout << out;
    }
    std::cout << "\n";
};

void extern_function(std::vector<std::string> command) {
    std::string cmd;
    for (const auto& w : command) {
        cmd.append(w);
        cmd.append(" ");
    }
    std::system(cmd.c_str());
}

void breakln() {
    std::cout << "\n";
}
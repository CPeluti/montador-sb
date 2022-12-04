#include <bits/stdc++.h>
#include "commands/comando.h"
using namespace std;

map<string, Comando> initialize_commands(){
    map<string, Comando> commands;
    Comando diretiva(0,0);
    Comando variables(0,1);
    Comando add(1,2);
    Comando sub(2,2);
    Comando mul(3,2);
    Comando div(4,2);
    Comando jmp(5,2);
    Comando jmpn(6,2);
    Comando jmpp(7,2);
    Comando jmpz(8,2);
    Comando copy(9,3);
    Comando load(10,2);
    Comando store(11,2);
    Comando input(12,2);
    Comando output(13,2);
    Comando stop(14,1);
    commands["const"] = variables;
    commands["space"] = variables;
    commands["section"] = diretiva;
    commands["add"] = add;
    commands["sub"] = sub;
    commands["mult"] = mul;
    commands["div"] = div;
    commands["jmp"] = jmp;
    commands["jmpn"] = jmpn;
    commands["jmpp"] = jmpp;
    commands["jmpz"] = jmpz;
    commands["copy"] = copy;
    commands["load"] = load;
    commands["store"] = store;
    commands["input"] = input;
    commands["output"] = output;
    commands["stop"] = stop;
    return commands;
}

vector<string> split(string str){
    vector<string> splitted;
    string aux = "";
    char delimiter = ' ';
    char delimiter1 = ',';
    for(int i = 0; i<=(int)str.size(); i++){
        if((str[i] != delimiter && str[i] != delimiter1) && i != (int)str.size()){
            aux += tolower(str[i]);
        } else {
            if(aux.size()){
                splitted.push_back(aux);
                aux = "";
            }
            // cout << aux << endl;
        }
    }
    return splitted;
}

vector<int> parser(string path, map<string, Comando> command_list){
    string linha;
    bool context_data = false;
    vector<int> parsed_file;
    int count = 0;
    map<string, int> tab_simbolos;

    // Primeira Passagem
    ifstream arquivo;
    arquivo.open(path);
    while (getline (arquivo, linha)) {
        vector<string> tokens = split(linha);
        string key = tokens[0];
        if(key.back() == ':'){
            tab_simbolos[key] = count;
            tokens.erase(tokens.begin());
        }
        
        count += command_list[tokens[0]].size;
    }
    arquivo.close();
    count = 0;
    arquivo.open(path);
    while (getline (arquivo, linha)) {
        vector<string> tokens = split(linha);
        string key = tokens[0];
        if(key.back() == ':'){
            tokens.erase(tokens.begin());
        }
        if(tokens[0] == "const"){
            parsed_file.push_back(tokens[1][0] - '0');
        } else if (tokens[0] == "section") {
            // context_data = tokens[1] == "data";
            // cout << context_data;
        } else {
            parsed_file.push_back(command_list[tokens[0]].opcode);   
            for(int i = 1; i < command_list[tokens[0]].size; i++){
                parsed_file.push_back(tab_simbolos[tokens[i]+':']);           
            }
        }
        cout << endl;
    }   
    return parsed_file;
    //
}

int main(int argc, char *argv[]){
    map<string, Comando> command_list = initialize_commands();
    vector<int> new_file = parser(argv[argc-1], command_list);
    ofstream output;
    output.open("./teste.txt");
    for(int i = 0; i<new_file.size(); i++){
        output << new_file[i] << " ";
    }
    output.close();
    return 0;
}
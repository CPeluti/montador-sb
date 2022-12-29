#include <bits/stdc++.h>
#include "commands/comando.h"
#include "macros/macro.h"
using namespace std;

#define vvs vector<vector<string>>

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
    char oprtor = '+';
    for(int i = 0; i<=(int)str.size(); i++){
        if(str[i] == oprtor){
            if(aux.size()){
                splitted.push_back(aux);
                splitted.push_back(to_string(str[i]));
                aux = "";
            }
        }
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
// nome, def, n de parametros, linha_def, lugares onde foi substituida
pair<vvs, vector<Macro>> macro_parser(vvs tokens){
    vector<Macro> macros;
    for(int i = 0; i<(int)tokens.size(); i++){

        for(int j = 0; j<(int)tokens[i].size(); j++){
            if(tokens[i][j] == "macro"){
                string macro_name = tokens[i][j-1];

                vector<string> params;
                for(int nparams = 2; nparams<(int)tokens[i].size(); nparams++){
                    params.push_back(tokens[i][nparams]);
                }
                
                vvs macro_def;
                // contador alternativo para a macro;
                int i1 = i+1;
                
                while(tokens[i1][0] != "endmacro"){
                    macro_def.push_back(tokens[i1]);
                    i1++;
                }

                Macro m(i, macro_def);
                
                for(int k = i1; k<(int)tokens.size(); k++){
                    if(tokens[k][0]+':' == macro_name){
                        vector<string> params1;
                        
                        map<string, string> parametros;

                        //insere a macro nos tokens a partir da proxima linha dps de K
                        tokens.insert(tokens.begin()+k+1, macro_def.begin(), macro_def.end());

                        m.subs.push_back({k, k+(int)macro_def.size()});

                        params1.insert(params1.begin(), tokens[k].begin()+1, tokens[k].end());

                        for(int nparams = 0; nparams<(int)params.size(); nparams++){
                            string key = params[nparams];
                            parametros[key] = params1[nparams];
                        }
                        
                        for(int nlinha = k+1; nlinha<k+1+(int)m.def.size(); nlinha++){
                            
                            for(int ncoluna = 0; ncoluna<(int)tokens[nlinha].size(); ncoluna++){
                                // tokens[nlinha][ncoluna] = parametro
                                if(tokens[nlinha][ncoluna][0] == '&'){
                                   tokens[nlinha][ncoluna] = parametros[tokens[nlinha][ncoluna]];
                                }
                            }

                        }
                        // apaga chamada da macro
                        tokens.erase(tokens.begin()+k);
                        //adiciona macro no vetor;
                        macros.push_back(m);
                    }
                }

            }        

        }

    }
    // for(int i = 0; i<(int)tokens.size(); i++){
        // if(tokens[i][1] == "macro"){
            // while(tokens[i][0] != "endmacro"){
                // cout << tokens[i][0];
                // i++;
                // tokens.erase(tokens.begin()+i);
            // }
            // tokens.erase(tokens.begin()+i);
        // }
        
        // tokens.erase(tokens.begin()+m.linha_def, tokens.begin()+m.linha_def+(int)m.def.size()+2);
    // }
    return {tokens, macros};

}
// vvs equif_parser(vvs tokens){

// }

vvs preprocesser(vvs tokens, char flag){

    pair<vvs, vector<Macro>> res = macro_parser(tokens);
    tokens = res.first;
    // for(int i = 0; i<(int)tokens.size(); i++){
    //     for(int j = 0; j<(int)tokens[i].size(); j++){
    //         cout << tokens[i][j] << endl;
    //     }
    // }
    vector<Macro> macros = res.second;
}

vvs tokenizer(string path){
    string linha;
    vector<vector<string>> output;
    ifstream arquivo;
    arquivo.open(path);
    while (getline (arquivo, linha)) {
        vector<string> tokens = split(linha);
        output.push_back(tokens);
    }
    arquivo.close();
    return output;
}

// vector<int> parser(string path, map<string, Comando> command_list){
//     string linha;
//     bool context_data = false;
//     vector<int> parsed_file;
//     int count = 0, num_linha = 1;
//     map<string, int> tab_simbolos;

//     // Primeira Passagem
//     while (getline (arquivo, linha)) {
//         vector<string> tokens = split(linha);
//         string key = tokens[0];
//         if(key.back() == ':'){
//             tab_simbolos[key] = count;
//             tokens.erase(tokens.begin());
//         }
        
//         count += command_list[tokens[0]].size;
//     }
//     arquivo.close();
//     count = 0;

//     // Segunda Passagem
//     arquivo.open(path);
//     while (getline (arquivo, linha)) {
//         vector<string> tokens = split(linha);
//         string key = tokens[0];
        
//         if(key.back() == ':'){
//             tokens.erase(tokens.begin());
//         }
//         if(tokens[0] == "const"){
//             parsed_file.push_back(tokens[1][0] - '0');
//         } else if (tokens[0] == "section") {
//             // context_data = tokens[1] == "data";
//             // cout << context_data;
//         } else {
//             parsed_file.push_back(command_list[tokens[0]].opcode);   
//             for(int i = 1; i < command_list[tokens[0]].size; i++){

//                 // Checar se o rótulo se encontra definido na tabela de simbolos
//                 if(tab_simbolos.find(tokens[i]+':') != tab_simbolos.end()){
//                     parsed_file.push_back(tab_simbolos[tokens[i]+':']);           
//                 } else cout<< "Erro semântico na linha " << num_linha << ": rótulo \"" << tokens[i] << "\" não definido." <<endl;

//             }
//         }
//         num_linha++;
//     }   
//     return parsed_file;

// }

int main(int argc, char *argv[]){
    map<string, Comando> command_list = initialize_commands();

    vvs tokenized_file = tokenizer(argv[argc-1]);

    vvs preprocessed_file = preprocesser(tokenized_file, 'c');
    // vector<int> new_file = parser(, command_list);
    // ofstream output;
    // output.open("./teste.txt");
    // for(int i = 0; i<new_file.size(); i++){
        // output << new_file[i] << " ";
    // }
    // output.close();
    return 0;
}
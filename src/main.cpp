#include <bits/stdc++.h>
#include "commands/comando.h"
#include "macros/macro.h"
using namespace std;
#define vvs vector<vector<string>>
bool erro = false;

int offset_macro = 0;

int offset_equif = 0;

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
    commands["mul"] = mul;
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
                // cout << aux << endl;
                splitted.push_back(aux);
                aux = "";
            }
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
                offset_macro++;
                string macro_name = tokens[i][j-1];

                vector<string> params;
                params.insert(params.begin(), tokens[i].begin()+j+1, tokens[i].end());
                
                vvs macro_def;
                // contador alternativo para a macro;
                int i1 = i+1;
                
                while(tokens[i1][0] != "endmacro"){
                    macro_def.push_back(tokens[i1]);
                    i1++;
                    offset_macro++;
                }
                offset_macro++;
                Macro m(i, macro_def);
                
                for(int k = i1; k<(int)tokens.size(); k++){
                    if(tokens[k][0]+':' == macro_name){
                        vector<string> params1;
                        
                        map<string, string> parametros;
                        
                        for(int nparams = 0; nparams<(int)params.size(); nparams++){
                            string key = params[nparams];
                            parametros[key] = params1[nparams];
                        }

                        //insere a macro nos tokens a partir da proxima linha dps de K
                        tokens.insert(tokens.begin()+k+1, macro_def.begin(), macro_def.end());

                        m.subs.push_back({k, k+(int)macro_def.size()});

                        params1.insert(params1.begin(), tokens[k].begin()+1, tokens[k].end());
                        
                        for(int nlinha = k+1; nlinha<(int)m.def.size(); nlinha++){
                            
                            for(int ncoluna = 0; ncoluna<(int)tokens[nlinha].size(); ncoluna++){
                                
                                //par = parametro
                                string par = tokens[nlinha][ncoluna];
                                if(par[0] == '&'){
                                   par = parametros[par];
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
    vvs final_tokens;
    for(int i = 0; i<(int)tokens.size(); i++){
        if(tokens[i].size() > 1 && tokens[i][1] == "macro"){
            while(tokens[i][0] != "endmacro"){
                i++;
            }
        } else {
            final_tokens.push_back(tokens[i]);
        }
    }
    return {final_tokens, macros};

}

vvs equif_parser(vvs tokens){
    map<string, bool> equifs;

    for(int i = 0; i<(int)tokens.size(); i++){

        if(tokens[i].size() > 1 && tokens[i][1] == "equ"){
            offset_equif++;
            if(tokens[i][2] != "0"){
                equifs[tokens[i][0]] = true;
            } else {
                equifs[tokens[i][0]] = false;
            }
            tokens.erase(tokens.begin()+i);
            i--;
        }else if(tokens[i][0] == "if"){
            if(equifs[tokens[i][1]+":"] == true){
                tokens.erase(tokens.begin()+i);
                i--;
            } else if(equifs[tokens[i][1]+":"] == false){
                tokens.erase(tokens.begin()+i);
                tokens.erase(tokens.begin()+i);
                i--;
            }
        }
    }

    // for(int i = 0; i<(int)tokens.size(); i++){
    //     for (auto j = 0; j < tokens[i].size(); j++)
    //     {
    //         cout<< tokens[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return tokens;
}

pair<vvs, vector<Macro>> preprocesser(vvs tokens, char flag){
    return macro_parser(equif_parser(tokens));
}

vvs tokenizer(string path){
    string linha;
    vector<vector<string>> output;
    ifstream arquivo;
    arquivo.open(path);
    while (getline (arquivo, linha)) {
        regex comment(";.*$");
        linha = regex_replace(linha, comment, "");
        vector<string> tokens = split(linha);
        output.push_back(tokens);
        
    }
    
    arquivo.close();
    return output;
}

vvs parser(pair<vvs, vector<Macro>> preprocessed_file, map<string, Comando> command_list){
    string linha;
    bool context_text = false, flg = false;
    vector<string> parsed_file;
    int cont = 0, c=0;
    regex const rgx(".+:");
    map<string, int> tab_simbolos;

    // Primeira Passagem
    vvs tokens = preprocessed_file.first;
    for(auto i = 0; i<tokens.size(); i++){

        // Conta quantos rótulos há em cada linha
        c = count_if(tokens[i].begin(), tokens[i].end(), [=](string s) {return regex_match(s, rgx);});
        if(c > 1){
            cout<< "Erro sintático na linha " << (offset_macro+offset_equif+i+1) << ": dois rótulos definidos na mesma linha." <<endl;
        } else{
            string key = tokens[i][0];
            /* if(tokens.size() > 1) {
                cout << tokens[1] << endl;
            } */
            if(key.back() == ':'){
                tab_simbolos[key] = cont;
                tokens[i].erase(tokens[i].begin());
            }
            
            if(command_list.count(tokens[i][0])){
                cont += command_list[tokens[i][0]].size;
            }
        }
    }

    
    // Segunda Passagem

    for(auto i = 0; i<tokens.size(); i++){
        string s="";
        if(tokens[i][0] == "const"){
            parsed_file.push_back(s+tokens[i][1][0]);
        } else if (tokens[i][0] == "section") {
            if(tokens[i][1] == "text"){
                context_text = true;
            }
        } else {
            if(context_text == false && flg == false){
                cout<< "Erro semântico na linha " << (offset_macro+offset_equif+i+1) << ": SECTION TEXT não definida." <<endl;
                flg = true;
            }
            // Verificar se a instrucao ou diretiva existem
            cout << tokens[i][0] << ": "  << command_list.count(tokens[i][0]) << endl;
            if(command_list.count(tokens[i][0])){
                parsed_file.push_back(to_string(command_list[tokens[i][0]].opcode));
            } else {
                cout<< "Erro léxico na linha " << (offset_macro+offset_equif+i+1) << ": instrução ou diretiva \"" << tokens[i][0] << "\" não definida." <<endl;
            }
            if(command_list.count(tokens[i][0])){
                // Verifica se a quantidade de argumentos está certa
                if(tokens[i].size() == command_list[tokens[i][0]].size){
                    for(int j = 1; j < command_list[tokens[i][0]].size; j++){
                        // Checar se o rótulo se encontra definido na tabela de simbolos
                        if(tab_simbolos.find(tokens[i][j]+':') != tab_simbolos.end()){
                            parsed_file.push_back(to_string(tab_simbolos[tokens[i][j]+':']));           
                        } else cout<< "Erro semântico na linha " << (offset_macro+offset_equif+i+1) << ": rótulo \"" << tokens[i][j] << "\" não definido." <<endl;

                    }
                } else{
                    cout<< "Erro sintático na linha " << (offset_macro+offset_equif+i+1) << ": quantidade de argumentos de \"" << tokens[i][0] << "\" incorreta." <<endl;
                }
            }
        }

    }
    vvs res;
    res.push_back(parsed_file);
    return res;

}

void write_file(vvs res, string path, string ext){
    ofstream output;
    if(!erro){
        output.open(path+ext);
        for(int i = 0; i<res.size(); i++){
            for(int j = 0; j<res[i].size(); j++){
                output << res[i][j] << " ";
            }
            output << "\n";
        }
        output.close();
    }
}

int main(int argc, char *argv[]){
    map<string, Comando> command_list = initialize_commands();

    vvs tokenized_file = tokenizer(argv[argc-1]);
    
    string file_name = argv[argc-1];
    regex ext("\\.[a-zA-Z]+");
    file_name = regex_replace(file_name, ext, "");

    char flag = argv[argc-2][1];

    switch(flag){
        case 'p':{
            vvs res = equif_parser(tokenized_file);
            write_file(res, file_name, ".PRE");
            break;
        }
        case 'm':{
            pair<vvs, vector<Macro>> res =  macro_parser(tokenized_file);
            write_file(res.first, file_name, ".MCR");
            break;
        }
        default:{
            pair<vvs, vector<Macro>> preprocessed_file = preprocesser(tokenized_file, 'c');
            vvs new_file = parser(preprocessed_file, command_list);
            write_file(new_file, file_name, ".OBJ");
            break;
        }
    }
    
    return 0;
}
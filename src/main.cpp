#include <bits/stdc++.h>
#include "comando.h"
#include "macro.h"
using namespace std;

#define vvs vector<pair<vector<string>,int>>
#define pvs pair<vector<string>,int>
bool erro = false;


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

vector<string> split(string str, char delimiter){
    vector<string> splitted;
    string aux = "";
    for(int i = 0; i<=(int)str.size(); i++){

        if(str[i] != delimiter && i != (int)str.size()){
            aux += tolower(str[i]);
        } else {
            if(aux.size()){
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

        for(int j = 0; j<(int)tokens[i].first.size(); j++){
            if(tokens[i].first[j] == "macro"){
                string macro_name = tokens[i].first[j-1];

                vector<string> params;
                params.insert(params.begin(), tokens[i].first.begin()+j+1, tokens[i].first.end());
                
                vvs macro_def;
                // contador alternativo para a macro;
                int i1 = i+1;
                
                while(tokens[i1].first[0] != "endmacro"){
                    macro_def.push_back(tokens[i1]);
                    i1++;
                }
                
                Macro m(i, macro_def);
                
                for(int k = i1; k<(int)tokens.size(); k++){
                    if(tokens[k].first[0]+':' == macro_name){
                        //parametros da chamada da macro
                        vector<string> params1;
                        
                        map<string, string> parametros;
                        params1.insert(params1.begin(), tokens[k].first.begin()+1, tokens[k].first.end());
                        
                        for(int nparams = 0; nparams<(int)params.size(); nparams++){
                            string key = params[nparams];
                            parametros[key] = params1[nparams];
                        }

                        //insere a macro nos tokens a partir da proxima linha dps de K
                        tokens.insert(tokens.begin()+k+1, macro_def.begin(), macro_def.end());

                        m.subs.push_back({k, k+(int)macro_def.size()});
                        
                        for(int nlinha = k+1; nlinha<=(int)m.def.size()+k; nlinha++){
                            
                            for(int ncoluna = 0; ncoluna<(int)tokens[nlinha].first.size(); ncoluna++){
                                
                                //par = parametro
                                if(tokens[nlinha].first[ncoluna][0] == '&'){
                                   tokens[nlinha].first[ncoluna] = parametros[tokens[nlinha].first[ncoluna]];
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
        if(tokens[i].first.size() > 1 && tokens[i].first[1] == "macro"){
            while(tokens[i].first[0] != "endmacro"){
                i++;
            }
        } else {
            final_tokens.push_back(tokens[i]);
        }
    }
    return {final_tokens, macros};

}

vvs equif_parser(vvs tokens){
    map<string, int> equifs;

    for(int i = 0; i<(int)tokens.size(); i++){

        if(tokens[i].first.size() > 1 && tokens[i].first[1] == "equ"){
			equifs[tokens[i].first[0]] = stoi(tokens[i].first[2]);
            tokens.erase(tokens.begin()+i);
            i--;
        }else if(tokens[i].first[0] == "if"){
            if(equifs[tokens[i].first[1]+":"]){
                tokens.erase(tokens.begin()+i);
                i--;
            } else {
                tokens.erase(tokens.begin()+i);
                tokens.erase(tokens.begin()+i);
                i--;
            }
        } else {
			for(int j = 0; j<tokens[i].first.size(); j++){
				if(equifs.count(tokens[i].first[j]+':')){
					tokens[i].first[j] = to_string(equifs[tokens[i].first[j]+':']);
				}
			}
		}
    }

    return tokens;
}

pair<vvs, vector<Macro>> preprocesser(vvs tokens, char flag){
    return macro_parser(equif_parser(tokens));
}

vvs tokenizer(string path){
    string linha;
    vvs output;
    ifstream arquivo;
    arquivo.open(path);
    int count = 1;
    while (getline (arquivo, linha)) {
        regex comment(";.*$");
		regex tab("\\t");
        regex comma(",");
        linha = regex_replace(linha, comment, "");
		linha = regex_replace(linha, tab, " ");
		linha = regex_replace(linha, comma, " ");
        if(linha != ""){
            vector<string> tokens = split(linha, ' ');
            pvs p = {tokens,count};
            output.push_back(p);
        }
        count++;
        
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
    regex const nome("^[a-zA-Z][1-9|a-zA-Z|\\_]*:$");
    map<string, int> tab_simbolos;

    // Primeira Passagem
    vvs tokens = preprocessed_file.first;
    for(auto i = 0; i<tokens.size(); i++){


        // Conta quantos rótulos há em cada linha
        c = count_if(tokens[i].first.begin(), tokens[i].first.end(), [=](string s) {return regex_match(s, rgx);});
        if(c > 1){
            cout<< "Erro sintático na linha " << tokens[i].second << ": dois rótulos definidos na mesma linha." <<endl;
            erro = true;
        } else if(c==1){
            string key = tokens[i].first[0];
            bool flag_erase = true;
            if(tokens[i].first.size() <= 1){
                if(i+1<tokens.size()){
                    c = count_if(tokens[i+1].first.begin(), tokens[i+1].first.end(), [=](string s) {return regex_match(s, rgx);});
                    if(c > 1){
                        cout<< "Erro sintático na linha " << tokens[i+1].second << ": dois rótulos definidos na mesma linha." <<endl;
                        erro = true;
                    } 
                    tokens.erase(tokens.begin()+i);
                    flag_erase = false;
                }
                
            }
            if(key.back() == ':'){
                // Verifica erros léxicos nos rótulos
                if(!regex_match(tokens[i].first[0], nome) && !flg && flag_erase){
                    cout<< "Erro léxico na linha " << tokens[i].second << ": rótulo definido incorretamente." <<endl;
                    erro = true;
                    flg = true;
                }
                tab_simbolos[key] = cont;
                if(flag_erase){
                    tokens[i].first.erase(tokens[i].first.begin());
                }
            }
        }
        if(command_list.count(tokens[i].first[0])){
            cont += command_list[tokens[i].first[0]].size;
        }
    }

    
    // Segunda Passagem
    flg = false;
    for(auto i = 0; i<tokens.size(); i++){
        string s="";
        if(tokens[i].first[0] == "space"){
            int j = 1;
            if(tokens[i].first.size() > 1){
                vector<string> s = split(tokens[i].first[1],'+');
                if(s.size() > 1){
                    j = stoi(s[0]) + stoi(s[1]);
                } else{
                    j = stoi(s[0]);
                }
            }
            while(j--){
                parsed_file.push_back("0");
            }
        }
        else if(tokens[i].first[0] == "const"){
            regex hexa("^0x[0-9a-fA-F]*");
            regex start("^0x");
            if(regex_match(tokens[i].first[1], hexa)){
                string s = regex_replace(tokens[i].first[1], start, "");
                unsigned int j = stoul(s, nullptr, 16);
                tokens[i].first[1] = to_string(j);

            }
            parsed_file.push_back(tokens[i].first[1]);
        } else if (tokens[i].first[0] == "section") {
            if(tokens[i].first[1] == "text"){
                context_text = true;
            } 
        } else {
            // Verificar se a instrucao ou diretiva existem
            
            if(command_list.count(tokens[i].first[0])){
                vector<string> splitted;
                parsed_file.push_back(to_string(command_list[tokens[i].first[0]].opcode));
                for(int j = 1; j < tokens[i].first.size(); j++){
                    // Checar se o rótulo se encontra definido na tabela de simbolos
                    splitted = split(tokens[i].first[j], '+');
                    bool flag_checked = false;
                    if(splitted.size() > 1 ){
                        if(tab_simbolos.count(splitted[0]+':') == 0 && tab_simbolos.count(splitted[1]+':')){
                            cout<< "Erro semântico na linha " << tokens[i].second << ": rótulo \"" << tokens[i].first[j] << "\" não definido." <<endl;
                            erro = true;
                        }
                        flag_checked = true;
                    }
                    if(tab_simbolos.count(tokens[i].first[j]+':') || flag_checked){
                        int aux;
                        if(splitted.size() > 1){
                            if(tab_simbolos.count(splitted[0]+':')){
                                aux = tab_simbolos[splitted[0]+':'] + stoi(splitted[1]);
                            } else {
                                aux += stoi(splitted[0]) + tab_simbolos[splitted[1]+':'];
                            }
                            tokens[i].first[j] = to_string(aux);
                        }else{
                            tokens[i].first[j] = to_string(tab_simbolos[tokens[i].first[j]+':']);
                        }
                        parsed_file.push_back(tokens[i].first[j]);           
                    } else{
                        cout<< "Erro semântico na linha " << tokens[i].second << ": rótulo \"" << tokens[i].first[j] << "\" não definido." <<endl;
                        erro = true;
                    } 

                }
                if(command_list[tokens[i].first[0]].size != tokens[i].first.size()){
                    cout<< "Erro sintático na linha " << tokens[i].second << ": quantidade de argumentos de \"" << tokens[i].first[0] << "\" incorreta." <<endl;
                    erro = true;
                }
            } else {
                cout<< "Erro léxico na linha " << tokens[i].second << ": instrução ou diretiva \"" << tokens[i].first[0] << "\" não definida." <<endl;
                erro = true;
            }
            
        }
		if(context_text == false && flg == false){
			cout<< "Erro semântico na linha " << tokens[i].second << ": SECTION TEXT não definida." <<endl;
			flg = true;
			erro = true;
		}
    }


    vvs res;
    res.push_back({parsed_file,0});
    return res;

}

void write_file(vvs res, string path, string ext){
    ofstream output;
    if(!erro){
        output.open(path+ext);
        for(int i = 0; i<res.size(); i++){
            for(int j = 0; j<res[i].first.size(); j++){
                output << res[i].first[j] << " ";
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
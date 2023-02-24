#include <bits/stdc++.h>
#include "comando.h"
#include "macro.h"
using namespace std;

#define vvs vector<pair<vector<string>,int>>
#define vs vector<string>
#define pvs pair<vector<string>,int>
bool erro = false;


map<string, Comando> initialize_commands(){
    map<string, Comando> commands;
    Comando space("resd &0",1);
    Comando constant("&0 &1",2);
    Comando add("add eax, [&0]",1);
    Comando sub("sub eax, [&0]",1);
    Comando mul("mov ebx, [&0]\nmul ebx",1);
    Comando div("cdq\nmov ebx, [&0]\ndiv ebx\n",1);
    Comando jmp("jmp &0",1);
    Comando jmpn("cmp eax, 0\njl &0",1);
    Comando jmpp("cmp eax, 0\njg &0",1);
    Comando jmpz("cmp eax, 0\nje &0",1);
    Comando copy("mov ebx, [&0]\nmov [&1], ebx",2);
    Comando load("mov eax, [&0]",1);
    Comando store("mov [&0], eax",1);
    Comando input("push eax\npush &0\ncall input\npop eax",1);
    Comando input_c("push eax\npush &0\ncall input_c\npop eax",1);
    Comando input_s("push eax\npush &0\npush &1\ncall input_s\npop eax",2);
    Comando output("push eax\npush dword [&0]\ncall output\npop eax",1);
    Comando output_c("push eax\npush &0\n call output_c\npop eax",1);
    Comando output_s("push eax\npush &0\npush &1\ncall output_s\npop eax",2);
    Comando stop("mov eax, 1\nmov ebx, 0\nint 80h",0);
    commands["const"] = constant;
    commands["space"] = space;
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
    commands["input_c"] = input_c;
    commands["output_c"] = output_c;
    commands["input_s"] = input_s;
    commands["output_s"] = output_s;
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

vs parser(pair<vvs, vector<Macro>> preprocessed_file, map<string, Comando> command_list){
    vs text;
    vs bss;
    vs data;
    for(auto& linha : preprocessed_file.first){
        int type=0;
        string codigo_processado = "";
        int counter = 0;
        for(string s : linha.first){
            if(s.back() == ':'){
                codigo_processado += s;
                codigo_processado += " ";
            } else {
                if(command_list.find(s) != command_list.end()){
                    if(s == "const")
                        type = 2;
                    else if(s == "space"){
                        type = 1;
                    }
                    Comando c = command_list[s];
                    codigo_processado += c.inst;
                } else {
                    string search;
                    if(type == 2){
                        search = "&0";
                        regex e(search);
                        if(s[0] == '\''){
                            codigo_processado = regex_replace(codigo_processado, e, "db");
                        } else {
                            codigo_processado = regex_replace(codigo_processado, e, "dd");
                        }
                        counter++;
                    }
                    search = "&";
                    string counter1 = to_string(counter);
                    search+= counter1;
                    regex ext(search);
                    codigo_processado = regex_replace(codigo_processado, ext, s);
                    counter++;
                }
            }
        }
        if(codigo_processado.size() != 0){
            regex ext(":");
            switch(type){
                //se for space
                case 1:{
                    regex e("&0");
                    codigo_processado = regex_replace(codigo_processado, e, "1");
                    codigo_processado = regex_replace(codigo_processado, ext, "");
                    bss.push_back(codigo_processado);

                    break;
                }
                //se for const
                case 2:
                    codigo_processado = regex_replace(codigo_processado, ext, "");
                    data.push_back(codigo_processado);
                    break;
                //se for inst
                default:
                    text.push_back(codigo_processado);
                    break;
            }
        }
    }
    vs final_file;
    final_file.push_back("section .bss");
    for(string s : bss){
        final_file.push_back(s);
    }
    final_file.push_back("section .data");
    for(string s : data){
        final_file.push_back(s);
    }
    final_file.push_back("section .text");
    final_file.push_back("global _start");
    // adicionar funcoes de input
    final_file.push_back("input_c:\nenter 0, 0\nmov eax, 3\nmov ebx, 0\nmov ecx, [ebp+8]\nmov edx, 2\nint 80h\nleave\nret 4");
    final_file.push_back("output_c:\nenter 0, 0\nmov eax, 4\nmov ebx, 1\nmov ecx, [ebp+8]\nmov edx, 2\nint 80h\nleave\nret 4");
    final_file.push_back("input_s:\nenter 0, 0\nmov ecx, [ebp+12]\nmov edx, [ebp+8]\nmov eax, 3\nmov ebx, 0\nint 80h\nleave\nret 8");
    final_file.push_back("output_s:\nenter 0, 0\nmov ecx, [ebp+12]\nmov edx, [ebp+8]\nmov eax, 4\nmov ebx, 1\nint 80h\nleave\nret 8");
    final_file.push_back("input:\nenter 2, 0\npush dword 0\nmov byte [ebp-2], 0\nloop_i:\nmov ecx, ebp\ndec ecx\nmov eax, 3\nmov ebx, 0\nmov edx, 1\nint 80h\ncmp byte [ebp-1], \'-\'\njne nNegativo\nmov byte [ebp-2], 1\njmp loop_i\nnNegativo:\ncmp byte [ebp-1], 0ah\nje nPush\ncmp byte [ebp-1], 0dH\nje nPush\npop eax\nmov ecx, 10\nmul ecx\nmov ecx, [ebp-1]\nsub ecx, 0x30\nadd eax,ecx\npush eax\njmp loop_i\nnPush:\npop eax\ncmp byte [ebp-2], 1\njne fim\nneg eax\nfim:\nmov ebx, [ebp+8]\nmov [ebx], eax\nleave\nret 4");
    final_file.push_back("output:\nenter 1,0\nmov eax, [ebp+8]\ncheck_signal:\ncmp eax, 0\njge loop_o\nmov byte [ebp-1], \'-\'\npush eax\nmov ecx, ebp\ndec ecx\nmov eax, 4\nmov ebx, 1\nmov edx, 1\nint 80h\npop eax\nneg eax\nloop_o:\nxor edx, edx\nmov ecx, 10\ndiv ecx\nadd edx, 0x30\npush edx\ncmp eax, 0\njne loop_o\nloop_print:\npop eax\nmov [ebp-1], eax\nmov ecx, ebp\ndec ecx\nmov eax, 4\nmov ebx, 1\nmov edx, 1\nint 80h\ncmp ecx, esp\njne loop_print\nleave\nret 4");
    final_file.push_back("_start:");
    for(string s : text){
        final_file.push_back(s);
    }
    return final_file;
}

void write_file(vs res, string path, string ext){
    ofstream output;
    if(!erro){
        output.open(path+ext);
        for(int i = 0; i<res.size(); i++){
            output << res[i] << "\n";
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


    pair<vvs, vector<Macro>> preprocessed_file = preprocesser(tokenized_file, 'c');
    vs final_file = parser(preprocessed_file, command_list);
    write_file(final_file, file_name, ".s");
    // switch(flag){
    //     case 'p':{
    //         vvs res = equif_parser(tokenized_file);
    //         write_file(res, file_name, ".PRE");
    //         break;
    //     }
    //     case 'm':{
    //         pair<vvs, vector<Macro>> res =  macro_parser(tokenized_file);
    //         write_file(res.first, file_name, ".MCR");
    //         break;
    //     }
    //     default:{
    //         pair<vvs, vector<Macro>> preprocessed_file = preprocesser(tokenized_file, 'c');
    //         vvs new_file = parser(preprocessed_file, command_list);
    //         write_file(new_file, file_name, ".OBJ");
    //         break;
    //     }
    // }
    
    return 0;
}
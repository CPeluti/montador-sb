#include <bits/stdc++.h>
using namespace std;

class Macro {
    public:
        Macro();
        Macro(int linha_def, vector<vector<string>> def);
        int linha_def;
        vector<vector<string>> def;

        //lugares em que foram substituidas
        vector<pair<int,int>> subs;
};
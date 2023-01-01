#include <bits/stdc++.h>
using namespace std;

#define vvs vector<pair<vector<string>,int>>

class Macro {
    public:
        Macro();
        Macro(int linha_def, vvs def);
        int linha_def;
        vvs def;

        //lugares em que foram substituidas
        vector<pair<int,int>> subs;
};
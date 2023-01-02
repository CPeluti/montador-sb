#include <bits/stdc++.h>
#define vvs vector<pair<vector<string>,int>>
using namespace std;


class Macro {
    public:
        Macro();
        Macro(int linha_def, vvs def);
        int linha_def;
        vvs def;

        //lugares em que foram substituidas
        vector<pair<int,int>> subs;
};
#include "macro.h"
#define vvs vector<pair<vector<string>,int>>
using namespace std;

Macro::Macro(int linha_def, vvs def)
{
    this->linha_def = linha_def;
    this->def = def;
}
Macro::Macro()
{
}
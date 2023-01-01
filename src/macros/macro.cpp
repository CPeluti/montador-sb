#include "macro.h"

using namespace std;
#define vvs vector<pair<vector<string>,int>>
Macro::Macro(int linha_def, vvs def)
{
    this->linha_def = linha_def;
    this->def = def;
}
Macro::Macro()
{
}
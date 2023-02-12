#include "comando.h"
#include <string>
using namespace std;

Comando::Comando(string inst, int size)
{
    this->inst = inst;
    this->size = size;
}
Comando::Comando()
{
}
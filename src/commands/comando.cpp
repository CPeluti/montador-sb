#include "comando.h"

using namespace std;

Comando::Comando(int op, int s)
{
    this->opcode = op;
    this->size = s;
}
Comando::Comando()
{
}
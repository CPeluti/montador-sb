#include <string>
using namespace std;
class Comando {
    public:
        Comando();
        Comando(string inst, int size);
        string inst;
        int size;
};
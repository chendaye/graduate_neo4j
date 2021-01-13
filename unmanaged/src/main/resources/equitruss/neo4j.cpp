#include "algo.h"
using namespace std;
int main(int argc, char **argv)
{
    string res = algorithm_neo4j(argv[1], argv[2], stoi(argv[3]), argv[4], stoi(argv[5]), stoi(argv[6]), stoi(argv[7]));
    cout << res <<endl;
    return 0;
}

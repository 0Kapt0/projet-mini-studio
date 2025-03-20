#include "../include/Useful.hpp"

#include <cstdlib>
#include <ctime>
using namespace std;

int randomNumber(int min, int max)
{
    static bool initialized = false;
    if (!initialized) 
    {
        srand(time(0));
        initialized = true;
    }
    return min + rand() % ((max - min) + 1);
}
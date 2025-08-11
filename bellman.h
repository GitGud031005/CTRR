#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <string>

using namespace std;

void BF(int [20][20], int, char, int [20], int [20]);
string BF_Path(int [20][20],int , char, char);


#endif // BELLMAN_H
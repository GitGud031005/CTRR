#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

void BF(int G[60][60], int n, char start, int Value[60], int Prev[60]);
string BF_Path(int G[60][60], int n, char start, char goal);

#endif // BELLMAN_H

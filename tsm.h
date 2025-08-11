#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <string>

using namespace std;

string Traveling(int [20][20], int, char);
vector<int> reconstructPath(int, const vector<vector<int> >&, int, int);
void findMinCost(int [20][20], int, vector<vector<int> >, int, int&, int&);
void DPtable(int [20][20], int, vector<vector<int> >&, vector<vector<int> >&);


#endif // TSP_H
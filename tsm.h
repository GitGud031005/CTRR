#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <string>

using namespace std;
// Ant Colony Optimization for TSP.
// Returns a space-separated tour that ends at the start (e.g., "A B C ... A").
string Traveling(int G[60][60], int n, char start);

#endif // TSP_H

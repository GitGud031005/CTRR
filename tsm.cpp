#include "tsm.h"

void DPtable(int G[20][20], int n, vector<vector<int>>& value, vector<vector<int>>& previ)
{
    // Iterate over all subsets of nodes (1<<n also means 2^n)
    for (int sub = 1; sub < (1 << n); sub++) 
    {
        // Iterate over all nodes to find the end node of the current subset
        for (int i = 0; i < n; i++) 
        {
            // operator & check if there is (1<<i) in sub
            if ((sub & (1 << i)))
            {
                // Iterate over all nodes to find the previous node in the subset
                for (int j = 0; j < n; j++) 
                {
                    if (i != j && (sub & (1 << j)) && G[j][i] != 0)
                    {
                        // Previous subset excluding the current node (operator ^ means xor)
                        int prev_sub = sub ^ (1 << i); 
                        if (value[prev_sub][j] != 999999 && value[prev_sub][j] + G[j][i] < value[sub][i]) 
                        {
                            // Update the DP table with the minimum cost & prev table
                            value[sub][i] = value[prev_sub][j] + G[j][i]; 
                            previ[sub][i] = j; 
                        }
                    }
                }
            }
        }
    }
}

void findMinCost(int G[20][20], int n, vector<vector<int>> value, int startIdx, int &minCost, int &end)
{
    minCost = 999999;
    end = 999999;
    for (int i = 0; i < n; i++) 
    {
        if (i != startIdx) // Skip the start node
        {
            // Check if there is a route that visits all cities and ends at city i,
            // the distance from the last city of the route to the start city is valid,
            // total distance of route and from the last city of the route to the start city < minCost
            if (value[(1 << n) - 1][i] != 999999 && G[i][startIdx] != 0 && value[(1 << n) - 1][i] + G[i][startIdx] < minCost) 
            {
                // Update the minimum cost & end node
                minCost = value[(1 << n) - 1][i] + G[i][startIdx]; 
                end = i;
            }
        }
    }
}

vector<int> reconstructPath(int n, const vector<vector<int>>& previ, int endCity, int startIdx) 
{
    vector<int> path;
    int current = endCity;
    
    // Subset includes all nodes
    int sub = (1 << n) - 1; 

    // Reconstruct the path until the start node is reached
    while (current != 999999) 
    { 
        // Add the current node to the path
        path.push_back(current); 
        
        // Get the prev node
        int temp = previ[sub][current]; 
        
        // Update the subset to exclude the current node
        sub = sub ^ (1 << current); 
        
        // Move to the prev node
        current = temp; 
    }
    
    // Reverse the path to get the correct order
    reverse(path.begin(), path.end()); 
    
    // Add the start node at the end to complete the tour
    path.push_back(startIdx); 

    return path;
}

string Traveling(int G[20][20], int n, char start) 
{
    int startIdx = start - 'A';
    
    // DP table stores min cost (initialize as 999999)
    vector<vector<int>> value(1 << n, vector<int>(n, 999999));
    
    // Table to store the prev node for path reconstruction
    vector<vector<int>> previ(1 << n, vector<int>(n, 999999)); 

    // Initialize the start position
    value[1 << startIdx][startIdx] = 0;

    // Fill the DP table
    DPtable(G, n, value, previ);

    // Find the min cost to return to the start node from any node
    // minCost is the minimum distance to the start city and end is the last city before returning to the start city
    int minCost, end;
    findMinCost(G,n,value,startIdx,minCost,end);

    // Reconstruct the path
    vector<int> path = reconstructPath(n, previ, end, startIdx);

    // Print the minimum cost and path
    string result;
    for (int i = 0; i < path.size(); i++) 
    { 
        // Print the path excluding the first start node (it is the same as the last node)
        char city = 'A' + path[i];
        result = result + city + " ";
    }
    
    // cout<<minCost<<'\n';
    return result;
}

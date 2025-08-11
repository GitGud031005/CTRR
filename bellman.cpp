#include "bellman.h"

void BF(int G[20][20], int numberOfVertices, char start, int Value[20], int Prev[20])
{
    int source = start - 'A';

    if(Value[source] == -1)
    {
        Value[source] = 0;
        Prev[source] = -1;
    }
    int* temp = new int[numberOfVertices];

    for (int i = 0; i < numberOfVertices; i++)
    {
        temp[i] = Value[i];
    }

    for(int u=0; u<numberOfVertices; u++)
    {
        for(int v=0; v<numberOfVertices; v++)
        {
            if(temp[u] != -1 && G[u][v]!=0 && (temp[u] + G[u][v] < Value[v] || Value[v] == -1))
            {
                Value[v] = G[u][v] + temp[u];  
                Prev[v] = u;
            }
        }
    }
    delete[] temp;
}

string BF_Path(int G[20][20], int numberOfVertices, char start, char goal) {
    int Value[20];
    int Prev[20];

    for (int i = 0; i < numberOfVertices; i++)
    {
        Value[i] = -1;
        Prev[i] = -1;
    }

    for(int i=1; i<=numberOfVertices-1; i++)
    {
        BF(G, numberOfVertices, start, Value, Prev);
    }

    for(int u=0; u<numberOfVertices; u++)
    {
        for(int v=0; v<numberOfVertices; v++)
        {
            if(Prev[u]!= -1 && G[v][u]!=0 && G[v][u] + Value[v] < Value[u])
            {
                
            }
        }
    }

    if (Value[int(goal - 'A')] == -1 && goal != start) {
        return "No path exists";
    }

    string str;
    for(int i=int(goal-'A'); i!=-1; i=Prev[i])
    {   
        if(i == int(start-'A'))
        {
            reverse(str.begin(), str.end());
            break;
        }
        str = str + char(i + 'A') + " ";
    }
    return start + str;
}
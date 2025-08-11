#include "bellman.h"

// --- helpers to meet the label spec: A..Z -> 0..25, a..z -> 26..51 ---
int idx1(char v)
{
    return (v >= 'a' && v <= 'z') ? 26 + (v - 'a') : (v - 'A');
}
char label1(int i)
{
    return (i < 26) ? char('A' + i) : char('a' + (i - 26));
}

void BF(int G[60][60], int numberOfVertices, char start, int Value[60], int Prev[60])
{
    const int source = idx1(start); // <-- support lowercase too (A..Z then a..z):contentReference[oaicite:2]{index=2}

    if (Value[source] == -1)
    {
        Value[source] = 0;
        Prev[source] = -1;
    }

    // keep your snapshot-then-relax "one step" idea
    int *temp = new int[numberOfVertices];
    for (int i = 0; i < numberOfVertices; i++)
        temp[i] = Value[i];

    for (int u = 0; u < numberOfVertices; u++)
    {
        if (temp[u] == -1)
            continue; // unreachable so far
        for (int v = 0; v < numberOfVertices; v++)
        {
            if (G[u][v] == 0)
                continue; // 0 means "not connected":contentReference[oaicite:3]{index=3}
            int cand = temp[u] + G[u][v];
            if (Value[v] == -1 || cand < Value[v])
            { // standard relaxation
                Value[v] = cand;
                Prev[v] = u;
            }
        }
    }
    delete[] temp;
}

string BF_Path(int G[60][60], int numberOfVertices, char start, char goal)
{
    // per Q&A: Start==Goal -> string with 1 letter:contentReference[oaicite:4]{index=4}
    if (start == goal)
        return string(1, start);

    int Value[60], Prev[60];
    for (int i = 0; i < numberOfVertices; i++)
    {
        Value[i] = -1;
        Prev[i] = -1;
    }

    // run V-1 steps over the current graph:contentReference[oaicite:5]{index=5}
    for (int i = 1; i <= numberOfVertices - 1; i++)
    {
        BF(G, numberOfVertices, start, Value, Prev);
    }

    const int s = idx1(start);
    const int g = idx1(goal);

    // unreachable?
    if (g < 0 || g >= numberOfVertices || Value[g] == -1)
    {
        return "No path exists"; // inputs have positive weights; negative cycles not considered:contentReference[oaicite:6]{index=6}
    }

    // reconstruct: goal -> ... -> start using Prev, then reverse and join with spaces
    string out;
    // collect labels from goal back to start
    string rev;
    for (int v = g; v != -1; v = Prev[v])
    {
        rev.push_back(label1(v));
        if (v == s)
            break;
        rev.push_back(' ');
    }
    // rev currently like "D C B A"; reverse tokens to "A B C D"
    // simple token-based reverse to preserve single spaces
    vector<string> tokens;
    string cur;
    for (char c : rev)
    {
        if (c == ' ')
        {
            if (!cur.empty())
            {
                tokens.push_back(cur);
                cur.clear();
            }
        }
        else
            cur.push_back(c);
    }
    if (!cur.empty())
        tokens.push_back(cur);

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        out += tokens[tokens.size() - 1 - i];
        if (i + 1 < tokens.size())
            out.push_back(' ');
    }
    return out;
}
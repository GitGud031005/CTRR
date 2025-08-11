#include "bellman.h"
#include "tsm.h"
using namespace std;

/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Discrete Structure (CO1007)
* Author: Nguyen Huu Phuc
* ID: 2352938
* Email: phuc.nguyen0310@hcmut.edu.vn
*/

// g++ -o main main.cpp bellman.cpp tsm.cpp -I . -std=c++11
// ./main


int main(int argc, char** argv)
{
    int bellmanMatrix[20][20] = {
        {0, 72, 89, 77, 2, 58, 13, 69},
        {77, 0, 69, 31, 57, 93, 83, 48},
        {52, 21, 0, 62, 8, 77, 32, 14},
        {33, 1, 40, 0, 72, 99, 72, 59},
        {89, 24, 1, 61, 0, 12, 78, 63},
        {36, 91, 98, 79, 26, 0, 28, 1},
        {18, 77, 49, 36, 98, 77, 0, 45},
        {75, 9, 59, 7, 77, 65, 45, 0}
    };
    int BFValue[20];
    int BFPrev[20];
    for (int z = 0; z < 8; z++)
    {
        BFValue[z] = -1;
        BFPrev[z] = -1;
    }

    cout << "BF result: " << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << "step " << i+1 << ":\n";
        BF(bellmanMatrix, 8, 'D', BFValue, BFPrev);
        cout << "BFValue : ";
        for (int z = 0; z < 8; z++)
        {
            cout << BFValue[z] << " ";
        }
        cout << endl;

        cout << "BFPrev  : ";
        for (int z = 0; z < 8; z++)
        {
            cout << BFPrev[z] << " ";
        }
        cout << endl;
    }

    // BF Path
    cout << endl;
    cout << "BF_PATH result: " << endl;
    cout << BF_Path(bellmanMatrix, 8, 'A', 'D') << endl;

    int tspMatrix[20][20] = {
        {0, 2, 5, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 4, 0, 0, 0, 0, 0, 0},
        {4, 5, 0, 0, 0, 4, 0, 0, 0, 0, 4, 0, 0, 2, 0, 4, 0, 0, 0, 1},
        {2, 0, 1, 0, 0, 4, 0, 2, 5, 0, 0, 0, 0, 5, 3, 0, 0, 4, 3, 0},
        {0, 0, 1, 0, 0, 0, 3, 5, 4, 5, 0, 0, 5, 1, 4, 0, 0, 0, 5, 5},
        {5, 3, 0, 0, 3, 0, 3, 3, 0, 1, 5, 0, 0, 5, 0, 0, 1, 5, 0, 0},
        {1, 0, 1, 0, 4, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 5, 5, 0, 0, 0},
        {0, 2, 0, 5, 5, 0, 3, 0, 2, 0, 4, 4, 0, 0, 3, 2, 5, 0, 0, 4},
        {0, 0, 2, 3, 0, 0, 5, 0, 0, 3, 0, 0, 3, 0, 0, 0, 2, 2, 5, 0},
        {1, 3, 0, 0, 5, 3, 1, 2, 3, 0, 3, 5, 1, 1, 5, 0, 0, 0, 2, 0},
        {0, 3, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 2, 1, 3, 3, 4, 0, 2, 0},
        {3, 0, 1, 0, 0, 4, 1, 0, 1, 0, 0, 5, 0, 0, 0, 0, 0, 4, 0, 2},
        {2, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 2, 2, 4, 5, 0},
        {0, 0, 0, 4, 0, 0, 2, 4, 1, 0, 0, 0, 4, 0, 0, 0, 2, 0, 3, 0},
        {0, 5, 0, 4, 0, 2, 1, 3, 0, 0, 0, 5, 0, 4, 0, 0, 1, 4, 0, 2},
        {4, 0, 1, 1, 4, 2, 0, 0, 0, 3, 0, 2, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 5, 0, 0, 0, 4, 4, 2, 2, 2, 0, 1, 0, 0, 3, 0, 5},
        {0, 0, 4, 2, 0, 0, 0, 4, 2, 0, 0, 1, 0, 0, 1, 2, 0, 0, 0, 0},
        {0, 2, 0, 0, 4, 0, 5, 0, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0},
        {1, 0, 5, 4, 0, 0, 1, 5, 0, 0, 0, 0, 5, 2, 0, 3, 4, 4, 0, 0},
        {0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0}
        };

        cout << "\nTSP result: " << endl;
        string result = Traveling(tspMatrix, 20, 'A');

        if (result == "No path exists")
        {
            cout << "Distance : " << -1 << endl;
        }
        else
        {
            cout << "Path : " << result << endl;
            int distance = 0;
            if (result.length() == 1)
                distance = 0;
            for (int i = 0; i < result.length() - 2; i++)
            {
                if (result[i] >= 'A' && result[i] <= 'Z')
                    distance += tspMatrix[result[i] - 'A'][result[i + 2] - 'A'];
            }
            cout << "Distance : " << distance << endl;
        }

    return 0;
}
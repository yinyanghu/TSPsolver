#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MaxNodes = 30;
const double Infty = 1000000000.0;
const double eps = 0.00001;

const double Cooling = 0.9999; //Cooling ratio
const double CriticalT = 0.000001; // Critical temperature
const double T0 = 50000.0; //Initial temperature
const int    R = 200;   //#Restart

struct NodeType
{
    int x, y;
    char name;
};

struct NodeType     Nodes[MaxNodes];

double      Dist[MaxNodes][MaxNodes];   //Distance between each pair of nodes

int N;

/* Input the Map */
void InputMap()
{
    cin >> N;
    for (int i = 0; i < N; ++ i)
        cin >> Nodes[i].name >> Nodes[i].x >> Nodes[i].y;
}

inline double GetDist(const struct NodeType &A, const struct NodeType &B)
{
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

/* Calculate the distance */
void FindDist()
{
    memset(Dist, 0, sizeof(0));
    for (int i = 0; i < N - 1; ++ i)
        for (int j = i + 1; j < N; ++ j)
            Dist[i][j] = Dist[j][i] = GetDist(Nodes[i], Nodes[j]);
}


/* Find the cost of the State */
double GetCost(const vector<int> &State)
{
    double ret = 0;
    for (int i = 1; i < State.size(); ++ i)
        ret += Dist[State[i - 1]][State[i]];
    ret += Dist[0][State[0]] + Dist[State[State.size() - 1]][0];
    return ret;
}

vector<int>     State; // 0(******)0

/* Swap pointers */
int ptrA, ptrB;

/* Swap 2 nodes each time */
double Swap2(const vector<int> &State)
{
    ptrA = rand() % (State.size() - 1);
    ptrB = (rand() % (State.size() - 1 - ptrA)) + ptrA + 1;
    double ret = 0;
    if (ptrA + 1 != ptrB)
    {
        ret += Dist[State[ptrB]][State[ptrA + 1]] - Dist[State[ptrA]][State[ptrA + 1]];
        ret += Dist[State[ptrB - 1]][State[ptrA]] - Dist[State[ptrB - 1]][State[ptrB]];
    }
    if (ptrA == 0)
        ret += Dist[0][State[ptrB]] - Dist[0][State[ptrA]];
    else
        ret += Dist[State[ptrA - 1]][State[ptrB]] - Dist[State[ptrA - 1]][State[ptrA]];

    if (ptrB == State.size() - 1)
        ret += Dist[State[ptrA]][0] - Dist[State[ptrB]][0];
    else
        ret += Dist[State[ptrA]][State[ptrB + 1]] - Dist[State[ptrB]][State[ptrB + 1]];

    return ret;
}

double SimulatedAnnealing()
{
    /* Initializing */
    srand(time(NULL));

    /* Special Case */
    if (N == 1)
        return 0;
    if (N == 2)
        return Dist[0][1] + Dist[1][0];
    if (N == 3)
        return Dist[0][1] + Dist[1][2] + Dist[2][0];

    pair<int, int> SwapPtr;

    int Restart = R;
    double Opt = Infty;
    while (Restart --)
    {
        /* Initial State */
        State.clear();
        for (int i = 1; i < N; ++ i)
            State.push_back(i);
        double cost = GetCost(State);

        /* Simulated Annealing */
        double T = T0;
        while (T > CriticalT)
        {
            double delta = Swap2(State);
            double Pr = (double)rand() / RAND_MAX;
            if (delta < 0 || exp(-delta / T) > Pr) // if better than current state or probability in accept probability
            {
                cost += delta;
                int k = State[ptrA]; State[ptrA] = State[ptrB]; State[ptrB] = k;
            }
            /* For plot the Cost(T) ~ T curve */
            //printf("%lf,%lf\n", T, cost);
            T = T * Cooling; // Annealing schedule
        }
        if (cost < Opt) Opt = cost;
    }
    return Opt;
}


/* Print the Path friendly */
/*
void PrintPath(const vector<int> &path)
{
    printf("%c", Nodes[path[0]].name);
    for (int i = 1; i < path.size(); ++ i)
        printf(" --> %c", Nodes[path[i]].name);
    printf(" --> %c\n", Nodes[path[0]].name);
}
*/
int main()
{
    InputMap();
    FindDist();
    double Opt = SimulatedAnnealing();
    printf("%lf\n", Opt);
    return 0;
}


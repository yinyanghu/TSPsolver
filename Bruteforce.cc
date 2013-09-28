#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

const int MaxNodes = 30;
const double Infty = 1000000000.0;
const double eps = 0.00001;

struct NodeType
{
    int x, y;
    char name;
};

struct NodeType     Nodes[MaxNodes];

double      Dist[MaxNodes][MaxNodes];

int N;

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

void FindDist()
{
    memset(Dist, 0, sizeof(0));
    for (int i = 0; i < N - 1; ++ i)
        for (int j = i + 1; j < N; ++ j)
            Dist[i][j] = Dist[j][i] = GetDist(Nodes[i], Nodes[j]);
}

void PrintPath(const vector<int> &path)
{
    printf("%c", Nodes[path[0]].name);
    for (int i = 1; i < path.size(); ++ i)
        printf(" --> %c", Nodes[path[i]].name);
    printf(" --> %c\n", Nodes[path[0]].name);
}

bool        flag[MaxNodes];

double      Opt;

vector<int>     path;

void DFS(int x, int depth, double current_dist)
{
    flag[x] = false;
    path.push_back(x);
    if (depth == N - 1)
    {
        current_dist += Dist[x][path[0]];
        if (current_dist < Opt)
        {
            Opt = current_dist;
            /*
            printf("Opt = %lf:\t", Opt);
            PrintPath(path);
            */
        }
        else if (abs(current_dist - Opt) < eps)
        {
            /*
            printf("\t\t\t", Opt);
            PrintPath(path);
            */
        }
    }
    else
    {
        for (int i = 0; i < N; ++ i)
            if (flag[i])
                DFS(i, depth + 1, current_dist + Dist[x][i]);
    }
    path.pop_back();
    flag[x] = true;
}

void BruteForce()
{
    if (N == 1)
    {
        printf("%lf\n", 0);
        return;
    }
    Opt = Infty;
    memset(flag, true, sizeof(flag)); 
    path.clear();
    DFS(0, 0, 0.0); 
    printf("%lf\n", Opt);
}

int main()
{
    InputMap();
    FindDist();
    BruteForce();
    return 0;
}

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>

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

/*
void PrintPath(const vector<int> &path)
{
    printf("%c", Nodes[path[0]].name);
    for (int i = 1; i < path.size(); ++ i)
        printf(" --> %c", Nodes[path[i]].name);
    printf(" --> %c\n", Nodes[path[0]].name);
}
*/

/* Data Structure for Minimum Spanning Tree */
struct MSTType
{
    int         N;
    double      dist[MaxNodes][MaxNodes];
    
    bool        flag[MaxNodes];
    double      f[MaxNodes];

    double Prim()   // Prim's Algorithm
    {
        double ret = 0;
        for (int i = 0; i < N; ++ i)
        {
            flag[i] = true;
            f[i] = Infty;
        }
        f[0] = 0;
        for (int i = 0; i < N; ++ i)
        {
            double temp_min = Infty;
            int next = -1;
            for (int k = 0; k < N; ++ k)
                if (flag[k] && f[k] < temp_min)
                {
                    temp_min = f[k];
                    next = k;
                }
            flag[next] = false;
            ret += f[next];
            for (int j = 0; j < N; ++ j)
                if (flag[j] && dist[next][j] < f[j])
                    f[j] = dist[next][j];
        }
        return ret;
    }
};

struct MSTType      MST;

/* Encoding: (26 bits Visiting State) + (5 bits Last Visited) */
typedef int     StateType;
typedef pair<StateType, double>     StateValueType;

struct StateValueType_Comp
{
    bool operator() (const StateValueType &A, const StateValueType &B)
    {
        return A.second < B.second;
    }
};

/*
struct StateType_Comp
{
    bool operator == (const StateType &A, const StateType &B)
    {
        return (A.first == B.first && A.second == B.second);
    }
};
*/

unordered_set<StateType>      ClosedSet, OpenSet;
StateType           StartState; // (Visiting state, Last visited)
int                 AllVisited;

// Openset <----> Priority Queue

unordered_map<StateType, double>      F, G;

set<StateValueType, StateValueType_Comp> PriorityQueue;


StateType CodingState(int Vstate, int LastV)
{
    return (Vstate << 5) + LastV;
}

int DecodingState_Vstate(const StateType &State)
{
    return (State >> 5);
}

int DecodingState_LastV(const StateType &State)
{
    return (State & 31);
}

/* Heuristic Function */
double H(const StateType &State)
{
    int     LastV = DecodingState_LastV(State);
    int     Vstate = DecodingState_Vstate(State);
    double  ret = 0;

    int     ptr;
    vector<int>     temp;
    temp.clear();
    for (int i = 0, ptr = 1; i < N; ++ i, ptr <<= 1)
        if ((ptr & Vstate) == 0)
            temp.push_back(i);
    
    if (temp.size() == 0) return 0;

    /* Minimize { Dist(LastV, V_k) } */
    double MinGo;
    MinGo = Infty;
    for (int i = 0; i < temp.size(); ++ i)
        if (Dist[LastV][temp[i]] < MinGo)
            MinGo = Dist[LastV][temp[i]];
    ret += MinGo;

    if (temp.size() == 1)
    {
        /*
        cout << "lightning!" << endl;
        cout << ret << endl;
        */
        return ret;
    }

    /* Minimum Spanning Tree */ 
    MST.N = temp.size();
    for (int i = 0; i < temp.size(); ++ i)
        MST.dist[i][i] = 0;
    for (int i = 0; i < temp.size() - 1; ++ i)
        for (int j = i + 1; j< temp.size(); ++ j)
                MST.dist[i][j] = MST.dist[j][i] = Dist[temp[i]][temp[j]];
    ret += MST.Prim();

    return ret;
}

/* For Statistic */
int Counter;

double Astar()
{
    /* Initializing */
    AllVisited = (1 << N) - 1;
    ClosedSet.clear();
    OpenSet.clear();
    PriorityQueue.clear();
    F.clear();
    G.clear();

    /* Starting from A */
    StartState = CodingState(0, 0);
    OpenSet.insert(StartState); 
    G[StartState] = 0;
    F[StartState] = H(StartState);
    PriorityQueue.insert(make_pair(StartState, F[StartState]));

    /* For Statistic */
    //Counter = 0;

    /* A star Searching */
    while (!PriorityQueue.empty())
    {
        /* For Statistic */
        //++ Counter;
        
        /* Current State */
        StateValueType  temp = *PriorityQueue.begin();
        StateType       Current = temp.first;
        int             Visited = DecodingState_Vstate(Current);
        int             LastV = DecodingState_LastV(Current);

        /* Bingo! */
        if (Visited == AllVisited && LastV == 0)
            return G[Current];

        PriorityQueue.erase(PriorityQueue.begin());
        OpenSet.erase(Current);
        ClosedSet.insert(Current);
        int     ptr;
        /* Adding new State */
        for (int i = 0, ptr = 1; i < N; ++ i, ptr <<= 1)
            if ((ptr & Visited) == 0) // If not visited
            {
                int next = ptr | Visited;
                double nextG = G[Current] + Dist[LastV][i];
                StateType   nextState = CodingState(next, i);
                if ((ClosedSet.find(nextState) != ClosedSet.end()) && (nextG >= G[nextState])) continue;
                bool NotFinding = (OpenSet.find(nextState) == OpenSet.end()); // whether in OpenSet
                if (NotFinding || nextG < G[nextState])
                {
                    G[nextState] = nextG;
                    double nextF = nextG + H(nextState);
                    if (!NotFinding)
                        PriorityQueue.erase(make_pair(nextState, F[nextState]));
                    else
                        OpenSet.insert(nextState); // Add to OpenSet
                    F[nextState] = nextF;
                    PriorityQueue.insert(make_pair(nextState, nextF));
                }
            }
    }
}

int main()
{
    InputMap();
    FindDist();
    double Opt = Astar();
    printf("%lf\n", Opt);
    /* For Statistic */
    /*
    cout << Counter << endl;
    cout << Counter / N << endl;
    */
    return 0;
}

/* Testing MST Prim's Algorithm
    MST.N = 5;
    for (int i = 0; i < MST.N; ++ i)
        for (int j = 0; j < MST.N; ++ j)
            MST.dist[i][j] = MST.dist[j][i] = ((i == j) ? 0 : Infty);
    MST.dist[0][1] = MST.dist[1][0] = 1;
    MST.dist[0][2] = MST.dist[2][0] = 2;
    MST.dist[1][3] = MST.dist[3][1] = 3;
    MST.dist[0][4] = MST.dist[4][0] = 2;
    MST.dist[1][4] = MST.dist[4][1] = 2;
    MST.dist[1][3] = MST.dist[3][1] = 1;
    MST.dist[2][4] = MST.dist[4][2] = 4;
    cout << MST.Prim() << endl;
*/
 
/*
    for (int i = 0; i < N; ++ i)
        printf("%c %d %d\n", Nodes[i].name, Nodes[i].x, Nodes[i].y);
*/

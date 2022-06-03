#include<bits/stdc++.h>
#include <chrono>
using namespace std::chrono;

using namespace std;

#define TOPOLOGY_INDEX 0
#define TOPOLOGY_COMMAND "topology"
#define SHOW_INDEX 0
#define SHOW_COMMAND "show"
#define MODIFY_COMMAND "modify"
#define DELETE_COMMAND "remove"
#define LSRP_COMMAND "lsrp"
#define DVRP_COMMAND "dvrp"

int const MAX_NODES = 500 + 10;

class Graph 
{
    public:
        void run_command(vector<string> new_command);
        void build_graph(vector<string> new_command);
        void show();
        void modify_edge(vector<string> new_command);
        void delete_edge(vector<string> new_command);
        void execute_lsrp_algorithm(vector<string> new_command);
    private:
        void print_dvrp(int node, int dis[MAX_NODES], int parent[MAX_NODES]);
        void execute_dvrp_algorithm(vector<string> new_command);
        void build_nxt_node(int node_inp);
        void print_shortest_path(vector<int>dis, vector<int> parent, int src);
        double execute_lsrp_on_node(int src);
        void print_iter_lsrp(int cnt, vector<int> &dis);
        int decode(int index, string edge);
        int adjacency[MAX_NODES][MAX_NODES];
        int nxt[MAX_NODES][MAX_NODES];
        int max_node_index;
};

void Graph::build_nxt_node(int node_inp)
{
    int lst = max_node_index + 1;
    for(int i = max_node_index; i >= 1; i--)
    {
        nxt[node_inp][i] = lst;
        if(adjacency[node_inp][i] != -1)
            lst = i;
    }
}

void Graph::modify_edge(vector<string> new_command)
{
    int node1 = decode(0, new_command[1]);
    int node2 = decode(1, new_command[1]);
    max_node_index = max(node1, max_node_index);
    max_node_index = max(node2, max_node_index);
    int weight = decode(2, new_command[1]);
    build_nxt_node(node1);
    build_nxt_node(node2);
    if(node1 == node2)
    {
        cout << "Error: Edge endpoints must be diffrent" << endl;
        return;
    }
    adjacency[node1][node2] = weight;
    adjacency[node2][node1] = weight;
}

void Graph::execute_lsrp_algorithm(vector<string> new_command)
{
    double time_taken = 0;
    if(new_command.size() == 2)
    {
        int node = decode(0, new_command[1]);
        time_taken = execute_lsrp_on_node(node);
    }
    else 
    {
        for(int i = 1; i <= max_node_index; i++)
        {
            double time_node = execute_lsrp_on_node(i);
            time_taken += time_node;
            cout << "Time taken by lsrp node i: "
             << time_node << " nanoseconds" << endl;
        }
    }
     cout << "Time taken by lsrp: "
         << time_taken << " nanoseconds" << endl;
}

void Graph::print_iter_lsrp(int cnt, vector<int> &dis)
{
    cout << "        " << "Iter " << cnt << ":" << endl << endl;

    cout << "Dest    " << "| ";
    for(int i = 1; i <= max_node_index; i++)
    {
        cout << i << " " << "|" << " ";
    }
    cout << endl << endl;
     cout << "Cost    " << "| ";
    for(int i = 1; i <= max_node_index; i++)
    {
        cout << dis[i] << " " << "|" << " ";
    }
    cout << endl << endl;
    for(int i = 1; i <= 5 * max_node_index; i++)
        cout << "-";
    cout << endl << endl;
}

void Graph::print_shortest_path(vector<int>dis, vector<int> parent, int src)
{
    cout << "Path: " << "[s] -----> [d]" << " " << "Min-Cost" << "          " << 
        "Shortest Path" << endl << endl;
    cout << "       " << "------------" << " " << "-----------" << "     ";
    for(int i = 1; i <= 4 * (int)dis.size(); i++)
    {
        cout << "-";
    }
    cout << endl << endl;
    for(int i = 1; i < dis.size(); i++)
    {
        if(i == src)
            continue;
        cout << "        ";
        cout << "[" << src << "]" << "--->" << "[" << i << "]" << "     ";
        cout << dis[i] << "            ";
        vector<int>path;
        if(dis[i] == -1)
        {
            cout << endl;
            continue;
        }
        int now = i;
        while(now != -1)
        {
            path.push_back(now);
            now = parent[now];
        }
        reverse(path.begin(), path.end());
        for(int j = 0; j < path.size(); j++)
        {
            cout << path[j];
            if(j != (int)path.size() - 1)
                cout<< " ---> ";
        }
        cout << endl;
    }
}

double Graph::execute_lsrp_on_node(int src)
{
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    vector<int> mark, dis, parent;
    for(int i = 1; i <= max_node_index + 1; i++)
    {
        mark.push_back(0);
        dis.push_back(-1);
        parent.push_back(-1);
    }
    set<pair<int, int> > dijkstra_set;
    dijkstra_set.insert(make_pair(0, src));
    dis[src] = 0;
    int cnt = 0;
    while(dijkstra_set.size())
    {
        cnt++;
        pair<int, int> next_node = *dijkstra_set.begin();
        dijkstra_set.erase(next_node);
        int now_vertex = next_node.second;
        for(int i = 1; i <= max_node_index; i++)
        {
            int w_edge = adjacency[now_vertex][i];
            if(w_edge == -1)
                continue;
            if(dis[now_vertex] + w_edge < dis[i] || dis[i] == -1)
            {
                dijkstra_set.erase(make_pair(dis[i], i));
                dis[i] = dis[now_vertex] + w_edge;
                parent[i] = now_vertex;
                dijkstra_set.insert(make_pair(dis[i], i));
                stop = high_resolution_clock::now();
            }
            i = nxt[now_vertex][i] - 1;
        }
        if(dijkstra_set.size())
            print_iter_lsrp(cnt, dis);
    }
    print_shortest_path(dis, parent, src);
    auto duration = duration_cast<nanoseconds>(stop - start);
    return duration.count();
}

int Graph::decode(int index, string edge)
{
    int cnt = 0;
    int value = 0;
    for(int i = 0; i < edge.size(); i++)
    {
        if(edge[i] == '-')
        {
            if(cnt == index)
                return value;
            value = 0;
            cnt++;
        }
        else 
        {
            value *= 10;
            value += (edge[i] - '0');
        }
    }
    return value;
}

void Graph::build_graph(vector<string>new_command)
{
    max_node_index = 0;
    for(int i = 1; i <= 500; i++)
    {
        for(int j = 1; j <= 500; j++)
        {
            adjacency[i][j] = -1;
        }
        adjacency[i][i] = 0;
    }
    for(int i = 1; i < new_command.size(); i++)
    {
        int node1 = decode(0, new_command[i]);
        int node2 = decode(1, new_command[i]);
        max_node_index = max(node1, max_node_index);
        max_node_index = max(node2, max_node_index);
        int weight = decode(2, new_command[i]);
        adjacency[node1][node2] = weight;
        adjacency[node2][node1] = weight;
    }
    for(int i = 1; i <= max_node_index; i++)
        build_nxt_node(i);
}

void Graph::show()
{
    cout << "u|v" << " " << "|" << " ";
    for(int i = 1; i <= max_node_index; i++)
    {
        cout << i << " ";
    }
    cout << endl;
    for(int i = 1; i <= 4 * max_node_index + 2; i++)
    {
        cout << "-";
    }
    cout << endl;
    for(int i = 1; i <= max_node_index; i++)
    {
        cout << i << " " << "|" << " ";
        for(int j = 1; j <= max_node_index; j++)
            cout << adjacency[i][j] << " ";
        cout << endl;
    }
}

void Graph::delete_edge(vector<string> new_command)
{
    int node1 = decode(0, new_command[1]);
    int node2 = decode(1, new_command[1]);
    max_node_index = max(node1, max_node_index);
    max_node_index = max(node2, max_node_index);
    if(node1 != node2)
    {
        adjacency[node1][node2] = -1;
        adjacency[node2][node1] = -1;
    }
    build_nxt_node(node1);
    build_nxt_node(node2);
}

void Graph::print_dvrp(int node, int dis[MAX_NODES], int parent[MAX_NODES])
{
    cout << "Dest " << "Next Hop " << "Dist " << "Shortest Path " << endl << endl;
    cout << "----------------------------------";
    for(int i = 1; i <= 4 * max_node_index; i++)
        cout << "-";
    cout << endl << endl;
    for(int i = 1; i <= max_node_index; i++)
    {
        int next_hop = parent[i];
        if(parent[i] == node || parent[i] == -1)
            next_hop = i;
        cout << i << " " << "     ";
        cout << next_hop << " " << "       " << dis[i] << "           ";
        if(dis[i] == -1)
        {
            cout << "[ ]";
            cout << endl;
            continue;
        }
        vector<int>path;
        int now = i;
        while(now != -1)
        {
            path.push_back(now);
            now = parent[now];
        }
        reverse(path.begin(), path.end());
        cout << "[ ";
        for(int j = 0; j < path.size(); j++)
        {
            cout << path[j];
            if(j != (int)path.size() - 1)
                cout<< " ---> ";
        }
        cout << " ]";
        cout << endl;
    }
}

void Graph::execute_dvrp_algorithm(vector<string> new_command)
{
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    int dis[MAX_NODES][MAX_NODES], parent[MAX_NODES][MAX_NODES];
    for(int i = 1; i <= max_node_index; i++)
    {
        for(int j = 1; j <= max_node_index; j++)
        {
            dis[i][j] = -1;
            parent[i][j] = -1;
            if(adjacency[i][j] != -1)
            {
                dis[i][j] = adjacency[i][j];
                parent[i][j] = i;
            }
        }
        dis[i][i] = 0;
        parent[i][i] = -1;
    }
    for(int i = 1; i <= max_node_index + 1; i++)
    {
        for(int j = 1; j <= max_node_index; j++)
        {
            for(int k = 1; k <= max_node_index; k++)
            {
                for(int l = 1; l <= max_node_index; l++)
                {
                    int w_edge = adjacency[j][k];
                    if(dis[l][j] == -1)
                        continue;
                    if(w_edge == -1)
                        continue;
                    if(dis[l][k] == -1 || dis[l][j] + w_edge < dis[l][k])
                    {
                        auto stop = high_resolution_clock::now();
                        // cout << j << " " << k << " " << l << " " << dis[j][l] << " " << dis[k][l] << "*****8" << endl;
                        dis[l][k] = dis[l][j] + w_edge;
                        parent[l][k] = j;
                    }
                }
                k = nxt[j][k] - 1;
            }
        }        
    }
    if(new_command.size() == 2)
    {
        int node = decode(0, new_command[1]);
        print_dvrp(node, dis[node], parent[node]);
    }
    else 
    {
        for(int i = 1; i <= max_node_index; i++)
        {
            print_dvrp(i, dis[i], parent[i]);
        }
    }
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Time taken by dvrp: "
         << duration.count() << " nanoseconds" << endl;
}

void Graph::run_command(vector<string> new_command)
{
    if(new_command[TOPOLOGY_INDEX] == TOPOLOGY_COMMAND)
        build_graph(new_command);
    else if(new_command[SHOW_INDEX] == SHOW_COMMAND)
        show();
    else if(new_command[0] == MODIFY_COMMAND)
        modify_edge(new_command);
    else if(new_command[0] == DELETE_COMMAND)
        delete_edge(new_command);
    else if(new_command[0] == LSRP_COMMAND)
        execute_lsrp_algorithm(new_command);
    else if(new_command[0] == DVRP_COMMAND)
        execute_dvrp_algorithm(new_command);
}

string del_space(string part)
{
    string output_del = "";
    for(int i = 0; i < part.size(); i++)
    {
        if(part[i] != ' ')
            output_del += part[i];
    }
    return output_del;
}


int main()
{
    Graph* topology_network = new Graph();
    while(true)
    {
        string command;
        getline(cin, command, '\n');
        vector<string> new_command;
        string part = " ";
        int pnt = 0;
        while(true)
        {
            if(command[pnt] == ' ')
            {
                new_command.push_back(del_space(part));
                part = "";
            }
            else 
            {
                part += command[pnt];
            }
            pnt++;
            if(pnt == command.size())
            {
                new_command.push_back(del_space(part));
                break;
            }
        }
        topology_network->run_command(new_command);
    }
    delete topology_network;
}

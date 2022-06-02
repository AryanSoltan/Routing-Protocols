#include<bits/stdc++.h>

using namespace std;

#define TOPOLOGY_INDEX 0
#define TOPOLOGY_COMMAND "topology"
#define SHOW_INDEX 0
#define SHOW_COMMAND "show"


int const MAX_NODES = 500 + 10;

/*struct Edge 
{
    int destination;
    int weight;
};
*/

class Graph 
{
    public:
        void run_command(vector<string> new_command);
        void build_graph(vector<string> new_command);
        void show();
    private:
        int decode(int index, string edge);
        int adjacency[MAX_NODES][MAX_NODES];
        int max_node_index;
};

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

void Graph::run_command(vector<string> new_command)
{
    if(new_command[TOPOLOGY_INDEX] == TOPOLOGY_COMMAND)
    {
        build_graph(new_command);
    }
    else if(new_command[SHOW_INDEX] == SHOW_COMMAND)
        show();
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

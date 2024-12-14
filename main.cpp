#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

class Node{
    public:
        char id;

    Node (char ID) : id(ID){};
};

class Graph{

    public:
        vector<Node> vertices;
        vector<vector<pair<int, int >>> edges; // neighbor, weight: for the edges represented by a list of pairs for each node
        void addNode(char id){
            vertices.push_back(Node(id)); // adding the node to the graph
            edges.push_back(vector<pair<int, int>>()); // initialising the list of pairs of each node
        }

        // a function to find the index of the node
        int getNodeIndex(char id){
            for (int i{0}; i < vertices.size(); i++){
                if(id == vertices.at(i).id){
                    return i;
                }
            }
            return -1; // in case the node is not found
        }

        void addEdge(char from, char to, int weight){
            int fromIndex = getNodeIndex(from); // get the index of the first node
            int toIndex = getNodeIndex(to); // get the index of the second node
            if(fromIndex != -1 && toIndex != -1){ // make sure both nodes are in the graph

                // add the index of second node and weight to the list of edges of the first node
                edges.at(fromIndex).push_back({toIndex, weight});
                edges.at(toIndex).push_back({fromIndex, weight});
            }
        }

        

        void printGraph(){
            for (int i{0}; i < vertices.size(); i++){
                cout << vertices.at(i).id << ": ";
                for (auto edge : edges[i]){
                    cout << "(" << vertices[edge.first].id << ", " << edge.second << ") ";
                }
                cout << endl;
            }
        }
};

int main(){
    fstream myFile;
    Graph graph;
    
    // getting the data from the text file
    myFile.open("testing.txt", ios::in);
    if(myFile.is_open()){
        string line;

        // do this till the last line in the text file
        while(getline(myFile, line)){
            stringstream ss(line);      // create a stringstream from the current line
            char from, to;
            int weight;

            ss >> from >> to >> weight;  // slice the stringstream

            // add the vertices to the graph if they are not added
            if (graph.getNodeIndex(from) == -1) graph.addNode(from);
            if (graph.getNodeIndex(to) == -1) graph.addNode(to);

            // add the edgges of the graph
            graph.addEdge(from, to, weight);
        }
        myFile.close();
    }

    graph.printGraph();

    return 0;
}

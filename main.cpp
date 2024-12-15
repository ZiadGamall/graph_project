#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<queue>

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

        void mst(char start) {
            // Priority queue to store edges in ascending order (to, weight)
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

            vector<bool> visited(vertices.size(), false); // Visited nodes
            vector<pair<pair<int, int>, int>> mstEdges;  // Edges in the MST ((from, to), weight)
            int totalWeight = 0;                         // Total weight of the MST

            int startIndex = getNodeIndex(start); // Get the starting node index

            // Check if the starting node is in the graph
            if(startIndex == -1){
                cout << "The node is not in the graph" << endl;
                return;
            }

            visited[startIndex] = true;

            // Add all edges of the starting node to the priority queue
            for (auto edge : edges[startIndex]) {
                pq.push({edge.first, edge.second}); // Push (weight, to)
            }

            // Main loop to process the priority queue
            while (!pq.empty()) {
                // Get the smallest edge
                int nextIndex = pq.top().first;   // The node this edge leads to
                int weight = pq.top().second;    // Weight of the edge
                pq.pop();                       // Remove the edge from the Q

                if (visited[nextIndex]) continue; // Skip if already visited
                visited[nextIndex] = true;       // Mark the node as visited

                // Find the parent of this node (nextIndex) by searching for the edge in the priority queue
                int parentIndex = -1;
                for (auto edge : edges[nextIndex]) {

                    // The graph is undirected, so we search for the parent node by looking for a node that's visited and has the same weight as the child
                    if (visited[edge.first] && edge.second == weight) {
                        parentIndex = edge.first;
                        break;
                    }
                }

                // Add the edge to the MST and update total weight
                if (parentIndex != -1) {
                    mstEdges.push_back({{parentIndex, nextIndex}, weight}); // Store the edge ((from, to), weight)
                    totalWeight += weight; // Add the weight
                }

                // Push all edges of the newly added node to the queue
                for (auto edge : edges[nextIndex]) {
                    if (!visited[edge.first]) {
                        pq.push({edge.first, edge.second});
                    }
                }
            }

            // Output the MST
            cout << "Edges in the MST:" << endl;
            for (auto edge : mstEdges) {
                cout << vertices[edge.first.first].id << " - " << vertices[edge.first.second].id << " with weight " << edge.second << endl;
            }
            cout << "Total weight of MST: " << totalWeight << endl;
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
    cout << "===========================" << endl;
    graph.mst('A');

    return 0;
}

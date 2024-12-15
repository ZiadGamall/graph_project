#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<queue>
#include<set>
#include<limits>

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
            }
        }

        void mst(char start) {
            // Priority queue to store edges in ascending order (weight, to)
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

            vector<bool> visited(vertices.size(), false); // Visited nodes
            vector<pair<pair<int, int>, int>> mstEdges;   // Edges in the MST ((from, to), weight)
            vector<int> parent(vertices.size(), -1);      // Parent nodes
            int totalWeight = 0;                          // Total weight of the MST

            int startIndex = getNodeIndex(start); // Get the starting node index

            // Check if the starting node is in the graph
            if (startIndex == -1) {
                cout << "The node is not in the graph" << endl;
                return;
            }

            visited[startIndex] = true;

            // Add all edges of the starting node to the priority queue
            for (auto edge : edges[startIndex]) {
                pq.push({edge.second, edge.first}); // Push (weight, to)
                parent[edge.first] = startIndex;    // Set the parent of the connected node to the start node
            }

            // Main loop to process the priority queue
            while (!pq.empty()) {
                // Get the smallest edge
                int weight = pq.top().first;  // Weight of the edge
                int nextIndex = pq.top().second; // The node this edge leads to
                pq.pop(); // Remove the edge from the queue

                if (visited[nextIndex]) continue; // Skip if already visited
                visited[nextIndex] = true;        // Mark the node as visited

                // Add the edge to the MST and update total weight
                mstEdges.push_back({{parent[nextIndex], nextIndex}, weight}); // Store the edge ((from, to), weight)
                totalWeight += weight; // Add the weight

                // Push all edges of the newly added node to the queue
                for (auto edge : edges[nextIndex]) {
                    if (!visited[edge.first]) {
                        pq.push({edge.second, edge.first});
                        parent[edge.first] = nextIndex; // Update parent of the neighbor
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

        vector<int> shortestPath(char source) {
            // Set to act as a priority queue: stores pairs of (distance, node index), sorted by distance
            set<pair<int, int>> st;
            // Distance vector initialized to a large value (INT_MAX) to represent infinity
            vector<int> distance(vertices.size(), INT_MAX);

            // Get the index of the source node
            int sourceIndex = getNodeIndex(source);
            if (sourceIndex == -1) {
                // If source node is not in the graph, print an error and return the distance vector
                cout << "Couldn't find the source in the graph" << endl;
                return distance;
            }

            // Initialize the source node with a distance of 0
            st.insert({0, sourceIndex});
            distance[sourceIndex] = 0;

            // Main loop: process nodes in increasing order of distance
            while (!st.empty()) {
                // Extract the node with the smallest distance
                auto itr = *(st.begin());
                int weight = itr.first;  // Current shortest distance to this node
                int node = itr.second;   // Node index
                st.erase(itr);

                // Relax all edges from the current node
                for (auto edge : edges[node]) {
                    int nextNode = edge.first;       // Neighboring node
                    int nextWeight = edge.second;   // Edge weight

                    // Check if a shorter path to nextNode is found
                    if (weight + nextWeight < distance[nextNode]) {
                        // Remove the old distance entry from the set (if exists)
                        if (distance[nextNode] != INT_MAX) {
                            st.erase({distance[nextNode], nextNode});
                        }

                        // Update the distance to nextNode and add it back to the set
                        distance[nextNode] = weight + nextWeight;
                        st.insert({distance[nextNode], nextNode});
                    }
                }
            }

            // Output the shortest path distances
            for (int i = 0; i < distance.size(); i++) {
                if (distance[i] == INT_MAX) {
                    cout << "Distance from " << source << " to " << vertices[i].id << " - Unreachable" << endl;
                } else {
                    cout << "Distance from " << source << " to " << vertices[i].id << " - " << distance[i] << endl;
                }
            }

            return distance;
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
    cout << "===========================" << endl;
    graph.shortestPath('A');

    return 0;
}

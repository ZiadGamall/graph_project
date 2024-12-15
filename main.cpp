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

        void mst(char start, const string& outputFilename) {
            ofstream outFile(outputFilename, ios::app); // Open file in append mode
            if (!outFile.is_open()) {
                cout << "Failed to open the output file for MST." << endl;
                return;
            }

            // Priority queue to store edges in ascending order (weight, to)
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

            vector<bool> visited(vertices.size(), false); // Visited nodes
            vector<pair<pair<int, int>, int>> mstEdges;   // Edges in the MST ((from, to), weight)
            vector<int> parent(vertices.size(), -1);      // Parent nodes
            int totalWeight = 0;                          // Total weight of the MST

            int startIndex = getNodeIndex(start); // Get the starting node index

            // Check if the starting node is in the graph
            if (startIndex == -1) {
                outFile << "The node is not in the graph" << endl;
                outFile.close();
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
                int weight = pq.top().first;  // Weight of the edge
                int nextIndex = pq.top().second; // The node this edge leads to
                pq.pop();

                if (visited[nextIndex]) continue; // Skip if already visited
                visited[nextIndex] = true;        // Mark the node as visited

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

            // Write MST to file
            outFile << "Edges in the MST:" << endl;
            for (auto edge : mstEdges) {
                outFile << vertices[edge.first.first].id << " - " << vertices[edge.first.second].id 
                        << " with weight " << edge.second << endl;
            }
            outFile << "Total weight of MST: " << totalWeight << endl;
            outFile << "===========================" << endl;

            outFile.close(); // Close the file
        }


        vector<int> shortestPath(char source, const string& outputFilename) {
            ofstream outFile(outputFilename, ios::app); // Open file in append mode
            if (!outFile.is_open()) {
                cout << "Failed to open the output file for Shortest Path." << endl;
                return {};
            }

            set<pair<int, int>> st; // Acts as a priority queue (distance, node index)
            vector<int> distance(vertices.size(), INT_MAX); // Distance to each node (initialized to infinity)

            int sourceIndex = getNodeIndex(source);
            if (sourceIndex == -1) {
                outFile << "Couldn't find the source in the graph" << endl;
                outFile.close();
                return distance;
            }

            st.insert({0, sourceIndex}); // Initialize the source node with distance 0
            distance[sourceIndex] = 0;

            while (!st.empty()) {
                auto itr = *(st.begin());
                int weight = itr.first;  // Current shortest distance to this node
                int node = itr.second;   // Node index
                st.erase(itr);

                for (auto edge : edges[node]) {
                    int nextNode = edge.first;       // Neighboring node
                    int nextWeight = edge.second;   // Edge weight

                    if (weight + nextWeight < distance[nextNode]) {
                        if (distance[nextNode] != INT_MAX) {
                            st.erase({distance[nextNode], nextNode});
                        }
                        distance[nextNode] = weight + nextWeight;
                        st.insert({distance[nextNode], nextNode});
                    }
                }
            }

            // Write Shortest Path to file
            outFile << "Shortest paths from " << source << ":" << endl;
            for (int i = 0; i < distance.size(); i++) {
                if (distance[i] == INT_MAX) {
                    outFile << source << " -> " << vertices[i].id << ": Unreachable" << endl;
                } else {
                    outFile << source << " -> " << vertices[i].id << ": " << distance[i] << endl;
                }
            }
            outFile << "===========================" << endl;

            outFile.close(); // Close the file
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
    graph.mst('A', "mst.txt");
    graph.shortestPath('A', "shortest path.txt");

    return 0;
}

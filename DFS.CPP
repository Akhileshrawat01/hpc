#include <bits/stdc++.h>
#include <ctime>
#include <omp.h>

using std :: chrono :: duration_cast;
using std :: chrono :: high_resolution_clock;
using std :: chrono :: milliseconds;

/*
    <chrono> is a C++ header that provides a collection of types and functions to work with time.
*/

using namespace std;


// Function to compute the performance of sequential and parallel execution of Bubblesort
int analysis(std :: function<void()> function);

void parallel_dfs(vector<vector<int>>& graph);
void dfs(int start_vertex, vector<bool>& visited, vector<vector<int>>& graph);

int main(void) {

    int vertices = 100000;
    int edges = 400000;

    vector<vector<int>> graph(vertices);
    vector<bool> visited(vertices, false);
    float speed_up = 0.0f;

    for (int i = 0; i < 90000000; i++)
    {
        int u = rand() % vertices;
        int v = rand() % vertices;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

     while (true) {
        cout << "Menu:\n";
        cout << "1. Execute DFS without parallel computation\n";
        cout << "2. Execute DFS with parallel computation\n";
        cout << "3. Display Speed Up\n";
        cout << "4. Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int execution_without_threads = analysis([&] {dfs(0, visited, graph); });    
                cout << "DFS without parallel Computation: " << execution_without_threads << endl;
                break;
            }
            case 2: {
                int parallel_execution = analysis([&] {parallel_dfs(graph); });
                cout << "Parallel Computing DFS With threads: " << parallel_execution << endl;
                break;
            }
            case 3: {
                int execution_without_threads = analysis([&] {dfs(0, visited, graph); });    
                int parallel_execution = analysis([&] {parallel_dfs(graph); });
                speed_up = (float)(execution_without_threads / (float)(parallel_execution));
                cout << "DFS without parallel Computation: " << execution_without_threads << endl;
                cout << "Parallel Computing DFS With threads: " << parallel_execution << endl;
                cout << "Speed up: " << speed_up << endl;
                break;
            }
            case 4:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice\n";
        }
    }


    // int execution_without_threads = analysis([&] {dfs(0, visited, graph); });    
    // int parallel_execution = analysis([&] {parallel_dfs(graph); });

    // speed_up = (float)(execution_without_threads/ (float)(parallel_execution));
    
    // cout << "DFS without parallel Computation    : " << execution_without_threads << endl;
    // cout << "Parallel Computing DFS With threads : " << parallel_execution << endl;
    // cout << "Speed up : " << speed_up << endl;

    return 0;
}

// Parallel Depth First Search

void parallel_dfs(vector<vector<int>>& graph) {

    int total_vertices = graph.size();
    vector<bool> visited(total_vertices, false);


    // Adding parallel directive.
    
    #pragma omp parallel for num_threads(16)
    for (int v = 0; v < total_vertices; v ++)
    {
        if (!visited[v])
        {
            dfs(v, visited, graph);
        }
    }
}


void dfs(int start_vertex, vector<bool>& visited, vector<vector<int>>& graph) {

    stack<int> Stack;

    visited[start_vertex] = true;
    Stack.push(start_vertex);

    while (!Stack.empty())
    {
        int vertex = Stack.top(); Stack.pop();

        #pragma omp parallel for
        for (int i = 0; i < graph[vertex].size(); i++)
        {
            int u = graph[vertex][i];
            #pragma omp critical
            {
                if (!visited[u])
                {
                    visited[u] = true;
                    Stack.push(u);
                }
            }
        }
    }
}

int analysis(std :: function<void()> function) {

    // subtract end from start timepoints and cast it to required unit, nanoseconds, microseconds, milliseconds etc.
    auto start = high_resolution_clock::now();
    
    // Executing function
    function();
    auto end   = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    return duration.count();
}
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

void bfs(int start_vertex, vector<bool>& visited, vector<vector<int>>& graph);
void parallel_BFS(vector<vector<int>>& graph);

int main(void) {

    int num_of_vertices = 100000;
    int num_of_edges = 400000;
    float speed_up = 0.0f;

    vector<vector<int>> graph(num_of_vertices);
    vector<bool> visited(num_of_vertices, false);

    for (int i = 0; i < 100000; i++)
    {
        int u = rand() % num_of_vertices;
        int v = rand() % num_of_vertices;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

     while (true) {
        cout << "Menu:\n";
        cout << "1. Sequential Execution\n";
        cout << "2. Parallel Execution\n";
        cout << "3. Speed Up\n";
        cout << "4. Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int sequential_execution = analysis([&] { bfs(0, visited, graph); });
                cout << "Sequential Execution : " << sequential_execution << " ms\n";
                break;
            }
            case 2: {
                int parallel_execution = analysis([&] { parallel_BFS(graph); });
                cout << "Parallel Execution   : " << parallel_execution << " ms\n";
                break;
            }
            case 3: {
                int sequential_execution = analysis([&] { bfs(0, visited, graph); });
                int parallel_execution = analysis([&] { parallel_BFS(graph); });
                float speed_up = (float)(sequential_execution) / parallel_execution;
                cout << "Sequential Execution : " << sequential_execution << " ms\n";
                cout << "Parallel Execution   : " << parallel_execution << " ms\n";
                cout << "Speed up             : " << speed_up << endl;
                break;
            }
            case 4:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice\n";
        }
    }

    return 0;
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

void bfs(int start_vertex, vector<bool>& visited, vector<vector<int>>& graph) {

    queue<int> Queue;

    visited[start_vertex] = true;
    Queue.push(start_vertex);

    while (!Queue.empty())
    {
        int vertex = Queue.front(); Queue.pop();
        // cout << vertex << endl;

        #pragma omp parallel for 
        for (int i = 0; i < graph[vertex].size(); i++)
        {
            int u = graph[vertex][i];
            #pragma omp critical 
            if (!visited[u])
            {
                visited[u] = true;
                Queue.push(u);
            }
        }
    }
}

void parallel_BFS(vector<vector<int>>& graph) {

    int num_of_vertices = graph.size();
    vector<bool> visited(num_of_vertices, false);

    //#pragma omp parallel for num_threads(16)

    for (int v = 0; v < num_of_vertices; v++)
    {
        if (!visited[v])
        {
            bfs(v, visited, graph);
        }
    }
}

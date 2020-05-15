// Алексеев Сергей АПО-13
// Модуль 3 задание 2

// Условие:
// Дан невзвешенный неориентированный граф.
// В графе может быть несколько кратчайших путей между какими-то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами.
// Требуемая сложность O(V+E).

#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

class ListGraph{
public:
    ListGraph(int size): table(size) {}
    ~ListGraph() = default;
    void addEdge(int from, int to) {
        assert(0 <= from && from < table.size());
        assert(0 <= to && to < table.size());
        table[from].push_back(to);
    }
    int verticesCount() const {
        return table.size();
    }
    vector<int> getNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < table.size());
        return table[vertex];
    }
    vector<int> getPrevVertices(int vertex) const {
        assert(0 <= vertex && vertex < table.size());
        vector<int> result;
        for(size_t i = 0; i < table.size(); ++i) {
            for(auto ver: table[i]) {
                if(ver == vertex) {
                    result.push_back(i);
                    break;
                }
            }
        }
        return result;
    }
private:
    vector<vector<int>> table;
};


int numberShortPaths(ListGraph & graph, int from, int to) {
    vector<pair<int, int>> dist(graph.verticesCount(), pair<int, int>(-1, 0));
    // первый инт - размер кратчайшего пути, второй - количество кратчайших путей
    queue<int> q;
    q.push(from);
    dist[from].first = 0;
    while(!q.empty()) {
        int currentV = q.front(); q.pop();
        for(int v : graph.getNextVertices(currentV)) {
            if(dist[v].first == dist[currentV].first + 1) {
                dist[v].second += dist[currentV].second;
            } else if(dist[v].first == -1 || dist[v].first > dist[currentV].first + 1) {
                q.push(v);
                dist[v].first = dist[currentV].first + 1;
                dist[v].second += dist[currentV].second;
                if(dist[v].second == 0) {
                    dist[v].second = 1;
                }
            }
        }
    }
    return dist[to].second;
}

int main() {
    int n;
    cin >> n;   //колво вершин
    ListGraph graph(n);
    cin >> n;   //колов ребер
    int v1, v2;
    for(size_t i = 0; i < n; ++i) {
        cin >> v1 >> v2;
        graph.addEdge(v1, v2);
        graph.addEdge(v2, v1);
    }
    cin >> v1 >> v2;
    cout << numberShortPaths(graph, v1, v2);
    return 0;
}


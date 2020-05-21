// Алексеев Сергей АПО-13
// Модуль 3 задание 3

// Условие:
// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
// Требуемое время работы O((N + M)log N), где N – количество городов,
// M – известных дорог между ними.
// N ≤ 10000, M ≤ 250000.
// Длина каждой дороги ≤ 10000.

#include <iostream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

class ListGraph {
public:
    ListGraph(int size): table(size) {}
    ~ListGraph() = default;
    void addEdge(int from, int to, int weight) {
        assert(0 <= from && from < table.size());
        assert(0 <= to && to < table.size());
        assert(0 <= weight);
        table[from].emplace_back(weight, to);
    }
    size_t verticesCount() const {
        return table.size();
    }
    vector< pair<int, int> > getNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < table.size());
        return table[vertex];
    }

private:
    vector<vector< pair<int, int> >> table;
};

int profitRoute(const ListGraph & graph, int from, int to) {
    vector<int> dist(graph.verticesCount(), -1);
    set< pair<int, int> > heap; //первый - расстояние, второй - вершина
    dist[from] = 0;
    heap.emplace(dist[from], from);
    while(!heap.empty()) {
        auto it = heap.begin();
        int v = it->second;
        heap.erase(it);
        for(auto tmpPair: graph.getNextVertices(v)) {
            int weight = tmpPair.first;
            int tmpV = tmpPair.second;
            if(dist[tmpV] == -1) {
                dist[tmpV] = dist[v] + weight;
                heap.emplace(dist[tmpV], tmpV);
            } else if (dist[tmpV] > weight + dist[v]) {
                auto tmpIt = heap.find(pair<int, int>(dist[tmpV], tmpV));
                heap.erase(tmpIt);
                dist[tmpV] = weight + dist[v];
                heap.emplace(dist[tmpV], tmpV);
            }
        }
    }
    return dist[to];
}

int main() {
    int n;
    cin >> n; //кол-во городов
    ListGraph graph(n);
    cin >> n; //кол-во дорог
    int from, to, w;
    for(size_t i = 0; i < n; ++i) {
        cin >> from >> to >> w;
        graph.addEdge(from, to, w);
        graph.addEdge(to, from, w);
    }
    cin >> from >> to;
    cout << profitRoute(graph, from, to);
    return 0;
}

// Алексеев Сергей АПО-13
// Модуль 3 задание 5(1)

// Условие:
// Дан неориентированный связный граф.
// Требуется найти вес минимального остовного
// дерева в этом графе.
// Вариант 1.
// Для построения минимального остовного дерева используйте алгоритм Крускала. (из задания в google документах)

#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
#include <algorithm>

using namespace std;

struct Edge {
    Edge(size_t from, size_t to, size_t w): start(from), end(to), weight(w) {}
    size_t start;
    size_t end;
    size_t weight;
};

class EdgeGraph {
public:
    EdgeGraph(size_t size): nTops(size) {}
    ~EdgeGraph() = default;
    void addEdge(int from, int to, int w) {
        edges.emplace_back(from, to, w);
    }
    size_t verticesCount() const {
        return nTops;
    }
    vector<Edge> & getEdges() {
        return edges;
    }
private:
    vector<Edge> edges;
    size_t nTops;
};

class DSU {
public:
    DSU(size_t size) : rank(size, 1), parent(size) {
        for (auto i = 0; i < size; i++) {
            parent[i] = i;
        }
    }
    size_t find_set(size_t u) {
        stack<size_t> stack;
        size_t current = u;

        stack.push(current);
        while (current != parent[current]) {
            stack.push(parent[current]);
            current = parent[current];
        }
        size_t root = current;
        while (!stack.empty()) {
            parent[stack.top()] = root;
            stack.pop();
        }
        return root;
    }

    void union_set(size_t u, size_t v) {
        u = find_set(u);
        v = find_set(v);
        if (u != v) {
            if (rank[u] < rank[v]) {
                swap(u, v);
            }
            parent[v] = u;
            rank[u] += rank[v];
        }
    }
private:
    vector<size_t> parent;
    vector<size_t> rank;
};

size_t minOstTree(EdgeGraph & graph) {
    auto edges = graph.getEdges();
    sort(edges.begin(), edges.end(), [](const Edge & e1, const Edge & e2) {
        return e1.weight < e2.weight;
    });
    DSU dsu(graph.verticesCount());
    size_t result = 0;
    for(const auto & edge: edges) {
        if(dsu.find_set(edge.start) != dsu.find_set(edge.end)) {
            dsu.union_set(edge.end, edge.start);
            result += edge.weight;
        }
    }
    return result;
}

int main() {
    int n, m;
    cin >> n;
    EdgeGraph graph(n + 1); // n не с нуля, а с 1
    cin >> m;
    for(size_t i = 0; i < m; ++i) {
        int from, to, w;
        cin >> from >> to >> w;
        graph.addEdge(from, to, w);
    }
    cout << minOstTree(graph);
    return 0;
}

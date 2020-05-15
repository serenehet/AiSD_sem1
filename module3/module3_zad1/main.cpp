#include <iostream>
#include <queue>
#include <vector>
#include <cassert>
#include <set>

using namespace std;

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph: public IGraph {
public:
    ListGraph(int size): table(size) {}
    ListGraph(const IGraph & graph): table(graph.VerticesCount()) {
        for(int i = 0; i < table.size(); ++i) {
            table[i] = graph.GetNextVertices(i);
        }
    }
    ~ListGraph() override = default;
    void AddEdge(int from, int to) override {
        assert(0 <= from && from < table.size());
        assert(0 <= to && to < table.size());
        table[from].push_back(to);
    }
    int VerticesCount() const override {
        return table.size();
    }
    vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < table.size());
        return table[vertex];
    }
    vector<int> GetPrevVertices(int vertex) const override {
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

class MatrixGraph: public IGraph {
public:
    MatrixGraph(int size): table(size, vector<bool>(size, false)) {}
    MatrixGraph(const IGraph & graph): table(graph.VerticesCount(), vector<bool>(graph.VerticesCount(), false)) {
        for(size_t i = 0; i < graph.VerticesCount(); ++i) {
            auto toArr = graph.GetNextVertices(i);
            for(auto to: toArr) {
                table[i][to] = true;
            }
        }
    }
    ~MatrixGraph() override = default;
    void AddEdge(int from, int to) override {
        assert(0 <= from && from < table.size());
        assert(0 <= to && to < table.size());
        table[from][to] = true;
    }
    int VerticesCount() const override {
        return table.size();
    }
    vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < table.size());
        vector<int> result;
        for(size_t i = 0; i < table.size(); ++i) {
            if(table[vertex][i]) {
                result.push_back(i);
            }
        }
        return result;
    }
    vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < table.size());
        vector<int> result;
        for(size_t i = 0; i < table.size(); ++i) {
            if(table[i][vertex]) {
                result.push_back(i);
            }
        }
        return result;
    }
private:
    vector<vector<bool>> table;
};

class SetGraph: public IGraph {
public:
    SetGraph(int size): table(size) {}
    SetGraph(const IGraph & graph): table(graph.VerticesCount()) {
        for(size_t i = 0; i < table.size(); ++i) {
            auto toArr = graph.GetNextVertices(i);
            for(auto to: toArr) {
                table[i].insert(to);
            }
        }
    }
    ~SetGraph() override = default;
    void AddEdge(int from, int to) override {
        assert(0 <= from && from < table.size());
        assert(0 <= to && to < table.size());
        table[from].insert(to);
    }
    int VerticesCount() const override {
        return table.size();
    }
    vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < table.size());
        vector<int> result;
        for(auto v: table[vertex]) {
            result.push_back(v);
        }
        return result;
    }
    vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < table.size());
        vector<int> result;
        for(size_t i = 0; i < table.size(); ++i) {
            if(table[i].count(vertex)) {
                result.push_back(i);
            }
        }
        return result;
    }
private:
    vector<set<int>> table;
};

class ArcGraph: public IGraph {
public:
    ArcGraph(int size) : size(size) {}
    ArcGraph(const IGraph & graph): size(graph.VerticesCount()) {
        for(size_t i = 0; i < size; ++i) {
            auto toArr = graph.GetNextVertices(i);
            for(int to: toArr) {
                edges.emplace_back(i, to);
            }
        }
    }
    ~ArcGraph() override = default;
    void AddEdge(int from, int to) override {
        assert(0 <= from && from < size);
        assert(0 <= to && to < size);
        edges.emplace_back(from, to);
    }
    int VerticesCount() const override {
        return size;
    }
    vector<int> GetNextVertices(int vertex) const override {
        assert(0 <= vertex && vertex < size);
        vector<int> result;
        for(auto edge: edges) {
            if(edge.first == vertex) {
                result.emplace_back(edge.second);
            }
        }
        return result;
    }
    vector<int> GetPrevVertices(int vertex) const override {
        assert(0 <= vertex && vertex < size);
        vector<int> result;
        for(auto edge: edges) {
            if(edge.second == vertex) {
                result.emplace_back(edge.first);
            }
        }
        return result;
    }

private:
    vector<pair<int, int>> edges;
    int size;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, std::function<void(int)> &func) {
    queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int currentVertex = qu.front();
        qu.pop();
        func(currentVertex);
        for (int nextVertex: graph.GetNextVertices(currentVertex)) {
            if (!visited[nextVertex]) {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, std::function<void(int)> func) {
    vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

int main() {
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    mainBFS(listGraph, [](int vertex){
        cout << vertex << " ";
    });
    cout << endl;
    MatrixGraph matrixGraph(listGraph);
    SetGraph setGraph(matrixGraph);
    ArcGraph arcGraph(setGraph);
    mainBFS(arcGraph, [](int vertex){
        cout << vertex << " ";
    });
    cout << endl;
    return 0;
}

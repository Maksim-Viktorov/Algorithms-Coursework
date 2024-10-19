#include <vector>
#include <iostream>
#include <queue>

struct Edge {
  int u = 0;
  int f = 0;
  int c = 0;
  size_t back = 0;
  Edge(int u0, int c0) : u(u0), c(c0) {
  }
};

class Graph {
  std::vector<std::vector<Edge>> graph_;
  int size_;

 public:
  explicit Graph(int n) : graph_(n + 1), size_(n) {
  }
  void AddEdge(int v, int u, int c) {
    graph_[v].emplace_back(Edge(u, c));
    graph_[u].emplace_back(Edge(v, 0));
    graph_[v].back().back = graph_[u].size() - 1;
    graph_[u].back().back = graph_[v].size() - 1;
  }
  std::vector<Edge*> BFS(int start, int finish, int& cmin) {
    int inf = static_cast<int>(graph_.size());
    std::vector<std::pair<int, Edge*>> parent(graph_.size());
    parent[start].first = start;
    std::vector<int> dist(graph_.size(), inf);
    dist[start] = 0;
    std::queue<int> queue;
    queue.push(start);
    while (!queue.empty()) {
      int v = queue.front();
      queue.pop();
      for (auto& edge : graph_[v]) {
        int u = edge.u;
        if (edge.f < edge.c && dist[u] == inf) {
          dist[u] = dist[v] + 1;
          parent[u].first = v;
          parent[u].second = &edge;
          queue.push(u);
        }
      }
    }
    if (dist[finish] == inf) {
      return {};
    }
    std::vector<Edge*> path(dist[finish]);
    int i = dist[finish] - 1;
    int cur_vert = finish;
    Edge* cur_edge = parent[finish].second;
    while (cur_vert != start) {
      if (cmin > (*cur_edge).c - (*cur_edge).f) {
        cmin = (*cur_edge).c - (*cur_edge).f;
      }
      path[i] = cur_edge;
      cur_vert = parent[cur_vert].first;
      cur_edge = parent[cur_vert].second;
      --i;
    }
    return path;
  }
  int EK() {
    int flow = 0;
    while (true) {
      int cmin = 1'000'000'000;
      auto path = BFS(1, size_, cmin);
      if (path.empty()) {
        return flow;
      }
      for (auto& edge : path) {
        edge->f += cmin;
        graph_[edge->u][edge->back].f -= cmin;
      }
      flow += cmin;
    }
    return flow;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n = 0;
  int m = 0;
  int v = 0;
  int u = 0;
  int c = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> v >> u >> c;
    graph.AddEdge(v, u, c);
  }
  std::cout << graph.EK() << '\n';
}

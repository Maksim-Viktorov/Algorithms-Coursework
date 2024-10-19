#include <iostream>
#include <queue>
#include <vector>

class UnorientedGraph {
  std::vector<std::vector<std::pair<int, int>>> graph_;

public:
  UnorientedGraph() = default;
  explicit UnorientedGraph(int n) : graph_(n + 1) {
  }

 public:
  void AddEdge(int start, int finish) {
    graph_[start].emplace_back(finish, weight);
  }
  std::vector<int> BFS(int start, int finish) {
    int inf = graph_.size();
    std::vector<int> parent(graph_.size() + 1);
    parent[start] = start;
    std::vector<int> dist(graph_.size() + 1);
    for (size_t i = 1; i < dist.size(); ++i) {
      dist[i] = inf;
    }
    dist[start] = 0;
    std::queue<int> queue;
    queue.push(start);
    while (!queue.empty()) {
      int v = queue.front();
      queue.pop();
      for (size_t i = 0; i < graph[v].size(); ++i) {
        int u = graph[v][i];
        if (dist[u] > dist[v] + 1) {
          dist[u] = dist[v] + 1;
          parent[u] = v;
          queue.push(u);
        }
      }
    }
    if (dist[finish] == inf) {
      return std::vector<int>();
    }
    std::vector<int> path(dist[finish] + 1);
    size_t i = dist[finish];
    int cur_pos = finish;
    while (cur_pos != start) {
      path[i] = cur_pos;
      cur_pos = parent[cur_pos];
      --i;
    }
    path[i] = cur_pos;
    return path;
  }
};

int main() {
  size_t n, m;
  std::cin >> n >> m;
  UnorientedGraph graph(n);
  int start, finish, a, b;
  std::cin >> start >> finish;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> a >> b;
    graph.graph[a].emplace_back(b);
    graph.graph[b].emplace_back(a);
  }
  std::vector<int> path(graph.BFS(start, finish));
  std::cout << int(path.size()) - 1 << '\n';
  for (size_t i = 0; i < path.size(); ++i) {
    std::cout << path[i] << ' ';
  }
  return 0;
}

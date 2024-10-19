#include <iostream>
#include <deque>
#include <vector>

class OrientedGraph {
  int max_weight_ = 0;
  int edge_count_ = 0;
  std::vector<std::vector<std::pair<int, int>>> graph_;

 public:
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph_(n + 1) {
  }
  void AddEdge(int start, int finish, int weight) {
    if (weight > max_weight_) {
      max_weight_ = weight;
    }
    ++edge_count_;
    graph_[start].emplace_back(finish, weight);
  }
  int BFS(int start, int finish) {
    int inf = (edge_count_ + 1) * max_weight_;
    std::vector<int> dist(graph_.size() + 1, inf);
    dist[start] = 0;
    std::deque<int> deque;
    deque.push_back(start);
    while (!deque.empty()) {
      int v = deque.front();
      deque.pop_front();
      for (size_t i = 0; i < graph_[v].size(); ++i) {
        int u = graph_[v][i].first;
        if (dist[u] > dist[v] + graph_[v][i].second) {
          dist[u] = dist[v] + graph_[v][i].second;
          if (graph_[v][i].second == 0) {
            deque.push_front(u);
          } else {
            deque.push_back(u);
          }
        }
      }
    }
    return dist[finish] == inf ? -1 : dist[finish];
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n, m, k;
  std::cin >> n >> m;
  OrientedGraph graph(n);
  int start, finish, a, b;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    graph.AddEdge(a, b, 0);
    graph.AddEdge(b, a, 1);
  }
  std::cin >> k;
  for (int i = 0; i < k; ++i) {
    std::cin >> start >> finish;
    std::cout << graph.BFS(start, finish) << '\n';
  }
  return 0;
}

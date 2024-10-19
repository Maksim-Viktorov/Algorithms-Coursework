#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int v = 0;
  int u = 0;
  int w = 0;
  explicit Edge(int v0, int u0, int w0) : v(v0), u(u0), w(w0) {
  }
};

class SetSystem {
  int count_;
  std::vector<int> rank_;
  std::vector<int> parent_;

 public:
  explicit SetSystem(int n) : count_(n), rank_(n + 1), parent_(n + 1) {
    for (int i = 1; i < n + 1; ++i) {
      rank_[i] = 1;
      parent_[i] = i;
    }
  }
  int FindSet(int x) {
    while (x != parent_[x]) {
      x = parent_[x];
    }
    return x;
  }
  void Unite(int x, int y) {
    x = FindSet(x);
    y = FindSet(y);
    if (x == y) {
      return;
    }
    if (rank_[x] < rank_[y]) {
      parent_[x] = y;
    } else if (rank_[x] == rank_[y]) {
      ++rank_[y];
      parent_[x] = y;
    } else {
      parent_[y] = x;
    }
    --count_;
  }
  int SetCount() {
    return count_;
  }
};

class Graph {
  std::vector<Edge> edges_;
  int max_weight_ = 0;
  int size_ = 0;

 public:
  explicit Graph(int n) : size_(n) {
  }
  void AddEdge(int v, int u, int w) {
    if (w > max_weight_) {
      max_weight_ = w;
    }
    edges_.emplace_back(v, u, w);
  }
  int64_t Boruvka() {
    int64_t mst = 0;
    SetSystem sys(size_);
    while (sys.SetCount() > 1) {
      std::vector<Edge> min_edge(size_ + 1, Edge(size_ + 2, size_ + 2, max_weight_ + 1));
      for (auto edge : edges_) {
        int component_v = sys.FindSet(edge.v);
        int component_u = sys.FindSet(edge.u);
        if (component_v != component_u) {
          if (edge.w < min_edge[component_v].w ||
              (edge.w == min_edge[component_v].w && component_u < min_edge[component_v].u)) {
            min_edge[component_v] = Edge(component_v, component_u, edge.w);
          }
          if (edge.w < min_edge[component_u].w ||
              (edge.w == min_edge[component_u].w && component_v < min_edge[component_u].u)) {
            min_edge[component_u] = Edge(component_u, component_v, edge.w);
          }
        }
      }
      for (auto edge : min_edge) {
        if (edge.w != max_weight_ + 1) {
          mst += edge.w;
          if (min_edge[edge.v].v == min_edge[edge.u].u) {
            min_edge[edge.v].w = max_weight_ + 1;
            min_edge[edge.u].w = max_weight_ + 1;
          }
          sys.Unite(edge.v, edge.u);
        }
      }
    }
    return mst;
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
  int w = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> v >> u >> w;
    graph.AddEdge(v, u, w);
  }
  std::cout << graph.Boruvka() << '\n';
}

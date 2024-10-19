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

inline bool Cmp(const Edge& lhs, const Edge& rhs) {
  return lhs.w < rhs.w;
}

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
  bool Unite(int x, int y) {
    x = FindSet(x);
    y = FindSet(y);
    if (x == y) {
      return false;
    }
    if (rank_[x] < rank_[y]) {
      parent_[x] = y;
    } else if (rank_[x] == rank_[y]) {
      ++rank_[y];
      parent_[x] = y;
    } else {
      parent_[y] = x;
    }
    return --count_ == 1;
  }
};

class Graph {
  std::vector<Edge> edges_;
  int size_ = 0;

 public:
  explicit Graph(int n) : size_(n) {
  }
  void AddEdge(int v, int u, int w) {
    edges_.emplace_back(v, u, w);
  }
  int Kruskal() {
    int mst = 0;
    std::sort(edges_.begin(), edges_.end(), Cmp);
    SetSystem sys(size_);
    for (auto& edge : edges_) {
      if (sys.FindSet(edge.v) != sys.FindSet(edge.u)) {
        sys.Unite(edge.v, edge.u);
        mst += edge.w;
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
  std::cout << graph.Kruskal() << '\n';
}

#include <iostream>
#include <vector>

class Heap {
  int size_ = 0;
  std::vector<std::pair<int, int>> heap_;
  std::vector<int> map_;

 public:
  explicit Heap(int max_size) : heap_(max_size), map_(max_size, -1) {
  }
  int Parent(int i) {
    return (i - 1) / 2;
  }
  int LeftChild(int i) {
    return i * 2 + 1;
  }
  int RightChild(int i) {
    return i * 2 + 2;
  }
  int SiftUp(int i) {
    int parent = Parent(i);
    if (i > 0 && heap_[parent].first > heap_[i].first) {
      std::swap(heap_[parent], heap_[i]);
      std::swap(map_[heap_[parent].second], map_[heap_[i].second]);
      i = SiftUp(parent);
    }
    return i;
  }
  void Add(int x, int w) {
    heap_[size_++].first = w;
    heap_[size_ - 1].second = x;
    map_[x] = size_ - 1;
    SiftUp(size_ - 1);
  }
  int SiftDown(int i) {
    int left = LeftChild(i);
    int right = RightChild(i);
    int largest = i;
    if (left < size_ && heap_[left].first < heap_[largest].first) {
      largest = left;
    }
    if (right < size_ && heap_[right].first < heap_[largest].first) {
      largest = right;
    }
    if (largest != i) {
      std::swap(heap_[largest], heap_[i]);
      std::swap(map_[heap_[largest].second], map_[heap_[i].second]);
      i = SiftDown(largest);
    }
    return i;
  }
  std::pair<int, int> ExtractMin() {
    if (size_ > 1) {
      auto val = heap_[0];
      --size_;
      std::swap(heap_[0], heap_[size_]);
      std::swap(map_[heap_[0].second], map_[heap_[size_].second]);
      SiftDown(0);
      return val;
    }
    --size_;
    return heap_[0];
  }
  void DecreaseKey(int x, int new_w) {
    int i = map_[x];
    heap_[i].first = new_w;
    SiftUp(i);
  }
  bool Empty() {
    return size_ == 0;
  }
  bool Contains(int i) {
    return map_[i] < size_;
  }
};

class Graph {
  std::vector<std::vector<std::pair<int, int>>> graph_;
  int max_weight_ = 0;

 public:
  explicit Graph(int n) : graph_(n) {
  }
  void AddEdge(int start, int finish, int weight) {
    if (weight > max_weight_) {
      max_weight_ = weight;
    }
    graph_[start - 1].emplace_back(weight, finish - 1);
    graph_[finish - 1].emplace_back(weight, start - 1);
  }
  int MST() {
    int mst = 0;
    int inf = max_weight_ + 1;
    std::vector<int> dist(graph_.size(), inf);
    dist[0] = 0;
    std::vector<int> prev(graph_.size(), -1);
    Heap heap(static_cast<int>(graph_.size()));
    for (size_t i = 0; i < graph_.size(); ++i) {
      heap.Add(static_cast<int>(i), dist[i]);
    }
    while (!heap.Empty()) {
      auto min = heap.ExtractMin();
      int v = min.second;
      int w = min.first;
      mst += w;
      for (auto [w, u] : graph_[v]) {
        if (heap.Contains(u) && w < dist[u]) {
          prev[u] = v;
          dist[u] = w;
          heap.DecreaseKey(u, w);
        }
      }
    }
    for (int i = 0; i < graph_.size(); ++i) {
      std::cout << dist[i] << ' ';
    }
    std::cout << '\n';
    return mst;
  }
};

int main() {
  int n = 0;
  int m = 0;
  int start = 0;
  int finish = 0;
  int weight = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> start >> finish >> weight;
    graph.AddEdge(start, finish, weight);
  }
  std::cout << graph.MST();
  return 0;
}

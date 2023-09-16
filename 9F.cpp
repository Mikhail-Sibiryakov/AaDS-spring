#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <deque>
using std::cin, std::cout, std::vector;

enum class Color { WHITE, GRAY, BLACK };

const int INF = 2 * 1e9;

struct Node {
  int value = INF;
  int number = -INF;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  // Это есть только у вершины Splat-дерева
  // указатель на то, к какой ноде подвешено путь-splay-дерево
  Node* path_parent = nullptr;

  explicit Node(int val, int numb) : value(val), number(numb), left(nullptr),
                                     right(nullptr), parent(nullptr) {}
  Node() = default;
};

Node data[200'200];
int sz = 0;

Node* p(Node* v) {
  return v->parent;
}

Node* g(Node* v) {
  return p(p(v));
}

void RotateLeft(Node* v) {
  Node* p = v->parent;
  Node* r = v->right; // это x на картинке
  Node* pp = v->path_parent;
  v->path_parent = nullptr;
  // Если был родитель, то у него теперь сменился сын: либо левый, либо правый,
  // так как поворот
  if (p != nullptr) {
    if (p->left == v) {
      p->left = r;
    } else {
      p->right = r;
    }
  }
  // Перевешиваем штуку и проверяем, что все вершины теперь ок
  Node* tmp = r->left;
  r->left = v;
  v->right = tmp;
  v->parent = r;
  r->parent = p;
  r->path_parent = pp;
  if (v->right != nullptr) {
    v->right->parent = v;
  }
}

void RotateRight(Node* v) {
  Node* p = v->parent;
  Node* l = v->left;
  Node* pp = v->path_parent;
  v->path_parent = nullptr;
  if (p != nullptr) {
    if (p->left == v) {
      p->left = l;
    } else {
      p->right = l;
    }
  }
  Node* tmp = l->right;
  l->right = v;
  v->left = tmp;
  v->parent = l;
  l->parent = p;
  l->path_parent = pp;
  if (v->left != nullptr) {
    v->left->parent = v;
  }
}

Node* Splay(Node* v) {
  Node* old_v = v;
  while (p(v) != nullptr) {
    if (v == p(v)->left) {
      if (g(v) == nullptr) {
        RotateRight(p(v));
      } else if (p(v) == g(v)->left) {
        RotateRight(g(v));
        RotateRight(p(v));
      } else {
        RotateRight(p(v));
        RotateLeft(p(v));
      }
    } else {
      if (g(v) == nullptr) {
        RotateLeft(p(v));
      } else if (p(v) == g(v)->right) {
        RotateLeft(g(v));
        RotateLeft(p(v));
      } else {
        RotateLeft(p(v));
        RotateRight(p(v));
      }
    }
  }
  if (v != old_v) {
    std::cerr << "v != old_v in splay\n";
  }
  return v;
}

Node* insert(Node* root, int x, int number) {
  if (root == nullptr) {
    data[sz] = Node(x, number);
    ++sz;
    root = &data[sz - 1];
    return root;
  }
  Node* tmp = root;
  while (true) {
    if (tmp->value == x) {
      return Splay(tmp);
    }
    if (x < tmp->value) {
      if (tmp->left == nullptr) {
//        data.emplace_back(x, number);
        data[sz] = Node(x, number);
        ++sz;
        tmp->left = &data[sz - 1];
        tmp->left->parent = tmp;
        return Splay(tmp->left);
      } else {
        tmp = tmp->left;
        continue;
      }
    } else {
      if (tmp->right == nullptr) {
//        data.emplace_back(x, number);
        data[sz] = Node(x, number);
        ++sz;
        tmp->right = &data[sz - 1];
        tmp->right->parent = tmp;
        return Splay(tmp->right);
      } else {
        tmp = tmp->right;
        continue;
      }
    }
  }
}

bool Find(Node*& root, int x) {
  if (root == nullptr) {
    return false;
  }
  Node* tmp = root;
  while (true) {
    if (tmp->value == x) {
      root = Splay(tmp);
      return true;
    }
    Node* prev = tmp;
    if (x < tmp->value) {
      tmp = tmp->left;
    } else {
      tmp = tmp->right;
    }
    if (tmp == nullptr) {
      root = Splay(prev);
      return false;
    }
  }
}

// В t1 все эл-ты меньше, чем в t2
Node* Merge(Node*& t1, Node*& t2) {
  if (t1 == nullptr) {
    return t2;
  }
  if (t2 == nullptr) {
    return t1;
  }
  Node* tmp = t1;
  while (true) {
    if (tmp->right != nullptr) {
      tmp = tmp->right;
      continue;
    }
    t1 = Splay(tmp);
    break;
  }
  if (t1->right != nullptr) {
    std::cerr << "it is mistake...\n";
  }
  t1->right = t2;
  return t1;
}

void Erase(Node*& root, int x) {
  if (Find(root, x)) {
    Node* l = root->left;
    Node* r = root->right;
    root->left = root->right = nullptr;
    if (l != nullptr) {
      l->parent = nullptr;
    }
    if (r != nullptr) {
      r->parent = nullptr;
    }
    root = Merge(l, r);
  }
}

// В первой строго меньше, во второй больше или равно
std::pair<Node*, Node*> Split(Node*& root, int x) {
  Find(root, x);
  Node* l = root->left;
  if (l != nullptr) {
    l->parent = nullptr;
  }
  root->left = nullptr;
  return std::make_pair(l, root);
}

//class SplayTree {
// public:
//  Node* root = nullptr;
//  static inline std::deque<Node> data;
//
//  explicit SplayTree(Node* ptr = nullptr) : root(ptr) {}
//
//};

void expose(Node* v) {
  v = Splay(v);
  if (v->right != nullptr) {
    v->right->path_parent = v;
    v->right->parent = nullptr;
    v->right = nullptr;
  }
  while (v->path_parent != nullptr) {
    Node* u = v->path_parent;

    // Подготовка: если там был отросток - отрезаем
    u = Splay(u);
    if (u->right != nullptr) {
      u->right->path_parent = u;
      u->right->parent = nullptr;
      u->right = nullptr;
    }

    u->right = v;
    v->parent = u;
    v->path_parent = nullptr;

    v = u;
  }
}

// Отрезать дерево с корнем v
// Считаем, что v - не корень link cut tree...
void cut(Node* v) {
  expose(v);
  v->left->parent = nullptr;
  v->left = nullptr;
}

// Отрезать от v вершину u
void cut(Node* u, Node* v) {
  // Теперь от v точно не идет выделенных ребер
  v = Splay(v);
  if (v->right != nullptr) {
    v->right->path_parent = v;
    v->right->parent = nullptr;
    v->right = nullptr;
  }
  u = Splay(u);
  u->path_parent = nullptr;
}

// Присоединяет дерево с корнем u к вершине v
void link(Node* u, Node* v) {
  u = Splay(u);
  u->path_parent = v;
}

Node* lca(Node* u, Node* v) {
  expose(u);
  expose(v);
  u = Splay(u);
  if (u->path_parent == nullptr) {
    return (u->value < v->value ? u : v);
  }
  return u->path_parent;
}

Node* createVertex(int depth, int numb) {
  Node* v = nullptr;
  v = insert(v, depth, numb);
  return v;
}

// Ключ value для ноды - глубина вершины в link cut tree
int main() {
  Node* lab = createVertex(1, 1);
  vector<Node*> partners{nullptr, lab};
//  vector<std::set<int>> child{{}, {}};
  vector<bool> exist{true, true};

  vector<int> parent{0, 0};
  int q;
  cin >> q;
  for (int r = 0; r < q; ++r) {
    char select;
    cin >> select;
    if (select == '+') {
      int v;
      cin >> v;
      int number = static_cast<int>(partners.size());
      Node* new_partner = createVertex(partners[v]->value + 1, number);
//      child[v].insert(number);
      parent.push_back(v);
      exist.push_back(true);
      link(new_partner, partners[v]);
      partners.push_back(new_partner);
//      child.emplace_back();
    }
    if (select == '-') {
      int v;
      cin >> v;
      exist[v] = false;
//      for (int ch : child[v]) {
//        cut(partners[ch], partners[v]);
//        link(partners[ch], partners[parent[v]]);
//
//        parent[ch] = parent[v];
//        child[parent[v]].insert(ch);
//      }
//      cut(partners[v], partners[parent[v]]);
    }
    if (select == '?') {
      int u, v;
      cin >> u >> v;
      Node* lca_uv = lca(partners[u], partners[v]);
      Node* tmp = lca_uv;
      while (!exist[tmp->number]) {
        tmp = partners[parent[tmp->number]];
      }
      while (!exist[lca_uv->number]) {
        int k = lca_uv->number;
        lca_uv = partners[parent[k]];
        parent[k] = tmp->number;
      }
      cout << lca_uv->number << '\n';
    }
  }
}


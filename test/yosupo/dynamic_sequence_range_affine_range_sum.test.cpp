#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"
#include "../../include/mtl/splay_tree_list.hpp"
#include "../../include/mtl/modular.hpp"
#include "../../include/mtl/monoid.hpp"
#include <bits/stdc++.h>
using namespace std;

using mint = Modular998244353;
struct Sum {
    mint a;
    size_t sz;
    Sum(int a=0) : a(a), sz(1) {}
    Sum(mint a, size_t sz) : a(a), sz(sz) {}
    Sum operator*(const Sum& rhs) const {
        return Sum(a+rhs.a, sz+rhs.sz);
    }
};
struct Affine {
    mint b,c;
    Affine(mint b=1, mint c=0) : b(b), c(c) {}
    Affine& operator*=(const Affine& rhs) {
        b *= rhs.b;
        c = c*rhs.b + rhs.c;
        return *this;
    }
    Sum act(const Sum& a) const {
        return {b * a.a + c * a.sz, a.sz};
    }
};
using DynamicSequenceRangeAffineRangeSum = SplayTreeList<Sum, Affine>;

// using P = std::pair<mint,int>;
// P op(P a, P b) { return {a.first+b.first, a.second+b.second}; }
// P e() { return {0,1}; }
// using A = std::pair<mint,mint>;
// P mapping(A f, P x) { return {f.first * x.first + f.second * x.second, x.second}; }
// A composition(A f, A g) { return {f.first * g.first, f.first * g.second + f.second}; }
// A id() { return {1,0}; }
// struct ST {
//     struct Node;
//     using node_ptr = std::shared_ptr<Node>;
//     struct Node {
//         node_ptr l,r;
//         std::weak_ptr<Node> p;
//         int sum;
//         bool rev;
//         P m, prod;
//         A f;
//         Node(mint x) 
//             : l(nullptr), r(nullptr), p(), sum(1), rev(false), m({x,1}), prod({x,1}), f(id()) {}
//         bool is_root() const {
//             return p.expired() || (p.lock()->l.get() != this && p.lock()->r.get() != this); 
//         }
//     };
//     void rot_l(const node_ptr& u) const {
//         auto p = u->p.lock(), q = p->p.lock();
//         p->r = u->l;
//         if (p->r)
//             p->r->p = p;
//         u->l = p;
//         p->p = u;
//         u->p = q;
//         if (q) {
//             if (q->l == p)
//                 q->l = u;
//             else if (q->r == p)
//                 q->r = u;
//             else 
//                 throw "invalid tree";
//         }
//     }
//     void rot_r(const node_ptr& u) const {
//         auto p = u->p.lock(), q = p->p.lock();
//         p->l = u->r;
//         if (p->l)
//             p->l->p = p;
//         u->r = p;
//         p->p = u;
//         u->p = q;
//         if (q) {
//             if (q->l == p)
//                 q->l = u;
//             else if (q->r == p)
//                 q->r = u;
//             else 
//                 throw "invalid tree";
//         }
//     }
//     void reverse_prod(const node_ptr& u) const {}
//     void propagate(const node_ptr& u) const {
//         if (u->l) {
//             u->l->m = mapping(u->f, u->l->m);
//             u->l->prod = mapping(u->f, u->l->prod);
//             u->l->f = composition(u->f, u->l->f);
//         }
//         if (u->r) {
//             u->r->m = mapping(u->f, u->r->m);
//             u->r->prod = mapping(u->f, u->r->prod);
//             u->r->f = composition(u->f, u->r->f);
//         }
//         if (u->rev) {
//             std::swap(u->l, u->r);
//             if (u->l) {
//                 u->l->rev ^= true;
//                 reverse_prod(u->l);
//             }
//             if (u->r) {
//                 u->r->rev ^= true;
//                 reverse_prod(u->r);
//             }
//             u->rev = false;
//         }
//         u->f = id();
//     }
//     void aggregate(const node_ptr& u) const {
//         u->sum = 1;
//         u->prod = u->m;
//         if (u->l) {
//             u->sum += u->l->sum;
//             u->prod = op(u->l->prod, u->prod);
//         }
//         if (u->r) {
//             u->sum += u->r->sum;
//             u->prod = op(u->prod, u->r->prod);
//         }
//     }
//     void splay(const node_ptr& u) const {
//         if (u->is_root()) {
//             propagate(u);
//             aggregate(u);
//             return;
//         }
//         do {
//             assert(u);
//             auto p = u->p.lock();
//             assert(p);
//             if (p->is_root()) {
//                 propagate(p);
//                 propagate(u);
//                 if (p->l == u)
//                     rot_r(u);
//                 else if (p->r == u)
//                     rot_l(u);
//                 else throw "invalid tree";
//                 aggregate(p);
//                 aggregate(u);
//             } else {
//                 auto q = p->p.lock();
//                 propagate(q);
//                 propagate(p);
//                 propagate(u);
//                 if (q->l == p) {
//                     if (p->l == u) { // zig-zig
//                         rot_r(p);
//                         rot_r(u);
//                         aggregate(q);
//                         aggregate(p);
//                     } else if (p->r == u) { // zig-zag
//                         rot_l(u);
//                         rot_r(u);
//                         aggregate(p);
//                         aggregate(q);
//                     } else throw "invalid tree";
//                 } else if (q->r == p) { 
//                     if (p->r == u) { // zig-zig
//                         rot_l(p);
//                         rot_l(u);
//                         aggregate(q);
//                         aggregate(p);
//                     } else if (p->l == u) { // zig-zag
//                         rot_r(u);
//                         rot_l(u);
//                         aggregate(p);
//                         aggregate(q);
//                     } else throw "invalid tree";
//                 }
//                 aggregate(u);
//             }
//         } while (!u->is_root());
//     }
//     node_ptr root;
//     node_ptr kth_element(size_t i) {
//         assert(i < root->sum);
//         auto u = root;
//         while (true) {
//             assert(u);
//             auto lp = u->l ? u->l->sum : 0;
//             auto rp = u->r ? u->r->sum : 0;
//             // cerr<<u->m.first<<' '<<u->sum<<' '<<lp<<' '<<rp<<endl;
//             assert(u->sum == lp+rp+1);
//             propagate(u);
//             if (!u->l) {
//                 if (i == 0)
//                     break;
//                 i--;
//                 u = u->r;
//             } else if (u->l->sum == i) {
//                 break;
//             } else if (u->l->sum > i) {
//                 u = u->l;
//             } else {
//                 i -= u->l->sum + 1;
//                 u = u->r;
//             }
//         }
//         assert(u);
//         splay(u);
//         root = u;
//         return u;
//     }
//     template<class T>
//     void insert(size_t i, T&& x) {
//         auto u = std::make_shared<Node>(std::forward<T>(x));
//         if (i == 0) {
//             u->r = root;
//             if (root) root->p = u;
//             root = u;
//             aggregate(u);
//             return;
//         }
//         if (i == root->sum) {
//             u->l = root;
//             if (root) root->p = u;
//             root = u;
//             aggregate(u);
//             return;
//         }
//         auto p = kth_element(i);
//         u->l = p->l;
//         u->r = p;
//         if (u->l)
//             u->l->p = u;
//         u->r->p = u;
//         p->l = nullptr;
//         root = u;
//         aggregate(p);
//         aggregate(u);
//     }
//     void erase(size_t i) {
//         assert(i < root->sum);
//         auto p = kth_element(i);
//         if (i == 0) {
//             root = p->r;
//             if (root)
//                 root->p.reset();
//             return;
//         }
//         if (i == root->sum-1) {
//             root = p->l;
//             if (root)
//                 root->p.reset();
//             return;
//         }
//         auto r = p->r;
//         auto l = p->l;
//         root = r;
//         root->p.reset();
//         r = kth_element(0);
//         r->l = l;
//         l->p = r;
//         aggregate(r);
//     }
//     node_ptr between(size_t l, size_t r) {
//         assert(r <= root->sum);
//         if (l == 0) {
//             if (r == root->sum)
//                 return root;
//             else
//                 return kth_element(r)->l;
//         }
//         if (r == root->sum) {
//             return kth_element(l-1)->r;
//         }
//         auto rp = kth_element(r);
//         root = rp->l;
//         root->p.reset();
//         auto lp = kth_element(l-1);
//         rp->l = lp;
//         lp->p = rp;
//         root = rp;
//         aggregate(rp);
//         return lp->r;
//     }
//     void reverse(size_t l, size_t r) {
//         auto u = between(l, r);
//         u->rev ^= true;
//         reverse_prod(u);
//         splay(u);
//         root = u;
//     }
//     template<class... Args>
//     void update(size_t l, size_t r, Args&&... args) {
//         auto u = between(l, r);
//         A f(std::forward<Args>(args)...);
//         u->m = mapping(f, u->m);
//         u->prod = mapping(f, u->prod);
//         u->f = composition(f, u->f);
//         splay(u);
//         root = u;
//     }
//     mint prod(size_t l, size_t r) {
//         return between(l, r)->prod.first;
//     }
//     void print_tree(node_ptr u) {
//         if (!u) return;
//         propagate(u);
//         print_tree(u->l);
//         cerr<<u->m.first<<' '<<u->sum<<endl;
//         print_tree(u->r);
//     }
// };

int main() {
    int n,q; cin>>n>>q;
    vector<int> A(n);
    for (int i = 0; i < n; i++) cin>>A[i];
    DynamicSequenceRangeAffineRangeSum st(A.begin(), A.end());
    for (int i = 0; i < q; i++) {
        int t; cin>>t;
        switch (t) {
            case 0: {
                int i,x; cin>>i>>x;
                st.insert(i, x);
            }; break;
            case 1: {
                int i; cin>>i;
                st.erase(i);
            } break;
            case 2: {
                int l,r; cin>>l>>r;
                st.reverse(l,r);
            } break;
            case 3: {
                int l,r,b,c; cin>>l>>r>>b>>c;
                st.update(l,r,b,c);
            } break;
            case 4: {   
                int l,r; cin>>l>>r;
                cout << st.prod(l,r).a << endl;
            } break;
            default: break;
        }
    }
}
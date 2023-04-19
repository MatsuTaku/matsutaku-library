#pragma once
#include "splay_tree.hpp"
#include "monoid.hpp"

template<class M, class O>
struct SplayTreeListNode : SplayTreeNodeBase<SplayTreeListNode<M,O>> {
    size_t sum;
    M m, prod;
    O f;
    using Base = SplayTreeNodeBase<SplayTreeListNode<M,O>>;
    template<class... Args>
    SplayTreeListNode(Args&&... args) 
        : Base(), sum(1), m(args...), prod(std::forward<Args>(args)...), f() {}
};
template<class M, class O>
#if __cpp_concepts >= 202002L
requires IsMonoid<M> && IsOperatorMonoid<O, M>
#endif
struct SplayTreeList : SplayTreeBase<SplayTreeListNode<M, O>> {
    using node_type = SplayTreeListNode<M, O>;
    using base = SplayTreeBase<node_type>;
    using monoid_type = M;
    using operator_monoid_type = O;
    using node_shared = typename SplayTreeNodeTraits<node_type>::node_shared;
    node_shared root;

    SplayTreeList() : base(), root(nullptr) {}
    template<class InputIt>
    SplayTreeList(InputIt first, InputIt last) : SplayTreeList() {
        if (first == last) return;
        auto it = first;
        root = std::make_shared<node_type>(*it);
        ++it;
        for (; it != last; ++it) {
            auto u = std::make_shared<node_type>(*it);
            u->l = root;
            root->p = u;
            root = u;
            aggregate(u);
        }
    }
    void reverse_prod(const node_shared& u) const override {}
    void propagate(const node_shared& u) const override {
        if (u->l) {
            u->l->m = u->f.act(u->l->m);
            u->l->prod = u->f.act(u->l->prod);
            u->l->f *= u->f;
        }
        if (u->r) {
            u->r->m = u->f.act(u->r->m);
            u->r->prod = u->f.act(u->r->prod);
            u->r->f *= u->f;
        }
        if (u->rev) {
            std::swap(u->l, u->r);
            if (u->l) {
                u->l->rev ^= true;
                reverse_prod(u->l);
            }
            if (u->r) {
                u->r->rev ^= true;
                reverse_prod(u->r);
            }
            u->rev = false;
        }
        u->f = operator_monoid_type();
    }
    void aggregate(const node_shared& u) const override {
        u->sum = 1;
        u->prod = u->m;
        if (u->l) {
            u->sum += u->l->sum;
            u->prod = u->l->prod * u->prod;
        }
        if (u->r) {
            u->sum += u->r->sum;
            u->prod = u->prod * u->r->prod;
        }
    }
    node_shared kth_element(size_t k) {
        assert(k < root->sum);
        auto u = root;
        while (true) {
            assert(u);
            auto lp = u->l ? u->l->sum : 0;
            auto rp = u->r ? u->r->sum : 0;
            // cerr<<u->m.first<<' '<<u->sum<<' '<<lp<<' '<<rp<<endl;
            assert(u->sum == lp+rp+1);
            propagate(u);
            if (!u->l) {
                if (k == 0)
                    break;
                k--;
                u = u->r;
            } else if (u->l->sum == k) {
                break;
            } else if (u->l->sum > k) {
                u = u->l;
            } else {
                k -= u->l->sum + 1;
                u = u->r;
            }
        }
        assert(u);
        base::splay(u);
        root = u;
        return u;
    }
    template<class... Args>
    void insert(size_t i, Args&&... args) {
        auto u = std::make_shared<node_type>(std::forward<Args>(args)...);
        if (i == 0) {
            u->r = root;
            if (root) root->p = u;
            root = u;
            aggregate(u);
            return;
        }
        if (i == root->sum) {
            u->l = root;
            if (root) root->p = u;
            root = u;
            aggregate(u);
            return;
        }
        auto p = kth_element(i);
        u->l = p->l;
        u->r = p;
        if (u->l)
            u->l->p = u;
        u->r->p = u;
        p->l = nullptr;
        root = u;
        aggregate(p);
        aggregate(u);
    }
    void erase(size_t i) {
        assert(i < root->sum);
        auto p = kth_element(i);
        if (i == 0) {
            root = p->r;
            if (root)
                root->p.reset();
            return;
        }
        if (i == root->sum-1) {
            root = p->l;
            if (root)
                root->p.reset();
            return;
        }
        auto r = p->r;
        auto l = p->l;
        root = r;
        root->p.reset();
        r = kth_element(0);
        r->l = l;
        l->p = r;
        aggregate(r);
        // p has no referry so p is deleted outomatically.
    }
    node_shared between(size_t l, size_t r) {
        assert(r <= root->sum);
        if (l == 0) {
            if (r == root->sum)
                return root;
            else
                return kth_element(r)->l;
        }
        if (r == root->sum) {
            return kth_element(l-1)->r;
        }
        auto rp = kth_element(r);
        root = rp->l;
        root->p.reset();
        auto lp = kth_element(l-1);
        rp->l = lp;
        lp->p = rp;
        root = rp;
        aggregate(rp);
        return lp->r;
    }
    monoid_type prod(size_t l, size_t r) {
        return between(l, r)->prod;
    }
    void reverse(size_t l, size_t r) {
        auto u = between(l, r);
        u->rev ^= true;
        reverse_prod(u);
        base::splay(u);
        root = u;
    }
    template<class... Args>
    void update(size_t l, size_t r, Args&&... args) {
        auto u = between(l, r);
        operator_monoid_type f(std::forward<Args>(args)...);
        u->m = f.act(u->m);
        u->prod = f.act(u->prod);
        u->f *= f;
        base::splay(u);
        root = u;
    }
};
#pragma once
#include "splay_tree.hpp"
#include "monoid.hpp"

template<class M, class O>
struct SplayTreeListNode : SplayTreeNodeBase<SplayTreeListNode<M,O>> {
    size_t sum;
    M m, prod, rprod;
    O f;
    using Base = SplayTreeNodeBase<SplayTreeListNode<M,O>>;
    template<class... Args>
    SplayTreeListNode(Args&&... args) 
        : Base(), sum(1), m(std::forward<Args>(args)...), prod(m), rprod(m), f() {}
};
template<class M, class O=VoidOperatorMonoid>
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
    node_shared _dfs_init(InputIt first, InputIt last) {
        if (first == last) return nullptr;
        auto n = std::distance(first, last);
        auto mid = std::next(first, n / 2);
        auto u = std::make_shared<node_type>(*mid);
        u->l = _dfs_init(first, mid);
        if (u->l) u->l->p = u;
        u->r = _dfs_init(std::next(mid), last);
        if (u->r) u->r->p = u;
        this->aggregate(u);
        return u;
    }
    template<class InputIt>
    SplayTreeList(InputIt first, InputIt last) : SplayTreeList() {
        if (first == last) return;
        using iterator_category = typename std::iterator_traits<InputIt>::iterator_category;
        if constexpr (std::is_base_of<iterator_category, std::random_access_iterator_tag>::value) {
            root = _dfs_init(first, last);
        } else {
            auto it = first;
            root = std::make_shared<node_type>(*it);
            ++it;
            for (; it != last; ++it) {
                auto u = std::make_shared<node_type>(*it);
                u->l = root;
                root->p = u;
                root = u;
                this->aggregate(u);
            }
        }
    }
    void reverse_prod(const node_shared& u) const override {
        std::swap(u->prod, u->rprod);
    }
    void propagate(const node_shared& u) const override {
        if (u->l) {
            u->l->m = u->f.act(u->l->m);
            u->l->prod = u->f.act(u->l->prod);
            u->l->rprod = u->f.act(u->l->rprod);
            u->l->f *= u->f;
        }
        if (u->r) {
            u->r->m = u->f.act(u->r->m);
            u->r->prod = u->f.act(u->r->prod);
            u->r->rprod = u->f.act(u->r->rprod);
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
        u->rprod = u->m;
        if (u->l) {
            u->sum += u->l->sum;
            u->prod = u->l->prod * u->prod;
            u->rprod = u->rprod * u->l->rprod;
        }
        if (u->r) {
            u->sum += u->r->sum;
            u->prod = u->prod * u->r->prod;
            u->rprod = u->r->rprod * u->rprod;
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
        if (l == r) return;
        auto u = between(l, r);
        u->rev ^= true;
        reverse_prod(u);
        base::splay(u);
        root = u;
    }
    template<class... Args>
    void set(size_t i, Args&&... args) {
        auto u = kth_element(i);
        u->m = monoid_type(std::forward<Args>(args)...);
        this->aggregate(u);
    }
    void update(size_t i, const operator_monoid_type& f) {
        auto u = kth_element(i);
        u->m = f.act(u->m);
        this->aggregate(u);
    }
    void update(size_t l, size_t r, const operator_monoid_type& f) {
        assert(l < r);
        auto u = between(l, r);
        u->m = f.act(u->m);
        u->prod = f.act(u->prod);
        u->rprod = f.act(u->rprod);
        u->f *= f;
        base::splay(u);
        root = u;
    }
};
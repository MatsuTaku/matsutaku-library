#include "splay_tree.hpp"
#include "monoid.hpp"

template<class NodeType>
struct LinkCutTreeBase : public SplayTreeBase<NodeType> {
    using Base = SplayTreeBase<NodeType>;
    using node_traits = SplayTreeNodeTraits<NodeType>;
    using node_shared = typename node_traits::node_shared;
    void expose(const node_shared& x) const {
        node_shared r = nullptr;
        for (node_shared p = x; p; p = p->p.lock()) {
            Base::splay(p);
            p->r = r;
            r = p;
            this->aggregate(p);
        }
        Base::splay(x);
    }
    void evert(const node_shared& v) const {
        expose(v);
        v->rev ^= true;
        this->reverse_prod(v);
        this->propagate(v);
    }
    void cut(const node_shared& c) const {
        expose(c);
        auto l = c->l;
        c->l = nullptr;
        l->p.reset();
        this->aggregate(c);
    }
    void link(const node_shared& c, const node_shared& p) const {
        evert(c);
        expose(p);
        p->r = c;
        c->p = p;
        this->aggregate(p);
    }
    void print_tree(const node_shared& u) const {
        if (!u) return;
        if (u->l and u->l->p.lock() == u) {
            print_tree(u->l);
        }
        std::cerr<<u->m.x<<' ';
        if (u->r and u->r->p.lock() == u) {
            print_tree(u->r);
        }
    }
};

template<class M, class O>
struct LinkCutTreeNode : SplayTreeNodeBase<LinkCutTreeNode<M, O>> {
    M m, prod, rprod;
    O f;
    using Base = SplayTreeNodeBase<LinkCutTreeNode<M, O>>;
    LinkCutTreeNode() = default;
    template<class... Args>
    LinkCutTreeNode(Args&&... args) 
        : Base(), m(std::forward<Args>(args)...), prod(m), f() {}
};

template<class M, class O=VoidOperatorMonoid>
#if __cpp_concepts >= 202002L
requires IsMonoid<M> && IsOperatorMonoid<O, M>
#endif
struct LinkCutTree : LinkCutTreeBase<LinkCutTreeNode<M, O>> {
    using node_type = LinkCutTreeNode<M, O>;
    using Base = LinkCutTreeBase<LinkCutTreeNode<M, O>>;
    using monoid_type = M;
    using operator_monoid_type = O;
    using node_shared = typename SplayTreeNodeTraits<LinkCutTreeNode<M, O>>::node_shared;
    std::vector<node_shared> nodes;
    LinkCutTree(size_t n) : Base(), nodes(n) {
        for (size_t i = 0; i < n; ++i)
            nodes[i] = std::make_shared<node_type>();
    }
    template<class InputIt>
    LinkCutTree(InputIt first, InputIt last) : Base(), nodes(std::distance(first, last)) {
        size_t i = 0;
        for (auto it = first; it != last; ++it)
            nodes[i++] = std::make_shared<node_type>(*it);
    }
    void reverse_prod(const node_shared& u) const override {
        std::swap(u->prod, u->rprod);
    }
    void propagate(const node_shared& u) const override {
        bool cl = u->l and u->l->p.lock() == u;
        bool cr = u->r and u->r->p.lock() == u;
        if (cl) {
            u->l->m = u->f.act(u->l->m);
            u->l->prod = u->f.act(u->l->prod);
            u->l->rprod = u->f.act(u->l->rprod);
            u->l->f *= u->f;
        }
        if (cr) {
            u->r->m = u->f.act(u->r->m);
            u->r->prod = u->f.act(u->r->prod);
            u->r->rprod = u->f.act(u->r->rprod);
            u->r->f *= u->f;
        }
        if (u->rev) {
            std::swap(u->l, u->r);
            if (cr) {
                u->l->rev ^= true;
                reverse_prod(u->l);
            }
            if (cl) {
                u->r->rev ^= true;
                reverse_prod(u->r);
            }
            u->rev = false;
        }
        u->f = operator_monoid_type();
    }
    void aggregate(const node_shared& u) const override {
        u->prod = u->m;
        u->rprod = u->m;
        if (u->l and u->l->p.lock() == u) {
            u->prod = u->l->prod * u->prod;
            u->rprod = u->rprod * u->l->rprod;
        }
        if (u->r and u->r->p.lock() == u) {
            u->prod = u->prod * u->r->prod;
            u->rprod = u->r->rprod * u->rprod;
        }
    }
    void cut(size_t u, size_t v) const {
        Base::evert(nodes[u]);
        Base::expose(nodes[v]);
        auto l = nodes[v]->l;
        nodes[v]->l = nullptr;
        l->p.reset();
        this->aggregate(nodes[v]);
    }
    void link(size_t u, size_t v) const {
        Base::link(nodes[v], nodes[u]);
    }
    monoid_type prod(size_t u, size_t v) const {
        Base::evert(nodes[u]);
        Base::expose(nodes[v]);
        return nodes[v]->prod;
    }
    template<class... Args>
    void set(size_t i, Args&&... args) const {
        auto u = nodes[i];
        Base::splay(u);
        u->m = monoid_type(std::forward<Args>(args)...);
        this->aggregate(u);
    }
    template<class... Args>
    void update(size_t i, Args&&... args) const {
        operator_monoid_type f(std::forward<Args>(args)...);
        auto u = nodes[i];
        Base::splay(u);
        u->m = f.act(u->m);
        this->aggregate(u);
    }
    template<class... Args>
    void update(size_t u, size_t v, Args&&... args) const {
        operator_monoid_type f(std::forward<Args>(args)...);
        Base::evert(nodes[u]);
        auto nv = nodes[v];
        Base::expose(nv);
        nv->m = f.act(nv->m);
        nv->prod = f.act(nv->prod);
        nv->rprod = f.act(nv->rprod);
        nv->f *= f;
        Base::splay(nv);
    }
};
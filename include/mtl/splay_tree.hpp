#pragma once
#include <memory>
#include <cassert>

template<class NodeType>
struct SplayTreeNodeBase {
    using node_type = NodeType;
    using node_shared = std::shared_ptr<node_type>;
    using node_weak = std::weak_ptr<node_type>;
    node_shared l,r;
    node_weak p;
    bool rev;
    bool is_root() const {
        return p.expired() || (p.lock()->l.get() != this && p.lock()->r.get() != this); 
    }
};
template<class T>
struct SplayTreeNodeTraits {
    using node_type = typename T::node_type;
    using node_shared = typename T::node_shared;
    using node_weak = typename T::node_weak;
};

template<class Node>
struct SplayTreeBase {
    using node_traits = SplayTreeNodeTraits<Node>;
    using node_type = typename node_traits::node_type;
    using node_shared = typename node_traits::node_shared;
    using node_weak = typename node_traits::node_weak;
    SplayTreeBase() = default;

    void rotate_left(const node_shared& u) const {
        auto p = u->p.lock(), q = p->p.lock();
        p->r = u->l;
        if (p->r)
            p->r->p = p;
        u->l = p;
        p->p = u;
        u->p = q;
        if (q) {
            if (q->l == p)
                q->l = u;
            else if (q->r == p)
                q->r = u;
        }
    }
    void rotate_right(const node_shared& u) const {
        auto p = u->p.lock(), q = p->p.lock();
        p->l = u->r;
        if (p->l)
            p->l->p = p;
        u->r = p;
        p->p = u;
        u->p = q;
        if (q) {
            if (q->l == p)
                q->l = u;
            else if (q->r == p)
                q->r = u;
        }
    }
    virtual void reverse_prod(const node_shared& u) const {}
    virtual void propagate(const node_shared& u) const {}
    virtual void aggregate(const node_shared& u) const {}
    void splay(const node_shared& u) const {
        if (u->is_root()) {
            this->propagate(u);
            this->aggregate(u);
            return;
        }
        do {
            assert(u);
            auto p = u->p.lock();
            assert(p);
            if (p->is_root()) {
                this->propagate(p);
                this->propagate(u);
                if (p->l == u)
                    rotate_right(u);
                else if (p->r == u)
                    rotate_left(u);
                else throw "invalid tree";
                this->aggregate(p);
                this->aggregate(u);
            } else {
                auto q = p->p.lock();
                this->propagate(q);
                this->propagate(p);
                this->propagate(u);
                if (q->l == p) {
                    if (p->l == u) { // zig-zig
                        rotate_right(p);
                        rotate_right(u);
                        this->aggregate(q);
                        this->aggregate(p);
                    } else if (p->r == u) { // zig-zag
                        rotate_left(u);
                        rotate_right(u);
                        this->aggregate(p);
                        this->aggregate(q);
                    } else throw "invalid tree";
                } else if (q->r == p) { 
                    if (p->r == u) { // zig-zig
                        rotate_left(p);
                        rotate_left(u);
                        this->aggregate(q);
                        this->aggregate(p);
                    } else if (p->l == u) { // zig-zag
                        rotate_right(u);
                        rotate_left(u);
                        this->aggregate(p);
                        this->aggregate(q);
                    } else throw "invalid tree";
                }
                this->aggregate(u);
            }
        } while (!u->is_root());
    }

};
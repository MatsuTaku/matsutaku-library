#include "splay_tree.hpp"

template<class NodeType>
struct LinkCutTreeBase : public SplayTreeBase<NodeType> {
    using node_traits = SplayTreeNodeTraits<NodeType>;
    using node_shared = typename node_traits::node_shared;
    node_shared expose(node_shared x) const {
        node_shared r = nullptr;
        for (node_shared p = x; p; p = p->p) {
            splay(p);
            p->r = r;
            r = p;
        }
        splay(x);
        return x;
    }
    void cut(node_shared c) {
        expose(c);
        auto l = c->l;
        c->l = nullptr;
        l->p = nullptr;
    }
    void link(node_shared c, node_shared p) {
        expose(c);
        expose(p);
        c->p = p;
        p->r = c;
    }
    void evert(node_shared v) {
        auto r = expose(v);
        r->rev = true;
    }
};
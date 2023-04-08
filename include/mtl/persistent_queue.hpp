#pragma once
#include <memory>
#include <cstdint>
#include <algorithm>
#include <cassert>

/**
 * @brief Persistent Queue
*/
template<class T>
class PersistentQueue {
public:
    using value_type = T;
private:
    struct Node;
    struct Stream;
    using node_ptr = std::shared_ptr<Node>;
    using raw_ptr = Node*;
    using stream_ptr = std::shared_ptr<Stream>;
    template<class... Args>
    static node_ptr make_node(Args&&... args) {
        return std::make_shared<Node>(std::forward<Args>(args)...);
    }
    template<class... Args>
    static stream_ptr make_stream(Args&&... args) {
        return std::make_shared<Stream>(std::forward<Args>(args)...);
    }
    struct Node {
        node_ptr next;
        value_type v;
        template<class N, class... Args>
        Node(N&& n, Args&&... args) 
            : next(std::forward<N>(n)), v(std::forward<Args>(args)...) {}
    };
    struct Stream {
        node_ptr scan, rotate;
        template<class Rptr>
        Stream(const node_ptr& nptr, Rptr&& rptr) 
            : scan(nptr), rotate(std::forward<Rptr>(rptr)) {}
        
        node_ptr next() {
            node_ptr ret;
            if (scan) {
                ret = make_node(nullptr, scan->v);
                scan = scan->next;
            } else {
                while (rotate) {
                    ret = make_node(ret, rotate->v);
                    rotate = rotate->next;
                }
            }
            return ret;
        }
    };

    node_ptr f_;
    raw_ptr proc_;
    stream_ptr stream_;
    size_t size_;
    node_ptr r_;

    PersistentQueue(stream_ptr&& stream, size_t size) : 
        f_(stream->next()), 
        proc_(f_.get()), 
        stream_(std::move(stream)), 
        size_(size),
        r_(nullptr) {}
    template<class Nptr>
    PersistentQueue(const node_ptr& f, const raw_ptr&& proc, const stream_ptr& stream, size_t size, 
                    Nptr&& r) 
        : f_(f), proc_(proc), stream_(stream), size_(size), r_(std::forward<Nptr>(r)) {}

    template<class... Args>
    [[nodiscard]] PersistentQueue _push(Args&&... args) const {
        if (!proc_) {
            return PersistentQueue(
                make_stream(f_, make_node(r_, std::forward<Args>(args)...)), size()+1);
        }
        if (!proc_->next) {
            proc_->next = stream_->next();
        }
        return PersistentQueue(f_, proc_->next.get(), stream_, size()+1,
                               make_node(r_, std::forward<Args>(args)...));
    }
    [[nodiscard]] PersistentQueue _pop() const {
        assert(!empty());
        if (!proc_) {
            return PersistentQueue(make_stream(f_->next, r_), size()-1);
        }
        if (!proc_->next) {
            proc_->next = stream_->next();
        }
        return PersistentQueue(f_->next, proc_->next.get(), stream_, size()-1, r_);
    }

public:
    PersistentQueue() 
        : f_(nullptr), proc_(nullptr), stream_(nullptr), size_(0), r_(nullptr) {}
    size_t size() const { return size_; }
    bool empty() const { return size() == 0; }
    const value_type& front() const {
        assert(f_);
        return f_->v;
    }
    template<class V>
    [[nodiscard]] PersistentQueue push(V&& x) const {
        return _push(std::forward<V>(x));
    }
    [[nodiscard]] PersistentQueue push(const T& x) const {
        return _push(x);
    }
    [[nodiscard]] PersistentQueue push(T&& x) const {
        return _push(std::move(x));
    }
    template<class... Args>
    [[nodiscard]] PersistentQueue emplace(Args&&... args) const {
        return _push(std::forward<Args>(args)...);
    }
    [[nodiscard]] PersistentQueue pop() const {
        return _pop();
    }
};
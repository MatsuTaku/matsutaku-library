#pragma once
#include "persistent_stack.hpp"
#include <algorithm>
#include <cassert>

template<class T>
class MemorablePersistentStack {
private:
    using Base = PersistentStack<T>;
    Base st_;
    std::shared_ptr<Base> reversed_;
public:
    MemorablePersistentStack() = default;
    MemorablePersistentStack(const Base& st) : st_(st), reversed_() {}
    MemorablePersistentStack(Base&& st) : st_(std::move(st)), reversed_() {}

    const T& top() const {
        return st_.top();
    }
    size_t size() const {
        return st_.size();
    }
    bool empty() const {
        return st_.empty();
    }
    template<class V>
    [[nodiscard]] MemorablePersistentStack push(V&& v) const {
        return st_.push(std::forward<V>(v));
    }
    [[nodiscard]] MemorablePersistentStack pop() const {
        return st_.pop();
    }
    [[nodiscard]] MemorablePersistentStack concat(const MemorablePersistentStack& rhs) const {
        return st_.concat(rhs.st_);
    }

    MemorablePersistentStack reverse() {
        if (!reversed_) 
            reversed_ = std::make_shared<Base>(st_.reverse());
        return *reversed_;
    }
};

template<class T>
class PersistentQueue {
private:
    using stack_type = MemorablePersistentStack<T>;
    stack_type f_, r_;
    PersistentQueue(stack_type f, stack_type r)
        : f_(f), r_(r) {}

public:
    PersistentQueue() = default;

    const T& front() const {
        assert(!f_.empty());
        return f_.top();
    }
private:
    template<class U>
    PersistentQueue _push(U&& x) {
        return PersistentQueue(f_, r_.push(std::forward<U>(x))).normalize();
    }
public:
    template<class U>
    [[nodiscard]] PersistentQueue push(U&& x) {
        return _push(std::forward<U>(x));
    }
    [[nodiscard]] PersistentQueue push(const T& x) {
        return _push(x);
    }
    [[nodiscard]] PersistentQueue push(T&& x) {
        return _push(std::move(x));
    }
    [[nodiscard]] PersistentQueue pop() {
        assert(!f_.empty());
        return PersistentQueue(f_.pop(), r_).normalize();
    }

    size_t size() const { return f_.size() + r_.size(); }
    bool empty() const { return size() == 0; }

private:
    [[nodiscard]] PersistentQueue normalize() {
        if (f_.size() >= r_.size()) 
            return *this;
        else 
            return PersistentQueue(f_.concat(r_.reverse()), stack_type());
    }
};
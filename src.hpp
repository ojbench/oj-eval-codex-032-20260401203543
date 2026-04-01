#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class pylist {
    struct Node {
        enum class Kind { INT, LIST };
        Kind kind;
        long long ival{};
        std::shared_ptr<std::vector<pylist>> lst;
        static std::shared_ptr<Node> make_int(long long v) {
            auto n = std::make_shared<Node>();
            n->kind = Kind::INT;
            n->ival = v;
            return n;
        }
        static std::shared_ptr<Node> make_list() {
            auto n = std::make_shared<Node>();
            n->kind = Kind::LIST;
            n->lst = std::make_shared<std::vector<pylist>>();
            return n;
        }
    };

    std::shared_ptr<Node> p;

public:
    pylist() : p(Node::make_list()) {}
    pylist(long long v) : p(Node::make_int(v)) {}
    pylist(const pylist&) = default;
    pylist& operator=(const pylist&) = default;

    pylist& operator=(long long v) {
        p = Node::make_int(v);
        return *this;
    }

    void append(const pylist& x) {
        ensure_list();
        p->lst->push_back(x);
    }
    void append(long long v) { append(pylist(v)); }

    pylist pop() {
        ensure_list();
        auto& v = *p->lst;
        if (v.empty()) throw std::out_of_range("pop from empty list");
        pylist back = v.back();
        v.pop_back();
        return back;
    }

    pylist& operator[](size_t i) {
        ensure_list();
        return p->lst->at(i);
    }
    const pylist& operator[](size_t i) const {
        ensure_list();
        return p->lst->at(i);
    }

    operator long long() const {
        if (!p || p->kind != Node::Kind::INT) {
            throw std::runtime_error("pylist is not an integer");
        }
        return p->ival;
    }

    friend std::ostream& operator<<(std::ostream& os, const pylist& ls) {
        if (ls.p->kind == Node::Kind::INT) {
            os << ls.p->ival;
        } else {
            os << '[';
            const auto& v = *ls.p->lst;
            for (size_t i = 0; i < v.size(); ++i) {
                if (i) os << ", ";
                os << v[i];
            }
            os << ']';
        }
        return os;
    }

private:
    void ensure_list() const {
        if (!p || p->kind != Node::Kind::LIST) {
            throw std::runtime_error("operation requires list");
        }
    }
};

#endif // SRC_HPP

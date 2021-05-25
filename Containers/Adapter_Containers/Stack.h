//
// Created by arthurdovlat on 4/13/21.
//

#ifndef OURSTLV2_STACK_H
#define OURSTLV2_STACK_H

#include <bits/concept_check.h>
#include <debug/debug.h>
# include <bits/uses_allocator.h>
#include <deque>
namespace OurSTL {
    template<typename T, typename sequence = std::deque<T> >
    class stack {

        typedef typename sequence::value_type Sequence_value_type;
        template<typename Alloc>
        using Uses = typename std::enable_if<std::uses_allocator<sequence, Alloc>::value>::type;
        static_assert(std::is_same<T, typename sequence::value_type>::value,"value_type must be the same as the underlying container");
    public:
        typedef typename sequence::value_type value_type;
        typedef typename sequence::reference reference;
        typedef typename sequence::const_reference const_reference;
        typedef typename sequence::size_type size_type;
        typedef sequence container_type;

    protected:
        sequence c;
    public:

        template<typename Seq = sequence, typename Requires = typename std::enable_if<std::is_default_constructible<Seq>::value>::type>
        stack(): c() {}

        explicit stack(const sequence &c) : c(c) {}

        explicit stack(sequence &&c) : c(std::move(c)) {}

        template<typename Alloc, typename Requires = Uses<Alloc>>
        explicit stack(const Alloc &a): c(a) {}

        template<typename Alloc, typename Requires = Uses<Alloc>>
        stack(const sequence &c, const Alloc &a) : c(c, a) {}

        template<typename Alloc, typename Requires = Uses<Alloc>>
        stack(sequence &&c, const Alloc &a): c(std::move(c), a) {}

        template<typename Alloc, typename Requires = Uses<Alloc>>
        stack(const stack &q, const Alloc &a): c(q.c, a) {}

        template<typename Alloc, typename Requires = Uses<Alloc>>
        stack(stack &&q, const Alloc &a) : c(std::move(q.c), a) {}

        bool empty() const { return c.empty(); }

        size_type size() const { return c.size(); }

        reference top() {
            return c.back();
        }

        const_reference top() const {
            return c.back();
        }

        void push(const value_type &x) { c.push_back(x); }


        void push(value_type &&x) { c.push_back(std::move(x)); }

        template<typename... Args>
        decltype(auto) emplace(Args &&... args) { return c.emplace_back(std::forward<Args>(args)...); }

        void pop() {
            c.pop_back();
        }

        void swap(stack &s) noexcept(std::__is_nothrow_swappable<sequence>::value)
        {
            std::swap(c, s.c);
        }

    };


    template<typename Container,typename = RequireAlloc<Container>>
    stack(Container) -> stack<typename Container::value_type, Container>;

    template<typename Container, typename Allocator,
            typename = RequireNotAllocator <Container>,
            typename = RequireAllocator <Allocator>>
    stack(Container, Allocator)-> stack<typename Container::value_type, Container>;


    template<typename T, typename Seq>
    inline bool
    operator==(const stack<T, Seq> &x, const stack<T, Seq> &y) { return x.c == y.c; }


    template<typename T, typename Seq>
    inline bool operator<(const stack<T, Seq> &x, const stack<T, Seq> &y) { return x.c < y.c; }

    template<typename T, typename Seq>
    inline bool operator!=(const stack<T, Seq> &x, const stack<T, Seq> &y) { return !(x == y); }

    template<typename T, typename seq>
    inline bool operator>(const stack<T, seq> &x, const stack<T, seq> &y) { return y < x; }

    template<typename T, typename Seq>
    inline bool operator<=(const stack<T, Seq> &x, const stack<T, Seq> &y) { return !(y < x); }

    template<typename T, typename Seq>
    inline bool operator>=(const stack<T, Seq> &x, const stack<T, Seq> &y) { return !(x < y); }



    template<typename T, typename Seq>
    inline typename std::enable_if<std::__is_swappable<Seq>::value>::type
    swap(stack<T, Seq> &x, stack<T, Seq> &y)
    noexcept(noexcept(x.swap(y))) { x.swap(y); }

    template<typename T, typename Seq, typename Alloc>
    struct uses_allocator<stack<T, Seq>, Alloc>: public std::uses_allocator<Seq, Alloc>::type {
    };


}


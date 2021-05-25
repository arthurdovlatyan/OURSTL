//
// Created by arthurdovlat, sargiskh, hovhannes on 3/18/21.
//

#ifndef OURSTLV2_QUEUE_H
#define OURSTLV2_QUEUE_H

#include <bits/concept_check.h>
#include <debug/debug.h>
# include <bits/uses_allocator.h>
#include <vector>
#include <deque>


namespace OurSTL {
    template<typename T, typename Sequence = std::deque<_Tp> >
    class Queue
    {
        typedef typename Sequence::value_type Sequence_value_type;
        template<typename Alloc>
        using Uses = typename std::enable_if<std::uses_allocator<Sequence, Alloc>::value>::type;
        static_assert(std::is_same<T, typename Sequence::value_type>::value,"value_type must be the same as the underlying container");
    public:
        typedef typename	Sequence::value_type		value_type;
        typedef typename	Sequence::reference		reference;
        typedef typename	Sequence::const_reference	const_reference;
        typedef typename	Sequence::size_type		size_type;
        typedef		Sequence			container_type;

    protected:
        Sequence c;
    public:
        explicit Queue(const Sequence& c = Sequence()) : c(c) { }

        template<typename Seq = Sequence, typename Req = typenamestd::enable_if<is_default_constructible<Seq>::value>::type>
        Queue(): c() { }

        explicit Queue(const Sequence& c): c(c) { }

        explicit Queue(Sequence&& c): c(std::move(c)) { }

        template<typename Alloc, typename Rquires = Uses<Alloc>>
        explicit Queue(const Alloc& a): c(a) { }

        template<typename Alloc, typename Req = Uses<Alloc>>
        Queue(const Sequence& c, const Alloc& a): c(c, a) { }

        template<typename Alloc, typename Req = Uses<Alloc>>
        Queue(Sequence&& c, const Alloc& a): c(std::move(c), a) { }

        template<typename Alloc, typename Req = Uses<Alloc>>
        Queue(const Queue& q, const Alloc& a): c(q.c, a) { }

        template<typename Alloc, typename Req = Uses<Alloc>>
        Queue(Queue&& q, const Alloc& a): c(std::move(q.c), a) { }

        bool empty() const
        { return c.empty(); }


        size_type size() const
        { return c.size(); }


        reference front()
        {
            __glibcxx_requires_nonempty();
            return c.front();
        }

        const_reference
        front() const
        {
            __glibcxx_requires_nonempty();
            return c.front();
        }

        reference back()
        {
            __glibcxx_requires_nonempty();
            return c.back();
        }


        const_reference
        back() const
        {
            __glibcxx_requires_nonempty();
            return c.back();
        }


        void push(const value_type& x)
        { c.push_back(x); }

        void push(value_type&& x)
        { c.push_back(std::move(x)); }


        template<typename... Args>
        decltype(auto) emplace(Args&&... args){
            return c.emplace_back(std::forward<Args>(args)...);
        }

        void pop()
        {
            __glibcxx_requires_nonempty();
            c.pop_front();
        }


        void swap(Queue& q)noexcept(__is_nothrow_swappable<Sequence>::value)
        {
            using std::swap;
            swap(c, q.c);
        }

    };

    template<typename T, typename Seq>
    inline bool operator==(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return x.c == y.c; }


    template<typename T, typename Seq>
    inline bool operator<(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return x.c < y.c; }


    template<typename T, typename Seq>
    inline bool operator!=(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return !(x == y); }


    template<typename T, typename Seq>
    inline bool operator>(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return y < x; }


    template<typename T, typename Seq>
    inline bool operator<=(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return !(y < x); }


    template<typename T, typename Seq>
    inline bool operator>=(const Queue<T, Seq>& x, const Queue<T, Seq>& y)
    { return !(x < y); }


    template<typename T, typename Seq>
    inline typename enable_if<__is_swappable<Seq>::value>::type
    swap(Queue<T, Seq>& x, Queue<T, Seq>& y)noexcept(noexcept(x.swap(y)))
    { x.swap(y); }

    template<typename T, typename Seq, typename Alloc>
    struct uses_allocator<Queue<T, Seq>, Alloc>: public std::uses_allocator<Seq, Alloc>::type { };



    template<typename T, typename Sequence = std::vector<T>,typename Compare  = std::less<typename Sequence::value_type> >
    class Qriority_Queue
    {
      typedef typename Sequence::value_type Sequence_value_type;

        template<typename Alloc>
        using Uses = typename std::enable_if<std::uses_allocator<Sequence, Alloc>::value>::type;
        static_assert(std::is_same<T, typename Sequence::value_type>::value,"value_type must be the same as the underlying container");

    public:
        typedef typename	Sequence::value_type		value_type;
        typedef typename	Sequence::reference		reference;
        typedef typename	Sequence::const_reference	const_reference;
        typedef typename	Sequence::size_type		size_type;
        typedef		Sequence			container_type;
        typedef	       Compare				value_compare;

    protected:
        Sequence  c;
        Compare   comp;
    public:
        template<typename Seq = Sequence, typename Requires = typename
        std::enable_if<std::__and_<std::is_default_constructible<Compare>,std::is_default_constructible<Seq>>::value>::type>
        Qriority_Queue(): c(), comp() { }

        explicit Qriority_Queue(const Compare& x, const Sequence& s): c(s), comp(x)
        { std::make_heap(c.begin(), c.end(), comp); }

        explicit Qriority_Queue(const Compare& x, Sequence&& s = Sequence()): c(std::move(s)), comp(x)
        { std::make_heap(c.begin(), c.end(), comp); }

        template<typename Alloc, typename Requires = Uses<Alloc>>
        explicit Qriority_Queue(const Alloc& a): c(a), comp() { }

        template<typename Alloc, typename Requires = Uses<Alloc>>
        Qriority_Queue(const Compare& x, const Alloc& a): c(a), comp(x) { }


        template<typename Alloc, typename Requires = Uses<Alloc>>
        Qriority_Queue(const Compare& x, const Sequence& c, const Alloc& a): c(c, a), comp(x)
        { std::make_heap(c.begin(), c.end(), comp); }

        template<typename Alloc, typename Requires = Uses<Alloc>>
        Qriority_Queue(const Compare& x, Sequence&& c, const Alloc& a): c(std::move(c), a), comp(x)
        { std::make_heap(c.begin(), c.end(), comp); }

        template<typename Alloc, typename Requires = Uses<Alloc>>
        Qriority_Queue(const Qriority_Queue& q, const Alloc& a): c(q.c, a), comp(q.comp) { }

        template<typename Alloc, typename Requires = Uses<Alloc>>
        Qriority_Queue(Qriority_Queue&& q, const Alloc& a): c(std::move(q.c), a), comp(std::move(q.comp)) { }


        template<typename InputIt>
        Qriority_Queue(InputIt first, InputIt last, const Compare& x, const Sequence& s): c(s), comp(x)
        {
            __glibcxx_requires_valid_range(first, last);
            c.insert(c.end(), first, last);
            std::make_heap(c.begin(), c.end(), comp);
        }

        template<typename InputIt>
        Qriority_Queue(InputIt first, InputIt last, const Compare& x = Compare(), Sequence&& s = Sequence()): c(std::move(s)), comp(x)
        {
            __glibcxx_requires_valid_range(first, last);
            c.insert(c.end(), first, last);
            std::make_heap(c.begin(), c.end(), comp);
        }

        bool empty() const
        { return c.empty(); }

        size_type size() const
        { return c.size(); }

        const_reference top() const
        {
            __glibcxx_requires_nonempty();
            return c.front();
        }

        void push(const value_type& x)
        {
            c.push_back(x);
            std::push_heap(c.begin(), c.end(), comp);
        }

        void push(value_type&& x)
        {
            c.push_back(std::move(x));
            std::push_heap(c.begin(), c.end(), comp);
        }

        template<typename... Args>
        void emplace(Args&&... args)
        {
            c.emplace_back(std::forward<Args>(args)...);
            std::push_heap(c.begin(), c.end(), comp);
        }

        void pop()
        {
            __glibcxx_requires_nonempty();
            std::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }


        void swap(Qriority_Queue& pq)noexcept(std::__and_<std::__is_nothrow_swappable<Sequence>,std::__is_nothrow_swappable<Compare>>::value)
        {
            using std::swap;
            swap(c, pq.c);
            swap(comp, pq.comp);
        }

    };

    // No equality/comparison operators are provided for Qriority_Queue.
    template<typename T, typename Sequence, typename Compare>
    inline typename std::enable_if<std::__and_<std::__is_swappable<Sequence>,
    std::__is_swappable<Compare>>::value>::type
    swap(Priority_Queue<T, Sequence, Compare>& x,Priority_Queue<T, Sequence, Compare>& y)noexcept(noexcept(x.swap(y)))
    { x.swap(y); }

    template<typename T, typename Sequence, typename Compare,typename Alloc>
    struct uses_allocator<Priority_Queue<T, Sequence, Compare>, Alloc>: public std::uses_allocator<Sequence, Alloc>::type { };
#endif // __cplusplus >= 201103L
}


#endif //OURSTLV2_QUEUE_H

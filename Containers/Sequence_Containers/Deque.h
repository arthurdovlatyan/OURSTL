//
// Created by arthurdovlat, sargiskh, hovhannes on 3/18/21.
//

#pragma once


#include <bits/concept_check.h>
#include <bits/stl_iterator_base_types.h>
#include <bits/stl_iterator_base_funcs.h>
#include <debug/assertions.h>
#include <initializer_list>
#include <bits/stl_uninitialized.h>



namespace OurSTL {
    const int Deque_Buffer_Size = 512;

    size_t deque_buff_size(size_t s) {
        return (s < Deque_Buffer_Size ? size_t(Deque_Buffer_Size/ s) : size_t(1));
    }

    template<typename T,typename Ref,typename Ptr>
    class Deque_iterator {
    private:
        template<typename Up>
        using ptr_to = typename std::pointer_traits<Ptr>::template rebind<Up>;
        template<typename CvTp>
        using iter = Deque_iterator<T, CvTp&, ptr_to<CvTp>>;
    public:
        typedef iter<T> iterator;
        typedef iter<const T> const_iterator;
        typedef ptr_to<T> Elem_pointer;
        typedef ptr_to<Elem_pointer> Map_pointer;

        static size_t s_buffer_size() noexcept {
            return deque_buff_size(sizeof(T));
        }

        typedef std::random_access_iterator_tag iterator_category;


        Elem_pointer m_cur;
        Elem_pointer m_first;
        Elem_pointer m_last;
        Map_pointer m_node;

        Deque_iterator(Elem_pointer x, Map_pointer y) noexcept :
        m_cur(x) , m_first(*y), m_last(*y + s_buffer_size()), m_node(y) {}

        Deque_iterator() noexcept : m_cur(), m_first(), m_last(), m_node() {}

        template<typename Iter, typename  = std::_Require<std::is_same<Deque_iterator,const_iterator>,std::is_same<Iter,iterator>>>
        Deque_iterator(const Iter& x) noexcept : m_cur(x.m_cur) , m_first(x.m_first), m_last(x.m_last), m_node(x.m_node) {}

        Deque_iterator(const Deque_iterator& x) noexcept : m_cur(x.m_cur), m_first(x.m_first), m_last(x.m_last), m_node(x.m_node) {}

        Deque_iterator& operator=(const Deque_iterator& ) = default;

        iterator m_const_cast() const noexcept {
            return iterator(m_cur,m_node);
        }

        Ref operator*() const noexcept {
            return *m_cur;
        }

        Ptr operator->() const noexcept {
            return m_cur;
        }

        void m_set_node(Map_pointer new_node) noexcept {
            m_node = new_node;
            m_first = *new_node;
            m_last = m_first + std::ptrdiff_t(s_buffer_size());

        }

        Deque_iterator& operator++() noexcept {
            ++m_cur;
            if (m_cur == m_last) {
                m_set_node(m_node + 1);
                m_cur = m_first;
            }
            return *this;
        }

        Deque_iterator operator++(int) noexcept {
            Deque_iterator tmp = *this;
            **this;
            return tmp;
        }

        Deque_iterator& operator--() noexcept {
            if (m_cur == m_first)
            {
                m_set_node(m_node - 1);
                m_cur = m_last;
            }
            --m_cur;
            return *this;
        }

        Deque_iterator operator--(int) noexcept {
            Deque_iterator tmp = *this;
            --*this;
            return tmp;
        }

        Deque_iterator& operator+=(std::ptrdiff_t n) noexcept {
            const std::ptrdiff_t offset = n + (m_cur - m_first);
            if (offset >= 0 && offset < std::ptrdiff_t(s_buffer_size()))
                m_cur += n;
            else {
                const std::ptrdiff_t node_offset =
                        offset > 0 ? offset / std::ptrdiff_t(s_buffer_size())
                                   : -std::ptrdiff_t((-offset - 1)
                                                     / s_buffer_size()) - 1;
                m_set_node(m_node + node_offset);
                m_cur = m_first + (offset - node_offset
                                            * std::ptrdiff_t(s_buffer_size()));
            }
            return *this;
        }


        Deque_iterator operator+(std::ptrdiff_t n) const noexcept {
            Deque_iterator tmp = *this;
            return tmp += n;
        }

        Deque_iterator& operator-=(std::ptrdiff_t n) noexcept {
            return *this += -n;
        }

        Deque_iterator operator-(std::ptrdiff_t n) const noexcept {
            Deque_iterator tmp = *this;
            return tmp -= n;
        }

        Ref operator[](std::ptrdiff_t n) const noexcept {
            return *(*this + n);
        }

        friend bool operator==(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return x.m_cur == y.m_cur;
        }

        template<typename Refr,typename PtrR>
        friend bool operator==(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return x.m_cur == y.m_cur;
        }
        friend bool operator!=(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return !(x.m_cur == y.m_cur);
        }

        template<typename Refr,typename PtrR>
        friend bool operator!=(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return !(x.m_cur == y.m_cur);
        }

        friend bool operator<(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return (x.m_node == y.m_node) ? (x.m_cur < y.m_cur)
                                                : (x.m_node < y.m_node);
        }

        template<typename Refr,typename PtrR>
        friend bool operator<(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return (x.m_node == y.m_node) ? (x.m_cur < y.m_cur)
                                          : (x.m_node < y.m_node);
        }

        friend bool operator>(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return y < x;
        }

        template<typename Refr,typename PtrR>
        friend bool operator>(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return y < x;
        }

        friend bool operator<=(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return !(y < x);
        }

        template<typename Refr,typename PtrR>
        friend bool operator<=(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return !(y < x);
        }

        friend bool operator>=(const Deque_iterator& x, const Deque_iterator& y) noexcept {
            return !(y > x);
        }

        template<typename Refr,typename PtrR>
        friend bool operator>=(const Deque_iterator& x, const Deque_iterator<T,Refr,PtrR>& y) noexcept {
            return !(y > x);
        }






    };

    template<typename T>
    inline void fill(const Deque_iterator<T, T&, T*>& first,
              const Deque_iterator<T, T&, T*>& last, const T& value) {
        typedef typename Deque_iterator<T, T&, T*>::Deque_iterator Self;

        for (typename Self::_Map_pointer node = first.m_node + 1;
             node < last.m_node; ++node)
            std::fill(*node, *node + Self::s_buffer_size(), value);

        if (first.m_node != last.m_node)
        {
            std::fill(first.m_cur, first.m_last, value);
            std::fill(last.m_first, last.m_cur, value);
        }
        else
            std::fill(first.m_cur, last.m_cur, value);
    }

    template<typename T>
    Deque_iterator<T,T&,T*> copy(Deque_iterator<T, const T&, const T*> first,
                        Deque_iterator<T, const T&, const T*> last,
                        Deque_iterator<T, T&, T*> result)
    {
        typedef typename Deque_iterator<T, T&, T*>::Deque_iterator Self;


        std::ptrdiff_t len = last - first;
        while (len > 0)
        {
            const std::ptrdiff_t clen
                    = std::min(len, std::min(first.m_last - first.m_cur,
                                             result.m_last - result.m_cur));
            copy(first.m_cur, first.m_cur + clen, result.m_cur);
            first += clen;
            result += clen;
            len -= clen;
        }
        return result;
    }

    template<typename T>
    Deque_iterator<T,T&,T*> copy(Deque_iterator<T,T&,T*> first, Deque_iterator<T,T&,T*> last,Deque_iterator<T,T&,T*> result){
        return copy(Deque_iterator<T, const T&, const T*>(first),
                    Deque_iterator<T, const T&, const T*>(last),result);
    }


    template<typename T>
    Deque_iterator<T,T&,T*> copy_backwards(Deque_iterator<T, const T&, const T*>, Deque_iterator<T, const T&, const T*>,
                                  Deque_iterator<T, const T&, const T*>) {

    }

    template<typename T>
    Deque_iterator<T,T&,T*> copy_backwards(Deque_iterator<T, T&, T*> first, Deque_iterator<T, T&, T*> last, Deque_iterator<T, T&, T*> result) {
        return copy_backwards(Deque_iterator<T, const T&, const T*>(first),Deque_iterator<T, const T&, const T*>(last),Deque_iterator<T, const T&, const T*>(result));
    }

    template<typename T>
    Deque_iterator<T,T&,T*> move(Deque_iterator<T, const T&, const T*> first, Deque_iterator<T, const T&, const T*> last,
                        Deque_iterator<T, const T&, const T*> result) {
        typedef typename Deque_iterator<T, T&, T*>::Deque_iterator Self;
        typedef typename std::ptrdiff_t difference_type;

        difference_type len = last - first;
        while (len > 0)
        {
            const difference_type clen
                    = std::min(len, std::min(first.m_last - first.m_cur,
                                             result.m_last - result.m_cur));
            move(first.m_cur, first.m_cur + clen, result.m_cur);
            first += clen;
            result += clen;
            len -= clen;
        }
        return result;
    }

    template<typename T>
    Deque_iterator<T,T&,T*> move(Deque_iterator<T, T&, T*> first, Deque_iterator<T, T&, T*> last, Deque_iterator<T, T&, T*> result) {
        return move(Deque_iterator<T, const T&, const T*>(first),Deque_iterator<T, const T&, const T*>(last),Deque_iterator<T, const T&, const T*>(result));
    }


    template<typename T>
    Deque_iterator<T,T&,T*> move_backwards(Deque_iterator<T, const T&, const T*> first, Deque_iterator<T, const T&, const T*> last,
                                  Deque_iterator<T, const T&, const T*> result) {
        typedef typename Deque_iterator<T, T&, T*>::Deque_iterator Self;
        typedef typename std::ptrdiff_t difference_type;

        difference_type len = last - first;
        while (len > 0)
        {
            difference_type llen = last.m_cur - last.m_first;
            T* lend = last.m_cur;

            difference_type rlen = result.m_cur - result.m_first;
            T* rend = result.m_cur;

            if (!llen)
            {
                llen = Self::s_buffer_size();
                lend = *(last.m_node - 1) + llen;
            }
            if (!rlen)
            {
                rlen = Self::s_buffer_size();
                rend = *(result.m_node - 1) + rlen;
            }

            const difference_type clen = std::min(len,
                                                  std::min(llen, rlen));
            move_backward(lend - clen, lend, rend);
            last -= clen;
            result -= clen;
            len -= clen;
        }
        return result;
    }

    template<typename T>
    Deque_iterator<T,T&,T*> move_backwards(Deque_iterator<T, T&, T*> first, Deque_iterator<T, T&, T*> last, Deque_iterator<T, T&, T*> result) {
        return move_backwards(Deque_iterator<T, const T&, const T*>(first),Deque_iterator<T, const T&, const T*>(last),Deque_iterator<T, const T&, const T*>(result));
    }
    // TODO implement operator - and +

    template<typename T, typename Alloc>
    class Deque_base {
    protected:
        typedef typename __gnu_cxx::__alloc_traits<Alloc>::template rebind<T>::other T_alloc_type;
        typedef __gnu_cxx::__alloc_traits<T_alloc_type> Alloc_traits;
        typedef typename Alloc_traits::pointer Ptr;
        typedef typename Alloc_traits::const_pointer Ptr_const;
        typedef typename Alloc_traits::template rebind<Ptr>::other Map_alloc_type;
        typedef typename __gnu_cxx::__alloc_traits<Map_alloc_type> Map_alloc_traits;



    public:
        typedef Deque_iterator<T, T&, Ptr>	  iterator;
        typedef Deque_iterator<T, const T&, Ptr_const>   const_iterator;
    protected:
        typedef typename iterator::Map_pointer Map_pointer;

        class Deque_impl : public T_alloc_type
        {
        public:
            Map_pointer m_map;
            size_t m_map_size;
            iterator m_start;
            iterator m_finish;

            Deque_impl() : T_alloc_type(), m_map(), m_map_size(),m_start(),m_finish() {

            }

            Deque_impl(const T_alloc_type& a) noexcept : T_alloc_type(a) , m_map(), m_map_size(0), m_start(),m_finish() {

            }

            Deque_impl(Deque_impl&& ) = default;

            Deque_impl(T_alloc_type&& a) noexcept : T_alloc_type((std::move(a))), m_map(), m_map_size(0), m_start(), m_finish() {

            }

            void m_swap_data(Deque_impl& a) noexcept {
                std::swap(this->m_start, a.m_start);
                std::swap(this->m_finish, a.m_finish);
                std::swap(this->m_map, a.m_map);
                std::swap(this->m_map_size, a.m_map_size);
            }
        };

        Deque_impl m_impl;

        T_alloc_type& m_get_T_allocator() noexcept {
            return *static_cast<T_alloc_type*>(&this->m_impl);
        }

        const T_alloc_type& m_get_T_allocator() const noexcept {
            return *static_cast<T_alloc_type*>(&this->m_impl);
        }

        Map_alloc_type m_get_map_allocator() const noexcept {
            return Map_alloc_type(m_get_T_allocator());
        }

        Ptr m_allocate_node() {
            typedef __gnu_cxx::__alloc_traits<T_alloc_type> Traits;
            return Traits::allocate(m_impl, deque_buff_size(sizeof(T)));
        }

        void m_deallocate_node(Ptr p) noexcept {
            typedef __gnu_cxx::__alloc_traits<T_alloc_type> Traits;
            Traits::deallocate(m_impl,p, deque_buff_size(sizeof(T)));
        }

        Map_pointer m_allocate_map(size_t n) {
            Map_alloc_type map_alloc = m_get_map_allocator();
            return Map_alloc_traits::allocate(map_alloc,n);
        }

        void m_deallocate_map(Map_pointer p, size_t n) noexcept {
            Map_alloc_type  map_alloc = m_get_map_allocator();
            Map_alloc_traits::deallocate(map_alloc,p,n);
        }

        void m_initialize_map(size_t n) {
            const size_t num_nodes = (n / deque_buff_size(sizeof(T)) + 1);

            this->m_impl.m_map_size = std::max((size_t) map_initial_size, size_t(num_nodes + 2));
            this->m_impl.m_map = m_allocate_map(this->m_impl.m_map_size);

            Map_pointer  nstart = (this->m_impl.m_map + (this->m_impl.m_map_size - num_nodes) / 2);
            Map_pointer  nfinish = nstart + num_nodes;

            try {
                m_create_nodes(nstart,nfinish);
            } catch (...) {
                m_deallocate_map(this->m_impl.m_map, this->m_impl.m_map_size);
                this->m_impl.m_map = Map_pointer();
                this->m_impl.m_map_size = 0;
                __throw_exception_again;
            }

            this->m_impl.m_start.m_set_node(nstart);
            this->m_impl.m_finish.m_set_node(nfinish - 1);
            this->m_impl.m_start.m_cur = m_impl.m_start.m_first;
            this->m_impl.m_finish.m_cur = (this->m_impl.m_finish.m_first
                                              + n % deque_buff_size(sizeof(T)));
        }

        void m_create_nodes(Map_pointer nstart,Map_pointer nfinish) {
            Map_pointer cur;
            try {
                for (cur = nstart; cur < nfinish; ++cur) {
                    *cur = this->m_allocate_node();
                }
            } catch (...) {
                m_destroy_nodes(nstart,cur);
                __throw_exception_again;
            }
        }

        void m_destroy_nodes(Map_pointer nstart, Map_pointer nfinish) noexcept {
            for (Map_pointer n = nstart; n < nfinish ; ++n) {
                m_deallocate_node(*n);
            }
        }

        const int map_initial_size = 8;
    private:
        Deque_impl m_move_impl() {
            if (!m_impl.m_map)
                return std::move(m_impl);
            T_alloc_type __alloc{m_get_T_allocator()};
            T_alloc_type __sink __attribute((__unused__)) {std::move(__alloc)};
            Deque_base __empty{__alloc};
            __empty.m_initialize_map(0);
            Deque_impl ret{std::move(m_get_T_allocator())};
            m_impl.m_swap_data(ret);
            m_impl.m_swap_data(__empty.m_impl);
            return ret;
        }
    public:

        Alloc get_allocator() const noexcept {
            return Alloc(m_get_T_allocator());
        }

        Deque_base() : m_impl() {
            m_initialize_map(0);
        }

        Deque_base(size_t num) : m_impl() {
            m_initialize_map(num);
        }

        Deque_base(const Alloc& a, size_t num) : m_impl(a) {
            m_initialize_map(num);
        }

        Deque_base(const Alloc& a) : m_impl(a){}

        Deque_base(Deque_base&& x, std::false_type) : m_impl(x.m_move_impl()) {

        }

        Deque_base(Deque_base&& x,std::true_type) : m_impl(std::move(x.m_get_T_allocator())) {
            m_initialize_map(0);
            if (x.m_impl.m_map) {
                this->m_impl.m_swap_data(x.m_impl);
            }
        }

        Deque_base(Deque_base&& x) : Deque_base(std::move(x), typename Alloc_traits::is_always_equal{}) {

        }

        Deque_base(Deque_base&& x,const Alloc& a, size_t n) : m_impl(a) {
            if (x.get_allocator() == a) {
                if (x.m_impl.m_map) {
                    m_initialize_map(0);
                    this->m_impl.m_swap_data(x.m_impl);
                }
            } else {
                m_initialize_map(n);
            }
        }

        ~Deque_base() noexcept {
            if (this->m_impl.m_map) {
                m_destroy_nodes(this->m_impl.m_start.m_node, this->m_impl.m_finish.m_node + 1);
                m_deallocate_map(this->m_impl.m_map, this->m_impl.m_map_size);
            }
        }

    };

    template<typename T, typename Alloc = std::allocator<T>>
    class Deque : protected Deque_base<T,Alloc> {
        static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,
                      "std::Deque must have a non-const, non-volatile value_type");

        typedef Deque_base<T, Alloc> Base;
        typedef typename Base::T_alloc_type T_alloc_type;
        typedef typename Base::Alloc_traits Alloc_traits;
        typedef typename Base::Map_pointer Map_pointer;
    public:
        typedef typename Alloc_traits::pointer pointer;
        typedef typename Alloc_traits::const_pointer const_pointer;
        typedef typename Alloc_traits::reference reference;
        typedef typename Alloc_traits::const_reference const_reference;
        typedef typename Base::iterator iterator;
        typedef typename Base::const_iterator const_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;

        size_t size() const noexcept {
            return this->m_impl.m_finish - this->m_impl.m_start;
        }

        bool empty() const noexcept { return this->m_impl.m_finish == this->m_impl.m_start; }


    protected:
        static size_t s_buffer_size() noexcept {
            return deque_buff_size(sizeof(T));
        }

        // Functions controlling memory layout, and nothing else.
        using Base::m_initialize_map;
        using Base::m_create_nodes;
        using Base::m_destroy_nodes;
        using Base::m_allocate_node;
        using Base::m_deallocate_node;
        using Base::m_allocate_map;
        using Base::m_deallocate_map;
        using Base::m_get_Tp_allocator;

        using Base::m_impl;

        void m_range_check(size_t n) const {
            if (n >= this->size())
                __throw_out_of_range_fmt(__N("Deque::_M_range_check: __n "
                                             "(which is %zu)>= this->size() "
                                             "(which is %zu)"),
                                         n, this->size());
        }

    public:
        Alloc get_allocator() const noexcept {
            Base::get_allocator();
        }

        iterator begin() noexcept {
            return this->m_impl.m_start;
        }

        const_iterator begin() const noexcept {
            return this->m_impl.m_start;
        }

        iterator end() noexcept {
            return this->m_impl.m_finish;
        }

        const_iterator end() const noexcept {
            return this->m_impl.m_finish;
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(this->m_impl.m_finish);
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(this->m_impl.m_finish);
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(this->m_impl.m_start);
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(this->m_impl.m_start);
        }

        const_iterator cbegin() const noexcept {
            return this->m_impl.m_start;
        }

        const_iterator cend() const noexcept {
            return this->m_impl.m_finish;
        }

        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(this->m_impl.m_finish);
        }

        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(this->m_impl.m_start);
        }

    protected:

        static size_t s_max_size(const T_alloc_type &__a) _GLIBCXX_NOEXCEPT {
            const size_t __diffmax = __gnu_cxx::__numeric_traits<ptrdiff_t>::__max;
            const size_t __allocmax = Alloc_traits::max_size(__a);
            return (std::min)(__diffmax, __allocmax);
        }
//TODO


        template<typename InputIt>
        void m_range_initialize(InputIt first, InputIt last, std::input_iterator_tag) {
            this->m_initialize_map(0);
            __try {
                for (; first != last; ++first)
                    emplace_back(*first);
            }
            __catch(...) {
                clear();
                __throw_exception_again;
            }
        }

        template<typename ForwardIt>
        void m_range_initialize(ForwardIt first, ForwardIt last, std::forward_iterator_tag) {
            this->m_initialize_map(0);
            __try {
                for (; first != last; ++first)
                    emplace_back(*first);
            }
            __catch(...) {
                clear();
                __throw_exception_again;
            }
        }

        void m_fill_initialize(const T &value) {
            Map_pointer __cur;
            __try {
                for (__cur = this->m_impl.m_start.m_node;
                     __cur < this->m_impl.m_finish.m_node;
                     ++__cur)
                    std::__uninitialized_fill_a(*__cur, *__cur + s_buffer_size(),
                                                value, m_get_Tp_allocator());
                std::__uninitialized_fill_a(this->m_impl.m_finish.m_first,
                                            this->m_impl.m_finish.m_cur,
                                            value, m_get_Tp_allocator());
            }
            __catch(...) {
                std::_Destroy(this->m_impl.m_start, iterator(*__cur, __cur),
                              m_get_Tp_allocator());
                __throw_exception_again;
            }
        }

        template<typename InputIt>
        void m_initialize_dispatch(InputIt first, InputIt last, std::false_type) {
            m_range_initialize(first, last, std::__iterator_category(first));
        }

        template<typename Integer>
        void m_initialize_dispatch(Integer n, Integer x, std::true_type) {
            m_initialize_map(s_check_init_len(static_cast<size_t>(n),
                                              m_get_Tp_allocator()));
            m_fill_initialize(x);
        }


        static size_t s_check_init_len(size_t n, const Alloc &a) {
            if (n > s_max_size(a))
                std::__throw_length_error(__N("cannot create std::Deque larger than max_size()"));
            return n;
        }


        void m_default_initialize() {
            Map_pointer cur;
            __try {
                for (cur = this->m_impl.m_start.m_node; cur < this->m_impl.m_finish.m_node; ++cur)
                    std::__uninitialized_default_a(*cur, *cur + s_buffer_size(),
                                                   m_get_Tp_allocator());
                std::__uninitialized_default_a(this->m_impl.m_finish.m_first,
                                               this->m_impl.m_finish.m_cur,
                                               m_get_Tp_allocator());
            }
            __catch(...) {
                std::_Destroy(this->m_impl.m_start, iterator(*cur, cur),
                              m_get_Tp_allocator());
                __throw_exception_again;
            }
        }

        template<typename Integer>
        void m_assign_dispatch(Integer n, Integer val, std::true_type) {
            m_fill_initialize(n, val);
        }

        template<class InputIt>
        void m_assign_aux(InputIt first, InputIt last, std::input_iterator_tag) {
            iterator cur = begin();
            for (; first != last && cur != end(); ++cur, (void) ++first)
                *cur = *first;
            if (first == last)
                m_erase_at_end(cur);
            else
                m_range_insert_aux(end(), first, last, std::__iterator_category(first));
        }

        template<class ForwardIt>
        void m_assign_aux(ForwardIt first, ForwardIt last, std::forward_iterator_tag) {
            const size_t len = std::distance(first, last);
            if (len > size()) {
                ForwardIt mid = first;
                std::advance(mid, size());
                std::copy(first, mid, begin());
                m_range_insert_aux(end(), mid, last,
                                   std::__iterator_category(first));
            } else
                m_erase_at_end(std::copy(first, last, begin()));
        }

        template<typename ForwardIterator>
        void m_range_insert_aux(iterator pos, ForwardIterator first, ForwardIterator last, std::forward_iterator_tag) {
            const size_t n = std::distance(first, last);
            if (pos.m_cur == this->m_impl.m_start.m_cur) {
                iterator newStart = m_reserve_elements_at_front(n);
                __try {
                    std::__uninitialized_copy_a(first, last, newStart, m_get_Tp_allocator());
                    this->m_impl.m_start = newStart;
                }
                __catch(...) {
                    m_destroy_nodes(newStart.m_node, this->m_impl.m_start.m_node);
                    __throw_exception_again;
                }
            } else if (pos.m_cur == this->m_impl.m_finish.m_cur) {
                iterator __new_finish = m_reserve_elements_at_back(n);
                __try {
                    std::__uninitialized_copy_a(first, last, this->m_impl.m_finish, m_get_Tp_allocator());
                    this->m_impl.m_finish = __new_finish;
                }
                __catch(...) {
                    m_destroy_nodes(this->m_impl.m_finish.m_node + 1, __new_finish.m_node + 1);
                    __throw_exception_again;
                }
            } else
                m_insert_aux(pos, first, last, n);
        }

        void m_fill_insert(iterator pos, size_t n, const T &__x) {
            if (pos.m_cur == this->m_impl.m_start.m_cur) {
                iterator __new_start = m_reserve_elements_at_front(n);
                __try {
                    std::__uninitialized_fill_a(__new_start, this->m_impl.m_start,
                                                __x, m_get_Tp_allocator());
                    this->m_impl.m_start = __new_start;
                }
                __catch(...) {
                    m_destroy_nodes(__new_start.m_node, this->m_impl.m_start.m_node);
                    __throw_exception_again;
                }
            } else if (pos.m_cur == this->m_impl.m_finish.m_cur) {
                iterator __new_finish = m_reserve_elements_at_back(n);
                __try {
                    std::__uninitialized_fill_a(this->m_impl.m_finish,
                                                __new_finish, __x,
                                                m_get_Tp_allocator());
                    this->m_impl.m_finish = __new_finish;
                }
                __catch(...) {
                    m_destroy_nodes(this->m_impl.m_finish.m_node + 1,
                                    __new_finish.m_node + 1);
                    __throw_exception_again;
                }
            } else
                m_insert_aux(pos, n, __x);
        }


        template<typename... Args>
        iterator m_insert_aux(iterator pos, Args &&... args) {
            T __x_copy(std::forward<Args>(args)...); // XXX copy

            std::ptrdiff_t __index = pos - this->m_impl.m_start;
            if (static_cast<size_t>(__index) < size() / 2) {
                push_front(std::move(front()));
                iterator __front1 = this->m_impl.m_start;
                ++__front1;
                iterator __front2 = __front1;
                ++__front2;
                pos = this->m_impl.m_start + __index;
                iterator __pos1 = pos;
                ++__pos1;
                _GLIBCXX_MOVE3(__front2, __pos1, __front1);
            } else {
                push_back(_GLIBCXX_MOVE(back()));
                iterator __back1 = this->m_impl.m_finish;
                --__back1;
                iterator __back2 = __back1;
                --__back2;
                pos = this->m_impl.m_start + __index;
                _GLIBCXX_MOVE_BACKWARD3(pos, __back2, __back1);
            }
            *pos = _GLIBCXX_MOVE(__x_copy);
            return pos;
        }

        void m_insert_aux(iterator __pos, size_t __n, const T &__x) {
            const std::ptrdiff_t __elems_before = __pos - this->m_impl.m_start;
            const size_t __length = this->size();
            T __x_copy = __x;
            if (__elems_before < std::ptrdiff_t(__length / 2)) {
                iterator __new_start = m_reserve_elements_at_front(__n);
                iterator __old_start = this->m_impl.m_start;
                __pos = this->m_impl.m_start + __elems_before;
                __try {
                    if (__elems_before >= std::ptrdiff_t(__n)) {
                        iterator __start_n = (this->m_impl.m_start
                                              + std::ptrdiff_t(__n));
                        std::__uninitialized_move_a(this->m_impl.m_start,
                                                    __start_n, __new_start,
                                                    m_get_Tp_allocator());
                        this->m_impl.m_start = __new_start;
                        _GLIBCXX_MOVE3(__start_n, __pos, __old_start);
                        std::fill(__pos - std::ptrdiff_t(__n), __pos, __x_copy);
                    } else {
                        std::__uninitialized_move_fill(this->m_impl.m_start,
                                                       __pos, __new_start,
                                                       this->m_impl.m_start,
                                                       __x_copy,
                                                       m_get_Tp_allocator());
                        this->m_impl.m_start = __new_start;
                        fill(__old_start, __pos, __x_copy);
                    }
                }
                __catch(...) {
                    m_destroy_nodes(__new_start.m_node,
                                    this->m_impl.m_start.m_node);
                    __throw_exception_again;
                }
            } else {
                iterator __new_finish = m_reserve_elements_at_back(__n);
                iterator __old_finish = this->m_impl.m_finish;
                const std::ptrdiff_t __elems_after =
                        std::ptrdiff_t(__length) - __elems_before;
                __pos = this->m_impl.m_finish - __elems_after;
                __try {
                    if (__elems_after > std::ptrdiff_t(__n)) {
                        iterator __finish_n = (this->m_impl.m_finish
                                               - std::ptrdiff_t(__n));
                        std::__uninitialized_move_a(__finish_n,
                                                    this->m_impl.m_finish,
                                                    this->m_impl.m_finish,
                                                    m_get_Tp_allocator());
                        this->m_impl.m_finish = __new_finish;
                        _GLIBCXX_MOVE_BACKWARD3(__pos, __finish_n, __old_finish);
                        fill(__pos, __pos + std::ptrdiff_t(__n), __x_copy);
                    } else {
                        std::__uninitialized_fill_move(this->m_impl.m_finish,
                                                       __pos + std::ptrdiff_t(__n),
                                                       __x_copy, __pos,
                                                       this->m_impl.m_finish,
                                                       m_get_Tp_allocator());
                        this->m_impl.m_finish = __new_finish;
                        std::fill(__pos, __old_finish, __x_copy);
                    }
                }
                __catch(...) {
                    m_destroy_nodes(this->m_impl.m_finish.m_node + 1,
                                    __new_finish.m_node + 1);
                    __throw_exception_again;
                }
            }
        }

        template<typename ForwardIterator>
        void m_insert_aux(iterator pos, ForwardIterator first, ForwardIterator last, size_t n) {
            const std::ptrdiff_t __elemsbefore = pos - this->m_impl.m_start;
            const size_t __length = size();
            if (static_cast<size_t>(__elemsbefore) < __length / 2) {
                iterator __new_start = m_reserve_elements_at_front(n);
                iterator __old_start = this->m_impl.m_start;
                pos = this->m_impl.m_start + __elemsbefore;
                __try {
                    if (__elemsbefore >= std::ptrdiff_t(n)) {
                        iterator __start_n = (this->m_impl.m_start
                                              + std::ptrdiff_t(n));
                        std::__uninitialized_move_a(this->m_impl.m_start,
                                                    __start_n, __new_start,
                                                    m_get_Tp_allocator());
                        this->m_impl.m_start = __new_start;
                        _GLIBCXX_MOVE3(__start_n, pos, __old_start);
                        std::copy(first, last, pos - std::ptrdiff_t(n));
                    } else {
                        ForwardIterator __mid = first;
                        std::advance(__mid, std::ptrdiff_t(n) - __elemsbefore);
                        std::__uninitialized_move_copy(this->m_impl.m_start,
                                                       pos, first, __mid,
                                                       __new_start,
                                                       m_get_Tp_allocator());
                        this->m_impl.m_start = __new_start;
                        copy(__mid, last, __old_start);
                    }
                }
                __catch(...) {
                    m_destroy_nodes(__new_start.m_node,
                                    this->m_impl.m_start.m_node);
                    __throw_exception_again;
                }
            } else {
                iterator __new_finish = m_reserve_elements_at_back(n);
                iterator __old_finish = this->m_impl.m_finish;
                const std::ptrdiff_t __elemsafter =
                        std::ptrdiff_t(__length) - __elemsbefore;
                pos = this->m_impl.m_finish - __elemsafter;
                __try {
                    if (__elemsafter > std::ptrdiff_t(n)) {
                        iterator __finish_n = (this->m_impl.m_finish
                                               - std::ptrdiff_t(n));
                        std::__uninitialized_move_a(__finish_n,
                                                    this->m_impl.m_finish,
                                                    this->m_impl.m_finish,
                                                    m_get_Tp_allocator());
                        this->m_impl.m_finish = __new_finish;
                        _GLIBCXX_MOVE_BACKWARD3(pos, __finish_n, __old_finish);
                        copy(first, last, pos);
                    } else {
                        ForwardIterator __mid = first;
                        std::advance(__mid, __elemsafter);
                        std::__uninitialized_copy_move(__mid, last, pos,
                                                       this->m_impl.m_finish,
                                                       this->m_impl.m_finish,
                                                       m_get_Tp_allocator());
                        this->m_impl.m_finish = __new_finish;
                        copy(first, __mid, pos);
                    }
                }
                __catch(...) {
                    m_destroy_nodes(this->m_impl.m_finish.m_node + 1,
                                    __new_finish.m_node + 1);
                    __throw_exception_again;
                }
            }
        }

        void m_destroy_data_aux(iterator first, iterator last) {
            for (Map_pointer __node = first.m_node + 1;
                 __node < last.m_node; ++__node)
                std::_Destroy(*__node, *__node + s_buffer_size(), m_get_Tp_allocator());

            if (first.m_node != last.m_node) {
                std::_Destroy(first.m_cur, first.m_last,
                              m_get_Tp_allocator());
                std::_Destroy(last.m_first, last.m_cur,
                              m_get_Tp_allocator());
            } else
                std::_Destroy(first.m_cur, last.m_cur,
                              m_get_Tp_allocator());
        }


        template<typename Alloc1>
        void m_destroy_data(iterator first, iterator last, const Alloc1 &) { m_destroy_data_aux(first, last); }

        void m_erase_at_begin(iterator pos) {
            m_destroy_data(begin(), pos, m_get_Tp_allocator());
            m_destroy_nodes(this->m_impl.m_start.m_node, pos.m_node);
            this->m_impl.m_start = pos;
        }


        void m_erase_at_end(iterator pos) {
            m_destroy_data(pos, end(), m_get_Tp_allocator());
            m_destroy_nodes(pos.m_node + 1, this->m_impl.m_finish.m_node + 1);
            this->m_impl.m_finish = pos;
        }

        iterator m_erase(iterator pos) {
            iterator __next = pos;
            ++__next;
            const std::ptrdiff_t __index = pos - begin();
            if (static_cast<size_t>(__index) < (size() >> 1)) {
                if (pos != begin())
                    _GLIBCXX_MOVE_BACKWARD3(begin(), pos, __next);
                pop_front();
            } else {
                if (__next != end())
                    _GLIBCXX_MOVE3(__next, end(), pos);
                pop_back();
            }
            return begin() + __index;
        }

        iterator m_erase(iterator first, iterator last) {
            if (first == last)
                return first;
            else if (first == begin() && last == end()) {
                clear();
                return end();
            } else {
                const std::ptrdiff_t __n = last - first;
                const std::ptrdiff_t __elems_before = first - begin();
                if (static_cast<size_t>(__elems_before) <= (size() - __n) / 2) {
                    if (first != begin())
                        _GLIBCXX_MOVE_BACKWARD3(begin(), first, last);
                    m_erase_at_begin(begin() + __n);
                } else {
                    if (last != end())
                        _GLIBCXX_MOVE3(last, end(), first);
                    m_erase_at_end(end() - __n);
                }
                return begin() + __elems_before;
            }
        }

        void m_default_append(size_t __n) {
            if (__n) {
                iterator __new_finish = m_reserve_elements_at_back(__n);
                __try {
                    std::__uninitialized_default_a(this->m_impl.m_finish,
                                                   __new_finish,
                                                   m_get_Tp_allocator());
                    this->m_impl.m_finish = __new_finish;
                }
                __catch(...) {
                    m_destroy_nodes(this->m_impl.m_finish.m_node + 1,
                                    __new_finish.m_node + 1);
                    __throw_exception_again;
                }
            }
        }

        bool m_shrink_to_fit() {
            const std::ptrdiff_t __front_capacity
                    = (this->m_impl.m_start.m_cur - this->m_impl.m_start.m_first);
            if (__front_capacity == 0)
                return false;

            const std::ptrdiff_t __back_capacity
                    = (this->m_impl.m_finish.m_last - this->m_impl.m_finish.m_cur);
            if (__front_capacity + __back_capacity < s_buffer_size())
                return false;

            return std::__shrink_to_fit_aux<Deque>::_S_do_it(*this);
        }

        iterator m_reserve_elements_at_back(size_t n) {
            const size_t vacancies = (this->m_impl.m_finish.m_last
                                      - this->m_impl.m_finish.m_cur) - 1;
            if (n > vacancies)
                m_new_elements_at_back(n - vacancies);
            return this->m_impl.m_finish + std::ptrdiff_t(n);
        }

        void m_new_elements_at_front(size_t __new_elems) {
            if (this->max_size() - this->size() < __new_elems)
                std::__throw_length_error(__N("Deque::_M_new_elements_at_front"));

            const size_t __new_nodes = ((__new_elems + s_buffer_size() - 1)
                                        / s_buffer_size());
            m_reserve_map_at_front(__new_nodes);
            size_t __i;
            __try {
                for (__i = 1; __i <= __new_nodes; ++__i)
                    *(this->m_impl.m_start.m_node - __i) = this->m_allocate_node();
            }
            __catch(...) {
                for (size_t __j = 1; __j < __i; ++__j)
                    m_deallocate_node(*(this->m_impl.m_start.m_node - __j));
                __throw_exception_again;
            }
        }

        void m_new_elements_at_back(size_t __new_elems) {
            if (this->max_size() - this->size() < __new_elems)
                std::__throw_length_error(__N("Deque::_M_new_elements_at_back"));

            const size_t __new_nodes = ((__new_elems + s_buffer_size() - 1)
                                        / s_buffer_size());
            m_reserve_map_at_back(__new_nodes);
            size_t __i;
            __try {
                for (__i = 1; __i <= __new_nodes; ++__i)
                    *(this->m_impl.m_finish.m_node + __i) = this->m_allocate_node();
            }
            __catch(...) {
                for (size_t __j = 1; __j < __i; ++__j)
                    m_deallocate_node(*(this->m_impl.m_finish.m_node + __j));
                __throw_exception_again;
            }
        }

        iterator m_reserve_elements_at_front(size_t n) {
            const size_t vacancies = this->m_impl.m_start.m_cur
                                     - this->m_impl.m_start.m_first;
            if (n > vacancies)
                m_new_elements_at_front(n - vacancies);
            return this->m_impl.m_start - std::ptrdiff_t(n);
        }

        void m_reallocate_map(size_t __nodes_to_add, bool __add_at_front) {
            const size_t __old_num_nodes
                    = this->m_impl.m_finish.m_node - this->m_impl.m_start.m_node + 1;
            const size_t __new_num_nodes = __old_num_nodes + __nodes_to_add;

            Map_pointer __new_nstart;
            if (this->m_impl._M_map_size > 2 * __new_num_nodes) {
                __new_nstart = this->m_impl._M_map + (this->m_impl._M_map_size
                                                      - __new_num_nodes) / 2
                               + (__add_at_front ? __nodes_to_add : 0);
                if (__new_nstart < this->m_impl.m_start.m_node)
                    copy(this->m_impl.m_start.m_node,
                         this->m_impl.m_finish.m_node + 1,
                         __new_nstart);
                else
                    copy_backward(this->m_impl.m_start.m_node,
                                  this->m_impl.m_finish.m_node + 1,
                                  __new_nstart + __old_num_nodes);
            } else {
                size_t __new_map_size = this->m_impl._M_map_size
                                        + std::max(this->m_impl._M_map_size,
                                                   __nodes_to_add) + 2;

                Map_pointer __new_map = this->m_allocate_map(__new_map_size);
                __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2
                               + (__add_at_front ? __nodes_to_add : 0);
                copy(this->m_impl.m_start.m_node,
                     this->m_impl.m_finish.m_node + 1,
                     __new_nstart);
                m_deallocate_map(this->m_impl._M_map, this->m_impl._M_map_size);

                this->m_impl._M_map = __new_map;
                this->m_impl._M_map_size = __new_map_size;
            }

            this->m_impl.m_start._M_set_node(__new_nstart);
            this->m_impl.m_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
        }


        void
        m_reserve_map_at_back(size_t nodesToAdd = 1) {
            if (nodesToAdd + 1 > this->m_impl.m_map_size
                                 - (this->m_impl.m_finish.m_node - this->m_impl.m_map))
                m_reallocate_map(nodesToAdd, false);
        }

        void m_reserve_map_at_front(size_t nodes_to_add = 1) {
            if (nodes_to_add > size_type(this->m_impl.m_start.m_node - this->m_impl.m_map))
                m_reallocate_map(nodes_to_add, true);
        }


        void m_move_assign1(Deque &&x, std::true_type) noexcept {
            this->m_impl.m_swap_data(x.m_impl);
            x.clear();
            std::__alloc_on_move(m_get_Tp_allocator(), x.m_get_Tp_allocator());
        }


        void m_move_assign1(Deque &&x, std::false_type) {
            constexpr bool __move_storage =
                    Alloc_traits::_S_propagate_on_move_assign();
            m_move_assign2(std::move(x), __bool_constant<__move_storage>());
        }

        template<typename... Args>
        void m_replace_map(Args &&... args) {
            // Create new data first, so if allocation fails there are no effects.
            Deque __newobj(std::forward<Args>(args)...);
            // Free existing storage using existing allocator.
            clear();
            m_deallocate_node(*begin().m_node); // one node left after clear()
            m_deallocate_map(this->m_impl.m_map, this->m_impl.m_map_size);
            this->m_impl.m_map = nullptr;
            this->m_impl.m_map_size = 0;
            // Take ownership of replacement memory.
            this->m_impl.m_swap_data(__newobj.m_impl);
        }


        void
        m_move_assign2(Deque &&x, std::true_type) {
            // Make a copy of the original allocator state.
            auto __alloc = x.m_get_Tp_allocator();
            // The allocator propagates so storage can be moved from x,
            // leaving x in a valid empty state with a moved-from allocator.
            m_replace_map(move(x));
            // Move the corresponding allocator state too.
            m_get_Tp_allocator() = std::move(__alloc);
        }


        void m_move_assign2(Deque &&x, std::false_type) {
            if (x.m_get_Tp_allocator() == this->m_get_Tp_allocator()) {
                // The allocators are equal so storage can be moved from x,
                // leaving x in a valid empty state with its current allocator.
                m_replace_map(move(x), x.get_allocator());
            } else {
                // The rvalue's allocator cannot be moved and is not equal,
                // so we need to individually move each element.
                m_assign_aux(std::__make_move_if_noexcept_iterator(x.begin()),
                             std::__make_move_if_noexcept_iterator(x.end()),
                             std::random_access_iterator_tag());
                x.clear();
            }
        }


        //TODO

        template<class InputIt>
        void m_assign_dispatch(InputIt first, InputIt last, std::false_type) {
            m_assign_aux(first, last, std::__iterator_category(first));
        }

        // TODO
        void m_fill_assign(size_t n, const T &val) {
            if (n > size()) {
                fill(begin(), end(), val);
                m_fill_insert(end(), n - size(), val);
            } else {
                m_erase_at_end(begin() + std::ptrdiff_t(n));
                fill(begin(), end(), val);
            }
        }

        template<typename... Args>
        void m_push_back_aux(Args &&... args) {
            if (size() == max_size())
                std::__throw_length_error(__N("cannot create std::Deque larger than max_size()"));

            m_reserve_map_at_back();
            *(this->m_impl.m_finish.m_node + 1) = this->m_allocate_node();
            __try {

                Alloc_traits::construct(this->m_impl,
                                        this->m_impl.m_finish.m_cur,
                                        std::forward<Args>(args)...);

                this->m_impl.m_finish._M_set_node(this->m_impl.m_finish.m_node + 1);
                this->m_impl.m_finish.m_cur = this->m_impl.m_finish.m_first;
            }
            __catch(...) {
                m_deallocate_node(*(this->m_impl.m_finish.m_node + 1));
                __throw_exception_again;
            }
        }

        template<typename... Args>
        void m_push_front_aux(Args &&... args) {
            if (size() == max_size())
                std::__throw_length_error(
                        __N("cannot create std::Deque larger than max_size()"));

            m_reserve_map_at_front();
            *(this->m_impl.m_start.m_node - 1) = this->m_allocate_node();
            __try {
                this->m_impl.m_start.m_set_node(this->m_impl.m_start.m_node - 1);
                this->m_impl.m_start.m_cur = this->m_impl.m_start.m_last - 1;
                Alloc_traits::construct(this->m_impl,
                                        this->m_impl.m_start.m_cur,
                                        std::forward<Args>(args)...);
            }
            __catch(...) {
                ++this->m_impl.m_start;
                m_deallocate_node(*(this->m_impl.m_start.m_node - 1));
                __throw_exception_again;
            }
        }


        void m_pop_back_aux() {
            m_deallocate_node(this->m_impl.m_finish.m_first);
            this->m_impl.m_finish._M_set_node(this->m_impl.m_finish.m_node - 1);
            this->m_impl.m_finish.m_cur = this->m_impl.m_finish.m_last - 1;
            Alloc_traits::destroy(m_get_Tp_allocator(), this->m_impl.m_finish.m_cur);
        }

        void pop_front_aux() {
            Alloc_traits::destroy(m_get_Tp_allocator(), this->m_impl.m_start.m_cur);
            m_deallocate_node(this->m_impl.m_start.m_first);
            this->m_impl.m_start._M_set_node(this->m_impl.m_start.m_node + 1);
            this->m_impl.m_start.m_cur = this->m_impl.m_start.m_first;
        }


        template<typename Integer>
        void m_insert_dispatch(iterator pos, Integer n, Integer x, std::true_type) {
            m_fill_initialize(pos, n, x);
        }

        template<typename InputIt>
        void m_insert_dispatch(iterator pos, InputIt first, InputIt last, std::false_type) {
            m_range_insert_aux(pos, first, last, std::__iterator_category(first));
        }


    public:


        size_t max_size() const noexcept {
            return s_max_size(m_get_Tp_allocator());
        }

        void swap(Deque &x) noexcept {
            m_impl.m_swap_data(x.m_impl);
            Alloc_traits::_S_on_swap(m_get_Tp_allocator(), x.m_get_T_allocator());
        }


        iterator erase(const_iterator first, const_iterator last) {
            return m_erase(first.m_const_cast(), last.m_const_cast());
        }

        iterator erase(const_iterator pos) {
            return m_erase(pos.m_const_cast());
        }

        template<class InputIt, typename = std::_RequireInputIter<InputIt>>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            std::ptrdiff_t offset = pos - cbegin();
            m_insert_dispatch(pos.m_const_cast(), first, last, std::false_type());
            return begin() + offset;
        }

        iterator insert(const_iterator pos, size_t n, const T &x) {
            std::ptrdiff_t offset = pos - cbegin();
            m_fill_insert(pos.m_const_cast(), n, x);
            return begin() + offset;
        }

        iterator insert(const_iterator p, std::initializer_list<T> l) {
            auto __offset = p - cbegin();
            m_range_insert_aux(p.m_const_cast(), l.begin(), l.end(),
                               std::random_access_iterator_tag());
            return begin() + __offset;
        }

        void clear() noexcept { m_erase_at_end(begin()); }


        iterator insert(const_iterator position, T &&x) { return emplace(position, std::move(x)); }

        iterator insert(const_iterator position, const T &x) {
            if (position.m_cur == this->m_impl.m_start.m_cur)
            {
                push_front(x);
                return this->m_impl.m_start;
            }
            else if (position.m_cur == this->m_impl.m_finish.m_cur)
            {
                push_back(x);
                iterator __tmp = this->m_impl.m_finish;
                --__tmp;
                return __tmp;
            }
            else
                return m_insert_aux(position._M_const_cast(), x);
        }

        template<typename... Args>
        iterator emplace(const_iterator position, Args &&... args) {
            if (position.m_cur == this->m_impl.m_start.m_cur)
            {
                emplace_front(std::forward<Args>(args)...);
                return this->m_impl.m_start;
            }
            else if (position.m_cur == this->m_impl.m_finish.m_cur)
            {
                emplace_back(std::forward<Args>(args)...);
                iterator __tmp = this->m_impl.m_finish;
                --__tmp;
                return __tmp;
            }
            else
                return m_insert_aux(position._M_const_cast(),
                                     std::forward<Args>(args)...);
        }



        void
        pop_back() noexcept {
            __glibcxx_requires_nonempty();
            if (this->m_impl.m_finish.m_cur
                != this->m_impl.m_finish.m_first) {
                --this->m_impl.m_finish.m_cur;
                Alloc_traits::destroy(this->m_impl,
                                       this->m_impl.m_finish.m_cur);
            } else
                m_pop_back_aux();
        }

        void
        pop_front() noexcept {
            __glibcxx_requires_nonempty();
            if (this->m_impl.m_start.m_cur
                != this->m_impl.m_start.m_last - 1) {
                Alloc_traits::destroy(this->m_impl,
                                      this->m_impl.m_start.m_cur);
                ++this->m_impl.m_start.m_cur;
            } else
                pop_front_aux();
        }

        template<typename... Args>
        reference emplace_back(Args &&... args) {
            if (this->m_impl.m_finish.m_cur
                != this->m_impl.m_finish.m_last - 1)
            {
                Alloc_traits::construct(this->m_impl,
                                         this->m_impl.m_finish.m_cur,
                                         std::forward<Args>(args)...);
                ++this->m_impl.m_finish.m_cur;
            }
            else
                m_push_back_aux(std::forward<Args>(args)...);

            return back();
        }

        void push_back(T &&__x) { emplace_back(std::move(__x)); }

        void
        push_back(const T &__x) {
            if (this->m_impl.m_finish.m_cur
                != this->m_impl.m_finish.m_last - 1) {
                Alloc_traits::construct(this->m_impl,this->m_impl.m_finish.m_cur, __x);
                ++this->m_impl.m_finish.m_cur;
            } else
                m_push_back_aux(__x);
        }


        template<typename... Args>
        reference emplace_front(Args &&... args) {
            if (this->m_impl.m_start.m_cur != this->m_impl.m_start.m_first)
            {
                Alloc_traits::construct(this->m_impl,
                                         this->m_impl.m_start.m_cur - 1,
                                         std::forward<Args>(args)...);
                --this->m_impl.m_start.m_cur;
            }
            else
                m_push_front_aux(std::forward<Args>(args)...);
            return front();
        }


        void push_front(T&& x) { emplace_front(std::move(x)); }

        void push_front(const T &x) {
            if (this->m_impl.m_start.m_cur != this->m_impl.m_start.m_first) {
                Alloc_traits::construct(this->m_impl,this->m_impl.m_start.m_cur - 1, x);
                --this->m_impl.m_start.m_cur;
            } else
                m_push_front_aux(x);
        }

        reference at(size_t nn) {
            m_range_check(nn);
            return (*this)[nn];
        }


        const_reference at(size_t n) const {
            m_range_check(n);
            return (*this)[n];
        }

        // TODO
        reference front() noexcept {
            __glibcxx_requires_nonempty();
            return *begin();
        }

        const_reference front() const noexcept {
            __glibcxx_requires_nonempty();
            return *begin();
        }

        reference back() noexcept {
            __glibcxx_requires_nonempty();
            iterator __tmp = end();
            --__tmp;
            return *__tmp;
        }

        const_reference back() const noexcept {
            __glibcxx_requires_nonempty();
            const_iterator __tmp = end();
            --__tmp;
            return *__tmp;
        }


        // element access

        reference operator[](size_t n) noexcept {
            __glibcxx_requires_subscript(n);
            return this->m_impl.m_start[std::ptrdiff_t (n)];
        }

        const_reference operator[](size_t n) const noexcept {
            __glibcxx_requires_subscript(n);
            return this->m_impl.m_start[std::ptrdiff_t (n)];
        }

        void shrink_to_fit() noexcept { m_shrink_to_fit(); }

        void resize(size_t newSize) {
            const size_t __len = size();
            if (newSize > __len)
                m_default_append(newSize - __len);
            else if (newSize < __len)
                m_erase_at_end(this->m_impl.m_start
                                + std::ptrdiff_t (newSize));
        }

        void resize(size_t newSize, const T &x) {
            const size_t __len = size();
            if (newSize > __len)
                m_fill_insert(this->m_impl.m_finish, newSize - __len, x);
            else if (newSize < __len)
                m_erase_at_end(this->m_impl.m_start
                                + std::ptrdiff_t (newSize));
        }

        void assign(std::initializer_list <T> l) {
            m_assign_aux(l.begin(), l.end(), std::random_access_iterator_tag());
        }

        template<typename InputIterator>
        void assign(InputIterator fi, InputIterator la) { m_assign_dispatch(fi, la, std::false_type()); }

        void assign(size_t n, const T &val) { m_fill_assign(n, val); }

        Deque & operator=(std::initializer_list <T> l) {
            m_assign_aux(l.begin(), l.end(), std::random_access_iterator_tag());
            return *this;
        }

        Deque & operator=(Deque &&x) noexcept(Alloc_traits::_S_always_equal()) {
            using __always_equal = typename Alloc_traits::is_always_equal;
            m_move_assign1(std::move(x), __always_equal{});
            return *this;
        }

        Deque &operator=(const Deque &x) {
            if (&x != this)
            {
                if (Alloc_traits::_S_propagate_on_copy_assign())
                {
                    if (!Alloc_traits::_S_always_equal()
                        && m_get_Tp_allocator() != x.m_get_Tp_allocator())
                    {
                        // Replacement allocator cannot free existing storage,
                        // so deallocate everything and take copy of x's data.
                        _M_replace_map(x, x.get_allocator());
                        std::__alloc_on_copy(m_get_Tp_allocator(),
                                             x.m_get_Tp_allocator());
                        return *this;
                    }
                    std::__alloc_on_copy(m_get_Tp_allocator(),
                                         x.m_get_Tp_allocator());
                }
                const size_t __len = size();
                if (__len >= x.size())
                    m_erase_at_end(std::copy(x.begin(), x.end(),
                                             this->m_impl.m_start));
                else
                {
                    const_iterator __mid = x.begin() + std::ptrdiff_t (__len);
                    std::copy(x.begin(), __mid, this->m_impl.m_start);
                    m_range_insert_aux(this->m_impl.m_finish, __mid, x.end(),
                                       std::random_access_iterator_tag());
                }
            }
            return *this;
        }


        ~Deque() { m_destroy_data(begin(), end(), m_get_Tp_allocator()); }

        template<typename InputIt,typename = std::_RequireInputIter<InputIt>>
        Deque(InputIt first, InputIt last, const Alloc &a = Alloc()): Base(a){
            m_initialize_dispatch(first, last, std::false_type());
        }

        Deque(std::initializer_list<T> l,const Alloc& a = Alloc()): Base(a)
                {
                        m_range_initialize(l.begin(), l.end(),std::random_access_iterator_tag());
                }


        Deque(Deque&& x, const Alloc& a): Base(std::move(x), a, x.size())
        {
            if (x.get_allocator() != a)
            {
                std::__uninitialized_move_a(x.begin(), x.end(),
                                            this->m_impl.m_start,
                                            m_get_Tp_allocator());
                x.clear();
            }
        }


        Deque(const Deque& x, const Alloc& a): Base(a, x.size()){
            std::__uninitialized_copy_a(x.begin(), x.end(),
                                        this->m_impl.m_start,
                                        m_get_Tp_allocator());
        }


         Deque(Deque&& __x): Base(std::move(__x)) { }


        Deque(const Deque& x): Base(Alloc_traits::_S_select_on_copy(x.m_get_Tp_allocator()), x.size())
        { std::__uninitialized_copy_a(x.begin(), x.end(),
                                      this->m_impl.m_start,
                                      m_get_Tp_allocator());
        }

        Deque(size_t n, const T& value,const Alloc& a = Alloc())
        : Base(a, s_check_init_len(n, a))
        { m_fill_initialize(value); }



        explicit Deque(size_t n, const Alloc& a = Alloc()): Base(a, _S_check_init_len(n, a))
        {
            m_default_initialize();
        }



        explicit Deque(const Alloc& a) : Base(a, 0) { }



        Deque() : Base() { }

    };

    template<typename T, typename Alloc>
    inline bool operator==(const Deque<T, Alloc>& x, const Deque<T, Alloc>& y)
    { return x.size() == y.size()
             && std::equal(x.begin(), x.end(), y.begin());
    }


    template<typename T, typename Alloc>
    inline bool operator<(const Deque<T, Alloc>& x,const Deque<T, Alloc>& y)
    { return std::lexicographical_compare(x.begin(), x.end(),
                                          y.begin(), y.end()); }


    template<typename T, typename Alloc>
    inline bool operator!=(const Deque<T, Alloc>& x,const Deque<T, Alloc>& y)
    { return !(x == y); }


    template<typename T, typename Alloc>
    inline bool operator>(const Deque<T, Alloc>& x, const Deque<T, Alloc>& y)
    { return y < x; }


    template<typename T, typename Alloc>
    inline bool operator<=(const Deque<T, Alloc>& x, const Deque<T, Alloc>& y)
    { return !(y < x); }


    template<typename T, typename Alloc>
    inline bool operator>=(const Deque<T, Alloc>& x, const Deque<T, Alloc>& y)
    { return !(x < y); }


    template<typename T, typename Alloc>
    inline void swap(Deque<T,Alloc>& x, Deque<T,Alloc>& y)noexcept(noexcept(x.swap(y)))
    { x.swap(y); }
}


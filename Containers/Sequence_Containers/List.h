//
// Created by arthurdovlat, sargiskh, hovhannes  on 3/18/21.
//

#ifndef OURSTLV2_LIST_H
#define OURSTLV2_LIST_H

#include <ext/alloc_traits.h>
#include <ext/aligned_buffer.h>
#include <bits/allocated_ptr.h>


namespace OurSTL {

    // Node basic part
    class Double_Linked_List_Node_Base {
    public:
        Double_Linked_List_Node_Base* next;
        Double_Linked_List_Node_Base* prev;

        static void swap(Double_Linked_List_Node_Base* const first,Double_Linked_List_Node_Base* const second) noexcept;
        void transfer(Double_Linked_List_Node_Base* const first,Double_Linked_List_Node_Base* const second) noexcept;
        void reverse() noexcept;
        void hook(Double_Linked_List_Node_Base* const position) noexcept;
        void unhook(Double_Linked_List_Node_Base* const position) noexcept;
    };

    // node header
    class Node_header : public Double_Linked_List_Node_Base {
    private:
        Double_Linked_List_Node_Base* base() {return this; }
    public:
        std::size_t size;
        void init() noexcept {
            this->next = this->prev = this;
            this->size = 0;
        }

        Node_header() noexcept {
            init();
        }
        Node_header(Node_header&& x) noexcept : Double_Linked_List_Node_Base{x.next,x.prev},size(x.size) {
            if (x.base()->next = x.base()) {
                this->next = this->prev = this;
            } else {
                this->next->prev = this->prev->next = this->base();
                x.init();
            }
        }
        void move_nodes(Node_header&& x) {
            Double_Linked_List_Node_Base* const node = x.base();
            if (node->next == node) {
                init();
            } else {
                Double_Linked_List_Node_Base* const n = this->base();
                n->next = node->next;
                n->prev = node->prev;
                n->next->prev = n->prev->next = n;
                size = x.size;
                x.init();
            }
        }
    };

    // Actual node
    template<typename T>
    class Doubly_Linked_Node : public Double_Linked_List_Node_Base {
    public:
        __gnu_cxx::__aligned_membuf<T> storage;
        T* valptr() {return storage._M_ptr();}
        const T* valptr() const { return storage._M_ptr(); }
    };

    // List iterator
    template<typename T>
    class list_iterator {
    public:
        typedef Doubly_Linked_Node<T> Node;
        typedef std::bidirectional_iterator_tag iterator_category;
        Double_Linked_List_Node_Base* node;

        list_iterator() noexcept : node() {}
        explicit list_iterator(Double_Linked_List_Node_Base* x) : node(x) {}


        list_iterator self_const_cast() const noexcept {
            return *this;
        }

        T& operator*() const noexcept {
            return *static_cast<Node*>(node)->valptr();
        }

        T* operator->() const noexcept {
            return static_cast<Node*>(node)->valptr();
        }

        list_iterator& operator++() noexcept {
            node = node->next;
            return *this;
        }

        list_iterator operator++(int) noexcept {
            list_iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        list_iterator& operator--() noexcept {
            node = node->prev;
            return *this;
        }

        list_iterator operator--(int) noexcept {
            list_iterator tmp = this;
            node = node->prev;
            return tmp;
        }

        friend bool operator==(const list_iterator& x, const list_iterator& y) noexcept{
            return x.node == y.node;
        }

        friend bool operator!=(const list_iterator& x,const list_iterator& y) noexcept{
            return x.node != y.node;
        }
    };

    template<typename T>
    class list_const_iterator {
        typedef const Doubly_Linked_Node<T> Node;
        typedef list_iterator<T> iterator;
        typedef std::bidirectional_iterator_tag iterator_category;
        const Double_Linked_List_Node_Base* node;

        list_const_iterator() noexcept : node() {}
        explicit list_const_iterator(Double_Linked_List_Node_Base* x) : node(x) {}


        iterator self_const_cast() const noexcept {
            return iterator(const_cast<Double_Linked_List_Node_Base*>(node));
        }

        T& operator*() const noexcept {
            return *static_cast<Node*>(node)->valptr();
        }

        T* operator->() const noexcept {
            return static_cast<Node*>(node)->valptr();
        }

        list_const_iterator& operator++() noexcept {
            node = node->next;
            return *this;
        }

        list_const_iterator operator++(int) noexcept {
            list_iterator tmp = *this;
            node = node->next;
            return tmp;
        }

        list_const_iterator& operator--() noexcept {
            node = node->prev;
            return *this;
        }

        list_const_iterator operator--(int) noexcept {
            list_iterator tmp = this;
            node = node->prev;
            return tmp;
        }

        friend bool operator==(const list_const_iterator& x, const list_const_iterator& y) noexcept{
            return x.node == y.node;
        }

        friend bool operator!=(const list_const_iterator& x,const list_const_iterator& y) noexcept{
            return x.node != y.node;
        }
    };

    // Contains Doubly linked list implementation
    template<typename T,typename Alloc>
    class List_base {
    protected:
        typedef typename __gnu_cxx::__alloc_traits<Alloc>::template rebind<T>::other T_alloc_type;
        typedef __gnu_cxx::__alloc_traits<T_alloc_type>	T_alloc_traits;
        typedef typename T_alloc_traits::template
        rebind<Doubly_Linked_Node<T> >::other Node_alloc_type;
        typedef __gnu_cxx::__alloc_traits<Node_alloc_type> Node_alloc_traits;


        static size_t s_distance(const Double_Linked_List_Node_Base* first, const Double_Linked_List_Node_Base* last) {
            size_t n = 0;
            while (first != last) {
                first = first->next;
                ++n;
            }
            return n;
        }

        struct List_impl : public Node_alloc_type  {
            Node_header node;

            List_impl() noexcept(std::is_nothrow_default_constructible_v<Node_alloc_type>) : Node_alloc_type() {
            }

            List_impl(const Node_alloc_type& a) noexcept : Node_alloc_type(a) {}

            List_impl(List_impl&& ) = default;

            List_impl(Node_alloc_type&& a, List_impl&& x) : Node_alloc_type(std::move(a)),node(std::move(x.node)) { }

            List_impl(Node_alloc_type&& a) noexcept : Node_alloc_type(std::move(a)) { }
        };

        List_impl impl;

        size_t my_get_size() const { return impl.node.size; }

        void my_set_size(size_t n) { impl.node.size = n; }

        void my_inc_size(size_t n) { impl.node.size = n; }

        void my_dec_size(size_t n) { impl.node.size = n; }

        size_t my_distance(const Double_Linked_List_Node_Base* first, const Double_Linked_List_Node_Base* last) const {
            return s_distance(first,last);
        }

        size_t my_node_count() const { return my_get_size(); }

        typename Node_alloc_traits::pointer  my_get_node() {
            return Node_alloc_traits::allocate(impl,1);
        }

        void my_put_node(typename Node_alloc_traits::pointer p) noexcept {
            Node_alloc_traits::deallocate(impl,p,1);
        }

    public:
        typedef Alloc allocator_type;

        Node_alloc_type & get_Node_allocator() noexcept {
            return impl;
        }

        const Node_alloc_type& get_Node_allocator() const noexcept {
            return impl;
        }

        void move_nodes(List_base&& x) {
            impl.node.move_nodes(std::move(x.impl.node));
        }

        List_base() = default;

        List_base(const Node_alloc_type& a) noexcept : impl(a)  {}

        List_base(List_base&& ) = default;

        List_base(List_base&& x, Node_alloc_type&& a): impl(std::move(a)) {
            if (x.get_Node_allocator() == get_Node_allocator()) {
                move_nodes(std::move(x));
            }
        }

        List_base(Node_alloc_type&& a, List_base&& x) : impl(std::move(a),std::move(x.impl)) { }

        List_base(Node_alloc_type&& a) : impl(std::move(a)) {}

        void my_clear() noexcept {
            Double_Linked_List_Node_Base* cur = impl.node.next;
            while (cur != &impl.node) {
                Doubly_Linked_Node<T>* tmp = static_cast<Doubly_Linked_Node<T>*>(cur);
                cur = tmp->next;
                T* val = tmp->valptr();
                Node_alloc_traits::destroy(get_Node_allocator(),val);
                my_put_node(tmp);
            }
        }

        void my_init() noexcept {
            this->impl.node.init();
        }

        ~List_base() noexcept {
            my_clear();
        }


    };

    template<typename T,typename Alloc = std::allocator<T>>
            class list : protected List_base<T,Alloc> {
                static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,"std::list must have a non-const, non-volatile value_type");
                typedef List_base<T, Alloc>			Base;
                typedef typename Base::T_alloc_type		T_alloc_type;
                typedef typename Base::T_alloc_traits		T_alloc_traits;
                typedef typename Base::Node_alloc_type		Node_alloc_type;
                typedef typename Base::Node_alloc_traits	Node_alloc_traits;

            public:
                typedef typename T_alloc_traits::pointer	 pointer;
                typedef typename T_alloc_traits::const_pointer	 const_pointer;
                typedef typename T_alloc_traits::reference	 reference;
                typedef typename T_alloc_traits::const_reference const_reference;
                typedef list_iterator<T>			 iterator;
                typedef list_const_iterator<T>			 const_iterator;
                typedef std::reverse_iterator<const_iterator>	 const_reverse_iterator;
                typedef std::reverse_iterator<iterator>		 reverse_iterator;
                typedef size_t					 size_type;
                typedef ptrdiff_t					 difference_type;
                typedef Alloc					 allocator_type;

            protected:
                typedef Doubly_Linked_Node<T> Node;
                using Base::impl;
                using  Base::my_put_node;
                using Base::my_get_node;
                using Base::get_Node_allocator;

                template<typename... Args>
                Node* my_create_node(Args... args) {
                    auto p = this->my_get_node();
                    auto& alloc = get_Node_allocator();
                    std::__allocated_ptr<Node_alloc_type> guard{alloc,p};
                    Node_alloc_traits::construct(alloc,p->valptr(),std::forward<Args>(args)...);
                    guard = nullptr;
                    return p;
                }

                static size_t S_distance(const_iterator first, const_iterator last) {
                    return std::distance(first,last);
                }

                size_t my_node_count() const {
                    return this->my_get_size();
                }

                template<typename... Args>
                void my_insert(iterator position, Args&&... args) {
                    Node* tmp = my_create_node(std::forward<Args>(args)...);
                    tmp->hook(position.node);
                    this->my_inc_size(1);
                }


            public:

                Alloc get_allocator() const _GLIBCXX_NOEXCEPT
                { return allocator_type(Base::get_Node_allocator()); }

                template<typename... Args>
                iterator emplace(const_iterator __position, Args&&... __args)
                {
                    Node* __tmp = my_create_node(std::forward<Args>(__args)...);
                    __tmp->hook(__position.self_const_cast().node);
                    this->my_inc_size(1);
                    return iterator(__tmp);
                }
                iterator insert(const_iterator position, const T& x) {
                    Node* tmp = my_create_node(x);
                    tmp->hook(position.self_const_cast().node);
                    this->my_inc_size(1);
                    return iterator(tmp);
                }
                bool empty() const _GLIBCXX_NOEXCEPT {
                    return this->impl.node.next == &this->impl.node;
                }
                void check_equal_allocators(list& __x) noexcept
                {
                    if (std::__alloc_neq<typename Base::_Node_alloc_type>::_S_do_it(get_allocator(), __x.get_allocator()))
                        __builtin_abort();
                }
                void transfer(iterator __position, iterator __first, iterator __last)
                { __position.node->transfer(__first._M_node, __last._M_node); }

                void splice(const_iterator position, list&& x) noexcept {
                    if (!x.empty()) {
                        check_equal_allocators(x);
                        this->transfer(position.self_const_cast(),x.begin(),x.end());
                        this->my_inc_size(x.my_get_size());
                        x.my_set_size(0);
                    }
                }
                void splice(const_iterator __position, list& __x) noexcept
                { splice(__position, std::move(__x)); }

                void splice(const_iterator __position, list&& __x, const_iterator __i) noexcept {
                    iterator __j = __i.self_const_cast();
                    ++__j;
                    if (__position == __i || __position == __j)
                        return;

                    if (this != std::__addressof(__x))
                        check_equal_allocators(__x);

                    this->transfer(__position.self_const_cast(),
                                      __i.self_const_cast(), __j);

                    this->my_inc_size(1);
                    __x.my_dec_size(1);
                }

                void splice(const_iterator __position, list& __x, const_iterator __i) noexcept
                { splice(__position, std::move(__x), __i); }

                void splice(const_iterator __position, list&& __x, const_iterator __first,
                       const_iterator __last) noexcept {
                    if (__first != __last)
                    {
                        if (this != std::__addressof(__x))
                            check_equal_allocators(__x);

                        size_t __n = S_distance(__first, __last);
                        this->my_inc_size(__n);
                        __x.my_dec_size(__n);

                        this->transfer(__position.self_const_cast(),
                                          __first.self_const_cast(),
                                          __last.self_const_cast());
                    }
                }

                void splice(const_iterator __position, list& __x, const_iterator __first,
                       const_iterator __last) noexcept
                { splice(__position, std::move(__x), __first, __last); }
                iterator insert(const_iterator position, size_t n, const T& x) {
                    if (n) {
                        list tmp(n,x,get_allocator());
                        iterator it = tmp.begin();
                        splice(position,tmp);
                        return it;
                    }
                    return position.self_const_cast();
                }

                template<typename InputIt,typename = std::_RequireInputIter<InputIt>>
                iterator insert(const_iterator position,InputIt first,InputIt last) {
                    list tmp(first,last,get_allocator());
                    if (!tmp.empty()) {
                        iterator it = tmp.begin();
                        splice(position,tmp);
                        return it;
                    }
                    return position.self_const_cast();
                }

                iterator erase(const_iterator position) noexcept {
                    iterator __ret = iterator(position.node->next);
                    _M_erase(position.self_const_cast());
                    return __ret;
                }

                iterator begin() noexcept {
                    return iterator(this->impl.node.next);
                }

                const_iterator begin() const noexcept {
                    return const_iterator(this->impl.node.next);
                }

                iterator end() noexcept {
                  return iterator(&this->impl.node);
                }

                const_iterator end() const noexcept {
                    return const_iterator(&this->impl.node);
                }

                reverse_iterator rbegin() noexcept {
                    return reverse_iterator(end());
                }

                const_reverse_iterator rbegin() const noexcept {
                    return const_reverse_iterator(end());
                }

                reverse_iterator rend() noexcept {
                    return reverse_iterator(begin());
                }

                const_reverse_iterator rend() const noexcept {
                    return const_reverse_iterator(begin());
                }

                const_iterator cbegin() const noexcept {
                    return const_iterator(this->impl.node.next);
                }

               const_iterator cend() const noexcept {
                   return const_iterator(&this->impl.node);
                }

                const_reverse_iterator crbegin() const noexcept {
                    return const_reverse_iterator(end());
                }

                const_reverse_iterator crend() const noexcept {
                    return const_reverse_iterator(begin());
                }


                size_t size() const noexcept {
                    return my_node_count();
                }

                size_t max_size() const noexcept {
                    return Node_alloc_traits::max_size(get_Node_allocator());
                }
                void resize(size_type __new_size)
                {
                    const_iterator __i = my_resize_pos(__new_size);
                    if (__new_size)
                        my_default_append(__new_size);
                    else
                        erase(__i, end());
                }
                void resize(size_type __new_size, const T& __x)
                {
                    const_iterator __i = my_resize_pos(__new_size);
                    if (__new_size)
                        insert(end(), __new_size, __x);
                    else
                        erase(__i, end());
                }


                T& front() noexcept {
                    return *begin();
                }

                const_reference front() const noexcept {
                    return *begin();
                }

                reference back() noexcept {
                    iterator tmp = end();
                    --tmp;
                    return *tmp;
                }

                const_reference back() const noexcept {
                    const_iterator tmp = end();
                    --tmp;
                    return *tmp;
                }

                void push_front(const T& x) {
                    this->insert(begin(),x);
                }

                void push_front(T&& x) {
                    this->insert(begin(),std::move(x));
                }

                template<typename... Args>
                reference emplace_front(Args... args) {
                    this->insert(begin(),std::forward<Args>(args)...);
                    return front;
                }

                void pop_front() noexcept {
                    this->erase(begin());
                }

                void push_back(const T& x) noexcept {
                    this->insert(end(), x);
                }

                void push_back(T&& x) noexcept {
                    this->insert(end(),std::move(x));
                }

                template<typename... Args>
                reference emplace_back(Args&&... args) {
                    this->insert(end(), std::forward<Args>(args)...);
                    return back();
                }

                void pop_back() noexcept {
                    this->erase(iterator(this->impl.node.prev));
                }

                iterator insert(const_iterator pos, T&& x) {
                    return emplace(pos,std::move(x));
                }

                iterator insert(const_iterator position,std::initializer_list<T> x) {
                    return this->insert(position,x.begin(),x.end());
                }

                iterator erase(const_iterator first, const_iterator last) noexcept {
                    while (first != last) {
                        first = erase(first);
                    }
                    return last.self_const_cast();
                }

                void clear() noexcept {
                    Base::my_clear();
                    Base::my_init();
                }

                void swap(list& x) noexcept {
                    Double_Linked_List_Node_Base::swap(this->impl.node, x.impl.node);
                    size_t xsize = x.my_get_size();
                    x.my_set_size(this->my_get_size());
                    this->my_set_size(xsize);

                    Node_alloc_traits::_S_in_swap(this->get_Node_allocator(), x.get_Node_allocator());
                }


                size_t remove(const T& value) {
                    size_type __removed __attribute__((__unused__)) = 0;
                    iterator __first = begin();
                    iterator __last = end();
                    iterator __extra = __last;
                    while (__first != __last)
                    {
                        iterator __next = __first;
                        ++__next;
                        if (*__first == value)
                        {
                            // _GLIBCXX_RESOLVE_LIB_DEFECTS
                            // 526. Is it undefined if a function in the standard changes
                            // in parameters?
                            if (std::__addressof(*__first) != std::__addressof(value))
                            {
                                erase(__first);
                                __removed++;
                            }
                            else
                                __extra = __first;
                        }
                        __first = __next;
                    }
                    if (__extra != __last)
                    {
                        _M_erase(__extra);
                        __removed++;
                    }
                    return __removed;
                }
            protected:
                const_iterator my_resize_pos(size_type& __new_size) const {
                    const_iterator __i;
                    const size_type __len = size();
                    if (__new_size < __len) {
                        if (__new_size <= __len / 2) {
                            __i = begin();
                            std::advance(__i, __new_size);
                        } else {
                            __i = end();
                            ptrdiff_t __num_erase = __len - __new_size;
                            std::advance(__i, -__num_erase);
                        }
                        __new_size = 0;
                        return __i;
                    } else
                        __i = end();
                    __new_size -= __len;
                    return __i;
                }

                void my_default_append(size_t __new_size) {
                    size_type __i = 0;
                    __try
                    {
                        for (; __i < __new_size; ++__i)
                            emplace_back();
                    }
                    __catch(...)
                    {
                        for (; __i; --__i)
                            pop_back();
                        __throw_exception_again;
                    }
                }



                template<typename InputIt>
                void my_assign_dispatch(InputIt first,InputIt last, std::false_type) {
                    iterator __first1 = begin();
                    iterator __last1 = end();
                    for (; __first1 != __last1 && first != last;
                           ++__first1, (void)++first)
                        *__first1 = *first;
                    if (first == last)
                        erase(__first1, __last1);
                    else
                        insert(__last1, first, last);
                }

                template<typename Integer>
                void my_assign_dispatch(Integer n, Integer val, std::true_type) {
                    my_fill_assign(n,val);
                }



                void my_fill_assign(size_type __n, const T& __val)
                {
                    iterator __i = begin();
                    for (; __i != end() && __n > 0; ++__i, --__n)
                        *__i = __val;
                    if (__n > 0)
                        insert(end(), __n, __val);
                    else
                        erase(__i, end());
                }

                void my_default_initialize(size_t n) {
                    for (;n;--n) {
                        emplace_back();
                    }
                }

                void my_fill_initialize(size_t n, const T& x) {
                    for (;  n ; --n) {
                        push_back(x);
                    }
                }
                template<class Integer>
                void my_initialize_dispatch(Integer n, Integer x, std::true_type) {
                    my_fill_initialize(static_cast<size_t>(n),x);
                }

                template<typename Iterator>
                void my_initialize_dispatch(Iterator first, Iterator last, std::false_type) {
                    for (; first != last ; ++first) {
                        emplace_back(*first);
                    }
                }

                void my_transfer(iterator pos,iterator first, iterator last) {
                    pos.node->transfer(first.node, last.node);
                }

                void my_erase(iterator pos) noexcept {
                    this->my_dec_size(1);
                    pos.node->unhook();
                    Node* __n = static_cast<Node*>(pos.node);
                    Node_alloc_traits::destroy(get_Node_allocator(), __n->valptr());
                    my_put_node(__n);
                }

                void my_move_assign(list&& x,std::true_type) noexcept {
                    this->clear();
                    this->move_nodes(std::move(x));
                    std::__alloc_on_move(this->get_Node_allocator(),x.get_Node_allocator());
                }


                void mt_move_assign(list&& x, std::true_type) {
                    if (x.get_Node_allocator() == this->get_Node_allocator()) {
                        mt_move_assign(std::move(x),std::true_type{});
                    } else {
                        my_assign_dispatch(std::__make_move_if_noexcept_iterator(x.begin()),
                                           std::__make_move_if_noexcept_iterator(x.end()),
                                           std::false_type{});
                    }

                }
            public:



                list() = default;

                explicit list(const Alloc& all) : Base(Node_alloc_type(all)) {}

                explicit list(size_t n, const Alloc& all = allocator_type()) : Base(Node_alloc_type(all)) {
                    my_default_initialize(n);
                }


                list(size_t n, const T& val, const allocator_type& a = allocator_type()) :
                        Base(Node_alloc_type(a)) {
                    my_fill_initialize(n,val);
                }

                list(const list& x) : Base(Node_alloc_traits::_S_select_on_copy(x.get_Node_allocator())) {
                    my_initialize_dispatch(x.begin(),x.end(),std::false_type());
                }

                list(list&&) = default;

                list(std::initializer_list<T&> l, const allocator_type& a = allocator_type()) : Base(Node_alloc_type(a)) {
                    my_initialize_dispatch(l.begin(),l.end(),std::false_type());
                }

                list(const list& x, const allocator_type& a) : Base(Node_alloc_type(a)) {
                    my_initialize_dispatch(x.begin(),x,end(), std::false_type());
                }

            private:
                list(list&& x, const allocator_type& a, std::true_type) noexcept : Base(Node_alloc_type(a),std::move(x))
                {}

                list(list&& x, const allocator_type& a, std::false_type) : Base(Node_alloc_type(a)) {
                    if (x.get_Node_allocator() == this->get_Node_allocator()) {
                        this->move_nodes(std::move(x));
                    } else {
                        insert(begin(),std::__make_move_if_noexcept_iterator(x.begin()),
                               std::__make_move_if_noexcept_iterator(x.end()));
                    }
                }


            public:
                list(list&& x, const allocator_type& a) noexcept(Node_alloc_traits::_S_always_equal()) :
                list(std::move(x), a, typename Node_alloc_traits::is_always_equal{}) {

                }

                template<typename InputIterator, typename  = std::_RequireInputIter<InputIterator>>
                list(InputIterator first, InputIterator last, const allocator_type& a = allocator_type()) :
                Base(Node_alloc_type(a)) {
                    my_initialize_dispatch(first,last,std::false_type());
                }

                ~list() = default;

                list& operator=(const list& x) {
                    if (this != std::__addressof(x))
                    {
                        if (Node_alloc_traits::_S_propagate_on_copy_assign())
                        {
                            auto& __this_alloc = this->get_Node_allocator();
                            auto& __that_alloc = x.get_Node_allocator();
                            if (!Node_alloc_traits::_S_always_equal()
                                && __this_alloc != __that_alloc)
                            {
                                // replacement allocator cannot free existing storage
                                clear();
                            }
                            std::__alloc_on_copy(__this_alloc, __that_alloc);
                        }
                        my_assign_dispatch(x.begin(), x.end(), std::__false_type());
                    }
                    return *this;
                }

                list& operator=(list&& x) noexcept(Node_alloc_traits::_S_nothrow_move()) {
                    constexpr bool move_strage= Node_alloc_traits::_S_propogate_on_move_assign() ||
                            Node_alloc_traits::_S_always_equal();
                    my_move_assign(std::move(x), std::__bool_constant<move_strage>());
                    return *this;
                }

                void assign(size_t n, const T& val) {
                    my_fill_assign(n,val);
                }


                template<typename InputIt, typename = std::_RequireInputIter<InputIt>>
                        void assign(InputIt first, InputIt last) {
                    my_assign_dispatch(first,last,std::false_type());
                }

                void assign(std::initializer_list<T> l) {
                    this->template my_assign_dispatch(l.begin(),l.end(),std::false_type());
                }
                list& operator=(std::initializer_list<T> l) {
                    this->assign(l.begin(),l.end());
                    return *this;
                }

                template<typename... Args>
                T& emplace_back(Args&&... args) {
                    this->insert(end(),std::forward<Args>(args)...);
                    return back();
                }


                // TODO implement unique merge remove_if
            };
}

#endif //OURSTLV2_LIST_H

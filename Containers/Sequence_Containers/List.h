//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTLV2_LIST_H
#define OURSTLV2_LIST_H

#include <ext/alloc_traits.h>
#include <ext/aligned_buffer.h>
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

            List_impl(Node_alloc_type&& a, List_impl&& x) : Node_alloc_type(std::move(a)),node(x.node) { }

            List_impl(Node_alloc_type&& a) noexcept : Node_alloc_type(std::move(a)) { }
        };

        List_impl impl;

        size_t get_size() const { return impl.node.size; }

        void set_size(size_t n) { impl.node.size = n; }

        void inc_size(size_t n) { impl.node.size = n; }

        void dec_size(size_t n) { impl.node.size = n; }

        size_t distance(const Double_Linked_List_Node_Base* first, const Double_Linked_List_Node_Base* last) const {
            return s_distance(first,last);
        }

        size_t node_count() const { return get_size(); }

        typename Node_alloc_traits::pointer  get_node() {
            return Node_alloc_traits::allocate(impl,1);
        }

        void put_node(typename Node_alloc_traits::pointer p) noexcept {
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

        void clear() noexcept;

        void init() noexcept {
            this->impl.node.init();
        }

        ~List_base() noexcept {
            clear();
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
            };
}

#endif //OURSTLV2_LIST_H

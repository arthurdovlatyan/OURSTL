//
// Created by arthurdovlat, sargiskh, hovhannes  on 3/18/21.
//

#ifndef OURSTL_FORWARD_LIST_H
#define OURSTL_FORWARD_LIST_H
#include <bits/stl_iterator.h>
#include <ext/alloc_traits.h>
#include <ext/type_traits.h>
#include <type_traits>
#include <ext/aligned_buffer.h>
#include <bits/stl_iterator_base_types.h>
#include <iterator>
#include <bits/stl_algobase.h>
#include <bits/stl_function.h>

namespace OurSTL {

    /*
     * SinglyLinkedListNodeBase class
     * Is a base for nodes of the forward list
     * Contains only a pointer to the next element
     */
    class SinglyLinkedListNodeBase {
    public:
        SinglyLinkedListNodeBase* next = nullptr;
        // constructors and assignment operators
        SinglyLinkedListNodeBase() = default;
        SinglyLinkedListNodeBase(SinglyLinkedListNodeBase&& x) noexcept : next(x.next) {x.next = nullptr;}
        SinglyLinkedListNodeBase& operator=(SinglyLinkedListNodeBase&& x) noexcept {
            next = x.next;
            x.next = nullptr;
            return *this;
        }
        SinglyLinkedListNodeBase(SinglyLinkedListNodeBase& x) = delete;
        SinglyLinkedListNodeBase& operator=(SinglyLinkedListNodeBase& x) = delete;

        SinglyLinkedListNodeBase* transfer_after(SinglyLinkedListNodeBase* begin, SinglyLinkedListNodeBase* end) noexcept {
            SinglyLinkedListNodeBase* keep = begin->next;
            if (end) {
                begin->next = end->next;
                end->next = next;
            } else {
                begin->next = nullptr;
            }
            next = keep;
            return end;
        }

        void reverse_after() noexcept {
            SinglyLinkedListNodeBase* tail = next;
            if(!tail) {
                return;
            }
            while (SinglyLinkedListNodeBase* tmp = tail->next) {
                SinglyLinkedListNodeBase* keep = next;
                next = tmp;
                tail->next = tmp->next;
                next->next = keep;
            }
        }
    };

    template<typename T>
    class Fwd_node : public SinglyLinkedListNodeBase {
    private:
        __gnu_cxx::__aligned_buffer<T> storage;
    public:
        Fwd_node() = default;
        T* valptr() noexcept { return storage._M_ptr(); }
        const T* valptr() const noexcept { return storage._M_ptr(); }
    };

    template<typename T>
    class Fwd_list_iterator{
    private:
        SinglyLinkedListNodeBase* node;
    public:
        typedef std::forward_iterator_tag iterator_category;
        Fwd_list_iterator() noexcept : node()  {}
        explicit Fwd_list_iterator(SinglyLinkedListNodeBase* n) noexcept : node(n) { }
        Fwd_list_iterator(const Fwd_list_iterator<T>& it) noexcept : node(it.node) { }
        const T& operator*() const noexcept { return *static_cast<Fwd_node<T>*>(this->node)->val.ptr(); }
        const T* operator->() const noexcept { return static_cast<Fwd_node<T>*>(this->node)->valptr(); }
        Fwd_node<T>& operator++() noexcept {
            node = node->next;
            return *this;
        }
        Fwd_node<T> operator++(int) noexcept {
            Fwd_list_iterator tmp(*this);
            node = node->next;
            return tmp;
        }
        friend bool operator==(const Fwd_list_iterator<T>& x, const Fwd_list_iterator<T>& y) noexcept {
            return x.node == y.node;
        }
        friend bool operator!=(const Fwd_list_iterator<T>& x, const Fwd_list_iterator<T>& y) noexcept {
            return x.node != y.node;
        }
        Fwd_list_iterator next() const noexcept {
            if (this->node) {
                return Fwd_list_iterator<T>(node->next);
            } else {
                return Fwd_list_iterator<T>(nullptr);
            }
        }
    };

    template<typename T>
    class Fwd_list_const_iterator {
    private:
        const SinglyLinkedListNodeBase* node;
    public:
        typedef std::forward_iterator_tag iterator_category;
        Fwd_list_const_iterator() noexcept : node()  {}
        explicit Fwd_list_const_iterator(SinglyLinkedListNodeBase* n) noexcept : node(n) { }
        Fwd_list_const_iterator(const Fwd_list_iterator<T>& it) noexcept : node(it.node) { }
        const T& operator*() const noexcept { return *static_cast<Fwd_node<T>*>(this->node)->valaptr(); }
        const T* operator->() const noexcept { return static_cast<Fwd_node<T>*>(this->node)->valptr(); }
        Fwd_node<T>& operator++() noexcept {
            node = node->next;
            return *this;
        }
        Fwd_node<T> operator++(int) noexcept {
            Fwd_list_iterator tmp(*this);
            node = node->next;
            return tmp;
        }
        friend bool operator==(const Fwd_list_iterator<T>& x, const Fwd_list_iterator<T>& y) noexcept {
            return x.node == y.node;
        }
        friend bool operator!=(const Fwd_list_iterator<T>& x, const Fwd_list_iterator<T>& y) noexcept {
            return x.node != y.node;
        }
        Fwd_list_const_iterator next() const noexcept {
            if (this->node) {
                return Fwd_list_const_iterator<T>(node->next);
            } else {
                return Fwd_list_const_iterator<T>(nullptr);
            }
        }
    };


    /*
     * Main function is to contain the singlylinked list implementation and some utils
     */
    template<typename T,typename Alloc>
    class Fwd_list_base
    {
    protected:
        typedef std::__alloc_rebind<Alloc,Fwd_node<T>> Node_alloc_type;
        typedef __gnu_cxx::__alloc_traits<Node_alloc_type> Node_alloc_traits;

        class SinglyLinkedList : public Node_alloc_type  {
        public:
            SinglyLinkedListNodeBase head;
            // TODO INSPECT this constructor
            SinglyLinkedList() noexcept(std::is_nothrow_default_constructible_v<Node_alloc_type>) : Node_alloc_type(), head() {}
            SinglyLinkedList(SinglyLinkedList&&) = default;
            SinglyLinkedList(SinglyLinkedList&& x, Node_alloc_type&& a) : Node_alloc_type(std::move(a)), head(std::move(x.head)) {}
            SinglyLinkedList(Node_alloc_type&& a) : Node_alloc_type(std::move(a)),head() { }
        };
        SinglyLinkedList list;

        // list util methods
        Fwd_node<T>* my_get_Node() {
            auto ptr = Node_alloc_type::allocate(get_Node_Allocator(),1);
            return std::__to_address(ptr);
        }
        void my_put_node(Fwd_node<T>* p) {
            typedef typename Node_alloc_traits::pointer Ptr;
            auto ptr = std::pointer_traits<Ptr>::pointer_to(*p);
            Node_alloc_type::deallocate(get_Node_Allocator(),ptr,1);
        }
        template<typename... Args>
        Fwd_node<T> my_create_node(Args&&... args) {
            Fwd_node<T>* node = this->get_Node();
            try {
                ::new ((void*)node) Fwd_node<T>;
                Node_alloc_type::construct(get_Node_Allocator(),node->valptr(),std::forward<Args>(args)...);
            } catch (...) {
                this->put_node(node);
                __throw_exception_again;
            }
            return node;
        }


        SinglyLinkedListNodeBase* my_erase_after(SinglyLinkedListNodeBase* pos) {
            Node* curr = static_cast<Node*>(pos->next);
            pos->next = curr->next;
            Node_alloc_traits::destroy(get_Node_Allocator(),curr->valptr());
            curr->~Node();
            my_put_node(curr);
            return pos->next;
        }
        SinglyLinkedListNodeBase* my_erase_after(SinglyLinkedListNodeBase* pos,SinglyLinkedListNodeBase* last) {
            Node* curr = static_cast<Node*>(pos->next);
            while (curr != last) {
                Node* tmp = curr;
                curr = static_cast<Node*>(curr->next);
                Node_alloc_traits::destroy(get_Node_Allocator(),tmp->valptr());
                tmp->~Node();
                my_put_node(tmp);
            }
            pos->next = last;
            return last;
        }
    public:
        typedef Fwd_list_iterator<T> iterator;
        typedef Fwd_list_const_iterator<T> const_iterator;
        typedef Fwd_node<T> Node;

        Node_alloc_type& get_Node_Allocator() noexcept { this->list; }
        const Node_alloc_type& get_Node_Allocator() const noexcept { this->list; }


        Fwd_list_base() = default;

        Fwd_list_base(Node_alloc_type&& a) : list(std::move(a)) {}

        // When allocators are always equal
        Fwd_list_base(Fwd_list_base&& l, Node_alloc_type&& a, std::true_type) : list(std::move(l.list),std::move(a)) {}

        // When allocators are not always equal
        Fwd_list_base(Fwd_list_base&& ) = default;
        Fwd_list_base(Fwd_list_base&& l, Node_alloc_type&& a) : list(std::move(l)) {
            if (l.get_Node_Allocator() == get_Node_Allocator()) {
                this->list.head = std::move(l.head);
            }
        }

        ~Fwd_list_base() { my_erase_after(&list.head,nullptr); }

    protected:
        Node* my_get_node() {
            auto ptr = Node_alloc_traits::allocate(get_Node_Allocator(),1);
            return ptr;
        }

        template<typename... Args>
        Node* my_create_node(Args&&... args) {
            Node* node = this->get_Node();
            try {
                ::new ((void*)node) Node;
                Node_alloc_traits::construct(get_Node_Allocator(),node->valptr(),std::forward<Args>(args)...);
            } catch(...) {
                this->put_node(node);
                __throw_exception_again;
            }
            return node;
        }

        // TODO
        template<typename... Args>
        SinglyLinkedListNodeBase* my_insert_after(const_iterator pos,Args&&... args) {
            SinglyLinkedListNodeBase* to = const_cast<SinglyLinkedListNodeBase*>(pos.node);
            Node* thing = my_create_node(std::forward<Args>(args)...);
            thing->next = thing;
            return to->next;
        }


    };

    template<typename T, typename Allocator = std::allocator<T>>
    class Forward_list : private Fwd_list_base<T,Allocator>{
        static_assert(std::is_same<typename std::remove_cv<T>::type,T>::value,"std::forward_list must have a non-const, non-volatile value_type");
#ifdef __STRICT_ANSI__
        static_assert(std::is_same<typename Alloc::value_type, T>::value,
	  "std::forward_list must have the same value_type as its allocator");
#endif

    private:
        typedef std::allocator_traits<std::__alloc_rebind<Allocator,T>> Alloc_traits;
        typedef Fwd_list_base<T,Allocator> Base;
        typedef typename Base::Node_alloc_type Node_alloc_type;
        typedef typename Base::Node_alloc_traits Node_alloc_traits;
        typedef typename Base::Node Node;
    public:
        typedef typename Base::iterator iterator;
        typedef typename Base::const_iterator const_iterator;


        // Iterators
        /*
         * Parameters
         * (none)
         * Return value
         * Iterator to the element before the first element.
         * Complexity
         * Constant.
         */
        iterator before_begin() noexcept {
            return iterator(&this->_M_impl._M_head);
        }
        const_iterator before_begin() const noexcept {
            return const_iterator(&this->list.head);
        }
        const_iterator cbefore_begin() const noexcept {
            return const_iterator(&this->list.head);
        }

        /*
         * Parameters
         * (none)
         * Return value
         * Iterator to the first element.
         * Complexity
         * Constant.
         */
        iterator begin() noexcept {
            return iterator(&this->list.head.next);
        }
        const_iterator begin() const noexcept {
            return const_iterator(&this->list.head.next);
        }
        const_iterator cbegin() const noexcept {
            return const_iterator(&this->list.head.next);
        }

        /*
         * Parameters
         * (none)
         * Return value
         * Iterator to the element following the last element.
         * Complexity
         * Constant.
         */
        iterator end() noexcept {
            return iterator(nullptr);
        }
        const_iterator end() const noexcept {
            return const_iterator(nullptr);
        }
        const_iterator cend() const noexcept {
            return const_iterator(nullptr);
        }

        /// Get a copy of the memory allocation object.
        Allocator get_allocator() const noexcept
        { return Allocator(this->get_Node_Allocator()); }
    private:
        Forward_list(Forward_list&& __list, Node_alloc_type&& __al,
                std::false_type): Base(std::move(__list), std::move(__al))
        {
            // If __list is not empty it means its allocator is not equal to __a,
            // so we need to move from each element individually.
            insert_after(cbefore_begin(),
                         std::__make_move_if_noexcept_iterator(__list.begin()),
                         std::__make_move_if_noexcept_iterator(__list.end()));
        }

        Forward_list(Forward_list&& __list, Node_alloc_type&& __al,
                std::true_type)noexcept: Base(std::move(__list), Node_alloc_type(__al), std::true_type{})
        { }



        template<typename InputIterator>void range_initialize(InputIterator first, InputIterator last) {
            SinglyLinkedListNodeBase* to = &this->list.head;
            for (; first != last ; ++first) {
                to->next = this->template my_create_node(*first);
                to = to->next;
            }
        }

        void fill_initialize(size_t n, const T& value) {
            SinglyLinkedListNodeBase* to = &this->list.head;
            for (; n; --n) {
                to->next = this->my_create_node(value);
                to = to->next;
            }
        }

        iterator splice_after(const_iterator pos, const_iterator before,const_iterator last) {
            SinglyLinkedListNodeBase* tmp = const_cast<SinglyLinkedListNodeBase*>(pos.node);
            SinglyLinkedListNodeBase* b = const_cast<SinglyLinkedListNodeBase*>(before.node);
            SinglyLinkedListNodeBase* end = b;

            while (end && end->next != last.node) {
                end = end->next;
            }
            if (b != end) {
                return iterator(tmp->transfer_after(b,end));
            } else {
                return iterator(tmp);
            }
        }

        void default_initialize(size_t n) {
            SinglyLinkedListNodeBase* to = *this->list.head;
            for(;n; --n) {
                to->next = this->my_create_node();
                to = to->next;
            }
        }

        void default_insert_after(const_iterator pos, size_t n) {
            const_iterator saved_pos = pos;
            try {
                for (; n; --n) {
                    pos = emplace_after(pos);
                }
            } catch (...) {
                erase_after(saved_pos,++pos);
                __throw_exception_again;
            }
        }

        void move_assign(Forward_list&& list, std::true_type) noexcept
        {
            clear();
            this->list.head.next = list.list.head.next;
            list.list.head.next = nullptr;
            std::__alloc_on_move(this->get_Node_allocator(),
                                 list.get_Node_allocator());
        }

        void move_assign(Forward_list&& __list, std::false_type)
        {
            if (__list.Node_allocator() == this->get_Node_allocator())
                move_assign(std::move(__list), std::true_type());
            else
                // The rvalue's allocator cannot be moved, or is not equal,
                // so we need to individually move each element.
                this->assign(std::__make_move_if_noexcept_iterator(__list.begin()),
                             std::__make_move_if_noexcept_iterator(__list.end()));
        }

        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last, std::true_type)
        {
            auto prev = before_begin();
            auto curr = begin();
            auto end = this->end();
            while (curr != end && first != last)
            {
                *curr = *first;
                ++prev;
                ++curr;
                ++first;
            }
            if (first != last)
                insert_after(prev, first, last);
            else if (curr != end)
                erase_after(prev, end);
        }

        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last, std::false_type)
        {
            clear();
            insert_after(cbefore_begin(), first, last);
        }

        //if Tp is CopyAssignable
        void assign_n(size_t n, const T& val, std::true_type )
        {
            auto prev = before_begin();
            auto curr = begin();
            auto end = this->end();
            while (curr != end && n > 0)
            {
                *curr = val;
                ++prev;
                ++curr;
                --n;
            }
            if (n > 0)
                insert_after(prev, n, val);
            else if (curr != end)
                erase_after(prev, end);
        }

        // if Tp is non-CopyAssignable
        void assign_n(size_t __n, const T __val, std::false_type)
        {
            clear();
            insert_after(cbefore_begin(), __n, __val);
        }


    public:


        // Constructors
        /*
         * Creates a forward list with no elements
         */
        Forward_list() = default;

        /*
         * Creates an empty forward list
         * allocator is an allocator object
         */
        explicit Forward_list(const Allocator& allocator) noexcept : Base(Node_alloc_type(allocator)) {}

        /*
         * *  @brief  Creates a %forward_list with copies of an exemplar element.
       *  @param  __n      The number of elements to initially create.
       *  @param  __value  An element to copy.
       *  @param  __al     An allocator object.
       *
       *  This constructor fills the %forward_list with @a __n copies of
       *  @a __value.
         */
        Forward_list(size_t count, const T& value, const Allocator& allocator = Allocator()) : Base(Node_alloc_type(allocator)) {
            fill_initialize(count,value);
        }

        //TODO
        explicit Forward_list(size_t count);

        /*
         * Creates a forward list with defaul constructed elements
         * count number of elements to initially create
         * alloc an allocator object
         * Creates n default constructed elements
         */
        explicit Forward_list(size_t count, const Allocator & alloc = Allocator()) : Base(Node_alloc_type(alloc)){
            default_initialize(count);
        }

        /*
        *  @brief  Builds a %forward_list from a range.
       *  @param  __first  An input iterator.
       *  @param  __last   An input iterator.
       *  @param  __al     An allocator object.
       *
       *  Create a %forward_list consisting of copies of the elements from
       *  [@a __first,@a __last).  This is linear in N (where N is
       *  distance(@a __first,@a __last)).
         */
        template<class _InputIterator, typename = std::_RequireInputIter<_InputIterator>> Forward_list(_InputIterator first, _InputIterator last, const Allocator& allocator = Allocator()) : Base(Node_alloc_type(allocator)) {
            range_initialize(first,last);
        }

        /*
         * Copy constructor with allocator argument
         * other - input list to copy
         * allocator An allocator object
         */
        Forward_list(const Forward_list<T>& other,const Allocator& allocator = Allocator()) : Base(Node_alloc_type(allocator)) {
            range_initialize(other.begin(),other.end());
        }

        Forward_list(const Forward_list<T>& other) : Base(Node_alloc_type::_S_select_on_copy(other.get_node_allocator())) {
            range_initialize(other.begin(),other.end());
        }
        Forward_list(Forward_list&& other) = default;

        /*
         * Move constructor with allocator argument
         */
        Forward_list(Forward_list<T>&& other,const Allocator& allocator = Allocator()) noexcept(Node_alloc_type::_S_always_equal()): Forward_list(std::move(other),Node_alloc_type(allocator),typename Node_alloc_traits::is_always_equal{})
        {}
        Forward_list(std::initializer_list<T> initializerList, const Allocator& allocator = Allocator()) : Base(Node_alloc_type(allocator)) {
            range_initialize(initializerList.begin(),initializerList.end());
        }

        ~Forward_list() noexcept {}

        // Assignment Operators
        // TODO
        Forward_list& operator=(const Forward_list& other) {
            if (std::__addressof(other) != this) {
                if (Node_alloc_traits::_S_propagate_on_copy_assign()) {
                    auto& this_alloc = this->get_Node_Allocator();
                    auto& that_alloc = other.get_Node_Allocator();
                    if (!Node_alloc_traits::_S_always_equal() && this_alloc != that_alloc) {
                        clear();
                    }
                    std::__alloc_on_copy(this_alloc,that_alloc);
                }
                assign(other.cbegin(),other.cend());
            }
            return *this;
        }
        Forward_list& operator=(Forward_list&& other) noexcept(Node_alloc_traits::_S_nothrow_move()) {
            constexpr bool move_storage = Node_alloc_traits::_S_propogate_on_move_assign() || Node_alloc_traits::_S_always_equal();
            move_assign(std::move(other),std::__bool_constant<move_storage>());
            return *this;
        }
        /**
       *  @brief  The %forward_list initializer list assignment operator.
       *  @param  __il  An initializer_list of value_type.
       *
       *  Replace the contents of the %forward_list with copies of the
       *  elements in the initializer_list @a __il.  This is linear in
       *  __il.size().
       */
        Forward_list& operator=(std::initializer_list<T> il) {
            assign(il);
            return *this;
        }

        // Element Access
        /*
         * Returns a reference to the first element in the container.
         * Calling front on an empty container is undefined.
         * Parameters
         * (none)
         * Return value
         * reference to the first element
         * Complexity
         * Constant
         */
        T& front() {
            Node* front = static_cast<Node*>(this->list.head.next);
            return *front->valptr();
        }
        const T& front() const {
            Node* front = static_cast<Node*>(this->list.head.next);
            return *front->valptr();
        }


        // Capacity
        /*
         * Parameters
         * (none)
         * Return value
         * true if the container is empty, false otherwise
         * Complexity
         * Constant.
         */
        [[nodiscard]] bool empty() const noexcept {
            return this->list.head.next = nullptr;
        }

        /*
         * Parameters
         * (none)
         * Return value
         * Maximum number of elements.
         * Complexity
         * Constant.
         */
        size_t max_size() const noexcept {
            return Node_alloc_traits::max_size(this->get_Node_Allocator());
        }

        // Modifiers
        /*
         * Parameters
         * (none)
         * Return value
         * (none)
         * Complexity
         * Linear in the size of the container, i.e., the number of elements.
         */
        void clear() noexcept {
            this->my_erase_after(&this->list.head, nullptr);
        }

        /*
         * Inserts elements after the specified position in the container.
         * 1-2) inserts value after the element pointed to by pos
         * 3) inserts count copies of the value after the element pointed to by pos
         * 4) inserts elements from range [first, last) after the element pointed to by pos.
         * The behavior is undefined if first and last are iterators into *this.
         * 5) inserts elements from initializer list ilist.
         * No iterators or references are invalidated.
         * Parameters
         * pos	-	iterator after which the content will be inserted
         * value	-	element value to insert
         * count	-	number of copies to insert
         * first, last	-	the range of elements to insert
         * il	-	initializer list to insert the values from
         * Type requirements
         * -InputIt must meet the requirements of LegacyInputIterator.
         * Return value
         * 1-2) Iterator to the inserted element.
         * 3) Iterator to the last element inserted, or pos if count==0.
         * 4) Iterator to the last element inserted, or pos if first==last.
         * 5) Iterator to the last element inserted, or pos if ilist is empty.
         * Exceptions
         * If an exception is thrown during insert_after there are no effects (strong exception guarantee).
         * Complexity
         * 1-2) Constant.
         * 3) Linear in count
         * 4) Linear in std::distance(first, last)
         * 5) Linear in ilist.size()
         */
        iterator insert_after(const_iterator pos,const T& value) {
            return iterator(this->my_insert_after(pos,value));
        }
        iterator insert_after(const_iterator pos,T&& value){
            return iterator(this->template my_insert_after(pos,std::move(value)));
        }
        // TODO
        iterator insert_after(const_iterator pos,size_t count, const T& value) {
            if (count) {
                Forward_list tmp(count,value,get_allocator());
                return splice_after(pos,tmp.before_begin(),tmp.end());
            } else {
                return iterator(const_cast<SinglyLinkedListNodeBase*>(pos.node));
            }
        }
        template<class InputIt> iterator insert_after(const_iterator pos, InputIt first, InputIt last) {
            Forward_list tmp(first,last,get_allocator());
            if (!tmp.empty()) {
                return splice_after(pos,tmp.before_begin(),tmp.end());
            } else {
                return iterator(const_cast<SinglyLinkedListNodeBase*>(pos.node));
            }
        }
        iterator insert_after(const_iterator pos,std::initializer_list<T> il) {
            return insert_after(pos,il.begin(),il.end());
        }

        /*
         *Parameters
         * pos	-	iterator after which the new element will be constructed
         * args	-	arguments to forward to the constructor of the element
         * Return value
         * iterator to the new element.
         * Complexity
         * Constant.
         * Exceptions
         * If an exception is thrown (e.g. by the constructor), the container is left unmodified,
         * as if this function was never called (strong exception guarantee).
         */
        template< class... Args >iterator emplace_after( const_iterator pos, Args&&... args );
//TODO
        /*
         * Removes specified elements from the container.
         * 1) Removes the element following pos.
         * 2) Removes the elements following first until last.
         * Parameters
         * pos	-	iterator to the element preceding the element to remove
         * first, last	-	range of elements to remove
         * Return value
         * 1) Iterator to the element following the erased one, or end() if no such element exists.
         * 2) last
         * Complexity
         * 1) Constant.
         * 2) Linear in distance between first and last.
         */
        iterator erase_after( const_iterator pos ) {
            return iterator(this->my_erase_after(const_cast<SinglyLinkedListNodeBase*>(pos.node)));
        }
        iterator erase_after( const_iterator first, const_iterator last ) {
            return iterator(this->my_erase_after(const_cast<SinglyLinkedListNodeBase*>(first.node),const_cast<SinglyLinkedListNodeBase*>(last.node)));
        }


        /*
         * Prepends the given element value to the beginning of the container.
         * No iterators or references are invalidated.
         * Parameters
         * value	-	the value of the element to prepend
         * Return value
         * (none)
         * Complexity
         * Constant.
         * Exceptions
         * If an exception is thrown, this function has no effect (strong exception guarantee).
         */
        void push_front( const T& value ) {
            this->my_insert_after(cbefore_begin(),value);
        }
        void push_front( T&& value ) {
            this->my_insert_after(cbefore_begin(),std::move(value));
        }

        /*
         * Parameters
args	-	arguments to forward to the constructor of the element
Type requirements
-T (the container's element type) must meet the requirements of EmplaceConstructible.
Return value

A reference to the inserted element.	(since C++17)
Complexity
Constant.

Exceptions
If an exception is thrown, this function has no effect (strong exception guarantee)
         */
        template< class... Args >T& emplace_front( Args&&... args ) {
            this->my_insert_after(cbefore_begin(),std::forward<Args>(args)...);
            return front();
        }

        /*
         * Parameters
         * (none)
         * Return value
         * (none)
         * Complexity
         * Constant.
         * Exceptions
         * Does not throw.
         */
        void pop_front() {
            this->my_erase_after(&this->list.head);
        }

        //TODO
        /*
         * Resizes the container to contain count elements.
         * If the current size is greater than count, the container is reduced to its first count elements.
         *
         * If the current size is less than count,
         * 1) additional default-inserted elements are appended
         * 2) additional copies of value are appended.
         * Parameters
         * count	-	new size of the container
         * value	-	the value to initialize the new elements with
         * Type requirements
         * -T must meet the requirements of DefaultInsertable in order to use overload (1).
         * -T must meet the requirements of CopyInsertable in order to use overload (2).
         * Return value
         * (none)
         * Complexity
         * Linear in the difference between the current size and count. Additional complexity
         * possible due to list traversal to reach the first element to erase/the end position to insert
         */
        void resize( size_t count ) {
            iterator k = before_begin();

            size_t len = 0;
            while (k.next() != end() && len < count) {
                ++k;
                ++len;
            }
            if (len == count) {
                erase_after(k,end());
            } else {
                default_insert_after(k,count - len);
            }
        }
        void resize( size_t count, const T& value ){
            iterator k = before_begin();

            size_t len = 0;
            while (k.next() != end() && len < count) {
                ++k;
                ++len;
            }
            if (len == count) {
                erase_after(k,end());
            } else {
                insert_after(k,count- count,value);
            }
        }

        /*
         * Parameters
         * other	-	container to exchange the contents with
         * Return value
         * (none)
         * Exceptions
         * noexcept specification:
         * noexcept(std::allocator_traits<Allocator>::is_always_equal::value)
         * (since C++17)
         * Complexity
         * Constant.
         */
        void swap( Forward_list& other ) noexcept {
            std::swap(this->list.head.next,other.list.head.next);
            Node_alloc_traits::_S_on_swap(this->get_Node_Allocator(),other.get_Node_Allocator());
        }




        /**
       *  @brief  Insert contents of another %forward_list.
       *  @param  __pos  Iterator referencing the element to insert after.
       *  @param  __list  Source list.
       *
       *  The elements of @a list are inserted in constant time after
       *  the element referenced by @a pos.  @a list becomes an empty
       *  list.
       *
       *  Requires this != @a x.
       */
        void
        splice_after(const_iterator pos, Forward_list&& list) noexcept
        {
            if (!list.empty())
                my_splice_after(pos, list.before_begin(), list.end());
        }

        void
        splice_after(const_iterator pos, Forward_list& list) noexcept
        { splice_after(pos, std::move(list)); }

        /**
         *  @brief  Insert element from another %forward_list.
         *  @param  __pos  Iterator referencing the element to insert after.
         *  @param  __list  Source list.
         *  @param  __i   Iterator referencing the element before the element
         *                to move.
         *
         *  Removes the element in list @a list referenced by @a i and
         *  inserts it into the current list after @a pos.
         */
        void splice_after(const_iterator pos, Forward_list&& list,const_iterator i) noexcept {
            const_iterator  j = i;
            ++j;

            if (pos == i || pos == j) {
                return;
            }

            SinglyLinkedListNodeBase* tmp = const_cast<SinglyLinkedListNodeBase*>(pos.node);
            tmp->transfer_after(const_cast<SinglyLinkedListNodeBase*>(i.node),const_cast<SinglyLinkedListNodeBase*>(j.node));
        }

        void
        splice_after(const_iterator pos, Forward_list& list,
                     const_iterator i) noexcept
        { splice_after(pos, std::move(list), i); }

        /**
         *  @brief  Insert range from another %forward_list.
         *  @param  __pos  Iterator referencing the element to insert after.
         *  @param  __list  Source list.
         *  @param  __before  Iterator referencing before the start of range
         *                    in list.
         *  @param  __last  Iterator referencing the end of range in list.
         *
         *  Removes elements in the range (__before,__last) and inserts them
         *  after @a __pos in constant time.
         *
         *  Undefined if @a __pos is in (__before,__last).
         *  @{
         */
        void splice_after(const_iterator pos, Forward_list&&,
                     const_iterator before, const_iterator last) noexcept
        { my_splice_after(pos, before, last); }

        void
        splice_after(const_iterator pos, Forward_list&,
                     const_iterator before, const_iterator last) noexcept
        { my_splice_after(pos, before, last); }

        void reverse() noexcept {
            this->list.head.reverse_after();
        }

        // Operations
        /*
         * https://en.cppreference.com/w/cpp/container/forward_list/merge
         */
        void merge( Forward_list& other ) {
            merge(std::move(other));
        }
        void merge( Forward_list&& other ) {
            merge(std::move(other),std::less<T>());
        }
        template <class Compare>void merge( Forward_list& other, Compare comp ) {
            merge(std::move(other),comp);
        }
        template <class Compare>void merge( Forward_list&& other, Compare comp ) {
            SinglyLinkedListNodeBase* node = &this->list.head;
            while (node->next && other.list.head.next) {
                if (comp(*static_cast<Fwd_node<T>*>(other.list.head.next)->valptr(), *static_cast<Fwd_node<T>*>(node->next)->valptr() )) {
                    node->transfer_after(&other.list.head.next);
                    node = node->next;
                }
            }
            if (other.list.head.next) {
                *node = std::move(other.list.head);
            }
        }
        // TODO
        template<typename Pred>
        auto remove_if(Pred pred) -> size_t;
        auto remove(const T& val) -> size_t;
        template<class binPred>auto unique(binPred binaty_pred) ->size_t;
        template<class Comp>void sort(Comp comp);

        friend bool operator==(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            //  We don't have size() so we need to walk through both lists
            //  making sure both iterators are valid.
            auto ix = lhs.cbegin();
            auto iy = rhs.cbegin();
            while (ix != lhs.cend() && iy != rhs.cend())
            {
                if (!(*ix == *iy))
                    return false;
                ++ix;
                ++iy;
            }
            if (ix == lhs.cend() && iy == rhs.cend())
                return true;
            else
                return false;
        }
        friend bool operator!=(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            return !(lhs == rhs);
        }
        friend bool operator<(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            return std::lexicographical_compare(lhs.cbegin(),lhs.cend(),rhs.cbegin(),rhs.cend());
        }
        friend bool operator>(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            return rhs < lhs;
        }
        friend bool operator<=(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            return !(rhs < lhs);
        }
        friend bool operator>=(const Forward_list<T,Allocator>& lhs, const Forward_list<T,Allocator>& rhs) {
            return !(lhs < rhs);
        }
        friend void swap(Forward_list<T,Allocator>& lhs, Forward_list<T,Allocator>& rhs) {
            lhs.swap(rhs);
        }
    };
}
#endif //OURSTLV2_FORWARD_LIST_H

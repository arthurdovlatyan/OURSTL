//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTL_FORWARD_LIST_H
#define OURSTL_FORWARD_LIST_H
#include <bits/stl_iterator.h>
#include <ext/alloc_traits.h>
#include <ext/aligned_buffer.h>
#include <bits/stl_iterator_base_types.h>
#include <iterator>
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

    template<typename T, typename Allocator = std::allocator<T>>
    class Forward_list {
    public:
        Forward_list();
        explicit Forward_list(const Allocator& allocator);
        Forward_list(size_t count, const T& value, const Allocator& allocator = Allocator());
        explicit Forward_list(size_t count);
        explicit Forward_list(size_t count, const Allocator & alloc = Allocator());
        template<class _InputIterator, typename = std::_RequireInputIter<_InputIterator>> Forward_list(_InputIterator first, _InputIterator last, const Allocator& allocator = Allocator());
        Forward_list(const Forward_list<T>& other,const Allocator& allocator = Allocator());
        Forward_list(const Forward_list<T>& other);
        Forward_list(Forward_list<T>&& other);
        Forward_list(Forward_list<T>&& other,const Allocator& allocator = Allocator());
        Forward_list(std::initializer_list<T> initializerList, const Allocator& allocator = Allocator());

        ~Forward_list();


    };
}
#endif //OURSTLV2_FORWARD_LIST_H

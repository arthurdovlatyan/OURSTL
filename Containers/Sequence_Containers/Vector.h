//
// Created by sargiskhachatryan on 3/18/21.
//

#ifndef OURSTLV2_VECTOR_H
#define OURSTLV2_VECTOR_H

#include <memory>
#include <algorithm>
#include <limits>
#include <cstddef>

namespace OurSTL {
    template<typename T, typename Alloc>
    class Vector;

    template<typename T, typename Alloc>
    bool operator==(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc>
    bool operator!=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc>
    bool operator<(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc>
    bool operator<=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc>
    bool operator>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc>
    bool operator>=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

    template<typename T, typename Alloc = std::allocator<T> >
    class Vector {
        friend bool operator==<T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        friend bool operator!=<T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        friend bool operator< <T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        friend bool operator<=<T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        friend bool operator><T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        friend bool operator>=<T, Alloc>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

        class reverse_iterator;

        class const_reverse_iterator;

    public:
        typedef T value_type;
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef reverse_iterator reverse_iterator;
        typedef const_reverse_iterator const_reverse_iterator;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Alloc allocator_type;

    private:
        class reverse_iterator {
        public:
            explicit reverse_iterator(iterator it = nullptr) : current_(it) {}

            iterator base() const { return current_; }

            reverse_iterator &operator++() {
                --current_;
                return *this;
            }

            reverse_iterator operator++(int) {
                reverse_iterator temp(*this);
                --current_;
                return temp;
            }

            reverse_iterator &operator--() {
                ++current_;
                return *this;
            }

            reverse_iterator operator--(int) {
                reverse_iterator temp(*this);
                ++current_;
                return temp;
            }

            reference operator*() { return *(current_ - 1); }

            const_reference operator*() const { return *(current_ - 1); }

            pointer operator->() { return current_ - 1; }

            const_pointer operator->() const { return current_ - 1; }

            friend bool operator==(reverse_iterator i, reverse_iterator j) { return i.current_ == j.current_; }

            friend bool operator!=(reverse_iterator i, reverse_iterator j) { return i.current_ != j.current_; }

            friend difference_type operator-(reverse_iterator i, reverse_iterator j) { return i.current_ - j.current_; }

        private:
            iterator current_;
        };

        class const_reverse_iterator {
        public:
            explicit const_reverse_iterator(const_iterator it = nullptr) : current_(it) {}

            const_reverse_iterator(reverse_iterator it) : current_(it.base()) {}

            const_iterator base() const { return current_; }

            const_reverse_iterator &operator++() {
                --current_;
                return *this;
            }

            const const_reverse_iterator operator++(int) {
                const_reverse_iterator temp(*this);
                --current_;
                return temp;
            }

            const_reverse_iterator &operator--() {
                ++current_;
                return *this;
            }

            const_reverse_iterator operator--(int) {
                const_reverse_iterator temp(*this);
                ++current_;
                return temp;
            }

            const_reference operator*() const { return *(current_ - 1); }

            const_pointer operator->() const { return current_ - 1; }

            friend bool operator==(const_reverse_iterator i, const_reverse_iterator j) {
                return i.current_ == j.current_;
            }

            friend bool operator!=(const_reverse_iterator i, const_reverse_iterator j) {
                return i.current_ != j.current_;
            }

            friend difference_type operator-(const_reverse_iterator i, const_reverse_iterator j) {
                return i.current_ - j.current_;
            }

        private:
            const_iterator current_;
        };

    public:

        Vector() {
            create();
        }

        explicit Vector(size_type n, const value_type &val = value_type()) {
            create(n, val);
        }

        template<typename In>
        Vector(In i, In j) {
            create(i, j);
        }

        Vector(const Vector &v) {
            create(v.begin(), v.end());
        }

        Vector &operator=(const Vector &v);

        ~Vector() {
            uncreate();
        }

        template<typename In>
        void assign(In i, In j) {
            uncreate();
            create(i, j);
        }

        void assign(size_type n, const T &val) {
            uncreate();
            create(n, val);
        }

        reference operator[](size_type n) {
            return data_[n];
        }

        const_reference operator[](size_type n) const {
            return data_[n];
        }

        reference at(size_type n) {
            return data_[n];
        }

        const_reference at(size_type n) const {
            return data_[n];
        }

        reference front() {
            return *begin();
        }

        reference back() {
            return *rbegin();
        }

        const_reference front() const {
            return *begin();
        }

        const_reference back() const {
            return *rbegin();
        }

        void push_back(const T &t) {
            if (avail_ == limit_) {
                grow();
            }
            unCheckedAppend(t);
        }

        void pop_back() {
            alloc_.destroy(--avail_);
        }

        void swap(Vector &other) {
            std::swap(data_, other.data_);
            std::swap(avail_, other.avail_);
            std::swap(limit_, other.limit_);
        }

        iterator insert(iterator position, const value_type &val);

        void insert(iterator position, size_type n, const value_type &val);

        template<typename InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);

        iterator erase(iterator position);

        iterator erase(iterator first, iterator last);

        void resize(size_type n, value_type val = value_type());

        void reserve(size_type n);

        bool empty() const {
            return data_ == avail_;
        }

        size_type size() const {
            return avail_ - data_;
        }

        size_type capacity() const {
            return limit_ - data_;
        }

        size_type max_size() const {
            return std::numeric_limits<size_type>::max() / sizeof(T);
        }

        iterator begin() {
            return data_;
        }

        iterator end() {
            return avail_;
        }

        const_iterator begin() const {
            return data_;
        }

        const_iterator end() const {
            return avail_;
        }

        reverse_iterator rbegin() {
            return reverse_iterator(avail_);
        }

        reverse_iterator rend() {
            return reverse_iterator(data_);
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(avail_);
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(data_);
        }

        allocator_type get_allocator() const {
            return alloc_;
        }

    private:
        iterator data_;
        iterator avail_;
        iterator limit_;

        std::allocator<T> alloc_;

        void create();

        void create(size_type, const value_type &);

        template<typename In>
        void create(In, In);

        void uncreate();

        void grow();

        void unCheckedAppend(const value_type &);

        void growToN(size_type n);
    };

    template<typename T, typename Alloc>
    Vector<T, Alloc> &Vector<T, Alloc>::operator=(const Vector &rhs) {
        if (this != &rhs) {
            uncreate();
            create(rhs.begin(), rhs.end());
        }
        return *this;
    }


    template<typename T, typename Alloc>
    void Vector<T, Alloc>::create() {
        data_ = avail_ = limit_ = nullptr;
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::create(size_type n, const value_type &val) {
        data_ = alloc_.allocate(n);
        std::uninitialized_fill(data_, data_ + n, val);
        avail_ = limit_ = data_ + n;
    }


    template<typename T, typename Alloc>
    template<typename In>
    void Vector<T, Alloc>::create(In i, In j) {
        data_ = alloc_.allocate(j - i);
        avail_ = limit_ = std::uninitialized_copy(i, j, data_);
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::uncreate() {
        if (data_) {
            iterator it(avail_);
            while (it != data_) {
                alloc_.destroy(--it);
            }
        }
        alloc_.deallocate(data_, limit_ - data_);
        data_ = limit_ = avail_ = nullptr;
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::grow() {
        size_type new_size = std::max(2 * (limit_ - data_), difference_type(1));
        growToN(new_size);
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::unCheckedAppend(const value_type &val) {
        alloc_.construct(avail_++, val);
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::growToN(size_type n) {
        iterator new_data = alloc_.allocate(n);
        iterator new_avail = std::uninitialized_copy(data_, avail_, new_data);
        uncreate();
        data_ = new_data;
        avail_ = new_avail;
        limit_ = data_ + n;
    }


    template<typename T, typename Alloc>
    typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(iterator position, const value_type &val) {
        difference_type pos = position - data_;
        insert(position, 1, val);
        return position + pos;
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::insert(iterator position, size_type n, const value_type &val) {
        difference_type pos = position - data_; //防止position失效
        while (static_cast<size_type>(limit_ - avail_) < n) {
            grow();
        }
        position = data_ + pos;
        size_type left = avail_ - position;
        if (n < left) {
            size_type len = avail_ - position;
            size_type len_copy = len - n;
            std::uninitialized_copy(position + len_copy, avail_, avail_);
            std::copy_backward(position, position + len_copy, avail_);
            std::fill_n(position, n, val);
        } else if (n > left) {
            std::uninitialized_copy(position, avail_, position + n);
            std::fill_n(position, avail_ - position, val);
            std::uninitialized_fill(avail_, position + n, val);
        } else {
            std::uninitialized_copy(position, avail_, avail_);
            std::fill_n(position, n, val);
        }
        avail_ = avail_ + n;
    }

    template<typename T, typename Alloc>
    template<typename InputIterator>
    void Vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        difference_type pos = position - data_;
        size_type n = last - first;
        while (static_cast<size_type>(limit_ - avail_) < n) {
            grow();
        }
        position = data_ + pos;
        //std::copy(first, last, position);

        size_type left = avail_ - position;
        if (n < left) {
            size_type len = avail_ - position;
            size_type len_copy = len - n;
            std::uninitialized_copy(position + len_copy, avail_, avail_);
            std::copy_backward(position, position + len_copy, avail_);
            std::copy(first, last, position);
        } else if (n > left) {
            std::uninitialized_copy(position, avail_, position + n);

            //std::fill_n(position, avail_ - position, val);
            //std::uninitialized_fill(avail_, position + n, val);

            std::copy(first, first + left, position);
            std::uninitialized_copy(first + left, last, avail_);
        } else {
            std::uninitialized_copy(position, avail_, avail_);
            //std::fill_n(position, avail_ - position, val);
            std::copy(first, last, position);
        }

        avail_ = avail_ + n;
    }

    template<typename T, typename Alloc>
    typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(iterator position) {
        //[position + 1, avail_)
        std::copy(position + 1, avail_, position);
        alloc_.destroy(--avail_);
        return position;
    }

    template<typename T, typename Alloc>
    typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(iterator first, iterator last) {
        difference_type left = avail_ - last;
        //first last avail
        std::copy(last, avail_, first);
        iterator it(first + left);
        while (avail_ != it) {
            alloc_.destroy(--avail_);
        }
        return first;
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::resize(size_type n, value_type val) {
        size_type current_size = size();
        if (n < current_size) {
            size_type diff = current_size - n;
            while (diff--) {
                alloc_.destroy(--avail_); //pop_back()
            }
        } else if (n > current_size) {
            size_type diff = n - current_size;
            auto left = static_cast<size_type>(limit_ - avail_);
            if (left < diff) {
                growToN(n);
            }

            while (size() < n) {
                unCheckedAppend(val);
            }
        }
    }

    template<typename T, typename Alloc>
    void Vector<T, Alloc>::reserve(size_type n) {
        size_type current_capacity = capacity();
        if (n > current_capacity) {
            growToN(n);
        }
    }

    template<typename T, typename Alloc>
    bool operator==(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        return lhs.size() == rhs.size() &&
               std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<typename T, typename Alloc>
    bool operator!=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        return !(lhs == rhs);
    }

    template<typename T, typename Alloc>
    bool operator<(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        typedef typename Vector<T, Alloc>::size_type size_type;
        size_type size1 = lhs.size();
        size_type size2 = rhs.size();
        size_type min_size = (size1 < size2) ? size1 : size2;
        size_type ix = 0;
        for (; ix != min_size; ++ix) {
            if (lhs[ix] < rhs[ix]) {
                return true;
            } else if (lhs[ix] > rhs[ix]) {
                return false;
            }
        }
        if (ix != size2) {
            return true;
        }
        return false;
    }

    template<typename T, typename Alloc>
    bool operator<=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        return !(rhs < lhs);        //lhs <= rhs
    }

    template<typename T, typename Alloc>
    bool operator>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        return rhs < lhs; //lhs > rhs
    }

    template<typename T, typename Alloc>
    bool operator>=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs) {
        return !(lhs < rhs);
    }
} // namespace OurSTL
#endif //OURSTLV2_VECTOR_H

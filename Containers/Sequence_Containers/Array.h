//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTL_ARRAY_H
#define OURSTL_ARRAY_H

#include <cstdlib>
#include <initializer_list>
#include <stdexcept>
#include <exception>


namespace OurSTL {
    template<typename T,std::size_t N>
    class Array {
    private:
        /*
         * Array of type T
         * With Size N
         */
        T array[N];
        /*
         * std::size_t type object hol
         */
        std::size_t Size;
    public:
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // Constructors
        /*
         * Creates an array of type T and size N
         * All elements are empty
         * size is initialized to 0
         */
        constexpr Array() : Size(0) {}

        /*
         * Support for aggregate initialization
         * Creates an array of type T and Size N
         * All elements are set to the elements of il
         */
        constexpr Array(const std::initializer_list<T>& il) {
            if (il.size() > N) {
                throw std::logic_error("Invalid initializerList");
            }
            Size = 0;
            for (auto i = il.begin(); i != il.end(); ++i) {
                array[Size++] = *i;
            }
        }

        /*
         * Creates a copy of the array1
         */
        constexpr Array(Array<T,N>& array1) : Size(array1.size()) {
            for (int i = 0; i < array1.size(); ++i) {
                array[i] = array1.array[i];
            }
        }

        /*
         * Creates a copy of the array1
         */
        constexpr Array& operator=(Array<T,N>& array1) {
            if (*this == array1) {
                return *this;
            }
            Size = array1.size();
            for (int i = 0; i < array1.size(); ++i) {
                array[i] = array1.array[i];
            }
            return *this;
        }

        /*
         * Moves the array1 rv
         */
        constexpr Array(Array<T,N>&& array1) : Size(array1.size()) {
            for (int i = 0; i < array1.size(); ++i) {
                array[i] = array1.array[i];
            }
        }

        /*
         * Moves the array1 rv
         */
        constexpr Array& operator=(Array<T,N>&& array1) {
            if (*this == array1) {
                return *this;
            }
            Size = array1.size();
            for (int i = 0; i < array1.size(); ++i) {
                array[i] = array1.array[i];
            }
        }

        ~Array() = default;

        // Element Access
        /*
         * Parameters
        *  pos	-	position of the element to return
        * Return value
         * Reference to the requested element.
         * Exceptions
         * std::out_of_range if !(pos < size()).
         * Complexity
         * Constant
         */
        constexpr T& at(std::size_t pos) {
            if (pos < 0 || pos > Size) {
                throw std::out_of_range("Invalid index");
            }
            return array[pos];
        }
        constexpr const T& at(std::size_t pos) const {
            if (pos < 0 || pos > Size) {
                throw std::out_of_range("Invalid index");
            }
            return array[pos];
        }

        /*
         *Parameters
         * pos	-	position of the element to return
         * Return value
         * Reference to the requested element.
         * Complexity
         * Constant.
         * Returns a reference to the element at specified location pos. No bounds checking is performed.
         */
        constexpr T& operator[](std::size_t pos ) {
            return array[pos];
        }
        constexpr const T& operator[](std::size_t pos) const {
            return array[pos];
        }

        /*
         *Parameters
         * (none)
         * Return value
         * reference to the first element
         * Complexity
         * Constant
         * Notes
         * For a container c, the expression c.front() is equivalent to *c.begin().
         * Returns a reference to the first element in the container.
         * Calling front on an empty container causes undefined behavior.
         */
        constexpr T& front() {
            return *begin();
        }
        constexpr const T& front() const {
            return *cbegin();
        }

        /*
         * Parameters
         * (none)
         * Return value
         * Reference to the last element.
         * Complexity
         * Constant.
         * Notes
         * For a non-empty container c, the expression c.back() is equivalent to *std::prev(c.end())
         * Returns a reference to the last element in the container.
         * Calling back on an empty container causes undefined behavior.
         */
        constexpr T& back() {
            return N ? *(end() - 1) : *end();
        }
        constexpr const T& back() const {
            return N ? *(cend() - 1) : *cend();
        }

        /*
         *Parameters
         * (none)
         * Return value
         * Pointer to the underlying element storage. For non-empty containers, the returned pointer compares equal to the address of the first element.
         * Complexity
         * Constant.
         * Returns pointer to the underlying array serving as element storage.
         * The pointer is such that range [data(); data() + size()) is always a valid range,
         * even if the container is empty (data() is not dereferenceable in that case).
         */
        constexpr T* data() noexcept {
            return array;
        }
        constexpr const T* data() const noexcept {
            return array;
        }


        // Iterators
        /*
         * Parameters
         * (none)
         * Return value
         * Iterator to the first element.
         * Complexity
         * Constant.
         */
        constexpr T* begin() noexcept {
            return iterator(array);
        }
        constexpr const T* begin() const noexcept {
            return const_iterator(array);
        }
        constexpr const T* cbegin() const noexcept {
            return const_iterator(array);
        }

        /*
         * Parameter
         * (none)
         * Return value
         * Iterator to the element following the last element.
         * Complexity
         * Constant.
         */
        constexpr T* end() noexcept {
            return iterator(array + Size);
        }
        constexpr const T* end() const noexcept {
            return const_iterator(array + Size);
        }
        constexpr const T* cend() const noexcept {
            return const_iterator(array + Size);
        }

        /*
         * Parameter
         * (none)
         * Return value
         * Iterator to the element following the last element.
         * Complexity
         * Constant.
         */
        constexpr T* rbegin() noexcept {
            return reverse_iterator(end());
        }
        constexpr const T* rbegin() const noexcept {
            return const_reverse_iterator(end());
        }
        constexpr const T* crbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        /*
         * Parameter
         * (none)
         * Return value
         * Iterator to the element following the last element.
         * Complexity
         * Constant.
         */
        constexpr T* rend() noexcept {
            return reverse_iterator(begin());
        }
        constexpr const T* rend() const noexcept {
            return const_reverse_iterator(cbegin());
        }
        constexpr const T* crend() const noexcept {
            return const_reverse_iterator(cbegin());
        }


        // Capacity
        /*
         *Parameters
         * (none)
         * Return value
         * true if the container is empty, false otherwise
         * Complexity
         * Constant.
         */
        [[nodiscard]] constexpr bool empty() const noexcept {
            return Size == 0;
        }

        /*
         *Parameters
         * (none)
         * Return value
         * The number of elements in the container.
         * Complexity
         * Constant.
         */
        constexpr size_t size() const noexcept {
            return Size;
        }

        /*
         *Parameters
         * (none)
         * Return value
         * Maximum number of elements.
         * Complexity
         * Constant.
         */
        constexpr size_t max_size() const noexcept {
            return N;
        }


        // Operations
        /*
         *Parameters
         * value	-	the value to assign to the elements
         * Return value
         * (none)
         * Complexity
         * Linear in the size of the container.
         */
        constexpr void fill(const T& value) {
            std::fill_n(begin(),size(),value);
        }

        /*
         *Parameters
         * other	-	container to exchange the contents with
         * Return value
         * (none)
         */
        constexpr void swap( Array<T,N> & other) noexcept(std::is_nothrow_swappable_v<T>) {
            std::swap_ranges(begin(),end(),other.begin());
        }


        // Non-Member functions
        /*
         * Parameters
         * lhs, rhs	-	arrays whose contents to compare
         * -T must meet the requirements of EqualityComparable in order to use overloads (1-2).
         * -T must meet the requirements of LessThanComparable in order to use overloads (3-6).
         * The ordering relation must establish total order.
         * Complexity
         * Linear in the size of the array
         */
        friend bool operator==(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return std::equal(lhs.begin(),lhs.end(),lhs.end());
        }
        friend bool operator!=(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return !(lhs == rhs);
        }
        friend bool operator<(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
        }
        friend bool operator>(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return rhs < lhs;
        }
        friend bool operator<=(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return !(lhs > rhs);
        }
        friend bool operator>=(const OurSTL::Array<T,N> &lhs,const OurSTL::Array<T,N> &rhs) {
            return !(lhs < rhs);
        }




        /*
         *Parameters
         * lhs, rhs	-	containers whose contents to swap
         * Return value
         * (none)
         * Complexity
         * Linear in size of the container
         */
        friend constexpr void swap(OurSTL::Array<T,N> &lhs, OurSTL::Array<T,N> & rhs) noexcept {
            lhs.swap(rhs);
        }

        /*
         *Parameters
         * a	-	the built-in array to be converted the std::array
         * Type requirements
         * -T must meet the requirements of CopyConstructible in order to use overload (1).
         * -T must meet the requirements of MoveConstructible in order to use overload (2).
         * Return value
         * 1) std::array<std::remove_cv_t<T>, N>{ a[0], ..., a[N - 1] }
         * 2) std::array<std::remove_cv_t<T>, N>{ std::move(a[0]), ..., std::move(a[N - 1]) }
         */
        friend constexpr OurSTL::Array<std::remove_cv_t<T>,N> to_array(T (&a) [N]);
        friend constexpr OurSTL::Array<std::remove_cv_t<T>,N> to_array(T (&&a) [N]);
    };
//TODO
//    /*
//     * Extracts the Ith element element from the array.I must be an integer value in range [0, N).
//     * This is enforced at compile time as opposed to at() or operator[].
//     * Parameters
//     * a	-	array whose contents to extract
//     * Return value
//     * A reference to the Ith element of a.
//     * Complexity
//     * Constant.
//     */
//    template< std::size_t I, class T, std::size_t N >
//    constexpr T& get( OurSTL::Array<T,N>& a ) noexcept {
//        if ()
//    }
//    template< std::size_t I, class T, std::size_t N >
//    constexpr T&& get( OurSTL::Array<T,N>&& a ) noexcept {
//
//    }
//    template< std::size_t I, class T, std::size_t N >
//    constexpr const T& get( const OurSTL::Array<T,N>& a ) noexcept {
//
//    }
//    template< std::size_t I, class T, std::size_t N >
//    constexpr const T&& get( const OurSTL::Array<T,N>&& a ) noexcept {
//
//    }
}

#endif //OURSTLV2_ARRAY_H

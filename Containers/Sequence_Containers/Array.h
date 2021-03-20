//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTLV2_ARRAY_H
#define OURSTLV2_ARRAY_H

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
        std::size_t size;
    public:
        // Constructors
        /*
         * Creates an array of type T and size N
         * All elements are empty
         * size is initialized to 0
         */
        constexpr Array();

        /*
         * Support for aggregate initialization
         * Creates an array of type T and Size N
         * All elements are set to the elements of il
         */
        constexpr Array(const std::initializer_list<T> &il);

        /*
         * Creates a copy of the array1
         */
        constexpr Array(Array<T,N> &array1);

        /*
         * Creates a copy of the array1
         */
        constexpr Array & operator=(Array<T,N> &array1);

        /*
         * Moves the array1 rv
         */
        constexpr Array(Array<T,N> &&array1);

        /*
         * Moves the array1 rv
         */
        constexpr Array & operator=(Array<T,N> &&array1);


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
        constexpr T& at(std::size_t pos );
        constexpr const T& at(std::size_t pos) const;

        /*
         *Parameters
         * pos	-	position of the element to return
         * Return value
         * Reference to the requested element.
         * Complexity
         * Constant.
         * Returns a reference to the element at specified location pos. No bounds checking is performed.
         */
        constexpr T& operator[](std::size_t pos );
        constexpr const T& operator[](std::size_t pos) const;

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
        constexpr T& front();
        constexpr const T& front() const;

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
        constexpr T& back();
        constexpr const T& back() const;

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
        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        // Iterators
        constexpr T* begin() noexcept;
        constexpr const T* begin() const noexcept;
        constexpr const

        // Capacity

        // Operators


        // Non-Member functions


    };
}

#endif //OURSTLV2_ARRAY_H

//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTLV2_VECTOR_H
#define OURSTLV2_VECTOR_H

#include <initializer_list>

namespace OurSTL {

    template<typename T>
    class vector {
    private:
        /**
         * Represents current size of a container
         */
        std::size_t _size_capacity;

        /**
         * Represents number of elements currently in container
         */
        std::size_t _size_elements;

        /**
         * Pointer to the beginning of an container.
         */
        T *_begin;

        /**
         * Pointer to the last element in the sequence.
         */
        T *_last_element;

        /**
         * Pointer to the past the end of the container.
         */
        T *_end;


    public:
        /**
         * @brief Constructs empty container
         * @note When default constructed size is initialized to 0
         */
        vector();

        /**
         * @brief Constructs sequential container without any elements.
         * @param size represents number of elements of type T
         * which should be reserved for the future.
         */
        constexpr explicit vector(const std::size_t &size);

        /**
         * @brief Constructs a sequential container.
         * @param il represents elements to be added into the container.
         */
        constexpr vector(const std::initializer_list<T> &il);

        /**
         * @brief Constructs sequential container using iterators.
         * @param begin
         * @param end
         * iterators of a given type
         */
        template<typename IT>
        constexpr vector(const IT begin, const IT end);

    public:
        /**
         * @brief Copy constructor
         */
        constexpr vector(const vector &rhs);

        /**
         * @brief Move constructor
         */
        constexpr vector(const vector &&rhs);

        /**
         * @brief Copy assignment operator
         */
        constexpr vector &operator=(const vector &rhs);

        /**
         * @brief Move assignment operator
         */
        constexpr vector &operator=(vector &&rhs) noexcept;

        /**
         * @brief Destructor
         */
        constexpr ~vector();

    public:
        /**
         * @brief Access specified element with bounds checking.
         * @return reference to the element under index if is valid
         * nullptr if out of bound.
         */
        constexpr T& at(const std::size_t &index);

        /**
         * @brief Access specified element with bounds checking.
         * @return const_reference to the element under index if is valid
         * nullptr if out of bound.
         */
        const T& at(const std::size_t &index) const;

        /**
         * @brief Access specified element without bounds checking.
         * @return reference to the element under index
         */
        constexpr T& operator[](const std::size_t &index);

        /**
         * @brief Access specified element without bounds checking.
         * @return const_reference to the element under index
         */
        const T& operator[](const std::size_t &index) const;

    };

} // namespace OurSTL
#endif //OURSTLV2_VECTOR_H

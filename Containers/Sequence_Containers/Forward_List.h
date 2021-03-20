//
// Created by arthurdovlat on 3/18/21.
//

#ifndef OURSTL_FORWARD_LIST_H
#define OURSTL_FORWARD_LIST_H

namespace OurSTL {

    template<typename T>
    class Singly_Linked_Node {
    private:
        T element;
    public:

    };

    template<typename T, typename Allocator = std::allocator<T>>
    class Forward_list {

    };
    namespace pmr {
        template<typename T>
        using Forward_list = OurSTL::Forward_list<T, std::pmr::polymorphic_allocator<T>>;
    }
}
#endif //OURSTLV2_FORWARD_LIST_H

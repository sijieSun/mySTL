#ifndef MYSTL_CONSTRUCT_H
#define MYSTL_CONSTRUCT_H

//  construct and destroy

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif


namespace mySTL
{
    // constructor
    template <class Ty>
    void construct(Ty* ptr)
    {
        ::new ((void*)ptr) Ty();
    }

    template <class Ty1, class Ty2>
    void construct(Ty1* ptr, const Ty2& value)
    {
        ::new ((void*)ptr Ty1(value));
    }

    template <class Ty, class... Args>
    void construct(Ty* ptr, Args&&... args)
    {
        ::new ((void*)ptr) Ty(mySTL::forward<Args>(args)...);
    }

    //  destroy
    template <class Ty>
    void destroy_one(Ty*, m_intergral_constant<bool, true>) {}

    template <class Ty>
    void destroy_one(Ty* pointer, m_intergral_constant<bool ,false>)
    {
        if (pointer != nullptr)
        {
            pointer->~Ty();
        }
    }

    template <class Ty>
    void destroy(Ty* pointer)
    {
        destroy_one(pointer, std::is_trivially_destructible<Ty>{});
    }


    template <class ForwardIter>
    void destroy_cat(ForwardIter , ForwardIter , m_intergral_constant<bool, true>)  {}

    template <class ForwardIter>
    void destroy_cat(ForwardIter first, ForwardIter last, m_intergral_constant<bool, false>)
    {
        for (; first != last; ++first)
        {
            destroy(&*first);
        }
    }

    template <class ForwardIter>
    void destroy(ForwardIter first, ForwardIter last)
    {
        destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIter>::value_type>{});
    }

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif 

#endif
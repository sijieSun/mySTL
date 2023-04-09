#ifndef MYSTL_UNINITIALIZED_H_
#define MYSTL_UNINITIALIZED_H_

//  对未初始化空间构造元素

#include "algobase.h"
#include "../allocator/construct.h"
// #include "iterator.h"
// #include "type_traits.h"
// #include "util.h"


namespace mySTL
{
    /* uninitialized_copy */

    //已初始化
    template <class InputIter, class ForwardIter>
    ForwardIter
    unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, m_intergral_constant<bool, true>)
    {
        return mySTL::copy(first, last, result);
    }

    template <class InputIter, class ForwardIter>
    ForwardIter
    unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, m_intergral_constant<bool, false>)
    {
        auto cur = result;
        try
        {
            for (; first != last; ++cur)
            {
                mySTL::construct(&*cur, *first);
            }
        }
        catch (...)
        {
            for (; result != cur; --cur)
            {
                mySTL::destroy(&*cur);
            }
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
    {
        return unchecked_uninit_copy(first, last, result, 
                                    std::is_trivially_copy_assignable<
                                    typename iterator_traits<ForwardIter>::
                                    value_type>{});
    }

    /* uninitialized_copy_n */
    template <class InputIter, class Size, class ForwardIter>
    ForwardIter 
    unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, m_intergral_constant<bool, true>)
    {
        return mySTL::copy_n(first, n, result).second;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, m_intergral_constant<bool, false>)
        {
        auto cur = result;
        try
        {
            for (; n > 0; --n, ++cur, ++first)
            {
                mySTL::construct(&*cur, *first);
            }
        }
        catch (...)
        {
            for (; result != cur; --cur)
            mySTL::destroy(&*cur);
        }
        return cur;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result)
    {
        return mySTL::unchecked_uninit_copy_n(first, n, result,
                                            std::is_trivially_copy_assignable<
                                            typename iterator_traits<InputIter>::
                                            value_type>{});
    }

    /* uninitialized_fill */
    template <class ForwardIter, class T>
    void 
    unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, m_intergral_constant<bool, true>)
    {
        mySTL::fill(first, last, value);
    }

    template <class ForwardIter, class T>
    void 
    unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, m_intergral_constant<bool, false>)
    {
        auto cur = first;
        try
        {
            for (; cur != last; ++cur)
            {
            mySTL::construct(&*cur, value);
            }
        }
        catch (...)
        {
            for (;first != cur; ++first)
            mySTL::destroy(&*first);
        }
    }

    template <class ForwardIter, class T>
    void  uninitialized_fill(ForwardIter first, ForwardIter last, const T& value)
    {
        mySTL::unchecked_uninit_fill(first, last, value, 
                                    std::is_trivially_copy_assignable<
                                    typename iterator_traits<ForwardIter>::
                                    value_type>{});
    }

    /* uninitialized_fill_n */
    template <class ForwardIter, class Size, class T>
    ForwardIter 
    unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, m_intergral_constant<bool, true>)
    {
        return mySTL::fill(first, n, value);
    }

    template <class ForwardIter, class Size, class T>
    ForwardIter 
    unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, m_intergral_constant<bool, false>)
    {
        auto cur = first;
        try
        {
            for (; n > 0; --n, ++cur)
            {
                mySTL::construct(&*cur, value);
            }
        }
        catch (...)
        {
            for (; first != cur; ++first)
                mySTL::destroy(&*first);
        }
        return cur;
    }

    template <class ForwardIter, class Size, class T>
    ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value)
    {
        return mySTL::unchecked_uninit_fill_n(first, n, value, 
                                            std::is_trivially_copy_assignable<
                                            typename iterator_traits<ForwardIter>::
                                            value_type>{});
    }

    /* uninitialized_move */
    template <class InputIter, class ForwardIter>
    ForwardIter 
    unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, m_intergral_constant<bool, true>)
    {
        return mySTL::move(first, last, result);
    }

    template <class InputIter, class ForwardIter>
    ForwardIter 
    unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, m_intergral_constant<bool, false>)
    {
        ForwardIter cur = result;
        try
        {
            for (; first != last; ++first, ++cur)
            {
                mySTL::construct(&*cur, mySTL::move(*first));
            }
        }
        catch (...)
        {
            mySTL::destroy(result, cur);
        }
        return cur;
    }

    template <class InputIter, class ForwardIter>
    ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
    {
        return mySTL::unchecked_uninit_move(first, last, result,
                                        std::is_trivially_move_assignable<
                                        typename iterator_traits<InputIter>::
                                        value_type>{});
    }

    /* uninitialized_move_n */
    template <class InputIter, class Size, class ForwardIter>
    ForwardIter 
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, m_intergral_constant<bool, true>)
    {
        return mySTL::move(first, first + n, result);
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter
    unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, m_intergral_constant<bool, false>)
    {
    auto cur = result;
    try
    {
        for (; n > 0; --n, ++first, ++cur)
        {
            mySTL::construct(&*cur, mySTL::move(*first));
        }
    }
    catch (...)
    {
        for (; result != cur; ++result)
            mySTL::destroy(&*result);
        throw;
    }
    return cur;
    }

    template <class InputIter, class Size, class ForwardIter>
    ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
    {
        return mySTL::unchecked_uninit_move_n(first, n, result,
                                            std::is_trivially_move_assignable<
                                            typename iterator_traits<InputIter>::
                                            value_type>{});
    }
}
#endif
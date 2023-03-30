#ifndef mySTL_ALGOBASE_H
#define mySTL_ALGOBASE_H

#include <cstring>

#include "./iterator/iterator.h"
#include "./util/util.h"

namespace mySTL
{
    #ifdef max
    #pragma message("#undefing marco max")
    #undef max
    #endif // max

    #ifdef min
    #pragma message("#undefing marco min")
    #undef min
    #endif // min

    //  max
    template <class T>
    const T& max(const T& lhs, const T& rhs)
    {
        return lhs < rhs ? rhs : lhs;   //语义相等时返回第一个参数
    }

    //  overload max
    template <class T, class cmpare>
    const T& max(const T& lhs, const T& rhs, cmpare cmp)
    {
        return cmp(lhs, rhs) ? rhs : lhs;
    }

    //  min
    template <class T>
    const T& min(const T& lhs, const T& rhs)
    {
        return rhs < lhs ? rhs : lhs;   //语义相等时返回第一个参数
    }

    //  overload min
    template <class T, class cmpare>
    const T& min(const T& lhs, const T& rhs, cmpare cmp)
    {
        return cmp(rhs, lhs) ? rhs : lhs;
    }

    //  iter_swap
    template <class Iter1, class Iter2>
    void iter_swap(Iter1 lhs, Iter2 rhs)
    {
        mySTL::swap(*lhs, *rhs);
    }

    // copy  input_iterator版本
    template <class InputIter, class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first, InputIter last, OutputIter result, mySTL::input_iterator_tag)
    {
        for (; first != last; ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    //  cpoy  random_access_iterator_tag 版本
    template <class RandomIter, class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result, mySTL::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    //  整合 cat
    template <class Iter, class OutputIter>
    OutputIter unchecked_copy(Iter first, Iter last, OutputIter result)
    {
        return unchecked_copy_cat(first, last, result, iterator_category(first));
    }

    //  特化 trivially_copy_assignable
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
    unchecked_copy(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + n;
    }

    //  整合 unchecked
    template <class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_copy(first, last, result);
    }

    //  copy_backward BidirectionalIter
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                BidirectionalIter2 result, mySTL::bidirectional_iterator_tag)
    {
        while (first != last)
        {
            *--result = *--last;
        }
        return result;
    }

    //  copy_backward random_access_iterator_tag
    template <class RandomIter1, class BidirectionalIter2>
    BidirectionalIter2 
    unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                                BidirectionalIter2 result, mySTL::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n)
        {
            *--result = *--last;
        }
        return result;
    }

    //  整合 cat 
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                            BidirectionalIter2 result)
    {
        return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
    }

    //  特化 trivially_copy_assignable
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
        Up*>::type
    unchecked_copy_backward(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }
    
    //  整合所有
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return unchecked_copy_backward(first, last, result);
    }

    //  copy_if
    template <class InputIter, class OutputIter, class Unary>
    OutputIter
    copy_if(InputIter first, InputIter last, OutputIter result, Unary unary_pred)
    {
        for (; first != last; ++first)
        {
            if (unary_pred(*first))
            {
                *result++ = *first;
            }
        }
        return result;
    }

    //  copy_n  返回一个pair分别指向两个区间的尾部
    template <class InputIter, class Size, class OutputIter>
    mySTL::pair<InputIter, OutputIter>
    unchecked_copy_n(InputIter first, Size n, OutputIter result, mySTL::input_iterator_tag)
    {
        for (; n > 0; --n; ++first, ++result)
        {
            *result = *first;
        }
        return mySTL::pair<InputIter, OutputIter>(first, result);
    }

    template <class RandomIter, class Size, class OutputIter>
    mySTL::pair<RandomIter, OutputIter>
    unchecked_copy_n(RandomIter first, Size n, OutputIter result, 
                    mySTL::random_access_iterator_tag)
    {
        auto last = first + n;
        return mySTL::pair<RandomIter, OutputIter>(last, mySTL::copy(first, last, result));
    }

    template <class InputIter, class Size, class OutputIter>
    mySTL::pair<InputIter, OutputIter> 
    copy_n(InputIter first, Size n, OutputIter result)
    {
    return unchecked_copy_n(first, n, result, iterator_category(first));
    }

    //  move input_iterator_tag 版本
    template <class InputIter, class OutputIter>
    OutputIter 
    unchecked_move_cat(InputIter first, InputIter last, OutputIter result, mySTL::input_iterator_tag)
    {
        for (; first != last; ++first, ++result)
        {
            *result = mySTL::move(*first);
        }
        return result;
    }

    //  move random_access_iterator_tag 版本
    template <class RandomIter, class OutputIter>
    OutputIter 
    unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result, mySTL::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
        {
            *result = mySTL::move(*first);
        }
        return result;
    }

    //  整合cat
    template <class InputIter, class OutputIter>
    OutputIter 
    unchecked_move(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_move_cat(first, last, result, iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
    unchecked_move(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + n;
    }

    template <class InputIter, class OutputIter>
    OutputIter move(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_move(first, last, result);
    }

    //  move_backward bidirectional_iterator_tag版本
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
    unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                BidirectionalIter2 result, mySTL::bidirectional_iterator_tag)
    {
        while (first != last)
        {
            *--result = mySTL::move(*--last);
        }
        return result;
    }

    //  move_backward random_access_iterator_tag版本
    template <class RandomIter1, class RandomIter2>
    RandomIter2
    unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
                                RandomIter2 result, mySTL::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n)
        {
            *--result = mySTL::move(*--last);
        }
        return result;
    }

    //  整合cat
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
    unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last, 
                            BidirectionalIter2 result)
    {
        return unchecked_move_backward_cat(first, last, result, iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
    unchecked_move_backward(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }

    //  整合所有
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
    move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return unchecked_move_backward(first, last, result);
    }

    //  equal
    template <class InputIter1, class InputIter2>
    bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (*first1 != *first2)
            {
                return false;
            }
        }
        return true;
    }

    //  equal overload cmp
    template <class InputIter1, class InputIter2, class cmpared>
    bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, cmpared cmp)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!cmp(*first1, *first2))
            {
                return false;
            }
        }
        return true;
    }

    //  fill_n
    template <class OutputIter, class Size, class T>
    OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
    {
        for (; n > 0; --n, ++first)
        {
            *first = value;
        }
        return first;
    }

    //  one_byte 特化版
    template <class Tp, class Size, class Up>
    typename std::enable_if<
        std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
        !std::is_same<Tp, bool>::value &&
        std::is_integral<Up>::value && sizeof(Up) == 1,
        Tp*>::type
    unchecked_fill_n(Tp* first, Size n, Up value)
    {
        if (n > 0)
        {
            std::memset(first, (unsigned char)value, (size_t)(n));
        }
        return first + n;   
    }

    //  fill
    template <class ForwardIter, class T>
    void fill_cat(ForwardIter first, ForwardIter last, const T& value, mySTL::forward_iterator_tag)
    {
        for (; first != last; ++first)
        {
            *first = value;
        }
    }

    template <class RandomIter, class T>
    void fill_cat(RandomIter first, RandomIter last, const T& value,
                mySTL::random_access_iterator_tag)
    {
        fill_n(first, last - first, value);
    }

    template <class ForwardIter, class T>
    void fill(ForwardIter first, ForwardIter last, const T& value)
    {
        fill_cat(first, last, value, iterator_category(first));
    }

    //  以字典序对两个序列排序
    template <class InputIter1, class InputIter2>
    bool lexicographical_cmpare(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (*first1 < *first2)
            {
                return true;
            }
            if (*first2 < *first1)
            {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    //  overload cmp
    template <class InputIter1, class InputIter2, class compred>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2, compred cmp)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (cmp(*first1, *first2))
            {
                return true;
            }
            if (cmp(*first2, *first1))
            {
                return false;
            }
        }
        return first1 == last1 && first2 != last2;
    }

    //  const unsigned char* 特化
    bool lexicographical_compare(const unsigned char* first1,
                                const unsigned char* last1,
                                const unsigned char* first2,
                                const unsigned char* last2)
    {
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        // 先比较相同长度的部分
        const auto result = std::memcmp(first1, first2, mySTL::min(len1, len2));
        // 若相等，长度较长的比较大
        return result != 0 ? result < 0 : len1 < len2;
    }

    //  mismatch 返回一对迭代器，指向失配的元素  
    template <class InputIter1, class InputIter2>
    mySTL::pair<InputIter1, InputIter2> 
    mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
    {
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1, InputIter2>(first1, first2);
    }

    //  overload cmp
    template <class InputIter1, class InputIter2, class Compred>
    mySTL::pair<InputIter1, InputIter2> 
    mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
    {
        while (first1 != last1 && comp(*first1, *first2))
        {
            ++first1;
            ++first2;
        }
        return mystl::pair<InputIter1, InputIter2>(first1, first2);
    }
}

#endif
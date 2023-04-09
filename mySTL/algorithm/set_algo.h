#ifndef MYSTL_SET_ALGO_H_
#define MYSTL_SET_ALGO_H_

//  union, intersection, difference, symmetric_difference
//  默认有序

#include "algobase.h"
#include "../iterator/iterator.h"

namespace mySTL
{
    /* 
    set_union
    S1US2并保存到result，返回指向输出结果尾部的迭代器
     */

    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
                        InputIter2 first2, InputIter2 last2, OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
            }
            else if (*first1 > *first2)
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
            }
        }

        //  copy 剩余元素
        return mySTL::copy(first2, last2, mySTL::copy(first1, last1, result));
    }

    //  overload
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
                        InputIter2 first2, InputIter2 last2, 
                        OutputIter result, Compared cmp)
    {
    while (first1 != last1 && first2 != last2)
    {
        if (cmp(*first1, *first2))
        {
            *result = *first1;
            ++first1;
        }
        else if (cmp(*first2, *first1))
        {
            *result = *first2;
            ++first2;
        }
        else
        {
            *result = *first1;
            ++first1;
            ++first2;
        }
            ++result;
    }
        // 将剩余元素拷贝到 result
        return mySTL::copy(first2, last2, mySTL::copy(first1, last1, result));
    }

    /* 
    set_intersection
    计算交集，返回指向结果尾部的迭代器
     */

    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2, OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                ++first1;
            }
            else if (*first2 < *first1)
            {
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    //  overload
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
                                InputIter2 first2, InputIter2 last2,
                                OutputIter result, Compared cmp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (cmp(*first1, *first2))
            {
                ++first1;
            }
            else if (cmp(*first2, *first1))
            {
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    /* 
    set_difference
    S1-S2 -> result, return iterator->last
     */

    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2, OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1)
            {
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mySTL::copy(first1, last1, result);
    }

    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2, OutputIter result, Compared cmp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (cmp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (cmp(*first2, *first1))
            {
            ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mySTL::copy(first1, last1, result);
    }

    /* 
    set_symmetric_difference
    (S1-S2)U(S2-S1), return iterator->last
     */

    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2, 
                                        OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1)
            {
                *result = *first2;
                ++first2;
                ++result;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mySTL::copy(first2, last2, mySTL::copy(first1, last1, result));
    }

    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
                                        InputIter2 first2, InputIter2 last2,
                                        OutputIter result, Compared cmp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (cmp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (cmp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
                ++result;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mySTL::copy(first2, last2, mySTL::copy(first1, last1, result));
    }

}


#endif
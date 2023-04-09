#ifndef MYSTL_HEAP_ALGO_H
#define MYSTL_HEAP_ALGO_H

//  push_heap, pop_heap, sort_heap, make_heap

#include "../iterator/iterator.h"

namespace mySTL
{
    /* push_heap */
    
    template <class RandomIter, class Distance, class T>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value)
    {
        auto parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && *(first + parent) < value)
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Distance>
    void push_heap_d(RandomIter first, RandomIter last, Distance*)
    {
        mySTL::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
    }

    template <class RandomIter>
    void push_heap(RandomIter first, RandomIter last)
    {
        mySTL::push_heap_d(first, last, distance_type(first));
    }

    // 重载版本使用函数对象 cmp 代替比较操作
    template <class RandomIter, class Distance, class T, class Compared>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,
                    Compared cmp)
    {
        auto parent = (holeIndex - 1) / 2;
        while (holeIndex > topIndex && cmp(*(first + parent), value))
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Compared, class Distance>
    void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared cmp)
    {
        mySTL::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
                        *(last - 1), cmp);
    }

    template <class RandomIter, class Compared>
    void push_heap(RandomIter first, RandomIter last, Compared cmp)
    {
        mySTL::push_heap_d(first, last, distance_type(first), cmp);
    }

    /* pop_heap */

    template <class RandomIter, class T, class Distance>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
    {
        //  先percolate down
        auto topIndex = holeIndex;
        auto rchild = 2 * holeIndex + 2;
        while (rchild < len)
        {
            if (*(first + rchild) < *(first + rchild - 1))
            {
                --rchild;
            }
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * rchild + 2;
        }
        if (rchild == len){
            *(first + holeIndex) = *(first + rchild - 1);
            holeIndex = rchild - 1;
        }
        //  后percolate up
        push_heap_aux(first, holeIndex, topIndex, value);

    }

    template <class RandomIter, class T, class Distance>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*)
    {
        *result = *first;   //首结点调整到最后，再调整[first, last-1)
        adjust_heap(first, static_cast<Distance>(0), last - first, value);
    }

    template <class RandomIter>
    void pop_heap(RandomIter first, RandomIter last)
    {
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
    }

    // 重载版本使用函数对象 cmp 代替比较操作
    template <class RandomIter, class T, class Distance, class Compared>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
                    Compared cmp)
    {
        //  先 percolate down
        auto topIndex = holeIndex;
        auto rchild = 2 * holeIndex + 2;
        while (rchild < len)
        {
            if (cmp(*(first + rchild), *(first + rchild - 1)))
            {
                --rchild;
            }
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * rchild + 2;
        }
        if (rchild == len)
        {
            *(first + holeIndex) = *(first + rchild - 1);
            holeIndex = rchild - 1;
        }
        // 再 percolate up
        push_heap_aux(first, holeIndex, topIndex, value, cmp);
    }

    template <class RandomIter, class T, class Distance, class Compared>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*,
                     Compared cmp)
    {
        *result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
        adjust_heap(first, static_cast<Distance>(0), last - first, value, cmp);
    }

    template <class RandomIter, class Compared>
    void pop_heap(RandomIter first, RandomIter last, Compared cmp)
    {
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first), cmp);
    }

    /*  sort_heap
        不断pop_heap, 每次将最大元素放到尾部，直到容器最多只有一个元素
    */

    template <class RandomIter>
    void sort_heap(RandomIter first, RandomIter last)
    {
        while (last - first > 1)
        {
            pop_heap(first, last--);
        }
    }

    // 重载版本使用函数对象 cmp 代替比较操作
    template <class RandomIter, class Compared>
    void sort_heap(RandomIter first, RandomIter last, Compared cmp)
    {
        while (last - first > 1)
        {
            pop_heap(first, last--, cmp);
        }
    }

    /*  make_heap
        将first和last之间的数据变成一个堆
     */

    template <class RandomIter, class Distance>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*)
    {
        if (last - first < 2)
        {
            return;
        }
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true)
        {
            //  从非叶子节点开始adjust
            adjust_heap(first, holeIndex, len, *(first + holeIndex));
            if (holeIndex == 0)
            {
                return;
            }
            holeIndex--;
        }
    }

    template <class RandomIter>
    void make_heap(RandomIter first, RandomIter last)
    {
        make_heap_aux(first, last, distance(first));
    }

    // 重载版本使用函数对象 cmp 代替比较操作
    template <class RandomIter, class Distance, class Compared>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared cmp)
    {
        if (last - first < 2)
            return;
        auto len = last - first;
        auto holeIndex = (len - 2) / 2;
        while (true)
        {
            // 重排以 holeIndex 为首的子树
            adjust_heap(first, holeIndex, len, *(first + holeIndex), cmp);
            if (holeIndex == 0)
            {
                return;
            }
            holeIndex--;
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared cmp)
    {
        make_heap_aux(first, last, distance_type(first), cmp);
    }

}

#endif
#ifndef MYSTL_ITERATOR_H_
#define MYSTL_ITERATOR_H_

#include "stddef.h"
#include "type_traits.h"

namespace mySTL
{

    //五种类型迭代器
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    //iterator模板
    template <class Category, class Type, class Distance = ptrdiff_t, class Pointer = Type*, class Reference = Type&>
    struct iterator
    {
        typedef Category               iterator_category;
        typedef Type                   value_type;
        typedef Distance               difference_type;
        typedef Pointer                pointer;
        typedef Reference              reference;
    };

    //判断是否有迭代器类别iterator_category
    template <class T>
    struct has_iterator_cat
    {
    private:
        struct two  {   char a; char b; };
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);

    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };


    template <class Iterator, bool>
    struct iterator_traits_impl {};

    // bool如果为true则继续执行，false则不继续萃取
    template <class Iterator>
    struct iterator_traits_impl <Iterator, true>
    {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
        typedef typename Iterator::difference_type      difference_type;
    };


    template <class Iterator, bool>
    struct iterator_traits_helper   {};

    // bool如果为true则继承impl，false则停止。
    // Iterator::iterator_category是否可以隐式转换为input_iterator/output_iterator，如果可以则实现impl
    template <class Iterator>
    struct iterator_traits_helper <Iterator, true> : public iterator_traits_impl <Iterator,
    std::is_convertible <typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible <typename Iterator::iterator_category, output_iterator_tag>::value>
    {
    };

    //萃取迭代器
    template <class Iterator>
    struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

    //针对原生指针的偏特化
    template <class T>
    struct iterator_traits <T*>
    {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef ptrdiff_t                       difference_type;
    };

    template <class T>
    struct iterator_traits <const T*>
    {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef ptrdiff_t                       difference_type;
    };

    //T可以隐式转换为U
    template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of : public m_intergral_constant<bool, 
        std::is_convertible <typename iterator_traits<T>::iterator_category, U>::value>
    {
    };
    //T不可以隐式转换为U
    template <class T, class U>
    struct has_iterator_cat_of <T, U, false> : public m_false_type {};


    /* 萃取某种迭代器 */
    template <class Iter>
    struct is_input_iterator : public has_iterator_cat_of <Iter, input_iterator_tag> {};

    template <class Iter>
    struct is_output_iterator : public has_iterator_cat_of <Iter, output_iterator_tag>  {};

    template <class Iter>
    struct is_forward_iterator : public has_iterator_cat_of <Iter, forward_iterator_tag>    {};

    template <class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of <Iter, bidirectional_iterator_tag>    {};

    template <class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of <Iter, random_access_iterator_tag>   {};

    template <class Iterator>
    struct is_iterator : public m_intergral_constant<bool, is_input_iterator<Iterator>::value ||
        is_output_iterator<Iterator>::value>
    {
    };

    //萃取迭代器的category
    template <class Iterator>
    typename iterator_traits <Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    //萃取迭代器的value_type
    template <class Iterator>
    typename iterator_traits <Iterator>::value_type*
    value_type(const Iterator&)
    {
        return static_cast<typename iterator_traits <Iterator>::value_type*>(0);
    }

    //萃取迭代器的distance_type
    template <class Iterator>
    typename iterator_traits <Iterator>::difference_type*
    distance_type(const Iterator&)
    {
        return static_cast<typename iterator_traits <Iterator>::difference_type*>(0);
    }

    /* 以下函数计算迭代器之间的距离 */

    //input_iterator_tag版本
    template <class Iter>
    typename iterator_traits <Iter>::difference_type
    distance_dispatch(Iter first, Iter last)
    {
        typename iterator_traits <Iter>::difference_type n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    //random_access_iterator_tag版本
    template <class Iter>
    typename iterator_traits <Iter>::difference_type
    distance_dispatch(Iter first, Iter last, random_access_iterator_tag)
    {
        return last - first;
    }

    //适用所有
    template <class Iter>
    typename iterator_traits <Iter>::difference_type
    distance(Iter first, Iter last)
    {
        return distance_dispatch(first, last, iterator_category(first));
    }

    /* 以下函数让迭代器前进 n 个距离 */

    //input_iterator_tag版本
    template <class Iter, class Distance>
    void advance_dispatch(Iter& i, Distance n, input_iterator_tag)
    {
        while (n--)
        {
            ++i;
        }
    }

    //bidirectional_iterator_tag版本
    template <class Iter, class Distance>
    void advance_dispatch(Iter& i, Distance n, random_access_iterator_tag)
    {
        if (n >= 0)
        {
            while (n--)
            {
                ++i;
            }
        }
        else
        {
            while (n++)
            {
                --i;
            }
        }
    }

    //random_access_iterator_tag版本
    template <class Iter, class Distance>
    void advance_patch(Iter& i, Distance n, random_access_iterator_tag)
    {
        i += n;
    }

    //适用所有
    template <class Iter, class Distance>
    void advance(Iter &i, Distance n)
    {
        advance_dispatch(i, n, iterator_category(i));
    }


    /* 反向迭代器 reverse_iterator */
    template <class Iterator>
    class reverse_iterator
    {
    private:
        Iterator current;   //对应的正向迭代器
    
    public:
        typedef typename iterator_traits <Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits <Iterator>::value_type        value_type;
        typedef typename iterator_traits <Iterator>::difference_type   difference_type;
        typedef typename iterator_traits <Iterator>::pointer           pointer;
        typedef typename iterator_traits <Iterator>::reference         reference;

        typedef Iterator                                               iterator_type;
        typedef reverse_iterator<Iterator>                             self;

    public:
        //构造函数
        reverse_iterator()  {}
        explicit reverse_iterator(iterator_type i) : current(i) {}
        reverse_iterator(const self& rhs) : current(rhs.current)    {}

    public:
        //取出对应的正向迭代器
        iterator_type base() const  
        {
            return current;
        }

        /* 重载操作符 */

        reference operator*() const
        {
            auto tmp = current;
            return *--tmp;  //对应正向迭代器的前一个位置
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        // ++ -> --
        self& operator++()
        {
            --current;
            return *this;
        }

        self operator++(int)
        {
            self tmp = *this;
            --current;
            return tmp;
        }

        // -- -> ++
        self& operator--()
        {
            ++current;
            return *this;
        }

        self operator--(int)
        {
            self tmp = *this;
            ++current;
            return tmp;
        }
        
        self& operator+=(difference_type n)
        {
            current -= n;
            return *this;
        }

        self operator+(difference_type n) const
        {
            return self(current - n);
        }

        self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }

        self operator-(difference_type n) const
        {
            return self(current + n);
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }
    };

    //重载 operator-
    template <class Iterator>
    typename reverse_iterator <Iterator>::difference_type
    operator-(const reverse_iterator <Iterator>& lhs,
              const reverse_iterator <Iterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    //重载 比较操作符
    template <class Iterator>
    bool operator==(const reverse_iterator <Iterator>& lhs,
                    const reverse_iterator <Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base(0) < lhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs)
    {
        return rhs < lhs;
    }
    
    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs < rhs);
    }

}

#endif
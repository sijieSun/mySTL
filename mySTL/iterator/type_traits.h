#ifndef MYSTL_TYPE_TRAITS_h
#define MYSTL_TYPE_TRAITS_H

#include <type_traits>

namespace mySTL
{

    //定义结构体/类中的常量
    template<class T, T v>
    struct m_intergral_constant
    {
        static constexpr T value = v;
    };

    //定义编译器的true/false
    typedef m_intergral_constant<bool, true> m_true_type;
    typedef m_intergral_constant<bool, false> m_false_type;

    //向前声明
    template <class T1, class T2>
    struct pair;

    //判断类型
    template <class T>
    struct is_pair : mySTL::m_false_type{};

    template <class T1, class T2>
    struct is_pair<mySTL::pair<T1, T2>> : mySTL::m_true_type{};

}


#endif
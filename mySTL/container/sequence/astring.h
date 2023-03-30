#ifndef MYTINYSTL_ASTRING_H_
#define MYTINYSTL_ASTRING_H_

// 定义了 string, wstring, u16string, u32string 类型

#include "basic_string.h"

namespace mySTL
{

using string    = mySTL::basic_string<char>;
using wstring   = mySTL::basic_string<wchar_t>;
using u16string = mySTL::basic_string<char16_t>;
using u32string = mySTL::basic_string<char32_t>;

}
#endif // !MYTINYSTL_ASTRING_H_


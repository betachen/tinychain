#if __cplusplus > 201402L || (defined(_MSC_VER) && _MSC_VER >= 1910)

#include <string_view>

#else

//CHENHAO FIX
#include <experimental/string_view>
namespace std {
  using string_view = std::experimental::string_view;
}

#endif

#ifndef CHENHAO_STD_TO_STRING_VIEW
#define CHENHAO_STD_TO_STRING_VIEW
namespace std {
  inline auto to_stringv(const string& ss) {
      return string_view{ss.c_str(), ss.size()};
  }
}
#endif

// The MIT License (MIT)
//
// Copyright (c) 2018 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cstdlib>

namespace units {

  template<typename CharT, std::size_t N>
  struct basic_fixed_string {
    CharT data_[N+1] = {};

    constexpr basic_fixed_string(const CharT (&txt)[N+1]) noexcept
    {
      for(std::size_t i = 0; i <= N; ++i)
        data_[i] = txt[i];
    }

    [[nodiscard]] constexpr bool size() const noexcept { return N; }
    [[nodiscard]] constexpr const CharT* c_str() const noexcept { return data_; }

    // auto operator==(const basic_fixed_string &) = default;

    [[nodiscard]] constexpr friend bool operator==(const basic_fixed_string& lhs, const basic_fixed_string& rhs) noexcept
    {
      for(size_t i = 0; i != size(lhs.data_); ++i)
        if(lhs.name_[i] != rhs.data_[i])
          return false;
      return true;
    }

    template<typename CharT2, std::size_t N2>
    [[nodiscard]] constexpr friend bool operator==(const basic_fixed_string&, const basic_fixed_string<CharT2, N2>&) noexcept
    {
      return false;
    }

    template<typename CharT2, std::size_t N2>
    [[nodiscard]] constexpr friend bool operator<(const basic_fixed_string& lhs, const basic_fixed_string<CharT2, N2>& rhs) noexcept
    {
      using std::begin, std::end;
      auto first1 = begin(lhs.data_);
      auto first2 = begin(rhs.data_);
      const auto last1 = std::prev(end(lhs.data_));  // do not waste time for '\0'
      const auto last2 = std::prev(end(rhs.data_));

      for(; (first1 != last1) && (first2 != last2); ++first1, (void)++first2 ) {
        if(*first1 < *first2) return true;
        if(*first2 < *first1) return false;
      }
      return first1 == last1 && first2 != last2;
    }
  };

  template<typename CharT, std::size_t N>
  basic_fixed_string(const CharT (&str)[N]) -> basic_fixed_string<CharT, N-1>;

  template<std::size_t N>
  using fixed_string = basic_fixed_string<char, N>;

  // TODO gcc:92101
  // hacked version to work with derived_unit
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92101

  template<typename CharT, CharT... Chars>
  struct basic_fixed_string_hack {
    static constexpr CharT txt[] = { Chars..., '\0' };

    static constexpr const CharT* c_str() noexcept
    {
      return txt;
    }
  };

  inline namespace hacks {

    template<typename T, T... chars>
    constexpr basic_fixed_string_hack<T, chars...> operator""_fs() { return {}; }

  }

}

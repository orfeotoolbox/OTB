/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbStringUtilities_h
#define otbStringUtilities_h

#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstring>
#include <limits>
#include <ostream>
#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <string>
#include <concepts>
#include "otbMacro.h"
#include "otbExpected.h"

namespace otb
{

/*-------------------------------[ string_view backport ]--------------------*/
/**\name String view + functions */
//@{
/** String view class.
 * Class inspired by C++17 `std::string_view` and `gsl::string_span`.
 *
 * This idea is to bring efficiency and correctness to C++ functions that take
 * strings. A function that receives a `string_view` can receive any kind of
 * string flavour and guarantee 0-copy.
 *
 * The only side effect to be aware of: no terminal-0 is to be expected. i.e. a
 * `string_view` data cannot be passed freely to a function with a C API that
 * expect a null-terminated string.
 *
 * @invariant there is no guarantee `data()[size()] == '\0'`. It may happen
 * though depending on the constructor used.
 * @todo deprecate since std::expected can be used now
 */
struct [[nodiscard]] string_view
{
  using value_type      = char const;
  using reference       = char const&;
  using const_reference = char const&;
  using const_pointer   = char const*;
  using pointer         = char const*;
  using size_type       = std::size_t;

private:
  /** Strict view iterator.*/
  struct char_iterator
  {
    using value_type        = char const;
    using reference         = char const&;
    using pointer           = char const*;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    explicit char_iterator(const_pointer p) : m_ptr(p) {}
    char_iterator&  operator++() { ++m_ptr; return *this; }
    char_iterator   operator++(int) { char_iterator tmp(*this); ++m_ptr; return tmp; }
    char_iterator&  operator--() { --m_ptr; return *this; }
    char_iterator   operator--(int) { char_iterator tmp(*this); --m_ptr; return tmp; }
    const_reference operator*() const { return *m_ptr; }

    char_iterator&  operator+=(difference_type off) { m_ptr += off; return *this; }
    char_iterator&  operator-=(difference_type off) { m_ptr -= off; return *this; }

    friend char_iterator operator+(char_iterator lhs, difference_type off) { return lhs += off; }
    friend char_iterator operator+(difference_type off, char_iterator rhs) { return rhs += off; }
    friend char_iterator operator-(char_iterator lhs, difference_type off) { return lhs -= off; }
    friend char_iterator operator-(difference_type off, char_iterator rhs) { return rhs -= off; }

    friend difference_type operator-(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr - rhs.m_ptr; }

    friend bool operator==(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr == rhs.m_ptr; }
    friend bool operator!=(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr != rhs.m_ptr; }
    friend bool operator<(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr < rhs.m_ptr; }
  private:
    friend bool operator<=(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr <= rhs.m_ptr; }
  private:
    const_pointer m_ptr;
  };

public:
  using const_iterator         = char_iterator;
  using const_reverse_iterator = std::reverse_iterator<char_iterator>;

  /** Constructor from a standard string.
   * @post `data()[size()] == '\0'`
   */
  string_view(std::string const& s)
  : m_size(s.size()), m_first(s.c_str())
  {}
  /** Constructor from an array of characters.
   * @post `data()[size()] == '\0'` <= the array is built with this kind of
   * syntax: `char const a[]="foobar";`
   */
  template <std::size_t N>
  string_view(char const (&array)[N])
  : m_size(N)
  , m_first(&array[0])
  {}
  /** Constructor from a pointer to a 0-terminated raw string.
   * @post `data()[size()] == '\0'`
   */
  string_view(char const* ptr)
  : m_size(std::strlen(ptr))
  , m_first(ptr)
  {}
#if 0
  string_view(char const* first, char const* last)
  : m_size(last-first), m_first(first)
  {
    assert(first <= last);
  }
#endif
  /** Constructor from an input iterator and a size.
   * @post there is no guarantee `data()[size()] == '\0'`.
   */
  template <typename II>
  string_view(II first, size_type size_)
  : m_size(size_)
  , m_first(first)
  {}

  /** Constructor from a pair of input iterators.
   * @post there is no guarantee `data()[size()] == '\0'`.
   */
  template <typename II>
  string_view(II first, II last)
  : m_size(last - first)
  , m_first(first)
  {
    assert(first <= last);
  }

  /** Builds a peculiar empty view to points to nullptr. */
  static string_view null() { return string_view(build_empty()); }

  /** Tells whether a view is within another view.
   * This functions doesn't test whether the characters from a view are found
   * in the same relative positions in another view.
   * @note There is no equivalent function in C++17 `std::string_view`.
   * C++23 has a `std::string_view::contains()` function however.
   */
  [[nodiscard]]
  bool belongs_to(char const* first, char const* last) const
  {
    return first <= &*begin() && &*end() <= last;
  }
  template <std::size_t N>
  [[nodiscard]]
  bool belongs_to(char const (&array)[N]) const
  {
    // N-1 because of terminal 0
    return belongs_to(&array[0], &array[N - 1]);
  }
  [[nodiscard]]
  bool belongs_to(char const* ptr) const
  {
    return belongs_to(&ptr[0], &ptr[std::strlen(ptr)]);
  }
  template <typename String>
  [[nodiscard]]
  bool belongs_to(String const& s) const
  {
    return belongs_to(s.data(), s.data() + s.size());
  }

  [[nodiscard]] size_type               size   () const { return m_size; }
  [[nodiscard]] bool                    empty  () const { return m_size == 0; }

  // iterator                begin  ()       { return iterator(m_first); }
  // iterator                end    ()       { return iterator(m_first+m_size); }
  [[nodiscard]] const_iterator          begin  () const { return m_first; }
  [[nodiscard]] const_iterator          end    () const { return m_first+m_size; }
  [[nodiscard]] const_iterator          cbegin () const { return m_first; }
  [[nodiscard]] const_iterator          cend   () const { return m_first+m_size; }

  // reverse_iterator        rbegin ()       { return reverse_iterator(m_first+m_size); }
  // reverse_iterator        rend   ()       { return reverse_iterator(m_first); }
  [[nodiscard]] const_reverse_iterator  rbegin () const { return const_reverse_iterator(m_first+m_size); }
  [[nodiscard]] const_reverse_iterator  rend   () const { return const_reverse_iterator(m_first); }
  [[nodiscard]] const_reverse_iterator  crbegin() const { return const_reverse_iterator(m_first+m_size); }
  [[nodiscard]] const_reverse_iterator  crend  () const { return const_reverse_iterator(m_first); }

  /** Indexed access.
   * @pre `p < size(),` checked with an assertion.
   */
  [[nodiscard]]
  const_reference operator[](size_type p)
  {
    assert(p < size());
    return *(m_first + p);
  }

  /** Returns first character of the view.
   * @pre `!empty()`, checked with an assertion.
   */
  [[nodiscard]]
  const_reference front() const { assert(!empty()); return *begin(); }
  /** Returns last character of the view.
   * @pre `!empty()`, checked with an assertion.
   */
  [[nodiscard]]
  const_reference back () const { assert(!empty()); return *(end()-1); }

  /** Returns a pointer to the first character of the view.
   * @warning there is no guarantee `data()[size()] == '\0'`
   */
  [[nodiscard]]
  const_pointer   data () const { return &*m_first; }

  /** Removes the n first characters.
   * @pre `n <= size()`, checked with an assertion
   * @post size is n characters more little
   * @post `begin()` iterator is n characters right.
   */
  void remove_prefix(std::size_t n) { assert(n <= m_size); m_first += n; m_size -= n; }
  /** Removes the n last characters.
   * @pre `n <= size()`, checked with an assertion
   * @post size is n characters more little
   * @post `end()` iterator is n characters left.
   */
  void remove_suffix(std::size_t n) { assert(n <= m_size); m_size -= n; }

private:
  struct build_empty{};
  string_view(build_empty) : m_size(0), m_first(NULL) {}

  std::size_t    m_size;
  const_iterator m_first;
};

[[nodiscard]]
inline
bool operator==(string_view const& lhs, string_view const& rhs)
{
  return lhs.size() == rhs.size()
    && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
[[nodiscard]]
inline
bool operator!=(string_view const& lhs, string_view const& rhs)
{
  return ! (lhs == rhs);
}

inline
std::ostream & operator<<(std::ostream & os, const string_view & v)
{
  std::copy(v.begin(), v.end(), std::ostream_iterator<char>(os));
  return os;
}

[[nodiscard]]
inline
std::string operator+(string_view const& lhs, string_view const& rhs)
{
  std::string res(lhs.begin(), lhs.end());
  res.append(rhs.begin(), rhs.end());
  return res;
}

/** Tells whether two views are exactly identical. */
[[nodiscard]]
inline
bool is_same_view(string_view const& lhs, string_view const& rhs)
{
  return lhs.begin() == rhs.begin() && lhs.size() == rhs.size();
}

/** Tells whether the haystack string starts with the needle string. */
[[nodiscard]]
inline
bool starts_with(string_view const& haystack, string_view const& needle)
{
  return needle.size() <= haystack.size()
    && std::equal(needle.begin(), needle.end(), haystack.begin());
}

/** Tells whether the haystack string starts with the needle string. */
[[nodiscard]]
inline
bool ends_with(string_view const& haystack, string_view const& needle)
{
  return needle.size() <= haystack.size()
    && std::equal(needle.rbegin(), needle.rend(), haystack.rbegin());
}

/** Searches needle string within the haystack string. */
[[nodiscard]]
inline
string_view find(string_view const& haystack, string_view const& needle)
{
  assert(!haystack.empty());
  assert(!needle.empty());
  string_view::size_type const size = needle.size();
  if (haystack.size() < size)
  {
    // std::cout << "haystack size < needle size!!!";
    return string_view::null();
  }
  string_view::const_iterator const end = haystack.end() - size + 1;
  // std::cout << "comp up to:"
  // << static_cast<void const*>(&*end) << "(" << (*end) << ")"
  // ;
  string_view::const_iterator       it  = haystack.begin();
  for ( ; it != end; ++it)
  {
    string_view v(it, size);
    assert(v.size() == size);
    assert(v.belongs_to(haystack));
    // std::cout << "  check "
    //   << static_cast<void const*>(v.data())<<"["<<v.size()<<"]  VS "
    //   << static_cast<void const*>(needle.data())<<"["<<needle.size()<<"]"
    //   ;
    if (v == needle)
    {
      return v;
    }
  }
  assert(it + size - 1 == haystack.end());
  return string_view::null();
}

/** Tells whether the haystack string contains the needle string. */
[[nodiscard]]
inline
bool contains(string_view const& haystack, string_view const& needle)
{
  return ! is_same_view(find(haystack, needle), string_view::null());
}
//@}

/*-------------------------------[ Splitting functions ]---------------------*/
/**\name Splitting functions */
//@{

/** Split parts iterator.
 * Not meant to be used directly.
 * @tparam Splitter split algorithm used.
 * @see `part_range`
 * @see `split_on()`
 */
template <typename Splitter>
struct part_iterator
{
  using reference         = string_view &;
  using const_reference   = string_view const&;
  using const_pointer     = string_view const*;
  using iterator_category = std::forward_iterator_tag;

  // build last
  part_iterator(string_view const& global_string, Splitter s)
  : m_global(global_string)
  , m_splitter(s)
  , m_crt(m_splitter(global_string))
  {}
  struct theend {};
  part_iterator(string_view const& global_string, Splitter s, theend)
  : m_global(global_string)
  , m_splitter(s)
  , m_crt(string_view::null())
  {}

  part_iterator & operator++()
  {
    assert(m_crt.belongs_to(m_global) && "Invariant check failure");
    assert(! is_same_view(m_crt, string_view::null()) && "Don't iterate past end");
    if (m_crt.end() == m_global.end()) {
      m_crt = string_view::null();
    } else {
      string_view within(m_crt.end()+m_splitter.next_start(), m_global.end());
      m_crt = m_splitter(within);
    }
    return *this;
  }
  part_iterator operator++(int)
  {
    part_iterator tmp(*this);
    ++(*this);
    return *this;
  }
  const_reference operator*()  const { return m_crt; }
  const_pointer   operator->() const { return &m_crt; }

  friend bool operator==(part_iterator const& lhs, part_iterator const& rhs)
  {
    // std::cout << "<<"<<lhs<<">> ==?== <<"<<rhs<<">>" << std::endl;
    assert(is_same_view(lhs.m_global, rhs.m_global));
    return is_same_view(lhs.m_crt, rhs.m_crt); // TODO: too many tests
  }
  friend bool operator!=(part_iterator const& lhs, part_iterator const& rhs)
  {
    return ! (lhs == rhs);
  }
#if defined(BOOST_TEST_MODULE)
  friend std::ostream& operator<<(std::ostream& os, part_iterator const& v)
  {
    return os << "split iterator on `" << v.m_crt
      << "'("<<static_cast<void const*>(&*v.m_crt.begin()) <<"-"<<v.m_crt.size()<<")"<<", within `"
      << v.m_global << "'";
  }
#endif
private:
  string_view m_global;
  Splitter    m_splitter;
  string_view m_crt;
};

namespace details
{
   inline std::size_t len(char) { return 1; }
   template <std::size_t N> inline std::size_t len(char const (&)[N])
   { assert(N>0); return N-1; }
   inline std::size_t len(char const* ptr) { return std::strlen(ptr); }
   template <typename String> inline std::size_t len(String const& s) { return s.length(); }
} // details namespace

/** Split parts range.
 * Not meant to be used directly.
 * @tparam Splitter split algorithm used.
 * @see `part_iterator`
 * @see `split_on()`
 */
template <typename Splitter>
struct part_range
{
  part_range(string_view const& global_string, Splitter s)
  : m_first(global_string, s)
  , m_last(global_string, s, typename const_iterator::theend())
  {}
  typedef part_iterator<Splitter> const_iterator;
  const_iterator const& begin() const { return m_first; }
  const_iterator const& end  () const { return m_last; }
private:
  const_iterator const m_first;
  const_iterator const m_last;
};

/** Split algorithm that splits on single characters.
 * Not meant to be used directly
 * @see `split_on()`
 */
struct OTBCommon_EXPORT splitter_on_delim
{
  splitter_on_delim(char delim) : m_delimiter(delim) {}
  string_view operator()(string_view const& within) const {
    string_view res(within.begin(),
                    std::find(within.begin(), within.end(), m_delimiter)
    );
    return res ;
  }
  std::size_t next_start() const { return details::len(m_delimiter); }
private:
  char m_delimiter;
};

/** Split entry point.
 * @tparam String Type of the string split ( `std::string`,
 * `string_view`, ...)
 * @param[in] str string to be split
 * @param[in] delim character delimiter used to split.
 * @return a range of the parts found in `str`.
 *
 * Example of use:
 * \code
   typedef part_range<splitter_on_delim>       range_type;
   typedef typename range_type::const_iterator const_iterator;
   const range_type parts = split_on(",abc,z,,,def,xy,,", ',')
   const_iterator it = parts.begin();
   const_iterator end = parts.end();
   for ( ; it != end ; ++it) {
       doStuff(*it);
   }
 * \endcode
 */
template <typename String>
part_range<splitter_on_delim> split_on(String const& str, char delim)
{
  return part_range<splitter_on_delim>(str, splitter_on_delim(delim));
}

/** Strip left side of a string
 * @brief returns a string_view with the leading characters removed
 * @param[in] v string to be stripped
 * @param[in] c string with characters to strip.
 * @return the stripped string_view
 */
[[nodiscard]]
OTBCommon_EXPORT string_view lstrip(string_view const& v, string_view const& c);

/** Strip right side of a string
 * @brief returns a string_view with the ending characters removed
 * @param[in] v string to be stripped
 * @param[in] c string with characters to strip.
 * @return the stripped string_view
 */
[[nodiscard]]
OTBCommon_EXPORT string_view rstrip(string_view const& v, string_view const& c);

//@}

/*-------------------------------[ String to number conversion ]-------------*/
/**\name string to number conversion */
//@{

namespace details
{

/**
 * \brief Internal generic string to whatever conversion.
 * Tries to convert the input string into the requested type.
 * The conversion is done with C++ streams.
 * \tparam Int Integral type
 * \param[in] v  input string
 *
 * \return [Expected] The string as an integer.
 * \return [Error] An explanation about the issue
 */
template <typename T>
[[nodiscard]]
inline
otb::expected<T, std::string>
to(string_view const& v)
{
  T res;
  std::stringstream ss;
  if (ss << v && (ss >> res >> std::ws).eof())
  {
    return res;
  }
  return otb::unexpected{"Cannot decode \"" + v + "\" as \"" + typeid(T).name()};
}


template <std::integral Int>
constexpr char const* precision()
{
  switch (sizeof(Int)) {
    case 1: return "8 bits";
    case 2: return "16 bits";
    case 4: return "32 bits";
    case 8: return "64 bits";
    case 16: return "128 bits";
  }
  return "invalid int";
}

/**
 * \brief Internal generic string to integer conversion.
 * Tries to convert the input string into a integer type.
 * \tparam Int Integral type
 * \param[in] v  input string
 *
 * \return [Expected] The string as an integer.
 * \return [Error] An explanation about the issue
 */
template <std::integral Int>
[[nodiscard]]
inline
otb::expected<Int, std::string>
to_integer(string_view const& v)
{
  constexpr Int min = std::numeric_limits<Int>::min();
  constexpr Int max = std::numeric_limits<Int>::max();
  // string_view::data() isn't compatible with strtol => we emulate it

  // TODO: handle HEX, OCT, BIN, locales?
  string_view::const_iterator it  = v.begin();
  string_view::const_iterator end = v.end();

  bool is_negative = false;
  Int res = 0;
  if (it != end)
  {
    switch (*it)
    {
      case '-': is_negative = true;
                OTB_FALLTHROUGH;
      case '+': ++it;
    }
    for ( ; it != end; ++it)
    {
      // only support arabic digits
      if (!std::isdigit(*it))
      {
        return otb::unexpected{"Cannot decode " + v + " as integer"};
      }
      auto digit = *it - '0';
      if (! is_negative) {
        if (res > max / 10) {
          return otb::unexpected{"Cannot decode " + v + " as "+precision<Int>()+" integer"};
        } else if (10 * res > max-digit) {
          return otb::unexpected{"Cannot decode " + v + " as "+precision<Int>()+" integer"};
        }
      } else {
        digit = - digit;
        if (res < min / 10) {
          return otb::unexpected{"Cannot decode " + v + " as "+precision<Int>()+" integer"};
        } else if (10 * res < min-digit) {
          return otb::unexpected{"Cannot decode " + v + " as "+precision<Int>()+" integer"};
    }
  }

      res  = 10 * res + digit;
    }
  }
  return res;
}

/**
 * \brief Internal generic string to unsigned integer conversion.
 * Tries to convert the input string into an unsigned integer type.
 * \tparam UInt Unsigned integral type
 * \param[in] v  input string
 *
 * \return [Expected] The string as an integer.
 * \return [Error] An explanation about the issue
 */
template <std::unsigned_integral UInt>
[[nodiscard]]
inline
otb::expected<UInt, std::string>
to_uinteger(string_view const& v)
{
  constexpr UInt max = std::numeric_limits<UInt>::max();
  // string_view::data() isn't compatible with strtol => we emulate it

  // TODO: handle HEX, OCT, BIN, locales?
  string_view::const_iterator it  = v.begin();
  string_view::const_iterator end = v.end();

  UInt res = 0;
  if (it != end)
  {
    for ( ; it != end ; ++it)
    {
      // only support arabic digits
      if (!std::isdigit(*it))
      {
        return otb::unexpected{"Cannot decode " + v + " as unsigned integer"};
      }
      auto digit = *it - '0';
      if (res > max / 10) {
        return otb::unexpected{"Cannot decode " + v + " as "+precision<UInt>()+" integer"};
      } else if (10 * res > max-digit) {
        return otb::unexpected{"Cannot decode " + v + " as "+precision<UInt>()+" integer"};
      }
      res  = 10 * res + *it - '0';
    }
  }
  return res;
}

/**
 * \brief Internal generic string to float conversion.
 * Tries to convert the input string into a floating point type.
 * \tparam FloatType floating point type (`float`, `double`, `long double`)
 * \param[in] v  input string
 *
 * \return [Expected] The string as a float. The special `"nan"` string is converted to `std::nan()`
 * \return [Error] An explanation about the issue
 *
 */
template <std::floating_point FloatType>
[[nodiscard]]
inline
otb::expected<FloatType, std::string>
to_float(string_view const& v)
{
  if (contains(v, "nan"))
  {
    return static_cast<FloatType>(std::nan(""));
  }
  FloatType res = FloatType(); // 0-construction
  if (!v.empty())
  {
    std::stringstream ss;
    if (!(ss << v && (ss >> res >> std::ws).eof()))
    {
      return otb::unexpected{"Cannot decode " + v + " as float value"};
    }
  }
  return res;
}

// TODO: what was this specialisation??
template <typename T>
[[nodiscard]]
inline
otb::expected<std::string, std::string>
to(T const& v) { return v; }

template <typename T>
[[nodiscard]]
inline
otb::expected<std::string, std::string>
to(T && v) { return otb::expected<std::string, std::string>(std::move(v)); }

#define OTB_GENERATE_CONV(internal_to, type)                                         \
template <> [[nodiscard]] inline otb::expected<type, std::string> to<type>(string_view const& v) \
{ return details::internal_to<type>(v); }

OTB_GENERATE_CONV(to_integer,  char);
OTB_GENERATE_CONV(to_uinteger, unsigned char);
OTB_GENERATE_CONV(to_integer,  signed char);
OTB_GENERATE_CONV(to_uinteger, unsigned short);
OTB_GENERATE_CONV(to_integer,  short);
OTB_GENERATE_CONV(to_integer,  int);
OTB_GENERATE_CONV(to_uinteger, unsigned int);
OTB_GENERATE_CONV(to_integer,  long);
OTB_GENERATE_CONV(to_uinteger, unsigned long);
OTB_GENERATE_CONV(to_integer,  long long);
OTB_GENERATE_CONV(to_uinteger, unsigned long long);
OTB_GENERATE_CONV(to_float,    float);
OTB_GENERATE_CONV(to_float,    double);
OTB_GENERATE_CONV(to_float,    long double);
#undef OTB_GENERATE_CONV

} // otb::details namespace

/**
 * \brief Generic string to whatever conversion -- failure means exception.
 * Tries to decode a value from a string.
 * \tparam  T Type of the value decoded from the input string
 * \param[in] v  input string to convert
 * \param[in] context  context message for the exception thrown
 *
 * \return The value decoded
 * \throw std::runtime_error if a value of type `T` cannot be decoded from the
 * string.
 * \note For efficiency reasons, the function has been specialized for integral
 * floating point, and string types.
 * \note That alternative to `ossimString::toXxxx()` function has been defined
 * to support generic programming in `get()` and `add()` helper functions for
 * keyword list.
 * \see `to_with_default()` for the version that never fails, but returns a
 * default value otherwise (being `T()` by default).
 *
 */
template <typename T>
[[nodiscard]]
inline
T to(string_view const& v, string_view const& context)
{
  auto conversion = details::to<T>(v);
  if (conversion) {
    return *conversion;
  }
  else {
    throw std::runtime_error(conversion.error() + " while " + context);
  }
}

/**
 * \brief Generic string to whatever conversion -- failure is hidden.
 * Tries to decode a value from a string.
 * \tparam  T Type of the value decoded from the input string
 * \param[in] v  input string to convert
 * \param[in] def  default value returned in the conversion isn't possible
 *
 * \return The value decoded
 * \return `def` if no value of type `T` can be decoded from `v`.
 * \throw None
 * \note For efficiency reasons, the function has been specialized for integral
 * floating point, and string types.
 * \note That alternative to `ossimString::toXxxx()` function has been defined
 * to support generic programming in `get()` and `add()` helper functions for
 * keyword list.
 * \see `to()` for the version that fails by throwing exceptions.
 */
template <typename T>
[[nodiscard]]
inline
T to_with_default(string_view const& v, T const& def = T())
{
  auto conversion = details::to<T>(v);
  if (conversion) {
    return *conversion;
  }
  else {
    return def;
  }
}

template <>
[[nodiscard]]
inline std::string to<std::string>(string_view const& v, string_view const& /*context*/)
{ return std::string(v.begin(), v.end()); }

template <typename T>
[[nodiscard]]
inline T const& to(T const& v, string_view const& /*context*/) { return v; }

template <typename T>
[[nodiscard]]
inline T const& to_with_default(T const& v, T const& /* default*/) { return v; }


/** Strip left side of a string
 * @brief returns a string_view with the leading characters removed
 * @param[in] v string to be stripped
 * @param[in] c string with characters to strip.
 * @return the stripped string_view
 */
OTBCommon_EXPORT string_view lstrip(string_view const& v, string_view const& c);


} // otb namespace

#endif

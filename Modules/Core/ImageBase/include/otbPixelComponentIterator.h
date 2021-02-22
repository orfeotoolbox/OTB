/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbPixelComponentIterator_h
#define otbPixelComponentIterator_h

#include "otbNotNull.h"
#include "otbArrayTraits.h"
#include "otbIteratorHelpers.h"
#include "itkNumericTraits.h"
#include <boost/type_traits/is_complex.hpp>
#include <type_traits>

namespace otb
{
namespace internals
{

/*===============================[ Generic => Arrays ]=======================*/
/**
 * Declares an iterator type over pixel components.
 * Given a pixel, a component iterator permits to iterates over the
 * components of a pixel.
 *
 * For instance, it can be used to convert from a
 * `itk::FixedArray<std::complex<double>, 5>` pixel into a
 * `itk::VariableLengthVector<float>` pixel of size 10 with `std::copy()`.
 *
 * \tparam TPixel          Any kind of pixel. It could be a scalar, a
 *                         `std::complex<>`, a statically bound array,
 *                         or a dynamic `itk::VariableLengthVector<>` of
 *                         scalars or even of complexes.
 * \tparam ConstOrMutable  Tags that tells whether the pixel iterated is
 *                         `const` or mutable.
 * \tparam Dispatch        Internal type to select the right specialization.
 *
 * \invariant `m_pixel != nullptr`, statically ensured
 * \invariant if not `is_at_end()` then the iterator can be dereferenced
 *
 * \note This is the internal type, it's not meant to be used directly.
 * \sa `otb::PixelComponentIterator`, `otb::PixelComponentConstIterator`,
 * `otb::PixelRange()`
 * \ingroup OTBImageBaseInternals
 */
template <typename TPixel, typename ConstOrMutable, class Dispatch = void>
class PixelComponentIterator
;

/** Component Iterator specialization for array pixel types.
 * \copydoc PixelComponentIterator
 */
template <typename TPixel, typename ConstOrMutable>
class PixelComponentIterator<
  TPixel,
  ConstOrMutable,
  typename std::enable_if_t<mpl::is_array_v<std::decay_t<TPixel>>>>
{
  static_assert(mpl::is_array_v<std::decay_t<TPixel>>, "This specialization is reserved to array pixel types");
public:

  /**\name ITK Constants and Typedefs */
  //@{
  static constexpr bool is_const        = std::is_same<ConstOrMutable, ConstTag>::value;
  static constexpr bool is_mutable      = std::is_same<ConstOrMutable, MutableTag>::value;
  /** Pixel Type. */
  using PixelType                       = TPixel;
  using InternalPixelType               = std::remove_reference_t<decltype(std::declval<PixelType>()[0])>;
  static constexpr bool is_scalar_array = std::is_arithmetic<InternalPixelType>::value;

  using Self                            = PixelComponentIterator;
  using ConstMut_IntPixelType           = std::conditional_t<is_const, std::add_const_t<InternalPixelType>, InternalPixelType>;
  using SubPixelComponentIteratorType   = PixelComponentIterator<ConstMut_IntPixelType, ConstOrMutable>;

  /** Run-time type information (and related methods). */
  itkTypeMacroNoParent(PixelComponentIterator);
  //@}

  /**@name C++ std typedefs */
  //@{
  using difference_type   = std::ptrdiff_t;
  using value_type        = std::conditional_t<is_mutable, InternalPixelType, InternalPixelType const>;
  using pointer           = value_type *;
  using reference         = value_type &;
  using const_reference   = value_type const&;
  using iterator_category = std::forward_iterator_tag;
  // static_assert(is_const >= std::is_const<InternalPixelType>::value, "should compile");
  //@}

  /**\name Constructions & Destruction
   *
   * This class follows the rule of 0/5
   */
  //@{
  PixelComponentIterator           ()                              = default;
  ~PixelComponentIterator          ()                              = default;
  PixelComponentIterator           (PixelComponentIterator const&) = default;
  PixelComponentIterator           (PixelComponentIterator     &&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator const&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator     &&) = default;

#if 1
  /** Convertion constructor.
   * Converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> const& rhs)
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    , m_subiter(rhs.m_subiter)
    {}

  /** Convertion move constructor.
   * Move converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> && rhs)
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    , m_subiter(rhs.m_subiter)
    {}
#endif

  /**
   * Init Constructor.
   * Constructs a `PixelComponentIterator` from a pixel
   * \param[in,out] pixel  Pixel over which components are iterated
   */
  explicit PixelComponentIterator(PixelType & pixel, std::size_t component = 0)
    : m_pixel(&pixel)
    , m_component(component)
    , m_subiter(SubPixelComponentIteratorType(get_current_pixel()))
    {
      assert(component <= mpl::GetNumberOfComponents(*m_pixel));
    }

  // static_assert(std::is_copy_constructible<Self>::value, "Requires copy construction");
  // static_assert(std::is_trivially_copy_constructible<Self>::value, "Requires trivial copy construction");
  //@}

  /**\name Comparison
   * Comparison operators.
   * They are all hidden friends. As a consequence, they need to be
   * defined within the class definition.
   */
  //@{
  bool is_at_end() const noexcept
  {
    return m_component == mpl::GetNumberOfComponents(*m_pixel);
  }

  friend bool operator==(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    // As is_scalar_array is constexpr it should be optimized away by
    // the compiler
    return (lhs.m_component == rhs.m_component) && (is_scalar_array || (lhs.m_subiter == rhs.m_subiter));
  }
  friend bool operator!=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs == rhs); }

  friend bool operator<=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    // As is_scalar_array is constexpr it should be optimized away by
    // the compiler
    return (lhs.m_component <= rhs.m_component)
      || ( !is_scalar_array &&
           (lhs.m_component == rhs.m_component) && (lhs.m_subiter <= rhs.m_subiter) );
  }
  friend bool operator<(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return (lhs.m_component < rhs.m_component)
      || ( !is_scalar_array &&
           (lhs.m_component == rhs.m_component) && (lhs.m_subiter < rhs.m_subiter) );
  }
  friend bool operator>=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs < rhs); }
  friend bool operator>(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs <= rhs); }
  //@}

  /**\name Iterator Interface */
  //@{
  /** Pre-increments the iterator.
   * \pre `! is_at_end()`
   */
  Self& operator++()
  {
    assert(m_component < mpl::GetNumberOfComponents(*m_pixel));
#if defined(__cpp_if_constexpr)
    // Optimization for array<scalar> cases
    if constexpr(std::is_arithmetic<InternalPixelType>::value)
    {
      ++m_component;
      return *this;
    }
#endif
    ++m_subiter;
    if (m_subiter.is_at_end())
    {
      ++m_component;
      m_subiter = SubPixelComponentIteratorType{get_current_pixel()};
    }
    return *this;
  }

  /** Post-increments the iterator.
   */
  Self operator++(int)
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  decltype(auto) operator*()
  {
    // TODO: proxy
    // - read: subiter->get_current_pixel()
    // - write: set_current_pixel(subiter->set_current_pixel(value))
#if defined(__cpp_if_constexpr)
    if constexpr (is_scalar_array)
    {
      return get_current_pixel();
    }
#endif
    return *m_subiter;
  }
  decltype(auto) operator*() const
  {
    // TODO: proxy
    // - read: subiter->get_current_pixel()
    // - write: set_current_pixel(subiter->set_current_pixel(value))
#if defined(__cpp_if_constexpr)
    if constexpr (is_scalar_array)
    {
      return get_current_pixel();
    }
#endif

    return *m_subiter;
  }
  //@}

private:
  template <bool IsMutable_ = is_mutable, class = std::enable_if_t<IsMutable_>>
  decltype(auto) get_current_pixel()
  {
    return (*m_pixel)[m_component];
  }

  decltype(auto) get_current_pixel() const
  {
    return (*m_pixel)[m_component];
  }

  NotNull<PixelType*>           m_pixel;
  std::size_t                   m_component;
  /** Iterator to subcomponents.
   * In array cases, components can be array-like pixels like
   * `std::complex<>`
   */
  SubPixelComponentIteratorType m_subiter;
};

/*===============================[ Complex Numbers ]=========================*/
/** Component Iterator specialization for complex pixel types.
 * \copydoc PixelComponentIterator
 */
template <typename TPixel, typename ConstOrMutable>
class PixelComponentIterator<
  TPixel,
  ConstOrMutable,
  typename std::enable_if_t<boost::is_complex<std::decay_t<TPixel>>::value>>
{
public:

  /**\name ITK Constants and Typedefs */
  //@{
  static constexpr bool is_const      = std::is_same<ConstOrMutable, ConstTag>::value;
  static constexpr bool is_mutable    = std::is_same<ConstOrMutable, MutableTag>::value;
  /** Pixel Type. */
  using PixelType                     = TPixel;
  // using InternalPixelType             = typename PixelType::ValueType;
  using InternalPixelType             = std::remove_reference_t<typename PixelType::value_type>;

  using Self                          = PixelComponentIterator;
  using ConstMut_IntPixelType         = std::conditional_t<is_const, std::add_const_t<InternalPixelType>, InternalPixelType>;
  using SubPixelComponentIteratorType = PixelComponentIterator<ConstMut_IntPixelType, ConstOrMutable>;

  /** Run-time type information (and related methods). */
  itkTypeMacroNoParent(PixelComponentIterator);
  //@}

  /**@name C++ std typedefs */
  //@{
  using difference_type   = std::ptrdiff_t;
  using value_type        = std::conditional_t<is_mutable, InternalPixelType, InternalPixelType const>;
  using pointer           = value_type *;
  using reference         = value_type &;
  using const_reference   = value_type const&;
  using iterator_category = std::forward_iterator_tag;
  // static_assert(is_const >= std::is_const<InternalPixelType>::value, "should compile");
  //@}

  /**\name Constructions & Destruction
   *
   * This class follows the rule of 0/5
   */
  //@{
  PixelComponentIterator           ()                              = default;
  ~PixelComponentIterator          ()                              = default;
  PixelComponentIterator           (PixelComponentIterator const&) = default;
  PixelComponentIterator           (PixelComponentIterator     &&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator const&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator     &&) = default;

#if 1
  /** Convertion constructor.
   * Converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> const& rhs)
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    {}

  /** Convertion move constructor.
   * Move converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> && rhs)
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    {}
#endif

  /**
   * Init Constructor.
   * Constructs a `PixelComponentIterator` from a pixel
   * \param[in,out] pixel  Pixel over which components are iterated
   */
  explicit PixelComponentIterator(PixelType & pixel, std::size_t component = 0)
    : m_pixel(&pixel)
    , m_component(component)
    {
      assert(component <= 2);
    }

  // static_assert(std::is_copy_constructible<Self>::value, "Requires copy construction");
  // static_assert(std::is_trivially_copy_constructible<Self>::value, "Requires trivial copy construction");
  //@}

  /**\name Comparison
   * Comparison operators.
   * They are all hidden friends. As a consequence, they need to be
   * defined within the class definition.
   */
  //@{
  bool is_at_end() const noexcept
  {
    return m_component == 2;
  }

  friend bool operator==(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component == rhs.m_component;
  }
  friend bool operator!=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs == rhs); }

  friend bool operator<=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component <= rhs.m_component;
  }
  friend bool operator<(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component < rhs.m_component;
  }
  friend bool operator>=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs < rhs); }
  friend bool operator>(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs <= rhs); }
  //@}

  /**\name Iterator Interface */
  //@{
  /** Pre-increment the iterator.
   * \pre `! is_at_end()`
   */
  Self& operator++()
  {
    // assert(m_component < size(*m_pixel));
    ++m_component;
    return *this;
  }

  /** Post-increment the iterator.
   */
  Self operator++(int)
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  decltype(auto) operator*()
  {
    // TODO: proxy
    // - read: subiter->get_current_pixel()
    // - write: set_current_pixel(subiter->set_current_pixel(value))
    return get_current_pixel();
  }
  decltype(auto) operator*() const
  {
    // TODO: proxy
    // - read: subiter->get_current_pixel()
    // - write: set_current_pixel(subiter->set_current_pixel(value))
    return get_current_pixel();
  }
  //@}

private:
  template <bool IsMutable_ = is_mutable, class = std::enable_if_t<IsMutable_>>
  decltype(auto) get_current_pixel()
  {
    return reinterpret_cast<value_type(&)[2]>((*m_pixel))[m_component];
  }

  decltype(auto) get_current_pixel() const
  {
    // Guaranteed compatibility with array of components since C++11
    return reinterpret_cast<value_type(&)[2]>((*m_pixel))[m_component];
  }

  NotNull<PixelType*>           m_pixel;
  std::size_t                   m_component;
};

/*===============================[ Numbers ]=================================*/
/** Component Iterator specialization for scalar pixel types.
 * \copydoc PixelComponentIterator
 */
template <typename TPixel, typename ConstOrMutable>
class PixelComponentIterator<TPixel, ConstOrMutable, typename std::enable_if_t<std::is_arithmetic<TPixel>::value>>
{
  static_assert(std::is_arithmetic<TPixel>::value, "Specialization for scalar pixels");
public:

  /**\name ITK Constants and Typedefs */
  //@{
  static constexpr bool is_const   = std::is_same<ConstOrMutable, ConstTag>::value;
  static constexpr bool is_mutable = std::is_same<ConstOrMutable, MutableTag>::value;
  /** Pixel Type. */
  using PixelType                  = TPixel;
  // using InternalPixelType       = TPixel;

  using Self                       = PixelComponentIterator;

  /** Run-time type information (and related methods). */
  itkTypeMacroNoParent(PixelComponentIterator);
  //@}

  /**@name C++ std typedefs */
  //@{
  using difference_type            = std::ptrdiff_t;
  using value_type                 = std::conditional_t<is_mutable, PixelType, PixelType const>;
  using pointer                    = value_type *;
  using reference                  = value_type &;
  using const_reference            = value_type const&;
  using iterator_category          = std::forward_iterator_tag;
  // static_assert(is_const >= std::is_const<PixelType>::value, "should compile");
  //@}

  /**\name Constructions & Destruction
   *
   * This class follows the rule of 0/5
   */
  //@{
  PixelComponentIterator           ()                              = default;
  ~PixelComponentIterator          ()                              = default;
  PixelComponentIterator           (PixelComponentIterator const&) = default;
  PixelComponentIterator           (PixelComponentIterator     &&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator const&) = default;
  PixelComponentIterator& operator=(PixelComponentIterator     &&) = default;

#if 1
  /** Convertion constructor.
   * Converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> const& rhs) noexcept
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    {}

  /** Convertion move constructor.
   * Move converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = is_const, class = std::enable_if_t<IsConst_>>
    PixelComponentIterator(PixelComponentIterator<PixelType, MutableTag> && rhs) noexcept
    : m_pixel(rhs.m_pixel)
    , m_component(rhs.m_component)
    {}
#endif

  /**
   * Init Constructor.
   * Constructs a `PixelComponentIterator` from a pixel
   * \param[in,out] pixel  Pixel over which components are iterated
   */
  explicit PixelComponentIterator(PixelType& pixel, std::size_t component = 0) noexcept
    : m_pixel(&pixel)
    , m_component(component)
    {}

  // static_assert(std::is_copy_constructible<Self>::value, "Requires copy construction");
  // static_assert(std::is_trivially_copy_constructible<Self>::value, "Requires trivial copy construction");
  //@}

  /**\name Comparison
   * Comparison operators.
   * They are all hidden friends. As a consequence, they need to be
   * defined within the class definition.
   */
  //@{
  bool is_at_end() const noexcept
  {
    return m_component == mpl::GetNumberOfComponents(*m_pixel);
  }

  friend bool operator==(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component == rhs.m_component;
  }
  friend bool operator!=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs == rhs); }

  friend bool operator<=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component <= rhs.m_component;
  }
  friend bool operator<(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  {
    assert(lhs.m_pixel == rhs.m_pixel);
    return lhs.m_component < rhs.m_component;
  }
  friend bool operator>=(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs < rhs); }
  friend bool operator>(PixelComponentIterator const& lhs, PixelComponentIterator const& rhs) noexcept
  { return ! (lhs <= rhs); }
  //@}

  /**\name Iterator Interface */
  //@{
  /** Pre-increment the iterator.
   * \pre `! is_at_end()`
   */
  Self& operator++() noexcept
  {
    assert(m_component < 1);
    ++m_component;
    return *this;
  }

  /** Post-increment the iterator.
   */
  Self operator++(int) noexcept
  {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

#if 0
  PixelType & get_current_pixel() noexcept
  {
    return *m_pixel;
  }
  PixelType const& get_current_pixel() const noexcept
  {
    return *m_pixel;
  }
#endif
  decltype(auto) operator*() const noexcept
  {
    return *m_pixel;
  }
  decltype(auto) operator*() noexcept
  {
    return *m_pixel;
  }
  //@}

  bool end() const noexcept { return m_component > 0; }

private:
  NotNull<PixelType*>           m_pixel;
  std::size_t                   m_component;
};

} // otb::internal namespace

template <typename TPixel>
using PixelComponentIterator      = internals::PixelComponentIterator<TPixel, std::conditional_t<std::is_const<TPixel>::value, internals::ConstTag, internals::MutableTag>>;
template <typename TPixel>
using PixelComponentConstIterator = internals::PixelComponentIterator<TPixel, internals::ConstTag>;



/**
 * Helper type for defining a range of pixel components.
 * \tparam TPixel  Pixel type
 *
 * \invariant `begin() <= end()`
 * \invariant `m_pixel != nullptr`
 * \invariant `std::distance(begin(), end()) == shallow_size() ? shallow_size() * mpl::GetNumberOfComponents(*begin()) : 0`
 */
template <typename TPixel>
struct PixelRange_t
{
  using PixelType = TPixel;

  explicit PixelRange_t(PixelType & pixel) : m_pixel(&pixel) {}
  auto begin() { return otb::PixelComponentIterator<PixelType>{*m_pixel, 0U}; }
  auto end()   { return otb::PixelComponentIterator<PixelType>{*m_pixel, shallow_size()}; }
  // TODO: GOTO last

  auto begin()  const { return otb::PixelComponentConstIterator<PixelType>{*m_pixel, 0U}; }
  auto end()    const { return otb::PixelComponentConstIterator<PixelType>{*m_pixel, shallow_size()}; }
  auto cbegin() const { return otb::PixelComponentConstIterator<PixelType>{*m_pixel, 0U}; }
  auto cend()   const { return otb::PixelComponentConstIterator<PixelType>{*m_pixel, shallow_size()}; }

  std::size_t shallow_size() const /*noexcept*/ { return  mpl::GetNumberOfComponents(*m_pixel);}
private:
  NotNull<PixelType*> m_pixel;
};

#if 1
template <typename TPixel>
inline
auto PixelRange(TPixel && pixel)
{
  // return PixelRange_t<TPixel>{pixel};
  static_assert(! std::is_rvalue_reference<decltype(std::forward<TPixel>(pixel))>::value, "Cannot build component iterator over rvalue pixels");
  return PixelRange_t<std::remove_reference_t<TPixel>>{pixel};
}
#else

template <typename TPixel>
inline
auto PixelRange(TPixel & pixel)
{
  return internals::PixelRange_t<TPixel>{pixel};
}

template <typename TPixel>
inline
auto PixelRange(TPixel const& pixel)
{
  return internals::PixelRange_t<TPixel const>{pixel};
}
#endif

} // end namespace otb

#endif

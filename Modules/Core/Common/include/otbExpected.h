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

#ifndef otbExpected_h
#define otbExpected_h

#include <cassert>
#include <type_traits>
#include <variant>

namespace otb
{
/** Simplified and minimalistic C++ backport of std::unexpected.
 *
 * \see `std::unexpected` documentation
 * \note `noexcept`, `explicit` and requirements are not as precisely supported as specified in the
 * standard.
 */
template <typename ErrorType>
class [[nodiscard]] unexpected
{
public:
  using error_type = ErrorType;

  constexpr unexpected()
  noexcept(std::is_nothrow_constructible_v<error_type>)
  : m_error{error_type{}}
  {}

  constexpr unexpected(unexpected const&) = default;
  constexpr unexpected(unexpected     &&) = default;

  /** Init constructor */
  template <typename Er = error_type>
  constexpr unexpected(Er && e)
  requires (   ! std::is_same_v<std::remove_cvref_t<Er>, unexpected>
            // && ! std::is_same_v<std::remove_cvref_t<Er>, std::in_place_t>
            &&   std::is_constructible_v<error_type, Er>
            )
  : m_error{std::forward<Er>(e)}
  {}

  [[nodiscard]] constexpr error_type const&  error() const&  noexcept { return m_error; }
  [[nodiscard]] constexpr error_type      &  error()      &  noexcept { return m_error; }
  [[nodiscard]] constexpr error_type const&& error() const&& noexcept { return std::move(m_error); }
  [[nodiscard]] constexpr error_type      && error()      && noexcept { return std::move(m_error); }

private:
  error_type m_error;
};

template< class ErrorType>
unexpected(ErrorType) -> unexpected<ErrorType>;

namespace details {
template<typename E> constexpr bool is_unexpected = false;
template<typename E> constexpr bool is_unexpected<unexpected<E>> = true;
} // details namespace

/** Simplified and minimalistic C++ backport of std::expected.
 *
 * \see `std::expected` documentation
 * \note Monamic operation are not supported for now
 * \note `noexcept`, `explicit` and requirements are not as precisely supported as specified in the
 * standard.
 */
template <typename ValueType, typename ErrorType>
class [[nodiscard]] expected : private std::variant<ValueType, ErrorType>
{
public:
  using value_type = ValueType;
  using error_type = ErrorType;

  template <class Up>
  using rebind = expected<Up, error_type>;

  using Super = std::variant<value_type, error_type>;

  using unexpected_type = unexpected<error_type>;

  /** Default constructor */
  constexpr expected()
  noexcept(std::is_nothrow_constructible_v<value_type>)
  : Super(value_type{})
  {}

  constexpr ~expected() = default;
  constexpr expected(expected const&) = default;
  constexpr expected(expected &&)     = default;

  constexpr expected& operator=(expected const&) = default;
  constexpr expected& operator=(expected &&)     = default;

  /** Conversion constructor */
  template <typename U, typename G>
    constexpr explicit expected(expected<U,G> const& e)
    : Super(e)
    {}

  /** Init constructor from value */
  template <class U = std::remove_cv_t<value_type>>
    requires(!std::is_same_v<std::remove_cvref_t<U>, expected>)
    // &&      (!std::is_same_v<std::remove_cvref_t<U>, in_place_t>)
    // &&      (!std::is_same_v<std::remove_cvref_t<U>, unexpect_t>)
    &&       std::is_constructible_v<value_type, U>
	  &&      (!details::is_unexpected<std::remove_cvref_t<U>>)
	  // &&      details::not_constructing_bool_from_expected<_Tp, U>
    constexpr explicit(!std::is_convertible_v<U, value_type>)
    expected(U&& v)
    noexcept(std::is_nothrow_constructible_v<value_type, U>)
    : Super(std::forward<U>(v))
    {}

  /** Init constructor from copied error */
  template <class G = error_type>
    requires std::is_constructible_v<error_type, G const&>
    constexpr
    explicit(!std::is_convertible_v<G const&, error_type>)
    expected(unexpected<G> const& e)
    noexcept(std::is_nothrow_constructible_v<error_type, G const&>)
    : Super(e.error())
    {}

  /** Init constructor from moved error */
  template<typename G = error_type>
    requires std::is_constructible_v<error_type, G>
    constexpr explicit(!std::is_convertible_v<G, error_type>)
    expected(unexpected<G>&& e)
    noexcept(std::is_nothrow_constructible_v<error_type, G>)
    : Super(std::move(e.error()))
    {}

  [[nodiscard]]
    constexpr bool has_value() const noexcept
    {
      return std::holds_alternative<value_type>(*this);
    }

  [[nodiscard]]
    explicit constexpr operator bool() const noexcept
    {
      return has_value();
    }

  using Super::swap;

  [[nodiscard]]
    constexpr value_type const& operator*() const noexcept
    {
      assert(has_value());
      return std::get<value_type>(*this);
    }

  [[nodiscard]]
    constexpr value_type const& value() const noexcept
    {
      assert(has_value());
      return std::get<value_type>(*this);
    }

  [[nodiscard]]
    constexpr error_type const& error() const noexcept
    {
      assert(! has_value());
      return std::get<error_type>(*this);
    }

  template <typename U = std::remove_cv_t<value_type>>
    [[nodiscard]]
    constexpr value_type const& value_or(U&& v) const
    {
      if (has_value()) {
        return std::get<value_type>(*this);
      }
      return std::forward<U>(v);
    }

  template <typename G = std::remove_cv_t<error_type>>
    [[nodiscard]]
    constexpr error_type const& error_or(G&& e) const
    {
      if (! has_value()) {
        return std::get<error_type>(*this);
      }
      return std::forward<G>(e);
    }

};

} // otb namespace

#endif // otbExpected_h

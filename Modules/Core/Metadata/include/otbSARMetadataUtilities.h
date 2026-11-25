/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSARMetadataUtilities_h
#define otbSARMetadataUtilities_h

#include "otbStringUtilities.h"
#include "otbMacro.h"
#include "otbConcepts.h"
#include <type_traits>
#include <unordered_map>
#include <optional>

namespace otb
{

using dict_t = std::unordered_map<std::string, std::string>;

/*-------------------------------[ OnMissing Policy ]------------------------*/
namespace OnMissing
{

template <template <typename> class P, typename T>
concept Policy = HasResultT<P, T> && requires
{
  typename P<T>::result_t;
  {
    P<T>::notify(std::declval<otb::string_view>(), std::declval<otb::string_view>())
  } -> std::same_as<typename P<T>::result_t>;
};

template <typename T>
struct IgnoreAndReturnNone
{
  using result_t = std::optional<T>;
  static result_t notify(otb::string_view /*key*/, otb::string_view /*context*/)
  {
    return {};
  }
};

template <typename T>
struct WarnAndReturnNone
{
  using result_t = std::optional<T>;
  static result_t notify(otb::string_view key, otb::string_view context)
  {
    otbLogMacro(
        Warning,
        << "Unable to find '" << key << "' while " << context);
    return {};
  }
};

template <typename T>
struct Throw
{
  using result_t = T;
  static result_t notify(otb::string_view key, otb::string_view context)
  {
    otbGenericExceptionMacro(
        itk::ExceptionObject,
        "Unable to find '" << key << "' while " << context);
  }
};
}

/*-------------------------------[ value_or function ]-----------------------*/
template <
  typename T,
  template <typename> class    MissingKeyPolicy,
  template <typename> class    DecodingPolicy,
  ReadOnlyAssociativeContainer Map
>
requires OnMissing::Policy<MissingKeyPolicy, T>
&& otb::OnDecodingError::Policy<otb::details::impl_t<T, DecodingPolicy>>
[[nodiscard]]
inline auto value_or(
    Map const& kwl,
    std::string const& key,
    otb::string_view context)
  -> std::common_type_t<typename MissingKeyPolicy<T>::result_t, typename otb::details::impl_t<T, DecodingPolicy>::result_t>
{
  auto const& wh = kwl.find(key);
  if (wh != kwl.end())
  {
    auto const& value = wh->second;
    return otb::details::impl_t<T, DecodingPolicy>::decode_to(value, "extracting metadata key '"+key+"' " + context);
  }
  else
  {
    return MissingKeyPolicy<T>::notify(key, "extracting metadata key '"+key+"' " + context);
  }
}

template <
  typename T,
  template <typename> class    MissingKeyPolicy,
  typename                     DecodingPolicy,
  ReadOnlyAssociativeContainer Map
>
requires OnMissing::Policy<MissingKeyPolicy, T>
&& otb::OnDecodingError::Policy<otb::details::nested_impl_t<T, DecodingPolicy>>
[[nodiscard]]
inline auto value_or(
    Map const& kwl,
    std::string const& key,
    otb::string_view context)
  -> std::common_type_t<typename MissingKeyPolicy<T>::result_t, typename otb::details::nested_impl_t<T, DecodingPolicy>::result_t>
{
  auto const& wh = kwl.find(key);
  if (wh != kwl.end())
  {
    auto const& value = wh->second;
    return otb::details::nested_impl_t<T, DecodingPolicy>::decode_to(value, "extracting metadata key '"+key+"' " + context);
  }
  else
  {
    return MissingKeyPolicy<T>::notify(key, "extracting metadata key '"+key+"' " + context);
  }
}

} // otb namespace

#endif // otbSARMetadataUtilities_h

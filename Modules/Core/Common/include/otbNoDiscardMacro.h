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

#ifndef otbNoDiscardMacro_h
#define otbNoDiscardMacro_h

// MSVC doesn't have __has_attribute, let's silence it!
#ifndef __has_attribute
#  define __has_attribute(attr) 0
#endif

#if __has_cpp_attribute(nodiscard)
/** Provides a portable way to mark returns as non-discardable.
 * \todo Deprecate it once OTB drops C++14 compatibility
 */
#  define OTB_NODISCARD [[nodiscard]]
#elif __has_attribute(__warn_unused_result__)
#  define OTB_NODISCARD __attribute__((__warn_unused_result__))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#  define OTB_NODISCARD _Check_return_
#else
#  define OTB_NODISCARD
#endif

#endif // otbNoDiscardMacro_h

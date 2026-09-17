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

#ifndef otbNewMacro_h
#define otbNewMacro_h

/** C++ alternative to `itkNewMacro()` that supports construction parameters.
 * Defines a static `New(...)` creation function that forwards construction
 * parameters to constructors with parameters.
 *
 * \note Unfortunatelly _forwarding references_ are not compatible with
 * `std::initializer_list`. IOW, this macro that defines a generic `New()`
 * function cannot be used with constructors that expect
 * `std::initializer_list`.
 *
 * \note Unlike `itkNewMacro()`, this macro doesn't attempt to create the
 * instance through `itk::ObjectFactory::Create()`.
 */
#define otbNewMacro(x)                                  \
  otbFactoryLessNewMacro(x)                             \
  itkCloneMacro(x)

#define otbFactoryLessNewMacro(x)                       \
  template <typename... T>                              \
  static Pointer New(T&&... args)                       \
  {                                                     \
    Pointer smartPtr = new x(std::forward<T>(args)...); \
    smartPtr->UnRegister();                             \
    return smartPtr;                                    \
  }


#endif // otbNewMacro_h

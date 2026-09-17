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

#ifndef otbDecorateInplace_h
#define otbDecorateInplace_h

#include "otbMPLImageTypeExtractorTraits.h"
#include <utility>

namespace otb
{

/**
 * Decorate an input (image, image filter...) with an in-place image filter.
 * Helper function to simplify the instantiation of an in-place image filter
 * that operates on top of another filter. Indeed, the input + output image
 * filter type is automagically deduced.
 *
 * \tparam TInplaceFilter Type of the in-place image filter to instantiate
 * \tparam TInput  Auto-deduced input type
 * \tparam Args    Auto-deduced variadic list of the types of the parameters
 *                 forwarded to the `TInplaceFilter` constructor through its
 *                 `New` function.
 *
 * \param[in] input  Input source to transform with the filter built.
 * \param[in] args   Construction parameters for the filter built.
 *
 * \return an instance of `TInplaceFilter`
 * \post the new filter input is initialized with `input->GetOutput()`.
 */
template <template <class, class> class TInplaceFilter, typename TInput, typename... Args>
inline
auto DecorateInplace(TInput input, Args&&... args)
{
  using input_t  = get_image_type_t<TInput>;
  using output_t = typename input_t::OutputImageType;

  // TODO: actually in-place image filters may have different values for their
  // template parameters...
  auto decorator = TInplaceFilter<output_t, output_t>::New(
      std::forward<Args>(args)...);

  decorator->SetInput(input->GetOutput());
  return decorator;
}

}

#endif // otbDecorateInplace_h

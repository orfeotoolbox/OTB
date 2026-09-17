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

#ifndef otbAlgoErase_h
#define otbAlgoErase_h

namespace otb
{

/**
 * Erase elements matching a predicate from a _forward-access-container_.
 * \warning This function only works with lists, maps... Not vectors.
 * Use _Erase-Remove idiom_ with `std::vector`
 * \tparam TForwardContainerWithErase  Container type that has a `erase` function
 * \tparam TPredicate                  Predicate type
 *
 * \param[in,out] c      Container updated in-place.
 * \param[in] predicate  Actual predicate instance applied on container elements.
 */
template <typename TForwardContainerWithErase, typename TPredicate>
void erase_if(TForwardContainerWithErase& c, TPredicate predicate)
{
  for (auto it = c.begin(); it != c.end();)
  {
    if (predicate(*it))
      it = c.erase(it);
    else
      ++it;
  }
}


} // otb namespace


#endif // otbAlgoErase_h

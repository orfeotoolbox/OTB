/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8Value_h
#define otbRCC8Value_h

namespace otb
{
// Coding enum system to represent the RCC8 relation values.
typedef enum
  {
  OTB_RCC8_DC,
  OTB_RCC8_EC,
  OTB_RCC8_PO,
  OTB_RCC8_TPP,
  OTB_RCC8_TPPI,
  OTB_RCC8_NTPP,
  OTB_RCC8_NTPPI,
  OTB_RCC8_EQ
  }
RCC8Value;
} // End namespace otb
#endif

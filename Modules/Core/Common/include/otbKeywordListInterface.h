/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbKeywordListInterface_h
#define otbKeywordListInterface_h

#include "OTBCommonExport.h"


#include "otbRPC.h"


namespace otb
{

/** \class KeywordListInterface
 *
 * \brief Interface class for image keyword-list adapters.
 *
 *
 * \ingroup OTBMetadata
 */
struct OTBCommon_EXPORT KeywordListInterface
{
  using Self = KeywordListInterface;

  virtual RPC GetRPC() const = 0;
};

} // end of namespace otb.

#endif // otbKeywordListInterface_h

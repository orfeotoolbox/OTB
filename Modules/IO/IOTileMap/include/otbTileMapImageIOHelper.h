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

#ifndef otbTileMapImageIOHelper_h
#define otbTileMapImageIOHelper_h


#include "itkObject.h"
#include "itkObjectFactory.h"

#include <string>
#include <sstream>

#include "OTBIOTileMapExport.h"

namespace otb
{

/** \class TileMapImageIOHelper
 *  \brief This class IO TileMapImage
 *
 * \deprecated in OTB 7.0
 *
 * \ingroup OTBIOTileMap
 */

class OTBIOTileMap_DEPRECATED_EXPORT TileMapImageIOHelper: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef TileMapImageIOHelper           Self;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef itk::Object                    Superclass;

  itkTypeMacro(TileMapImageIOHelper, itk::Object);
  itkNewMacro(Self);

  /** Correspondence between depth and scale for the quad tree paradigm*/
  const std::string ConvertDepthToScale(const unsigned int depth) const;

protected:
  TileMapImageIOHelper() {}
  ~TileMapImageIOHelper() override {}

private:
  TileMapImageIOHelper(const Self &) = delete;
  void operator =(const Self&) = delete;


}; // end class TileMapImageIOHelper

} // end namespace otb

#endif

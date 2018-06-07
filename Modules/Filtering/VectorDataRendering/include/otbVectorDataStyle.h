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


#ifndef otbVectorDataStyle_h
#define otbVectorDataStyle_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <mapnik/map.hpp>
#pragma GCC diagnostic pop
#else
#include <mapnik/map.hpp>
#endif

namespace otb
{
/** \class VectorDataStyle
 *   \brief Load style into a mapnik::Map
 *
 * \sa VectorDataToMapFilter
 *
 * \ingroup OTBVectorDataRendering
 */

class ITK_EXPORT VectorDataStyle : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataStyle               Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataStyle, LightObject);

  void LoadOSMStyle(mapnik::Map& mapnikMap) const;

  void LoadBinaryRasterizationStyle(mapnik::Map& mapnikMap) const;

  /** Get/Set the size of the scale factor. */
  double GetScaleFactor() const
  {
    return m_ScaleFactor;
  }
  void SetScaleFactor(double scale)
  {
    m_ScaleFactor = scale;
  }

protected:
  /** Constructor */
  VectorDataStyle() :
    m_ScaleFactor(1.0){};
  /** Destructor */
  virtual ~VectorDataStyle() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  VectorDataStyle(const Self &) = delete;
  void operator =(const Self&) = delete;

  //this parameter is used only in the case of sensor geometry
  //to adjust the scale
  double m_ScaleFactor;

}; // end class
} // end namespace otb

#endif

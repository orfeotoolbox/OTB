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

#ifndef otbRCC8VertexWithRegionCenter_h
#define otbRCC8VertexWithRegionCenter_h

#include "otbRCC8VertexBase.h"

namespace otb
{
/** \class RCC8VertexWithRegionCenter
 *  \brief Class to represent a vertex in a RCC8 Graph with the compacity value of
 *  the represented region.
 *
 * \sa RCC8Graph, RCC8Edge, RCC8VertexBase
 *
 * \ingroup OTBRCC8
 */
template <class TLabel, class TPrecision = float>
class ITK_EXPORT RCC8VertexWithRegionCenter
  : public RCC8VertexBase<TLabel>
{
public:
  /** Standard class typedefs */
  typedef RCC8VertexWithRegionCenter    Self;
  typedef otb::RCC8VertexBase<TLabel>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8VertexWithRegionCenter, RCC8VertexBase);
  /** Input image associated typedefs*/
  typedef TLabel LabelType;
  /** Precision  typedef */
  typedef TPrecision PrecisionType;
  /** char* vector attributes */
  typedef  typename Superclass::AttributesMapType AttributesMapType;

  itkGetMacro(RegionCenterXCoordinate, PrecisionType);
  itkSetMacro(RegionCenterXCoordinate, PrecisionType);
  itkGetMacro(RegionCenterYCoordinate, PrecisionType);
  itkSetMacro(RegionCenterYCoordinate, PrecisionType);
  /**
   * Set the VertexWithRegionCenter attributes from the attributes vector.
   * \param attributes The vector containing the parsed attributes.
   */
  void SetAttributesMap(AttributesMapType attributes);
  /**
   * Get an attributes vector representing the VertexWithRegionCenter attributes.
   * \return The attributes vector
   */
  AttributesMapType GetAttributesMap(void);

protected:
  /** Constructor */
  RCC8VertexWithRegionCenter();
  /** Desctructor */
  ~RCC8VertexWithRegionCenter() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** The compacity */
  PrecisionType m_RegionCenterXCoordinate;
  PrecisionType m_RegionCenterYCoordinate;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexWithRegionCenter.hxx"
#endif

#endif

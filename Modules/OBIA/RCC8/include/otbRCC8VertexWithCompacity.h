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

#ifndef otbRCC8VertexWithCompacity_h
#define otbRCC8VertexWithCompacity_h

#include "otbRCC8VertexBase.h"

namespace otb
{
/** \class RCC8VertexWithCompacity
 *  \brief Class to represent a vertex in a RCC8 Graph with the compacity value of
 *  the represented region.
 *
 * \sa RCC8Graph, RCC8Edge, RCC8VertexBase
 *
 * \ingroup OTBRCC8
 */
template <class TPath, class TPrecision = float>
class ITK_EXPORT RCC8VertexWithCompacity
  : public RCC8VertexBase<TPath>
{
public:
  /** Standard class typedefs */
  typedef RCC8VertexWithCompacity       Self;
  typedef otb::RCC8VertexBase<TPath>    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8VertexWithCompacity, RCC8VertexBase);
  /** Input image associated typedefs*/
  typedef TPath PathType;
  /** Precision  typedef */
  typedef TPrecision PrecisionType;
  /** char* vector attributes */
  typedef  typename Superclass::AttributesMapType AttributesMapType;

  /** Segmentation image index accessors */
  itkGetMacro(Compacity, PrecisionType);
  itkSetMacro(Compacity, PrecisionType);
  /**
   * Set the VertexWithCompacity attributes from the attributes vector.
   * \param attributes The vector containing the parsed attributes.
   */
  void SetAttributesMap(AttributesMapType attributes) override;
  /**
   * Get an attributes vector representing the VertexWithCompacity attributes.
   * \return The attributes vector
   */
  AttributesMapType GetAttributesMap(void) override;

protected:
  /** Constructor */
  RCC8VertexWithCompacity();
  /** Desctructor */
  ~RCC8VertexWithCompacity() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** The compacity */
  PrecisionType m_Compacity;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexWithCompacity.hxx"
#endif

#endif

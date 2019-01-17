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

#ifndef otbOrthoRectificationFilter_h
#define otbOrthoRectificationFilter_h

#include <sstream>
#include <stdio.h>

#include "otbGenericRSResampleImageFilter.h"
#include "otbCompositeTransform.h"
#include "otbInverseSensorModel.h"

#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{
/** \class OrthoRectificationFilter
 *
 * \brief Class for Orthorectifying an image
 *
 * This class is used to apply map projection and sensor model transformation
 * to orthorectify an image, with or without DEM.
 *
 * \ingroup Projection
 *
 * \example Projections/OrthoRectificationExample.cxx
 *
 *
 * \ingroup OTBProjection
 */

template <class TInputImage, class TOutputImage, class TMapProjection, class TInterpolatorPrecision = double>
class ITK_EXPORT OrthoRectificationFilter :
    public GenericRSResampleImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef GenericRSResampleImageFilter<TInputImage,
      TOutputImage>                     Superclass;
  typedef OrthoRectificationFilter      Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** types definitions */
  typedef typename TInputImage::IndexType   IndexType;
  typedef typename TInputImage::SizeType    SizeType;
  typedef typename TInputImage::SpacingType SpacingType;
  typedef typename TInputImage::PointType   PointType;
  typedef typename TInputImage::RegionType  RegionType;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** output map projection */
  typedef TMapProjection                   MapProjectionType;
  typedef typename TMapProjection::Pointer MapProjectionPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OrthoRectificationFilter, GenericRSResampleImageFilter);

  /** Accessors */
  virtual void SetMapProjection(MapProjectionType* arg)
  {
    if (this->m_MapProjection != arg)
      {
      this->m_MapProjection = arg;
      this->Modified();
      }
  }
  itkGetObjectMacro(MapProjection, MapProjectionType);

protected:
  OrthoRectificationFilter();
  ~OrthoRectificationFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateInputRequestedRegion() override;
  void GenerateOutputInformation(void) override;

private:
  OrthoRectificationFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Map Projection used to transform cartographic coordinates in geographic  coordinates */
  MapProjectionPointerType m_MapProjection;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOrthoRectificationFilter.hxx"
#endif

#endif

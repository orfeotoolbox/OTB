/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOrthoRectificationFilter_h
#define __otbOrthoRectificationFilter_h

#include <iostream>
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
  virtual ~OrthoRectificationFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  virtual void GenerateInputRequestedRegion();
  virtual void GenerateOutputInformation(void);

private:
  OrthoRectificationFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  
  /** Map Projection used to transform cartographic coordinates in geographic  coordinates */
  MapProjectionPointerType m_MapProjection;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOrthoRectificationFilter.txx"
#endif

#endif

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
#ifndef __otbHillShadingFilter_h
#define __otbHillShadingFilter_h

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbHillShadingFunctor.h"

namespace otb
{
/** \class HillShadingFilter
 *  \brief Filter to compute the lambertian of a surface given as an image
 *
 *  The light source is assumed to be at a given elevation and azimuth
 *  (by default \f$ \pi/4 \f$ and \f$ \pi/6 \f$ respectively). This is used
 *  to generate hill shading representation of relief. The output is
 *  a value between 0 and 1.
 *
 *  \example BasicFilters/HillShadingExample.cxx
*/
template <class TInputImage, class TOutputImage>
class ITK_EXPORT HillShadingFilter :
  public UnaryFunctorNeighborhoodImageFilter<TInputImage, TOutputImage,
      Functor::HillShadingFunctor<itk::ConstNeighborhoodIterator<TInputImage>,
          TInputImage, typename TOutputImage::PixelType>
      >
{

public:

  /** Standard class typedefs. */
  typedef  HillShadingFilter Self;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HillShadingFilter, UnaryFunctorNeighborhoodImageFilter);

  double GetAzimuthLight() const
  {
    return this->GetFunctor().GetAzimuthLight();
  }

  void SetAzimuthLight(double az)
  {
    this->GetFunctor().SetAzimuthLight(az);
  }

  double GetElevationLight() const
  {
    return this->GetFunctor().GetElevationLight();
  }

  void SetElevationLight(double el)
  {
    this->GetFunctor().SetElevationLight(el);
  }

protected:
  HillShadingFilter() {}
  virtual ~HillShadingFilter() {}

private:
  HillShadingFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

}

#endif

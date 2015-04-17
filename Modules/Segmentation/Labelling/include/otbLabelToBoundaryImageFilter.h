/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRelabelComponentImageFilter.h, v $
  Language:  C++
  Date:      $Date: 2009-04-27 22:58:48 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLabelToBoundaryImageFilter_h
#define __otbLabelToBoundaryImageFilter_h

#include "otbUnaryFunctorNeighborhoodImageFilter.h"

namespace otb
{
namespace Functor
{

/** 
 * \class LabelToBoundaryFunctor
 * 
 * \brief Functor to extract segmentation boundaries
 * 
 * Functor intended to work with 3x3 neighborhood and scalar label image
 * The generated boundary is 1-pixel wide, so it is not symetric. 
 * Output value is 1 on the boundaries and 0 in the background
 * 
 */
template < class TInput, class TOutput >
class LabelToBoundaryFunctor
{
public:
  LabelToBoundaryFunctor() { }
  virtual ~LabelToBoundaryFunctor() { }

  TOutput operator() ( const TInput & input )
  {
    unsigned char output = 1;
    if (input.GetCenterPixel() == input.GetPixel(5) &&
        input.GetCenterPixel() == input.GetPixel(7) &&
        input.GetCenterPixel() == input.GetPixel(8))
      {
      output = 0;
      }
    return static_cast<TOutput>(output);
  }
}; // end of class

} // end of Functor namespace

/** 
 * \class LabelToBoundaryImageFilter
 * 
 * \brief Filter to extract boundaries of a label image
 * 
 * Filter intended to work with a scalar label image.
 * The generated boundary is 1-pixel wide, so it is not symetric. 
 * Output value is 1 on the boundaries and 0 in the background
 * 
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT LabelToBoundaryImageFilter
  : public UnaryFunctorNeighborhoodImageFilter< TInputImage, TOutputImage,
            Functor::LabelToBoundaryFunctor<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  typedef LabelToBoundaryImageFilter                Self;
  typedef UnaryFunctorNeighborhoodImageFilter< TInputImage, TOutputImage,
            Functor::LabelToBoundaryFunctor<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;
  
  itkNewMacro(Self);
  
  itkTypeMacro(LabelToBoundaryImageFilter,UnaryFunctorNeighborhoodImageFilter);
  
protected:
  LabelToBoundaryImageFilter()
  {
    this->SetRadius(1);
  }
  virtual ~LabelToBoundaryImageFilter() { }

private:
  LabelToBoundaryImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class

} // end of otb namespace

#endif

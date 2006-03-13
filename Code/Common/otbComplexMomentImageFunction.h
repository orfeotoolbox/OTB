/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:15 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbComplexMomentImageFunction_h
#define _otbComplexMomentImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"
#include <complex>


namespace otb
{

/**
 * \class ComplexMomentImageFunction
 * \brief Calculate the complex moment value in the full image.
 *
 * Calculate the complex moment value over an image. 
 * The implemented equation is:
 *
 *  \f$ C_{p,q}=\int\int_{} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f$
 *
 * With:
 *  + \f$(x,y)$f\ pixel localization;
 *  + \f$ f(x,y) $f\  the pixel value over the \f$(x,y)$f\ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 * 
 * \ingroup ImageFunctions
 */
template <class TInputImage, class TCoordRep = float >
class ITK_EXPORT ComplexMomentImageFunction :
  public itk::ImageFunction< TInputImage, 
                        ITK_TYPENAME itk::NumericTraits< std::complex<float> >, 
			TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentImageFunction                                 Self;
  typedef itk::ImageFunction<TInputImage,std::complex<float>, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** OutputType typdef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;
  
  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  typedef std::complex<float>  ComplexType;
  
  /** Evaluate the function */
  virtual ComplexType Evaluate( );

  itkSetMacro(p, unsigned int);
  itkGetConstReferenceMacro(p, unsigned int);
  itkSetMacro(q, unsigned int);
  itkGetConstReferenceMacro(q, unsigned int);

protected:
  ComplexMomentImageFunction();
  ~ComplexMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

//  unsigned int m_NeighborhoodRadius;
  unsigned int m_p;
  unsigned int m_q;
   

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentImageFunction.txx"
#endif

#endif


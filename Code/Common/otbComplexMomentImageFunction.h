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
template < class TInput, 
           class TOutput = std::complex<float >,
	   class TCoordRep = float >
class ITK_EXPORT ComplexMomentImageFunction :
  public itk::ImageFunction< TInput, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentImageFunction                                 Self;
  typedef itk::ImageFunction< TInput, TOutput,TCoordRep >             Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInput                                    InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   ComplexType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputType::ImageDimension);
  			 
  /** Evaluate at the specified input position */
//  virtual ComplexType Evaluate(const InputType& input) const;

  /** Evalulate the function at specified index */
  virtual ComplexType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evaluate the function at non-integer positions */
  virtual ComplexType Evaluate( const PointType& point ) const
    { 
      IndexType index;
      this->ConvertPointToNearestIndex( point, index );
      return this->EvaluateAtIndex( index ); 
    }
  virtual ComplexType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
      IndexType index;
      this->ConvertContinuousIndexToNearestIndex( cindex, index );
      return this->EvaluateAtIndex( index ) ; 
    }
  itkSetMacro(P, unsigned int);
  itkGetConstReferenceMacro(P, unsigned int);
  itkSetMacro(Q, unsigned int);
  itkGetConstReferenceMacro(Q, unsigned int);

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro( NeighborhoodRadius, int );
  itkGetConstReferenceMacro( NeighborhoodRadius, int );

protected:
  ComplexMomentImageFunction();
  ~ComplexMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  unsigned int m_P;
  unsigned int m_Q;
  int m_NeighborhoodRadius;
  int m_NeighborhoodRadiusX;
  int m_NeighborhoodRadiusY;
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentImageFunction.txx"
#endif

#endif


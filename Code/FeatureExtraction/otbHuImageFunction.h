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
#ifndef __otbHuImageFunction_h
#define __otbHuImageFunction_h

#include "otbRealMomentImageFunction.h"

namespace otb
{

/**
 * \class HuImageFunction
 * \brief Calculate the Hu's invariant parameter.
 *
 * Calculate the Hu's invariant over an image defined as:
 *
 * - \f$ \phi_{1} = c_{11} \f$
 * - \f$ \phi_{2} = c_{20} c_{02} \f$
 * - \f$ \phi_{3} = c_{30} c_{03} \f$
 * - \f$ \phi_{4} = c_{21} c_{12} \f$
 * - \f$ \phi_{5} = Re (c_{30} c_{12}^{3}) \f$
 * - \f$ \phi_{6} = Re (c_{20} c_{12}^{2}) \f$
 * - \f$ \phi_{7} = Im (c_{30} c_{12}^{3}) \f$
 *
 * With :
 *
 *  \f[  c_{p,q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * And:
 *  - \f$(x,y)\f$ pixel localization;
 *  - \f$ f(x,y)\f$  the pixel value over the \f$(x,y)\f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 */
//  public itk::ImageFunction< TInput, TOutput,TCoordRep >

template < class TInput,
class TOutput    = double,
class TPrecision = double,
class TCoordRep  = float >
class ITK_EXPORT HuImageFunction :
      public RealMomentImageFunction< TInput, TOutput, TPrecision, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef HuImageFunction                                           Self;
  typedef RealMomentImageFunction< TInput, TOutput, TPrecision, TCoordRep >      Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HuImageFunction, RealMomentImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInput                                    InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;

  typedef typename Superclass::RealType             RealType;
  typedef typename std::complex<RealType>           ComplexType;

  /** Type for calculation precision */
  typedef typename Superclass::PrecisionType        PrecisionType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputType::ImageDimension);


  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex( const IndexType& index ) const;

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PointType& point ) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index );
  }
  virtual RealType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex ) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index );
  }


  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetClampMacro(MomentNumber,short,1,7);
  itkGetConstReferenceMacro( MomentNumber, short );

protected:
  HuImageFunction();
  ~HuImageFunction() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HuImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  short m_MomentNumber;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHuImageFunction.txx"
#endif

#endif


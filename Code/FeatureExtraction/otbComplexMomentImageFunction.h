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
#ifndef __otbComplexMomentImageFunction_h
#define __otbComplexMomentImageFunction_h

#include "itkImageFunction.h"

#include <complex>

namespace otb
{

/**
 * \class ComplexMomentImageFunction
 * \brief Calculate the complex moment value in the specified
 * neighborhood.
 *
 * Calculate the complex moment values over a specified neighborhood.
 * The implemented equation is:
 *
 *  \f[  c_{p,q}=\int_{-\infty}^{\infty} \int_{-\infty}^{\infty} (x+iy)^{p} \cdot (x-iy)^{q} \cdot f(x,y) \cdot
 dx \cdot dy \f]
 *
 * With:
 *
 *   - \f$ (x,y) \f$ pixel localization;
 *   - \f$ f(x,y) \f$  the pixel value over the \f$(x,y) \f$ coordinate.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT ComplexMomentImageFunction :
    public itk::ImageFunction <TInputImage,
      std::vector< std::vector< std::complex<double> > >,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef ComplexMomentImageFunction                                           Self;
  typedef itk::ImageFunction<TInputImage, 
                             std::vector<
                             std::vector<
                             std::complex<double> > >, 
                             TCoordRep>                                        Superclass;
  typedef itk::SmartPointer<Self>                                              Pointer;
  typedef itk::SmartPointer<const Self>                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComplexMomentImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::OutputType          OutputType;
  
  typedef double                                   ScalarRealType;
  typedef typename std::complex<ScalarRealType>    ScalarComplexType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  
  /** Evalulate the function at specified index */
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;
  
  /** Evaluate the function at non-integer positions */
  virtual OutputType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }
  
  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated 
   */
  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );
  
  itkSetMacro(Pmax, unsigned int);
  itkGetConstReferenceMacro(Pmax, unsigned int);
  itkSetMacro(Qmax, unsigned int);
  itkGetConstReferenceMacro(Qmax, unsigned int);

protected:
  ComplexMomentImageFunction();
  virtual ~ComplexMomentImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ComplexMomentImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComplexMomentImageFunction.txx"
#endif

#endif

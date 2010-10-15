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
#ifndef __otbRealMomentImageFunction_h
#define __otbRealMomentImageFunction_h

#include "itkImageFunction.h"

namespace otb
{

/**
 * \class RealMomentImageFunction
 * \brief Calculate the moment values in the specified
 * neighborhood.
 * 
 * Calculate the real moment values over a specified neighborhood.
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT RealMomentImageFunction :
    public itk::ImageFunction<TInputImage,
                              std::vector< std::vector< 
                              ITK_TYPENAME itk::NumericTraits<
                              typename TInputImage::PixelType>::RealType > >,
                              TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef RealMomentImageFunction                                              Self;
  typedef itk::ImageFunction<TInputImage, 
                             std::vector< std::vector< 
                             ITK_TYPENAME itk::NumericTraits<
                             typename TInputImage::PixelType>::RealType > >, 
                             TCoordRep>                                        Superclass;
  typedef itk::SmartPointer<Self>                                              Pointer;
  typedef itk::SmartPointer<const Self>                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType          RealType;
  typedef float                                    ScalarRealType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex(const IndexType& index) const;

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual RealType EvaluateAtContinuousIndex(
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
  RealMomentImageFunction();
  virtual ~RealMomentImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RealMomentImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);           //purposely not implemented

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRealMomentImageFunction.txx"
#endif

#endif

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
#ifndef __otbRealMomentsImageFunction_h
#define __otbRealMomentsImageFunction_h

#include "itkImageFunction.h"

namespace otb
{

/**
 * \class RealMomentsImageFunction
 * \brief Calculate the moment values in the specified
 * neighborhood.
 *
 * Calculate the real moment values over a specified neighborhood.
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT RealMomentsImageFunction :
    public itk::ImageFunction<TInputImage,
                              std::vector< std::vector<
                              typename itk::NumericTraits<
                              typename TInputImage::PixelType>::RealType > >,
                              TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef RealMomentsImageFunction                                             Self;
  typedef itk::ImageFunction<TInputImage,
                             std::vector< std::vector<
                             typename itk::NumericTraits<
                             typename TInputImage::PixelType>::RealType > >,
                             TCoordRep>                                        Superclass;
  typedef itk::SmartPointer<Self>                                              Pointer;
  typedef itk::SmartPointer<const Self>                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RealMomentsImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType          OutputType;
  typedef float                                    ScalarRealType;

  typedef TCoordRep                                CoordRepType;

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
  RealMomentsImageFunction();
  virtual ~RealMomentsImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RealMomentsImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);           //purposely not implemented

  unsigned int m_Pmax;
  unsigned int m_Qmax;
  unsigned int m_NeighborhoodRadius;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRealMomentsImageFunction.txx"
#endif

#endif

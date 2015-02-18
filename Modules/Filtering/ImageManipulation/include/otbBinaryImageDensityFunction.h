/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBinaryImageDensityFunction_h
#define __otbBinaryImageDensityFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * \class BinaryImageDensityFunction
 * \brief Calculate the density pixel of  in the neighborhood of a pixel
 *
 * Calculate the variance pixel value over the standard 8, 26, etc. connected
 * neighborhood.  This calculation uses a ZeroFluxNeumannBoundaryCondition.
 *
 * If called with a ContinuousIndex or Point, the calculation is performed
 * at the nearest neighbor.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double ).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT BinaryImageDensityFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef BinaryImageDensityFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::RealType,
      TCoordRep>                                          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryImageDensityFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::SizeType        RadiusType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Datatype used for the density */
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType
  RealType;

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
      statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, RadiusType);
  itkGetConstReferenceMacro(NeighborhoodRadius, RadiusType);
  void SetNeighborhoodRadius(unsigned int rad)
  {
    m_NeighborhoodRadius.Fill(rad);
    this->Modified();
  }

protected:
  BinaryImageDensityFunction();
  virtual ~BinaryImageDensityFunction(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  BinaryImageDensityFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  RadiusType m_NeighborhoodRadius;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbBinaryImageDensityFunction.txx"
#endif

#endif

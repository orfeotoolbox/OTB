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
#ifndef __otbGroundSpacingImageFunction_h
#define __otbGroundSpacingImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"

#include "otbGenericRSTransform.h"

namespace otb
{

/**
 * \class GroundSpacingImageFunction
 * \brief Calculate the approximate ground spacing in X and Y directions
 *
 * This uses the ‘Haversine’ formula to calculate great-circle distances between 
 * the two points – that is, the shortest distance over the earth’s surface – 
 * giving an ‘as-the-crow-flies’ distance between the points (ignoring any hills!).
 *
 * \ingroup ImageFunctions
 */
template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT GroundSpacingImageFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<std::complex<float> >::FloatType,
                            TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef GroundSpacingImageFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<std::complex<float> >::FloatType,
                             TCoordRep>                                          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GroundSpacingImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef otb::GenericRSTransform<double> TransformType;
  
  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Datatype used for the density */
  typedef  typename itk::NumericTraits<std::complex<float> >::FloatType
  FloatType;
  typedef  typename itk::NumericTraits<std::complex<float> >::ValueType
  ValueType;
  typedef typename IndexType::IndexValueType                IndexValueType;
  
  /** Evalulate the function at specified index */
  virtual FloatType EvaluateAtIndex(const IndexType& index) const;

  /** Evaluate the function at non-integer positions */
  virtual FloatType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual FloatType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the transformation definition of the input image **/
  itkSetObjectMacro(Transform, TransformType);
  itkGetObjectMacro(Transform, TransformType);

  PointType GetPixelLocation(const IndexType& index) const;
protected:
  GroundSpacingImageFunction();
  virtual ~GroundSpacingImageFunction(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GroundSpacingImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
  
  TransformType::Pointer    m_Transform;
  ValueType m_R;
  ValueType m_deg2radCoef;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbGroundSpacingImageFunction.txx"
#endif

#endif

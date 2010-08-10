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
#ifndef __otbSarParametricMapFunction_h
#define __otbSarParametricMapFunction_h

#include "itkNumericTraits.h"
#include "itkImageFunction.h"
#include "itkPointSet.h"
#include "itkPoint.h"

namespace otb
{

/**
 * \class SarParametricMapFunction
 * \brief Calculate the density pixel of  in the neighborhood of a pixel
 *
 *
 * \ingroup ImageFunctions
 */
 
template <class TInputImage, class TCoordRep = float>
class ITK_EXPORT SarParametricMapFunction :
  public itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::ScalarRealType,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef SarParametricMapFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::NumericTraits<typename TInputImage::PixelType>::ScalarRealType,
      TCoordRep>                                          Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarParametricMapFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  typedef itk::PointSet<OutputType, ImageDimension>      PointSetType;
  typedef typename PointSetType::Pointer                 PointSetPointer;
  typedef typename PointSetType::ConstPointer            PointSetConstPointer;
  typedef typename PointSetType::PointType               PointType;

//  typedef typename PointSetType::PointType PointType;

  /** Datatype used for the evaluation */
  typedef typename itk::NumericTraits<InputPixelType>::ScalarRealType                       RealType;

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


  /** Get/Set the PointSet pointer*/
//  itkSetMacro(PointSet, PointSetPointer);
  itkGetConstReferenceMacro(PointSet, PointSetPointer);
  void SetPointSet(PointSetPointer val)
  {
    m_IsInitialize = false;
    m_PointSet = val;
    EvaluateParametricCoefficient();
    this->Modified();
  }	

  /** Get/Set the Coeff pointer*/
  itkSetMacro(Coeff, PointSetPointer);
  itkGetConstReferenceMacro(Coeff, PointSetPointer);

  /** Get/Set the Coeff pointer*/
  itkSetMacro(IsInitialize, bool);
  itkGetConstReferenceMacro(IsInitialize, bool);

  /** Get/Set the Coeff pointer*/
  itkSetMacro(UsingClosestPointMethod, bool);
  itkGetConstReferenceMacro(UsingClosestPointMethod, bool);
  
  /** Get/Set the PolynomalSize pointer*/
  void SetPolynomalSize(const IndexType PolynomalSize);

  /** Evaluate parametric coefficent from pointset */
  void EvaluateParametricCoefficient();

  /** Set constante value for evaluation*/ 
  void SetConstantValue(const RealType& value);


protected:
  SarParametricMapFunction();
  virtual ~SarParametricMapFunction(){}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SarParametricMapFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented


  PointSetPointer m_PointSet;
  PointSetPointer m_Coeff;
  bool            m_IsInitialize;  
  bool            m_UsingClosestPointMethod;  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarParametricMapFunction.txx"
#endif

#endif

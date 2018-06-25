/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSarParametricMapFunction_h
#define otbSarParametricMapFunction_h

#include "itkImageFunction.h"
#include "itkPointSet.h"
#include "itkVariableSizeMatrix.h"

namespace otb
{

/**
 * \class SarParametricMapFunction
 * \brief Evaluates a parametric bidimensionnal polynomial model from a PointSet
 *
 *  \note This is an internal class used by the Sar calibration framework.
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBSARCalibration
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
      TCoordRep>                        Superclass;
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
  typedef typename PointSetType::PixelType               PixelType;

  typedef itk::VariableSizeMatrix<double>                MatrixType;

  /** Datatype used for the evaluation */
  typedef typename itk::NumericTraits<InputPixelType>::ScalarRealType                       RealType;

  /** Evaluate the function at specific positions */
  RealType Evaluate(const PointType& point) const override;

  /** Evalulate the function at specified index */
  RealType EvaluateAtIndex(const IndexType& index) const override
  {
    PointType point;
    point[0] = static_cast<typename PointType::ValueType>(index[0]);
    point[1] = static_cast<typename PointType::ValueType>(index[1]);
    return this->Evaluate(point);
  }

  RealType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the PointSet pointer*/
  itkGetObjectMacro(PointSet, PointSetType);
  itkGetConstObjectMacro(PointSet, PointSetType);
  void SetPointSet(PointSetPointer val)
  {
    m_IsInitialize = false;
    m_PointSet = val;
    this->Modified();
  }

  /** Get/Set the Coeff pointer*/
  itkSetMacro(Coeff, MatrixType);
  itkGetMacro(Coeff, MatrixType);
  itkGetConstMacro(Coeff, MatrixType);

  /** Get/Set the Coeff pointer*/
  itkGetConstReferenceMacro(IsInitialize, bool);

  /** Get/Set the PolynomalSize pointer*/
  void SetPolynomalSize(const IndexType PolynomalSize);

  /** Evaluate parametric coefficient from pointset */
  void EvaluateParametricCoefficient();

  /** Set constante value for evaluation*/
  void SetConstantValue(const RealType& value);

protected:
  SarParametricMapFunction();
  ~SarParametricMapFunction() override{}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SarParametricMapFunction(const Self &) = delete;
  void operator =(const Self&) = delete;

  double Horner(PointType point) const;

  PointSetPointer m_PointSet;
  MatrixType      m_Coeff;
  bool            m_IsInitialize;
  double          m_ProductWidth; /// the width of the complete product (read from metadata)
  double          m_ProductHeight; /// the height of the complete product (read from metadata)
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarParametricMapFunction.hxx"
#endif

#endif

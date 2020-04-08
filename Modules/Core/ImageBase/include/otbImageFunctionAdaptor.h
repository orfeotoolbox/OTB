/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageFunctionAdaptor_h
#define otbImageFunctionAdaptor_h

#include "itkImageFunction.h"

#include "otbVariableLengthVectorConverter.h"
#include "OTBImageBaseExport.h"

#include <complex>

namespace otb
{
/**
 * \class ImageFunctionAdaptor
 * \brief Adapt the output of the internal image function to be a
 * itk::VariableLengthVector.
 *
 * This class is templated over the ImageFunction type and the
 * coordinate representation type.
 *
 *
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBImageBase
 */

template <class TInternalImageFunctionType, class TOutputPrecision = double>
class OTBImageBase_EXPORT_TEMPLATE ImageFunctionAdaptor
    : public itk::ImageFunction<typename TInternalImageFunctionType::InputImageType, itk::VariableLengthVector<TOutputPrecision>,
                                typename TInternalImageFunctionType::CoordRepType>
{
public:
  // Standard class typedefs. //
  typedef ImageFunctionAdaptor Self;
  typedef itk::ImageFunction<typename TInternalImageFunctionType::InputImageType, itk::VariableLengthVector<TOutputPrecision>,
                             typename TInternalImageFunctionType::CoordRepType>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Run-time type information (and related methods). //
  itkTypeMacro(ImageFunctionAdaptor, ImageFunction);

  // Method for creation through the object factory. //
  itkNewMacro(Self);

  // InputImageType typedef support. //
  typedef typename TInternalImageFunctionType::InputImageType InputImageType;
  typedef typename TInternalImageFunctionType::CoordRepType   CoordRepType;
  typedef typename Superclass::IndexType                      IndexType;
  typedef typename Superclass::ContinuousIndexType            ContinuousIndexType;
  typedef typename Superclass::PointType                      PointType;
  typedef typename Superclass::OutputType                     OutputType;
  typedef typename OutputType::ValueType                      OutputValueType;

  // InternalImageFunction related typedefs //
  typedef TInternalImageFunctionType                     InternalImageFunctionType;
  typedef typename InternalImageFunctionType::OutputType InternalImageFunctionOutputType;

  // Converter related typedefs //
  typedef VariableLengthVectorConverter<InternalImageFunctionOutputType, TOutputPrecision> ConverterType;


  // Dimension of the underlying image. //
  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  // Evalulate the function at specified index //
  OutputType EvaluateAtIndex(const IndexType& index) const override;

  // Evaluate the function at non-integer positions //
  OutputType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  // Accessors //
  itkGetConstMacro(InternalImageFunction, typename InternalImageFunctionType::Pointer);
  itkSetMacro(InternalImageFunction, typename InternalImageFunctionType::Pointer);

protected:
  ImageFunctionAdaptor();
  ~ImageFunctionAdaptor() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageFunctionAdaptor(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Internal Image Function //
  typename InternalImageFunctionType::Pointer m_InternalImageFunction;
  // Converter //
  typename ConverterType::Pointer m_Converter;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFunctionAdaptor.hxx"
#endif

#endif

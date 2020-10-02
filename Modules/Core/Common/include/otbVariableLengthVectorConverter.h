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

#ifndef otbVariableLengthVectorConverter_h
#define otbVariableLengthVectorConverter_h

#include "itkProcessObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkHistogram.h"
#include "itkSmartPointer.h"


namespace otb
{
/**
 * \class VariableLengthVectorConverter
 * \brief Convert any data container type into a VariableLengthVector.
 *
 * To be usable, the desired conversion must be implemented through
 * partial specialisation mechanism.
 *
 *
 * \ingroup OTBCommon
 */

// Base
template <class TInputType, class TPrecisionType = double>
class ITK_EXPORT VariableLengthVectorConverter : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                          OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType> OutputType;
  typedef TInputType                                              InputType;

  OutputType Convert(InputType input)
  {
    OutputType output;
    output.SetSize(1);
    output.SetElement(0, static_cast<OutputPrecisionType>(input));
    return output;
  }

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Attempt to use inexistant implementation of the converter!" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

// VariableLengthVector
template <class TInternalInputType, class TPrecisionType>
class ITK_EXPORT VariableLengthVectorConverter<itk::VariableLengthVector<TInternalInputType>, TPrecisionType> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                          OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType> OutputType;
  typedef itk::VariableLengthVector<TInternalInputType>           InputType;

  OutputType Convert(InputType input)
  {
    return static_cast<OutputType>(input);
  }

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Attempt to use inexistant implementation of the converter!" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};


// Real Matrix
template <class TInternalInputType, class TPrecisionType>
class ITK_EXPORT VariableLengthVectorConverter<std::vector<std::vector<TInternalInputType>>, TPrecisionType> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                          OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType> OutputType;
  typedef typename std::vector<std::vector<TInternalInputType>>   InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: std::vector<std::vector<RealType>>  => VariableLengthVector<RealType>" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};


// Complex Matrix
template <class TInternalInputType, class TPrecisionType>
class ITK_EXPORT VariableLengthVectorConverter<std::vector<std::vector<std::complex<TInternalInputType>>>, TPrecisionType> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                                      OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>             OutputType;
  typedef typename std::vector<std::vector<std::complex<TInternalInputType>>> InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: std::vector<std::vector<std::complex<RealType>>>  => VariableLengthVector<RealType>" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};


// Fixed Array
template <class TInternalInputType, unsigned int VArrayDimension, class TPrecisionType>
class ITK_EXPORT VariableLengthVectorConverter<itk::FixedArray<TInternalInputType, VArrayDimension>, TPrecisionType> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                          OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType> OutputType;
  typedef typename itk::FixedArray<TInternalInputType, VArrayDimension> InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: itk::FixedArray<RealType, VArrayDimension>  => VariableLengthVector<RealType>" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

// Histogram
template <class TPixel, class TPrecisionType>
class ITK_EXPORT VariableLengthVectorConverter<typename itk::SmartPointer<itk::Statistics::Histogram<TPixel>>, TPrecisionType> : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TPrecisionType                                                 OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>        OutputType;
  typedef typename itk::SmartPointer<itk::Statistics::Histogram<TPixel>> InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter()
  {
  }
  ~VariableLengthVectorConverter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: itk::Statistics::Histogram<RealType, VMeasurementVectorSize, TFrequencyContainer>  => VariableLengthVector<RealType>" << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVariableLengthVectorConverter.hxx"
#endif

#endif

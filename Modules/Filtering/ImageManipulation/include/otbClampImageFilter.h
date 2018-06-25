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

#ifndef otbClampImageFilter_h
#define otbClampImageFilter_h

#include "otbConvertTypeFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class ClampImageFilter
 * \brief Clamp image values to be below, over, or between threhold values.
 *
 * ClampImageFilter  clamp image values to be between an upper
 * and lower value. Values lower than m_Lower values are set to lower,
 * and values greater than upper threshold are set to upper threshold
 * value.
 * This filter can also be used to cast any type of image into any other type
 * as long as those types are arithmetics or complex.
 *
 * By default lower and upper thresholds are set to the maximum and
 * minimum bounds of the image internal pixel value.
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
  template <class TInputImage, class TOutputImage=TInputImage>
  class ITK_EXPORT ClampImageFilter 
  : public itk::UnaryFunctorImageFilter< TInputImage , TOutputImage ,
      Functor::ConvertTypeFunctor <typename TInputImage::PixelType ,
                                   typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ClampImageFilter Self;
  typedef itk::UnaryFunctorImageFilter< TInputImage , TOutputImage ,
    Functor::ConvertTypeFunctor <typename TInputImage::PixelType ,
                                 typename TOutputImage::PixelType> >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( ClampImageFilter , itk::UnaryFunctorImageFilter );


  /** Some additional typedefs.  */
  typedef TInputImage                                  InputImageType;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageType::PixelType           InputImagePixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename itk::NumericTraits< OutputInternalPixelType >::ValueType OutputPixelValueType;


  /** The values greater than or equal to the value are set to \p thresh. */
  void ClampAbove(const OutputPixelValueType &thresh);

  /** The values less than or equal to the value are set to \p thresh. */
  void ClampBelow(const OutputPixelValueType &thresh);

  /** The values outside the range are set to \p lower or \p upper. */
  void ClampOutside(const OutputPixelValueType &lower, const OutputPixelValueType &upper);

  /** Set/Get methods to set the lower threshold */
  void SetLower(OutputPixelValueType val);

  itkGetConstMacro(Lower, OutputPixelValueType);

  /** Set/Get methods to set the upper threshold */
  void SetUpper(OutputPixelValueType val);

  itkGetConstMacro(Upper, OutputPixelValueType);


protected:
  ClampImageFilter();
  ~ClampImageFilter() override {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateOutputInformation(void) override
    {
    Superclass::GenerateOutputInformation();
    unsigned int sizeIn = this->GetInput()->GetNumberOfComponentsPerPixel();
    this->GetFunctor().SetInputComponents( sizeIn );
    this->GetOutput()->SetNumberOfComponentsPerPixel( 
      this->GetFunctor().GetOutputSize () );
    }

private:
  ClampImageFilter(const Self&) = delete ;
  void operator=(const Self&) = delete ;

  OutputPixelValueType m_Lower;
  OutputPixelValueType m_Upper;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbClampImageFilter.hxx"
#endif

#endif

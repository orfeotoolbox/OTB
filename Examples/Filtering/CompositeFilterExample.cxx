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


//  The composite filter we will build combines three filters: a gradient
//  magnitude operator, which will calculate the first-order derivative of
//  the image; a thresholding step to select edges over a given strength;
//  and finally a rescaling filter, to ensure the resulting image data is
//  visible by scaling the intensity to the full spectrum of the output
//  image type.
//
//  Since this filter takes an image and produces another image (of
//  identical type), we will specialize the ImageToImageFilter:


//  Next we include headers for the component filters:

#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "itkNumericTraits.h"
#include "otbImage.h"

//  Now we can declare the filter itself.  It is within the OTB namespace,
//  and we decide to make it use the same image type for both input and
//  output, thus the template declaration needs only one parameter.
//  Deriving from \code{ImageToImageFilter} provides default behavior for
//  several important aspects, notably allocating the output image (and
//  making it the same dimensions as the input).

namespace otb
{

template <class TImageType>
class ITK_EXPORT CompositeExampleImageFilter : public itk::ImageToImageFilter<TImageType, TImageType>
{
public:
  //  Next we have the standard declarations, used for object creation with
  //  the object factory:

  using Self         = CompositeExampleImageFilter<TImageType>;
  using Superclass   = itk::ImageToImageFilter<TImageType, TImageType>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(CompositeExampleImageFilter, itk::ImageToImageFilter);

  /** Display */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  //  Here we declare an alias (to save typing) for the image's pixel type,
  //  which determines the type of the threshold value.  We then use the
  //  convenience macros to define the Get and Set methods for this parameter.

  using PixelType = typename TImageType::PixelType;

  itkGetMacro(Threshold, PixelType);
  itkSetMacro(Threshold, PixelType);


protected:
  CompositeExampleImageFilter();

  //  Now we can declare the component filter types, templated over the
  //  enclosing image type:

protected:
  using ThresholdType = itk::ThresholdImageFilter<TImageType>;
  using GradientType  = itk::GradientMagnitudeImageFilter<TImageType, TImageType>;
  using RescalerType  = itk::RescaleIntensityImageFilter<TImageType, TImageType>;

  void GenerateData() override;

private:
  CompositeExampleImageFilter(Self&); // intentionally not implemented
  void operator=(const Self&);        // intentionally not implemented

  //  The component filters are declared as data members, all using the smart
  //  pointer types.

  typename GradientType::Pointer  m_GradientFilter;
  typename ThresholdType::Pointer m_ThresholdFilter;
  typename RescalerType::Pointer  m_RescaleFilter;

  PixelType m_Threshold;
};

} /* namespace otb */

//  The constructor sets up the pipeline, which involves creating the
//  stages, connecting them together, and setting default parameters.

namespace otb
{

template <class TImageType>
CompositeExampleImageFilter<TImageType>::CompositeExampleImageFilter()
{
  m_GradientFilter  = GradientType::New();
  m_ThresholdFilter = ThresholdType::New();
  m_RescaleFilter   = RescalerType::New();

  m_ThresholdFilter->SetInput(m_GradientFilter->GetOutput());
  m_RescaleFilter->SetInput(m_ThresholdFilter->GetOutput());

  m_Threshold = 1;

  m_RescaleFilter->SetOutputMinimum(itk::NumericTraits<PixelType>::NonpositiveMin());
  m_RescaleFilter->SetOutputMaximum(itk::NumericTraits<PixelType>::max());
}

//  The \code{GenerateData()} is where the composite magic happens.  First,
//  we connect the first component filter to the inputs of the composite
//  filter (the actual input, supplied by the upstream stage).  Then we
//  graft the output of the last stage onto the output of the composite,
//  which ensures the filter regions are updated.  We force the composite
//  pipeline to be processed by calling \code{Update()} on the final stage,
//  then graft the output back onto the output of the enclosing filter, so
//  it has the result available to the downstream filter.

template <class TImageType>
void CompositeExampleImageFilter<TImageType>::GenerateData()
{
  m_GradientFilter->SetInput(this->GetInput());

  m_ThresholdFilter->ThresholdBelow(this->m_Threshold);

  m_RescaleFilter->GraftOutput(this->GetOutput());
  m_RescaleFilter->Update();
  this->GraftOutput(m_RescaleFilter->GetOutput());
}

//  Finally we define the \code{PrintSelf} method, which (by convention)
//  prints the filter parameters.  Note how it invokes the superclass to
//  print itself first, and also how the indentation prefixes each line.
//
template <class TImageType>
void CompositeExampleImageFilter<TImageType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Threshold:" << this->m_Threshold << std::endl;
}

} /* end namespace otb */


//  It is important to note that in the above example, none of the internal
//  details of the pipeline were exposed to users of the class.  The interface
//  consisted of the Threshold parameter (which happened to change the value in
//  the component filter) and the regular ImageToImageFilter interface.  This
//  example pipeline is illustrated in
//  Figure~\ref{fig:CompositeExamplePipeline}.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile" << std::endl;
    return EXIT_FAILURE;
  }

  using ImageType  = otb::Image<short, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;

  using FilterType = otb::CompositeExampleImageFilter<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(argv[1]);
  filter->SetInput(reader->GetOutput());
  filter->SetThreshold(20);
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cerr << "Error: " << e << std::endl;
  }

  return EXIT_SUCCESS;
}

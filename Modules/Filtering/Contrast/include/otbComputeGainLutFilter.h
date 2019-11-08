/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbComputeGainLutFilter_h
#define otbComputeGainLutFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{

/** \class ComputeGainLutFilter
 *  \brief Compute the gain for each pixel value from a histogram
 *
 *  This class implements the second part of the CLAHE algorithm. It's aim
 *  is to compute a look up table filled with gain that need to be applied
 *  on the input to match the target histogram. To keep consistency with
 *  the other parts of the algorithm it needs the minimum and maximum value
 *  of the input image and also the theorical number of pixel per histogram.
 *
 * \ingroup OTBContrast
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComputeGainLutFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef ComputeGainLutFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename InputImageType::PixelType          HistoType;
  typedef typename OutputImageType::PixelType         LutType;
  typedef typename OutputImageType::InternalPixelType OutputPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeGainLutFilter, ImageToImageFilter);

  /** Get/Set macro to get/set the number of pixel by histogram */
  itkSetMacro(NbPixel, unsigned long);
  itkGetMacro(NbPixel, unsigned long);

  /** Get/Set macro to get/set the minimum value */
  itkSetMacro(Min, double);
  itkGetMacro(Min, double);

  /** Get/Set macro to get/set the maximum value */
  itkSetMacro(Max, double);
  itkGetMacro(Max, double);

protected:
  ComputeGainLutFilter();
  ~ComputeGainLutFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void BeforeThreadedGenerateData() override;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  ComputeGainLutFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Post-process the look up tabe to get a gain instead of a simple value */
  OutputPixelType PostProcess(unsigned int countMapValue, unsigned int countValue);

  /** Equalized input histogram regarding the target and filling the
    * corresponding look up table */
  void Equalized(const HistoType& inputHisto, HistoType& targetHisto, LutType& lut);

  /** Create target depending on the number of pixel in the input histogram */
  void CreateTarget(const HistoType& inputHisto, HistoType& targetHisto);
  // TODO Give the opportunity to choose the histogram target

  /** Check whether the input histogram has enough pixel to be meaningful */
  bool IsValid(const HistoType& inputHisto);

  double        m_Min;
  double        m_Max;
  double        m_Step;
  unsigned int  m_NbBin;
  unsigned long m_NbPixel;
};

// End namespace otb
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComputeGainLutFilter.hxx"
#endif

#endif

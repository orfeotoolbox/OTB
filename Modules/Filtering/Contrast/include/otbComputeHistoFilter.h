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

#ifndef otbComputeHistoFilter_h
#define otbComputeHistoFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "itkImageRegionIterator.h"

namespace otb
{

/** \class ComputeHistoFilter
 *  \brief Compute local histogram with several parameters
 *
 *  This class implements the first part of the CLAHE algorithm. It aims
 *  to compute local histogram with several input parameters such as
 *  nodata value, threshold, thumbnail size and number of bin. Mandatory parameters are min
 *  and max value as it will be used in the histogram computation.
 *
 * \ingroup OTBContrast
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComputeHistoFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef ComputeHistoFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename InputImageType::InternalPixelType InputPixelType;
  typedef typename InputImageType::IndexType         IndexType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename OutputImageType::RegionType       OutputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ComputeHistoFilter, ImageToImageFilter);

  /** Get/Set macro to get/set the number of bin. Default value is 256 */
  itkSetMacro(NbBin, unsigned int);
  itkGetMacro(NbBin, unsigned int);

  /** Get/Set macro to get/set the minimum value */
  itkSetMacro(Min, InputPixelType);
  itkGetMacro(Min, InputPixelType);

  /** Get/Set macro to get/set the maximum value */
  itkSetMacro(Max, InputPixelType);
  itkGetMacro(Max, InputPixelType);

  /** Get/Set macro to get/set the nodata value */
  itkSetMacro(NoData, InputPixelType);
  itkGetMacro(NoData, InputPixelType);

  /** Get/Set macro to get/set the nodata flag value */
  itkBooleanMacro(NoDataFlag);
  itkGetMacro(NoDataFlag, bool);
  itkSetMacro(NoDataFlag, bool);

  /** Get/Set macro to get/set the thumbnail's size */
  itkSetMacro(ThumbSize, SizeType);
  itkGetMacro(ThumbSize, SizeType);

  /** Get/Set macro to get/set the threshold parameter */
  itkSetMacro(Threshold, float);
  itkGetMacro(Threshold, float);

  typename OutputImageType::Pointer GetHistoOutput();

  virtual itk::ProcessObject::DataObjectPointer MakeOutput(itk::ProcessObject::DataObjectPointerArraySizeType idx) override;

  virtual itk::ProcessObject::DataObjectPointer MakeOutput(const itk::ProcessObject::DataObjectIdentifierType&) override;


protected:
  ComputeHistoFilter();
  ~ComputeHistoFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateInputRequestedRegion() override;

  void GenerateOutputInformation() override;

  // Call  BeforeThreadedGenerateData after getting the number of thread
  void GenerateData() override;


  void BeforeThreadedGenerateData() override;

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void AfterThreadedGenerateData() override;

  void GenerateOutputRequestedRegion(itk::DataObject* output) override;

private:
  ComputeHistoFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  void SetRequestedRegion(itk::ImageBase<2>* image);

  void ApplyThreshold(typename itk::ImageRegionIterator<OutputImageType> oit, unsigned int total);

  std::vector<typename OutputImageType::PixelType> m_HistoThread;
  InputPixelType                                   m_Min;
  InputPixelType                                   m_Max;
  InputPixelType                                   m_NoData;
  SizeType                                         m_ThumbSize{0,0};
  bool                                             m_NoDataFlag;
  double                                           m_Step;
  float                                            m_Threshold;
  unsigned int                                     m_NbBin;
  unsigned int                                     m_ValidThreads;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbComputeHistoFilter.hxx"
#endif

#endif

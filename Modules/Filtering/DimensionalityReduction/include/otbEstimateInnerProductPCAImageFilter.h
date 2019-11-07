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

#ifndef otbEstimateInnerProductPCAImageFilter_h
#define otbEstimateInnerProductPCAImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbStreamingInnerProductVectorImageFilter.h"

namespace otb
{
/** \class EstimateInnerProductPCAImageFilter
 * \brief Applies the Inner product to an vector image.
 * Implement the itk::ImagePCAShapeModelEstimator algorithms in streaming capabilities
 * Use by the  InnerProductPCAImageFilter composite filter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT EstimateInnerProductPCAImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef EstimateInnerProductPCAImageFilter Self;
  typedef typename itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EstimateInnerProductPCAImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                                 InputImageType;
  typedef typename InputImageType::Pointer            InputImagePointer;
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;

  typedef StreamingInnerProductVectorImageFilter<InputImageType> StreamingInnerProductType;
  typedef typename StreamingInnerProductType::Pointer            StreamingInnerProductPointer;
  typedef typename StreamingInnerProductType::MatrixType         MatrixType;
  typedef typename StreamingInnerProductType::MatrixObjectType   MatrixObjectType;

  /** Set/Get the number of required largest principal components. The filter produces
   the required number of principal components plus one outputs. Output index 0 represents
   the mean image and the remaining outputs the requested principal components.*/
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  /** Enable/Disable center data */
  itkSetMacro(CenterData, bool);
  itkGetMacro(CenterData, bool);
  itkBooleanMacro(CenterData);

protected:
  EstimateInnerProductPCAImageFilter();
  ~EstimateInnerProductPCAImageFilter() override
  {
  }

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  void GenerateOutputInformation(void) override;

  /**
   * BeforeThreadedGenerateData
   **/
  void BeforeThreadedGenerateData() override;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  EstimateInnerProductPCAImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The number of output Principal Components */
  unsigned int m_NumberOfPrincipalComponentsRequired;

  /** Enable/Disable center data */
  bool m_CenterData;

  /** Inner Product Matrix pre computed */
  MatrixType m_InnerProduct;

  /** Contains Eigen Vectors Of Inner Product Matrix computed */
  MatrixType m_EigenVectorsOfInnerProductMatrix;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEstimateInnerProductPCAImageFilter.hxx"
#endif

#endif

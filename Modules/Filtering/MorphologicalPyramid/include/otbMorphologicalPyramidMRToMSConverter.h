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

#ifndef otbMorphologicalPyramidMRToMSConverter_h
#define otbMorphologicalPyramidMRToMSConverter_h

#include "otbImageListToImageListFilter.h"

namespace otb
{
namespace MorphologicalPyramid
{
/**
 * \class MRToMSConverter
 * \brief This class convert a multi resolution decomposition from the morphological pyramid to a multi-scale one.
 *
 * This class convert a multi resolution decomposition from the morphological pyramid to a multi-scale one.
 *
 * The operation consists in iterativly upsampling and adding decimation details to a given detail image.
 *
 * \sa MorphologicalPyramidAnalyseFilter
 * \sa MorphologicalPyramidSynthesisFilter
 *
 * \ingroup OTBMorphologicalPyramid
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MRToMSConverter
  : public ImageListToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MRToMSConverter                                       Self;
  typedef ImageListToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(MRToMSConverter, ImageListToImageListFilter);
  /** Input parameters typedefs */
  typedef TInputImage                                    InputImageType;
  typedef typename Superclass::InputImagePointer         InputImagePointer;
  typedef typename Superclass::InputImageListType        InputImageListType;
  typedef typename Superclass::InputImageListPointerType InputImageListPointerType;
  typedef typename InputImageListType::Iterator          InputImageListIteratorType;
  /** Output parameters typedefs */
  typedef TOutputImage                                    OutputImageType;
  typedef typename Superclass::OutputImagePointer         OutputImagePointerType;
  typedef typename Superclass::OutputImageListType        OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType OutputImageListPointerType;

  /**
   * Set The input list.
   * \param imageList The analysed image at each level of the pyramid.
   */
  using Superclass::SetInput;
  void SetInput(const InputImageListType * imageList) override;
  /**
   * Set The SupFilter details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupFilter(InputImageListType * imageList);
  /**
   * Set The InfFilter details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfFilter(InputImageListType * imageList);
  /**
   * Set The SupDeci details
   * \param imageList The brighter details extracted from the filtering operation.
   */
  void SetSupDeci(InputImageListType * imageList);
  /**
   * Set The InfDeci details
   * \param imageList The darker details extracted from the filtering operation.
   */
  void SetInfDeci(InputImageListType * imageList);
  /**
   * Get the input list.
   * \return The list of the analysed image at each pyramid level.
   */
  InputImageListType* GetInput(void) override;
  /**
   * Get The SupFilter details
   * \return The brighter details extracted from the filtering operation.
   */
  InputImageListType* GetSupFilter(void);
  /**
   * Get The InfFilter details
   * \return The darker details extracted from the filtering operation.
   */
  InputImageListType* GetInfFilter(void);
  /**
   * Get The SupDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetSupDeci(void);
  /**
   * Get The InfDeci details
   * \return The brighter details extracted from the resampling operation.
   */
  InputImageListType* GetInfDeci(void);
  /**
   * Get The inputList at full resolution.
   * \return The analysed image at each pyramid level
   * resampled at full resolution.
   */
  OutputImageListType* GetOutput(void) override;
  /**
   * Get The SupFilter details at full resolution.
   * \return The brighter details extracted from the filtering operation
   * resampled at full resolution.
   */
  OutputImageListType * GetSupFilterFullResolution(void);
  /**
   * Get The InfFilter details at full resolution.
   * \return The darker details extracted from the filtering operation
   * resampled at full resolution.
   */
  OutputImageListType * GetInfFilterFullResolution(void);
  /**
   * Get The SupDeci details at full resolution.
   * \return The brighter details extracted from the resampling operation
   * resampled at full resolution.
   */
  OutputImageListType * GetSupDeciFullResolution(void);
  /**
   * Get The InfDeci details at full resolution.
   * \return The darker details extracted from the resampling operation
   * resampled at full resolution.
   */
  OutputImageListType * GetInfDeciFullResolution(void);

protected:
  /** Constructor */
  MRToMSConverter();
  /** Destructor */
  ~MRToMSConverter() override {}

  /** Main computation method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  MRToMSConverter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace MorphologicalPyramid
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMorphologicalPyramidMRToMSConverter.hxx"
#endif

#endif

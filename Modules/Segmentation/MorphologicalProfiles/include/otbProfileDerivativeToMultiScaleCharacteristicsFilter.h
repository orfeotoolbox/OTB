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

#ifndef otbProfileDerivativeToMultiScaleCharacteristicsFilter_h
#define otbProfileDerivativeToMultiScaleCharacteristicsFilter_h

#include "otbImageListToImageFilter.h"

namespace otb
{
/** \class ProfileDerivativeToMultiScaleCharacteristicsFilter
 *  \brief This filter extracts the maximum derivative value of the input profile.
 *
 *  This filter extracts the maximum derivative value of the input profile as well
 *  as the parameter (called characteristic) for which this maxima occurs.
 *
 *  The maxima can be retrieved by the GetOutput() image, while the characteristics
 *  can be retrieved by the GetOutputCharacteristics().
 *
 *  Please note that this characteristics can be scaled using the InitialValue and Step
 *  parameter corresponding to the input profile.
 *
 * \sa ProfileToProfileDerivativeImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
class ITK_EXPORT ProfileDerivativeToMultiScaleCharacteristicsFilter : public ImageListToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ProfileDerivativeToMultiScaleCharacteristicsFilter Self;
  typedef ImageListToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ProfileDerivativeToMultiScaleCharacteristicsFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TInputImage                             InputImageType;
  typedef TOutputImage                            OutputImageType;
  typedef TLabeledImage                           LabeledImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename OutputImageType::Pointer       OutputImagePointerType;
  typedef typename LabeledImageType::Pointer      LabeledImagePointerType;
  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename OutputImageType::RegionType    RegionType;
  typedef typename LabeledImageType::PixelType    LabeledPixelType;
  typedef typename OutputImageType::PixelType     OutputPixelType;

  /**
   * Returns the characteristics image.
   * \return The Characteristics image.
   */
  virtual TLabeledImage* GetOutputCharacteristics(void);

  /** Set/Get the initial profile value */
  itkSetMacro(InitialValue, LabeledPixelType);
  itkGetMacro(InitialValue, LabeledPixelType);
  /** Set/Get the profile step */
  itkSetMacro(Step, LabeledPixelType);
  itkGetMacro(Step, LabeledPixelType);

protected:
  /** Main computation method */
  void GenerateData(void) override;

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  void GenerateOutputInformation(void) override;

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  void GenerateInputRequestedRegion(void) override;

  /** Constructor */
  ProfileDerivativeToMultiScaleCharacteristicsFilter();
  /** Destructor */
  ~ProfileDerivativeToMultiScaleCharacteristicsFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ProfileDerivativeToMultiScaleCharacteristicsFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The initial value of the profile */
  LabeledPixelType m_InitialValue;
  /** The step of the profile */
  LabeledPixelType m_Step;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.hxx"
#endif

#endif

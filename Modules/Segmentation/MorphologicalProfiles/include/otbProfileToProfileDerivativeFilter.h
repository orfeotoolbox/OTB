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

#ifndef otbProfileToProfileDerivativeFilter_h
#define otbProfileToProfileDerivativeFilter_h

#include "otbImageListToImageListFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkAbsImageFilter.h"

namespace otb
{
/** \class ProfileToProfileDerivativeFilter
 *  \brief This filter computes the derivative of a given profile.
 *
 * For the profile \f$ \Pi_{\phi}(f)= \{\phi_{n}(f), n \in \{n_{1},\ldots, n_{N}\}\}\f$, the
 * profile derivative is defined by:
 *
 * \f[
 * \Delta\Pi_{\phi}(f)= \{\Delta\phi_{n}(f), n \in \{n_{1},\ldots, n_{N}\}\}
 * \f]
 *
 * with \f$ \Delta\phi_{n}(f) = \mid \Pi_{\phi_{n2}}(f)-\Pi_{\phi_{n1}}(f) \mid \f$
 *
 * \ingroup Streamed
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ProfileToProfileDerivativeFilter
  : public ImageListToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ProfileToProfileDerivativeFilter                      Self;
  typedef ImageListToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ProfileToProfileDerivativeFilter, ImageListToImageListFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputImageListType  InputImageListType;
  typedef typename InputImageListType::Pointer     InputImageListPointerType;
  typedef typename Superclass::OutputImageListType OutputImageListType;
  typedef typename OutputImageListType::Pointer    OutputImageListPointerType;
  typedef typename OutputImageListType::ImageType  OutputImageType;
  typedef typename InputImageListType::ImageType   InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointerType;

  /** typedefs of the filters used for the derivative */
  typedef itk::SubtractImageFilter<InputImageType, InputImageType, InputImageType> SubtractFilterType;
  typedef itk::AbsImageFilter<InputImageType, InputImageType>                      AbsFilterType;
  typedef typename SubtractFilterType::Pointer                                     SubtractFilterPointerType;
  typedef typename AbsFilterType::Pointer                                          AbsFilterPointerType;

  /** Generate output information for the ImageList and for each image
      in the list. */
  void GenerateOutputInformation(void) override;

  /** Generate input requested region for each image in the list. */
  void GenerateInputRequestedRegion(void) override;

protected:
  /** Main computation method */
  void GenerateData(void) override;
  /** Constructor */
  ProfileToProfileDerivativeFilter();
  /** Destructor */
  ~ProfileToProfileDerivativeFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ProfileToProfileDerivativeFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Subtract filter for the dervivative */
  SubtractFilterPointerType m_SubtractFilter;
  /** Abs filter for the derivative */
  AbsFilterPointerType m_AbsFilter;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProfileToProfileDerivativeFilter.hxx"
#endif

#endif

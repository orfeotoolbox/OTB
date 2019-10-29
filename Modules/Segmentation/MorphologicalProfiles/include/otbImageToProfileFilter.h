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

#ifndef otbImageToProfileFilter_h
#define otbImageToProfileFilter_h

#include "otbImageToImageListFilter.h"

namespace otb
{
/** \class ImageToProfileFilter
 *  \brief Base class to produce a profile of the response of a given filter for a range of parameter.
 *
 *  Let \f$ f \f$ denote an image, and \f$ \phi_{N}\f$ an image operator with the parameter \f$ N \f$ .
 *  A profile \f$ \Pi_{\phi}(f)\f$ of \f$ f \f$ by \f$ \phi \f$ is defined as follows:
 *  \f[
 *  \Pi_{\phi}(f)= \{\phi_{n}(f), n \in \{n_{1},\ldots, n_{N}\} \}
 *  \f]
 *
 * with \f$ \{n_{1},\ldots, n_{N}\} \f$ being a range of parameter. This class is a base class templated
 * by the type of the filter \f$ \phi \f$. The SetProfileParameter() is a virtual method meant to be
 * rewritten so that the filter can be correctly set up in sub-classes.
 *
 * \sa MorphologicalOpeningProfileFilter
 * \sa MorhologicalClosingProfileFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter = unsigned int>
class ITK_EXPORT ImageToProfileFilter : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef ImageToProfileFilter Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageToProfileFilter, ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TInputImage                                     InputImageType;
  typedef TOutputImage                                    OutputImageType;
  typedef TParameter                                      ParameterType;
  typedef TFilter                                         FilterType;
  typedef typename FilterType::Pointer                    FilterPointerType;
  typedef typename Superclass::OutputImageListType        OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType OutputImageListPointerType;
  typedef typename Superclass::InputImagePointer          InputImagePointerType;

  /** Get/Set the initial value */
  itkSetMacro(InitialValue, ParameterType);
  itkGetMacro(InitialValue, ParameterType);
  /** Get/Set the profile size */
  itkSetMacro(ProfileSize, unsigned int);
  itkGetMacro(ProfileSize, unsigned int);
  /** Get/Set the profile step */
  itkSetMacro(Step, ParameterType);
  itkGetMacro(Step, ParameterType);
  /** Get/Set the output index */
  itkSetMacro(OutputIndex, unsigned int);
  itkGetMacro(OutputIndex, unsigned int);

protected:
  /**
   * Set the profile parameter
   */
  virtual void SetProfileParameter(ParameterType /*param*/)
  {
  }
  /** Get the pointer to the filter */
  itkGetObjectMacro(Filter, FilterType);
  /** GenerateData method */
  void GenerateData(void) override;
  /** GenerateOutputInformation method */
  void GenerateOutputInformation(void) override;
  /** Generate input requested region */
  void GenerateInputRequestedRegion(void) override;
  /** Constructor */
  ImageToProfileFilter();
  /** Destructor */
  ~ImageToProfileFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToProfileFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The filter used to compute the profile */
  FilterPointerType m_Filter;
  /** The profile parameters */
  unsigned int m_ProfileSize;
  /** Initial value */
  ParameterType m_InitialValue;
  /** Step */
  ParameterType m_Step;
  /** The index of the output of the filter used for the profile */
  unsigned int m_OutputIndex;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToProfileFilter.hxx"
#endif

#endif

/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbAngularProjectionBinaryImageFilter_h
#define otbAngularProjectionBinaryImageFilter_h

#include <vector>

#include "itkImageToImageFilter.h"


namespace otb
{
/** \class AngularProjectionBinaryImageFilter
 * \brief Performs \f$ y_i = \cos \theta_i x_1 + \sin \theta_i x_2\f$
 *
 * This class performs the projections of the 2 input images to a set of N
 * output images according to N angular values
 *
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage, class TPrecision>
class ITK_EXPORT AngularProjectionBinaryImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef AngularProjectionBinaryImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(AngularProjectionBinaryImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputSizeType;
  typedef typename OutputImageType::IndexType  OutputIndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  typedef TPrecision PrecisionType;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Set/Get Angle set */
  std::vector<PrecisionType> GetAngleSet() const
  {
    return m_AngleSet;
  }
  void SetAngleSet(std::vector<PrecisionType>& angle);

  void                  SetInput1(const InputImageType*);
  const InputImageType* GetInput1() const;

  void                  SetInput2(const InputImageType*);
  const InputImageType* GetInput2() const;

protected:
  AngularProjectionBinaryImageFilter();
  ~AngularProjectionBinaryImageFilter() override
  {
  }

  void GenerateOutputInformation() override;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadID) override;

private:
  AngularProjectionBinaryImageFilter(const Self&); // not implemented
  void operator=(const Self&);                     // not implemented

  std::vector<PrecisionType> m_AngleSet;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAngularProjectionBinaryImageFilter.hxx"
#endif


#endif

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

#ifndef otbAngularProjectionImageFilter_h
#define otbAngularProjectionImageFilter_h

#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkImageToImageFilter.h"

namespace otb
{
/** \class AngularProjectionImageFilter
 * \brief Performs spherical transformation in ND space
 *
 * This class performs the projections of the N input images according
 * to angular value through the generalized spherical projection.
 *
 * The angle value are considered to come from a itk::FixedArray...
 *
 * \ingroup Threaded
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage, class TAngleArray, class TPrecision>
class ITK_EXPORT AngularProjectionImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef AngularProjectionImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(AngularProjectionImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  // itkStaticConstMacro(NumberOfInputImages,  unsigned int, TAngleArray::Dimension+1);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  typedef TPrecision                         PrecisionType;
  typedef TAngleArray                        AngleArrayType;
  typedef typename AngleArrayType::ValueType AngleValueType;

  typedef itk::ImageRegionConstIterator<InputImageType> ImageRegionConstIteratorType;
  typedef std::vector<ImageRegionConstIteratorType>     ImageRegionConstIteratorVectorType;

  /** Set/Get Angle set */
  itkGetMacro(AngleArray, AngleArrayType);
  itkSetMacro(AngleArray, AngleArrayType);

  void SetInput(unsigned int i, const InputImageType*) override;
  using Superclass::SetInput;

  const InputImageType* GetInput(unsigned int i) const;


protected:
  AngularProjectionImageFilter();
  ~AngularProjectionImageFilter() override
  {
  }

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadID) override;
  virtual OutputImagePixelType InternalGenerateData(const ImageRegionConstIteratorVectorType&) const;

private:
  AngularProjectionImageFilter(const Self&); // not implemented
  void operator=(const Self&);               // not implemented

  AngleArrayType m_AngleArray;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAngularProjectionImageFilter.hxx"
#endif


#endif

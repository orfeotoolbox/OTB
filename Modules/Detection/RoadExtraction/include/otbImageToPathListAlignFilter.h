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

#ifndef otbImageToPathListAlignFilter_h
#define otbImageToPathListAlignFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "otbImage.h"
#include "otbImageToPathListFilter.h"
#include <vector>

namespace otb
{

/** \class ImageToPathListAlignFilter
 * \brief Base class used to implement filter to extract align points or group of points and give the coordinates.
 *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInputImage, class TOutputPath>
class ITK_EXPORT ImageToPathListAlignFilter : public ImageToPathListFilter<TInputImage, TOutputPath>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPathListAlignFilter                      Self;
  typedef ImageToPathListFilter<TInputImage, TOutputPath> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPathListAlignFilter, ImageToPathListFilter);
//  itkTypeMacro(ImageToPathListAlignFilter, itk::ImageSource);

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Some convenient typedefs. */
  typedef typename Superclass::OutputPathListType OutputPathListType;

  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointer;
  typedef typename InputImageType::ConstPointer     InputImageConstPointer;

  typedef typename InputImageType::SizeType  SizeType;
  typedef typename InputImageType::ValueType ValueType;
  typedef typename InputImageType::PixelType PixelType;

  typedef typename Superclass::OutputPathType OutputPathType;
  // typedef typename Superclass::OutputPathListType     OutputPathListType;
  typedef typename Superclass::OutputPathPointerType OutputPathPointerType;

//  typedef          float                                        RealType;
//typedef typename itk::NumericTraits<PixelType>::RealType       RealType;
  typedef double RealType;
//  typedef typename itk::Image<RealType, InputImageDimension>      RealImageType;
  typedef typename otb::Image<RealType, InputImageDimension> RealImageType;
  typedef typename RealImageType::Pointer                    RealImageTypePointer;
  typedef typename RealImageType::IndexType                  RealImageTypeIndexType;

  /** Spacing (size of a pixel) of the output image. The
   * spacing is the geometric distance between image samples.
   * It is stored internally as double, but may be set from
   * float. \sa GetSpacing() */
  virtual void SetSpacing(const double* spacing);
  virtual void SetSpacing(const float* spacing);
  virtual const double* GetSpacing() const;

  /** Set/Get the value for pixels on and off the path.
  * By default, this filter will return a "0" image with path pixels set to 1 */
  itkSetMacro(PathValue, ValueType);
  itkGetMacro(PathValue, ValueType);
  itkSetMacro(BackgroundValue, ValueType);
  itkGetMacro(BackgroundValue, ValueType);

  /** The origin of the output image. The origin is the geometric
   * coordinates of the index (0, 0, ..., 0).  It is stored internally
   * as double but may be set from float.
   * \sa GetOrigin() */
  virtual void SetOrigin(const double* origin);
  virtual void SetOrigin(const float* origin);
  virtual const double * GetOrigin() const;

  /** Set/Get Size */
  itkSetMacro(Size, SizeType);
  itkGetMacro(Size, SizeType);

  itkSetMacro(isMeaningfulSegment, bool);
  itkSetMacro(NbGradDirection, int);
  itkSetMacro(NbLineDirection, int);
  itkSetMacro(MinGradNorm, double);
  itkSetMacro(Eps, double);
  itkGetConstReferenceMacro(isMeaningfulSegment, bool);
  itkGetConstReferenceMacro(NbGradDirection, int);
  itkGetConstReferenceMacro(NbLineDirection, int);
  itkGetConstReferenceMacro(MinGradNorm, double);
  itkGetConstReferenceMacro(Eps, double);

protected:
  ImageToPathListAlignFilter();
  ~ImageToPathListAlignFilter() override;

  void GenerateOutputInformation() override {}  // do nothing
  void GenerateData() override;
  virtual std::vector<double> tab(int n, double p, double m);
  virtual void AngleCalculate(const InputImageType*  InputImageIn);

  SizeType  m_Size;
  double    m_Spacing[InputImageDimension];
  double    m_Origin[InputImageDimension];
  ValueType m_PathValue;
  ValueType m_BackgroundValue;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ImageToPathListAlignFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  bool                m_isMeaningfulSegment; /// to get all meaningful segments (maximal or not
  int                 m_NbGradDirection; /// Number of allowed gradient direction, default 16
  int                 m_NbLineDirection; /// Number of line directions to scan, default 96)
  double              m_MinGradNorm; /// Minimum gradient norm to define a direction, default 2.
  double              m_Eps; /// -log10(max. number of false alarms), default 0
  std::vector<double> m_seglist;

  RealImageType * m_AngleImage; //Angle image use by GenerateData and AngleCalculate methods
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPathListAlignFilter.hxx"
#endif

#endif

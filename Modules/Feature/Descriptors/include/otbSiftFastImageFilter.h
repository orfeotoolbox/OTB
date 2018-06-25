/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSiftFastImageFilter_h
#define otbSiftFastImageFilter_h

#include "otbImageToPointSetFilter.h"
#include "otbImage.h"

namespace otb
{

/** \class SiftFastImageFilter
 *  \brief This class extracts key points from an input image through a pyramidal decomposition
 *
 * This class implements the SIFT key point detector proposed by David G. Lowe in proceedings of International
 * Conference on Computer Vision, Corfu, september 1999. The
 * implementation used is provided by the external library libsiftfast
 * (http://sourceforge.net/projects/libsift/),
 * using the 1.2 release.
 *
 * The code has been slightly modified in order to allow the user to
 * tune the number of scales used in pyramidal decomposition.
 *
 * Please note that OTB provides also a home-brewed version of the
 * SIFT (ImageToSIFTKeyPointSetFilter).
 *
 * \sa ImageToSIFTKeyPointSetFilter
 *
 * \example Patented/SIFTFastExample.cxx
 *
 * \ingroup OTBDescriptors
 */
template <class TInputImage, class TOutputPointSet>
class ITK_EXPORT SiftFastImageFilter
  : public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{
public:
  /** Standard typedefs */
  typedef SiftFastImageFilter                                 Self;
  typedef ImageToPointSetFilter<TInputImage, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(SiftFastImageFilter, ImageToPointSetFilter);

  /** Template parameters typedefs */

  typedef TInputImage                     InputImageType;
  typedef typename TInputImage::Pointer   InputImagePointerType;
  typedef typename TInputImage::PixelType PixelType;

  typedef TOutputPointSet                           OutputPointSetType;
  typedef typename TOutputPointSet::Pointer         OutputPointSetPointerType;
  typedef typename TOutputPointSet::PixelType       OutputPixelType;
  typedef typename TOutputPointSet::PointType       OutputPointType;
  typedef typename TOutputPointSet::PointIdentifier OutputPointIdentifierType;

  typedef otb::Image<float, 2>                             FloatImageType;
  typedef std::vector<std::pair<OutputPointType, double> > OrientationVectorType;

  itkSetMacro(ScalesNumber, unsigned int);
  itkGetMacro(ScalesNumber, unsigned int);

  //Set/Get the Orientation of all KeyPoints
  OrientationVectorType GetOrientationVector()
  {
    return m_OrientationVector;
  }

protected:
  /** Actually process the input */
  void GenerateData() override;

  /** Constructor */
  SiftFastImageFilter();

  /** Destructor */
  ~SiftFastImageFilter() override {}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** The number of scales */
  unsigned int          m_ScalesNumber;
  OrientationVectorType m_OrientationVector;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSiftFastImageFilter.hxx"
#endif

#endif

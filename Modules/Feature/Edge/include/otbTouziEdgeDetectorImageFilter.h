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

#ifndef otbTouziEdgeDetectorImageFilter_h
#define otbTouziEdgeDetectorImageFilter_h

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "otbImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class TouziEdgeDetectorImageFilter
 * \brief Application of a filter of detection of contours.
 *
 * This class implements the Touzi's ratio edge detector used to detect
 * contours.
 *
 * We define a square region of size 2n+1 that we divided in two regions.
 *
 * The response of the edge detector between two regions 1 and 2 in
 * one direction \f$ \theta_{i} \f$ is:
 *  \f[R(\theta_{i}) = 1 - \min (\frac{\mu_{1}}{\mu_{2}};\frac{\mu_{2}}{\mu_{1}}) \f]
 * where \f$ \mu_{1} \f$ and  \f$ \mu_{2} \f$ are the mean of regions 1 and 2.
 *
 * The intensity of contour is calculated in four directions
 * vertical , diagonal 1, horizontal and diagonal 2.
 *
 * The output is an image of intensity of the detection of contour R, the
 * maximum response of the ratio edge detector of \f$ R(\theta_{i}) \f$ :
 *  \f[R = \max ( R(\theta_{i}) ) \f]
 *
 * An image of the direction of contours can also be determined by this filter:
 *   \f[D = \frac{\sum_{i=1}^{4} s_{i}\theta_{i}R(\theta_{i})}{\sum_{i=1}^{4} R(\theta_{i}}  \f]
 * where if  \f[ \mu_{1}>\mu_{2} s_{i}=+1 \f]
 * else if \f[ \mu_{1}<\mu_{2} s_{i}=-1 \f]
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TOutputImageDirection = TOutputImage>
class ITK_EXPORT TouziEdgeDetectorImageFilter : public ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>
{
public:
  /** Extract input and output images sizes. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef TouziEdgeDetectorImageFilter Self;
  typedef ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(TouziEdgeDetectorImageFilter, ImageToModulusAndDirectionImageFilter);

  /** typedef to simplify variables definition and declaration. */
  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;

  typedef typename InputImageType::SizeType            SizeType;
  typedef typename OutputImageType::RegionType         OutputImageRegionType;
  typedef typename OutputImageType::PixelType          OutputPixelType;
  typedef typename OutputImageDirectionType::PixelType OutputPixelDirectionType;

  /** Set/Get radius methods */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

  /** To be allowed to use the pipeline method TouziEdgeDetectorImageFilter needs
   * a treatment input area larger than the output one.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError) override;

protected:
  TouziEdgeDetectorImageFilter();
  ~TouziEdgeDetectorImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void BeforeThreadedGenerateData() override;

  /** TouziEdgeDetectorImageFilter can be implemented for a multithreaded filter treatment.
   * Thus, this implementation give the ThreadedGenerateData() method.
   * that is called for each process thread. Image datas are automatically allocated
   * through the parent class calling the ThreadedGenerateData() method.
   * ThreadedGenerateData() can only write the area of the image specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  TouziEdgeDetectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Radius declaration */
  SizeType m_Radius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTouziEdgeDetectorImageFilter.hxx"
#endif

#endif

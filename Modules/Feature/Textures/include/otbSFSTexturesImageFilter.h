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

#ifndef otbSFSTexturesImageFilter_h
#define otbSFSTexturesImageFilter_h

#include "otbSFSTexturesFunctor.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/** \class SFSTexturesImageFilter
 *  \brief This functor computes the texture describes in the following publication
 *  It is based on line direction estimation.
 *
 * Please refer to Xin Huang, Liangpei Zhang and Pingxiang Li publication,
 * Classification and Extraction of Spatial Features in Urban Areas
 * Using High-Resolution Multispectral Imagery.
 * IEEE Geoscience and Remote Sensing Letters,
 * vol. 4, n. 2, 2007, pp 260-264
 *
 * The texture is computated for each pixel using its neighborhood.
 * User can set the spatial threshold that is the max line length, the spectral threshold
 * that is the max difference authorized between a pixel of the line and the center pixel
 * of the current neighborhood. Alpha and RatioMaxConsideration are used to compute
 * the \f$ \omega \f$ - mean value. Finally, The number of direction can be precised with
 * NumberOfDirections.
 * You can choose the computed textures using SetTextureStatus method (1:length, 2:width,
 * 3:PSI, 4:w-mean, 5:ratio, 6:SD).
 *
 * \sa SFSTexturesFunctor
 * \ingroup Textures
 *
 * \ingroup OTBTextures
*/

template <class TInputImage, class TOutputImage>
class ITK_EXPORT SFSTexturesImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SFSTexturesImageFilter                             Self;
  typedef TInputImage                                        InputImageType;
  typedef TOutputImage                                       OutputImageType;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorNeighborhoodImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef typename InputImageType::ConstPointer                                       InputImagePointerType;
  typedef typename InputImageType::RegionType                                         InputImageRegionType;
  typedef typename InputImageType::PixelType                                          InputImagePixelType;
  typedef typename InputImageType::SizeType                                           InputImageSizeType;
  typedef typename OutputImageType::Pointer                                           OutputImagePointerType;
  typedef typename OutputImageType::RegionType                                        OutputImageRegionType;
  typedef typename OutputImageType::PixelType                                         OutputImagePixelType;
  typedef itk::ConstNeighborhoodIterator<TInputImage>                                 NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType                               RadiusType;
  typedef Functor::SFSTexturesFunctor<NeighborhoodIteratorType, OutputImagePixelType> FunctorType;
  typedef typename FunctorType::OutputType                                            FunctorOutputType;
  typedef itk::ProcessObject                                                          ProcessObjectType;

  /**Set/Get the radius of neighborhood.*/
  itkGetMacro(Radius, unsigned int);

  /** Functor accessors */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }

  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  }
  void SetFunctor(const FunctorType& functor)
  {
    m_Functor = functor;
    this->Modified();
  }

  /** Spatial Threshold accessor */
  void SetSpatialThreshold(unsigned int thresh)
  {
    this->GetFunctor().SetSpatialThreshold(thresh);
    m_Radius = thresh;
    this->Modified();
  }
  unsigned int GetSpatialThreshold()
  {
    return this->GetFunctor().GetSpatialThreshold();
  }
  /** Spectral Threshold accessor */
  void SetSpectralThreshold(InputImagePixelType thresh)
  {
    this->GetFunctor().SetSpectralThreshold(thresh);
  }
  InputImagePixelType GetSpectralThreshold()
  {
    return this->GetFunctor().GetSpectralThreshold();
  }
  /** RatioMaxConsiderationNumber accessor */
  void SetRatioMaxConsiderationNumber(unsigned int value)
  {
    this->GetFunctor().SetRatioMaxConsiderationNumber(value);
  }
  unsigned int GetRatioMaxConsiderationNumber()
  {
    return this->GetFunctor().GetRatioMaxConsiderationNumber();
  }

  /** Alpha accessor */
  void SetAlpha(double alpha)
  {
    this->GetFunctor().SetAlpha(alpha);
  }
  double GetAlpha()
  {
    return this->GetFunctor().GetAlpha();
  }

  /** Number Of Directions */
  void SetNumberOfDirections(unsigned int D)
  {
    this->GetFunctor().SetNumberOfDirections(D);
    double step = CONST_PI / static_cast<double>(D);
    this->GetFunctor().SetDirectionStep(step);
  }
  unsigned int GetNumberOfDirections()
  {
    return this->GetFunctor().GetNumberOfDirections();
  }

  /** Texture selection accessors
   *  1: LENGTH
   *  2: WIDTH
   *  3: PSI
   *  4: WMEAN
   *  5: RATIO
   *  6: SD
   *  Set to 1 means the texture will be computed.
   **/
  typedef enum {LENGTH = 1, WIDTH, PSI, WMEAN, RATIO, SD} FeatureType;

  void SetFeatureStatus(FeatureType id, bool isSelected)
  {
    if (static_cast<unsigned int>(id) > this->GetTexturesStatus().size() || id == 0)
      {
      itkExceptionMacro(
        << "Invalid texture index " << id << ", must be in [1;" << this->GetTexturesStatus().size() << "]");
      }
    else
      {
      this->GetFunctor().SetTextureStatus(id - 1, isSelected);
      }
  }

  std::vector<bool> GetTexturesStatus()
  {
    return this->GetFunctor().GetTexturesStatus();
  }

  void InitFeatureStatus(bool status);

  /** Return output length image */
  const OutputImageType * GetLengthOutput() const;
  OutputImageType * GetLengthOutput();

  /** Return output width image */
  const OutputImageType * GetWidthOutput() const;
  OutputImageType * GetWidthOutput();

  /** Return output PSI image */
  const OutputImageType * GetPSIOutput() const;
  OutputImageType * GetPSIOutput();

  /** Return output WMean image */
  const OutputImageType * GetWMeanOutput() const;
  OutputImageType * GetWMeanOutput();

  /** Return output ratio image */
  const OutputImageType * GetRatioOutput() const;
  OutputImageType * GetRatioOutput();

  /** Return output SD image */
  const OutputImageType * GetSDOutput() const;
  OutputImageType * GetSDOutput();

  void GenerateOutputInformation() override;
  std::vector<FunctorType> m_FunctorList;

protected:
  SFSTexturesImageFilter();
  ~SFSTexturesImageFilter() override{}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void BeforeThreadedGenerateData() override;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  /** Pad the input requested region by radius */
  void GenerateInputRequestedRegion(void) override;

private:
  SFSTexturesImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_Radius;
  FunctorType  m_Functor;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSFSTexturesImageFilter.hxx"
#endif

#endif

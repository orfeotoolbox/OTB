/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbWaveletPacketTransform_h
#define otbWaveletPacketTransform_h

#include "itkProgressAccumulator.h"

#include "otbImageToImageListFilter.h"
#include "otbImageListToImageFilter.h"
#include "otbWaveletGenerator.h"
#include "otbWaveletOperatorBase.h"
#include "otbWaveletPacketDecompositionCosts.h"

namespace otb
{

/** \class WaveletPacketTransform
 * \brief Wavelet packet transformation framework
 *
 * This class defines the Wavelet Packet transformation of an image
 * by using a (templated) elementary wavelet transform and (templated)
 * cost criteria to stop the decomposition.
 *
 * It yields a list of images on the Wavelet::FORWARD decomposition. Conversely,
 * it takes an image list but yield a single list on the Wavelet::INVERSE transformation.
 * Hence, the Forward vs Inverse transformation has been split into two classes
 * (templated with otb::Wavelet::FORWARD and otb::Wavelet::INVERSE). The Forward class comes from
 * ImageToImageListFilter while the latter comes from a
 * ImageListToImageFilter. Thse two classes have specific declaration
 * and implementation.
 *
 * The user is supposed to initialize Cost properly (through GetCost() macro)
 * depending on its type before calling an Update(). The Cost class has to contain
 * a New() and Evaluate() function.
 *
 * \sa FullyDecomposedWaveletPacketCost
 * \sa WaveletFilterBank
 * \sa WaveletTransform
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter, Wavelet::WaveletDirection TDirectionOfTransformation,
          class TCost = FullyDecomposedWaveletPacketCost<TInputImage>>
class ITK_EXPORT WaveletPacketTransform : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletPacketTransform         Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletPacketTransform, ImageSource);

protected:
  /** But this class cannot do any thing! No constructor is implemented */
  WaveletPacketTransform();
  virtual ~WaveletPacketTransform();

private:
  WaveletPacketTransform(const Self&);
  void operator=(const Self&);
};

/** \class WaveletPacketTransform
 * \brief Wavelet packet forward transformation framework
 *
 * This class defines the Wavelet Packet transformation of an image
 * by using a (templated) elementary wavelet transform and (templated)
 * cost criteria to stop the decomposition.
 *
 * It yields a list of images on the Wavelet::FORWARD decomposition. Conversely,
 * it takes an image list but yield a single list on the Wavelet::INVERSE transformation.
 * Hence, the Forward vs Inverse transformation has been split into two classes
 * (templated with otb::Wavelet::FORWARD and otb::Wavelet::INVERSE). The Forward class comes from
 * ImageToImageListFilter while the latter comes from a
 * ImageListToImageFilter. Thse two classes have specific declaration
 * and implementation.
 *
 * The user is supposed to initialize Cost properly (through GetCost() macro)
 * depending on its type before calling an Update(). The Cost class has to contain
 * a New() and Evaluate() function.
 *
 * This is the specific declaration of the Forward transformation.
 *
 * \sa FullyDecomposedWaveletPacketCost
 * \sa WaveletFilterBank
 * \sa WaveletTransform
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter, class TCost>
class ITK_EXPORT WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD, TCost> : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletPacketTransform Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletPacketTransform, ImageToImageListFilter);

  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename OutputImageListType::Iterator      OutputImageIterator;

  typedef TFilter                           FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList<FilterType>            FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList, FilterListType);

  typedef TCost                      CostType;
  typedef typename CostType::Pointer CostPointerType;
  itkGetObjectMacro(Cost, CostType);

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::FORWARD);

  /** Get the recursive description of the packet decomposition */
  const std::vector<bool>& GetWaveletPacketRule() const
  {
    return this->m_WaveletPacketRule;
  }

  /** Get information on the decomposition */
  itkGetMacro(NumberOfFilters, unsigned int);
  itkGetMacro(DepthOfDecomposition, unsigned int);

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are not (yet?) dealing with M-band decomposition
   * then m_SubsampleImageFactor is most likely to be 1 or 2... but in any case integer
   * and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

protected:
  WaveletPacketTransform();
  ~WaveletPacketTransform() override
  {
  }

  /** Generate data redefinition.
   * This class does not performs multi-threading directly. But it uses step by step the
   * GenerateData() of TFilter. If This one can thread, the transformation is threaded
   * (e.g. WaveletFilterBank) */
  void GenerateData() override;

  /** Performs (if any) the local decomposition (called recursively) */
  virtual void GenerateData(unsigned int depth, OutputImageType* outputPtr, itk::ProgressAccumulator* accumulator);

private:
  WaveletPacketTransform(const Self&);
  void operator=(const Self&);

  /**
   * For multiscale decomposition, m_SubsampleImageFactor is set to 1.
   * It yields also a stationnary decomposition.
   * For multiresolution decomposition, most of the time, m_SubsampleImageFactor is
   * to be set to 2. This is its default value.
   */
  unsigned int m_SubsampleImageFactor;

  // Internal informative data
  unsigned int m_NumberOfFilters;
  unsigned int m_DepthOfDecomposition;

  FilterListPointerType m_FilterList;
  CostPointerType       m_Cost;
  std::vector<bool>     m_WaveletPacketRule;

}; // end of class

/** \class WaveletPacketTransform
 * \brief Wavelet packet inverse transformation framework
 *
 * This class defines the Wavelet Packet transformation of an image
 * by using a (templated) elementary wavelet transform and (templated)
 * cost criteria to stop the decomposition.
 *
 * It yields a list of images on the Wavelet::FORWARD decomposition. Conversely,
 * it takes an image list but yield a single list on the Wavelet::INVERSE transformation.
 * Hence, the Forward vs Inverse transformation has been split into two classes
 * (templated with otb::Wavelet::FORWARD and otb::Wavelet::INVERSE). The Forward class comes from
 * ImageToImageListFilter while the latter comes from a
 * ImageListToImageFilter. Thse two classes have specific declaration
 * and implementation.
 *
 * This is the specific declaration of the Inverse transformation.
 *
 * In this specialization, the Cost template class is not useful and then
 * declared to as FullyDecomposedWaveletPacketCost.
 *
 * \sa FullyDecomposedWaveletPacketCost
 * \sa WaveletFilterBank
 * \sa WaveletTransform
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE, FullyDecomposedWaveletPacketCost<TInputImage>>
    : public ImageListToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletPacketTransform Self;
  typedef ImageListToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletPacketTransform, ImageListToImageFilter);

  typedef typename Superclass::InputImageType   InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::IndexType    IndexType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::Iterator   InputImageIterator;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::SizeType          OutputImageSizeType;
  typedef typename OutputImageType::IndexType         OutputImageIndexType;

  typedef TFilter                           FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList<FilterType>            FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList, FilterListType);

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::INVERSE);

  /** Get/Set the decomposition rule */
  const std::vector<bool>& GetWaveletPacketRule() const
  {
    return this->m_WaveletPacketRule;
  }
  void SetWaveletPacketRule(const std::vector<bool>& rule)
  {
    this->m_WaveletPacketRule = rule;
    this->Modified();
  }

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are not (yet?) dealing with M-band decomposition
   * then m_SubsampleImageFactor is most likely to be 1 or 2... but in any case integer
   * and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

  /** Get information on the decomposition */
  itkGetMacro(NumberOfFilters, unsigned int);
  itkGetMacro(DepthOfDecomposition, unsigned int);

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

protected:
  WaveletPacketTransform();
  ~WaveletPacketTransform() override
  {
  }

  /** GenerateOutputInformation
    * Set the size of the output image depending on the decimation factor
    * Copy information from the input image if existing.
    **/
  void GenerateOutputInformation() override;

  /** Generate data redefinition.
   * This class does not performs multi-threading directly. But it uses step by step the
   * GenerateData() of TFilter. If This one can thread, the transformation is threaded
   * (e.g. WaveletFilterBank) */
  void GenerateData() override;

  /** Performs (if any) the local decomposition (called recursively) */
  virtual unsigned int SetInputFilters(unsigned int& ruleID, InputImageIterator& inputIter, unsigned int filterID);

  /** Get the depth of decomposition and the number of filters from m_WaveletPacketRule */
  void InterpretRule();
  void InterpretRule(unsigned int& ruleID, unsigned int curDepth);

private:
  WaveletPacketTransform(const Self&);
  void operator=(const Self&);

  /**
   * For multiscale decomposition, m_SubsampleImageFactor is set to 1.
   * It yields also a stationnary decomposition.
   * For multiresolution decomposition, most of the time, m_SubsampleImageFactor is
   * to be set to 2. This is its default value.
   */
  unsigned int m_SubsampleImageFactor;

  // Internal informative data
  unsigned int m_NumberOfFilters;
  unsigned int m_DepthOfDecomposition;

  FilterListPointerType m_FilterList;
  std::vector<bool>     m_WaveletPacketRule;

}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletPacketTransform.hxx"
#endif

#endif

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

#ifndef otbScalarImageToAdvancedTexturesFilter_h
#define otbScalarImageToAdvancedTexturesFilter_h

#include "otbGreyLevelCooccurrenceIndexedList.h"
#include "itkImageToImageFilter.h"

namespace otb
{
/**
 * \class ScalarImageToAdvancedTexturesFilter
 * \brief In this case, 10 advanced texture features will be processed. The 10
 * output image channels are: Mean, Variance, Dissimilarity, Sum Average, Sum
 * Variance, Sum Entropy, Difference of Entropies, Difference of Variances, IC1
 * and IC2. They are provided in this exact order in the output image. The
 * textures are computed over a sliding window with user defined radius.
 *
 * To improve the speed of computation, a variant of Grey Level Co-occurrence
 * Matrix(GLCM) called Grey Level Co-occurrence Indexed List (GLCIL) is
 * used. Given below is the mathematical explanation on the computation of each
 * textures. Here $ g(i, j) $ is the frequency of element in the GLCIL whose
 * index is i, j. GLCIL stores a pair of frequency of two pixels from the given
 * offset and the cell index (i, j) of the pixel in the neighborhood
 * window. :(where each element in GLCIL is a pair of pixel index and it's
 * frequency, $ g(i, j) $ is the frequency value of the pair having index is i, j).
 *
 * "Mean" \f$ = \sum_{i, j}i g(i, j) \f$
 *
 * "Sum of squares: Variance" \f$ = f_4 = \sum_{i, j}(i - \mu)^2 g(i, j) \f$
 *
 * "Dissimilarity" \f$ = f_5 = \sum_{i, j}(i - j) g(i, j)^2 \f$
 *
 * "Sum average" \f$ = f_6 = -\sum_{i}i g_{x+y}(i) \f$
 *
 * "Sum Variance" \f$ = f_7 = \sum_{i}(i - f_8)^2 g_{x+y}(i) \f$
 *
 * "Sum Entropy" \f$= f_8 = -\sum_{i}g_{x+y}(i) log (g_{x+y}(i)) \f$
 *
 * "Difference variance" \f$ = f_10 = variance of g_{x-y}(i) \f$
 *
 * "Difference entropy" \f$ = f_11 = -\sum_{i}g_{x-y}(i) log (g_{x-y}(i)) \f$
 *
 * "Information Measures of Correlation IC1" \f$ = f_12 = \frac{f_9 - HXY1}{H} \f$
 *
 * "Information Measures of Correlation IC2" \f$ = f_13 = \sqrt{1 - \exp{-2}|HXY2 - f_9|} \f$
 *
 * Above, \f$ \mu =  \f$ (weighted pixel average) \f$ = \sum_{i, j}i \cdot g(i, j) =
 * \sum_{i, j}j \cdot g(i, j) \f$ (due to matrix summetry), and
 *
 * \f$ \g_{x+y}(k) =  \sum_{i}\sum_{j}g(i)\f$ where \f$ i+j=k \f$ and \f$ k = 2, 3, .., 2N_[g}  \f$ and
 *
 * \f$ \g_{x-y}(k) =  \sum_{i}\sum_{j}g(i)\f$ where \f$ i-j=k \f$ and \f$ k = 0, 1, .., N_[g}-1  \f$
 *
 * Print references:
 *
 * Haralick, R.M., K. Shanmugam and I. Dinstein. 1973.  Textural Features for
 * Image Classification. IEEE Transactions on Systems, Man and Cybernetics.
 * SMC-3(6):610-620.
 *
 * David A. Clausi and Yongping Zhao. 2002. Rapid extraction of image texture by
 * co-occurrence using a hybrid data structure. Comput. Geosci. 28, 6 (July
 * 2002), 763-774. DOI=10.1016/S0098-3004(01)00108-X
 * http://dx.doi.org/10.1016/S0098-3004(01)00108-X
 *
 * de O.Bastos, L.; Liatsis, P.; Conci, A., Automatic texture segmentation based
 * on k-means clustering and efficient calculation of co-occurrence
 * features. Systems, Signals and Image Processing, 2008. IWSSIP 2008. 15th
 * International Conference on , vol., no., pp.141,144, 25-28 June 2008
 * doi: 10.1109/IWSSIP.2008.4604387
 *
 * Neighborhood size can be set using the SetRadius() method. Offset for co-occurence estimation
 * is set using the SetOffset() method.
 *
 * \sa otb::ScalarImageToCooccurrenceIndexedList
 * \sa otb::ScalarImageToTexturesFiler
 * \sa otb::ScalarImageToHigherOrderTexturesFilter
 * \ingroup Streamed
 * \ingroup Threaded
 *
 *
 * \ingroup OTBTextures
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToAdvancedTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToAdvancedTexturesFilter               Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToAdvancedTexturesFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputImageType::OffsetType  OffsetType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  typedef GreyLevelCooccurrenceIndexedList< InputPixelType >   CooccurrenceIndexedListType;
  typedef typename CooccurrenceIndexedListType::Pointer       CooccurrenceIndexedListPointerType;
  typedef typename CooccurrenceIndexedListType::ConstPointer  CooccurrenceIndexedListConstPointerType;
  typedef typename CooccurrenceIndexedListType::IndexType              CooccurrenceIndexType;
  typedef typename CooccurrenceIndexedListType::PixelValueType         PixelValueType;
  typedef typename CooccurrenceIndexedListType::RelativeFrequencyType  RelativeFrequencyType;
  typedef typename CooccurrenceIndexedListType::VectorType             VectorType;

  typedef typename VectorType::iterator                    VectorIteratorType;
  typedef typename VectorType::const_iterator              VectorConstIteratorType;

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset, OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset, OffsetType);

  /** Set the number of bin per axis */
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Get the number of bin per axis*/
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum, InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum, InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum, InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum, InputPixelType);

  /** Set the sub-sampling factor */
  itkSetMacro(SubsampleFactor, SizeType);

  /** Get the sub-sampling factor */
  itkGetMacro(SubsampleFactor, SizeType);

  /** Set the sub-sampling offset */
  itkSetMacro(SubsampleOffset, OffsetType);

  /** Get the sub-sampling offset */
  itkGetMacro(SubsampleOffset, OffsetType);

  /** Get the mean output image */
  OutputImageType * GetMeanOutput();

  /** Get the variance output image */
  OutputImageType * GetVarianceOutput();

  /** Get the dissimilarity output image */
  OutputImageType * GetDissimilarityOutput();

  /** Get the sum average output image */
  OutputImageType * GetSumAverageOutput();

  /** Get the sum of variances output image */
  OutputImageType * GetSumVarianceOutput();

  /** Get the sum of entropies output image */
  OutputImageType * GetSumEntropyOutput();

  /** Get the difference of entropies output image */
  OutputImageType * GetDifferenceEntropyOutput();

  /** Get the difference of variance output image */
  OutputImageType * GetDifferenceVarianceOutput();

  /** Get the IC1 image */
  OutputImageType * GetIC1Output();

  /** Get the IC2 output image */
  OutputImageType * GetIC2Output();

protected:
  /** Constructor */
  ScalarImageToAdvancedTexturesFilter();
  /** Destructor */
  ~ScalarImageToAdvancedTexturesFilter() override;
  /** Generate the output information */
  void GenerateOutputInformation() override;
  /** Generate the input requested region */
  void GenerateInputRequestedRegion() override;
  /** Before Parallel textures extraction */
  void BeforeThreadedGenerateData() override;
  /** Parallel textures extraction */
  void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId) override;

private:
  ScalarImageToAdvancedTexturesFilter(const Self&) = delete;
  void operator =(const Self&) = delete;

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** Offset for co-occurence */
  OffsetType m_Offset;

  /** Radius of the neighborhood iterator which is minimum of m_Radius */
  SizeType m_NeighborhoodRadius;

  /** Number of bins per axis */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;

  /** Sub-sampling factor */
  SizeType m_SubsampleFactor;

  /** Sub-sampling offset */
  OffsetType m_SubsampleOffset;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToAdvancedTexturesFilter.hxx"
#endif

#endif

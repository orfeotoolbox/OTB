/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFastNLMeansImageFilter_h
#define otbFastNLMeansImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class NLMeansFilter
 * \brief This class implements a fast approximated version of NL Means denoising
 * algorithm. This implementation is based on code in scikit module skimage.
 * This fast version of the NL Means algorithm
 * has been described in the following papers :
 *
 *  J. Darbon, A. Cunha, T.F. Chan, S. Osher, and G.J. Jensen,
 * Fast nonlocal filtering applied to electron cryomicroscopy,
 * in 5th IEEE International Symposium on Biomedical Imaging: From Nano to Macro,
 * 2008, pp. 1331-1334.
 *
 * Jacques Froment.
 * Parameter-Free Fast Pixelwise Non-Local Means Denoising.
 * Image Processing On Line, 2014, vol. 4, p. 300-326.
 *
 * \ingroup OTBSmoothing
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT NLMeansFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef NLMeansFilter                                      Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Typedef to image types */
  typedef TInputImage  InImageType;
  typedef TOutputImage OutImageType;

  /** Typedef to describe the image pointer types. */
  typedef typename InImageType::Pointer      InImagePointerType;
  typedef typename InImageType::ConstPointer InImageConstPointerType;
  typedef typename InImageType::RegionType   InRegionType;
  typedef typename InImageType::IndexType    InIndexType;
  typedef typename InImageType::SizeType     InSizeType;
  typedef typename InImageType::OffsetType   InOffsetType;
  typedef typename OutImageType::Pointer     OutImagePointerType;
  typedef typename OutImageType::RegionType  OutRegionType;
  typedef typename OutImageType::PixelType   OutPixelType;
  typedef typename OutImageType::SizeType    OutSizeType;
  typedef typename OutImageType::IndexType   OutIndexType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  itkTypeMacro(NLMeansFilter, ImageToImageFilter);

  void SetSigma(const float sigma)
  {
    m_Var = 2.0 * sigma * sigma;
  }

  void SetHalfWindowSize(const unsigned int hws)
  {
    m_HalfPatchSize.Fill(hws);
    // Update NormalizeDistance
    m_NormalizeDistance = (2 * hws + 1) * (2 * hws + 1) * m_CutoffDistance * m_CutoffDistance;
  }

  void SetHalfSearchSize(const unsigned int hss)
  {
    m_HalfSearchSize.Fill(hss);
  }
  void SetCutOffDistance(const float thresh)
  {
    m_CutoffDistance = thresh;
    // Update NormalizeDistance
    m_NormalizeDistance = (2 * m_HalfPatchSize[0] + 1) * (2 * m_HalfPatchSize[1] + 1) * m_CutoffDistance * m_CutoffDistance;
  }

protected:
  /** Constructor */
  NLMeansFilter();
  /** Destructor */
  ~NLMeansFilter() override = default;

  void ThreadedGenerateData(const OutRegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId)) override;

  void GenerateInputRequestedRegion() override;

  /** Compute the requested input region, given an output region.
   * If the input requested region is outside the largest input region, a mirror padding
   * is necessary. The returned tuple is composed of the following parameters :
   *  * input requested region (always lie inside the largest input region)
   *  * top rows, left cols, bottom rows, right cols : numbers of rows/cols to add with a mirror padding
   *  * boolean : if true, a mirror padding (in at least one direction) has to be computed
   */
  std::tuple<InRegionType, int, int, int, int, bool> OutputRegionToInputRegion(const OutRegionType& outputRegion) const;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  NLMeansFilter(const Self&) = delete;            // purposely not implemented
  NLMeansFilter& operator=(const Self&) = delete; // purposely not implemented

  /** For a given shift in rows and cols, this function computes
   * the squared difference between the image and its shifted version.
   * Results are added to form an integral image.
   */
  void ComputeIntegralImage(const std::vector<double>& dataInput,    /**< input data stored in a vector */
                            std::vector<double>&       imIntegral,   /**< output parameter. Contains the integral image of squared difference */
                            const OutIndexType         shift,        /**< Shift (dcol, drow) to apply to compute the difference */
                            const InSizeType           sizeIntegral, /**< Integral image size */
                            const InSizeType           sizeInput     /**< input data image size */
                            ) const;

  /** This function computes the squared euclidean distance
   * between a patch and its shifted version.
   * Computation relies on the integral image obtained before.
   */
  OutPixelType ComputeDistance(const unsigned int         row,        /**< Upper left corner row coordinate of patch*/
                               const unsigned int         col,        /**< Upper left corner col coordinate of patch*/
                               const std::vector<double>& imIntegral, /**< Integral image of squared difference*/
                               const unsigned int         nbCols      /**< Integral image number of columns */
                               ) const;

  // Define class attributes
  InSizeType m_HalfSearchSize;
  InSizeType m_HalfPatchSize;
  float      m_Var;
  float      m_CutoffDistance;
  float      m_NormalizeDistance; // cutoff**2 * windowSize**2

  static const int m_ROW = 1;
  static const int m_COL = 0;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastNLMeansImageFilter.hxx"
#endif

#endif

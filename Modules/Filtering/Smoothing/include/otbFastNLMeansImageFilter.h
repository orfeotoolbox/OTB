/*
 * Copyright (C) 2005-2018 Centre National d'Etudes Spatiales (CNES)
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

namespace otb{

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
  class ITK_EXPORT NLMeansFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
  {
  public:
    /** Standard class typedefs */
    typedef NLMeansFilter                  Self;
    typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef itk::SmartPointer<Self>        Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Typedef to image types */
    typedef TInputImage                           InImageType;
    typedef TOutputImage                          OutImageType;
    
    /** Typedef to describe the image pointer types. */
    typedef typename InImageType::Pointer         InImagePointerType;
    typedef typename InImageType::ConstPointer    InImageConstPointerType;
    typedef typename InImageType::RegionType      InRegionType;
    typedef typename InImageType::IndexType       InIndexType;
    typedef typename InImageType::SizeType        InSizeType;
    typedef typename InImageType::OffsetType      InOffsetType;
    typedef typename OutImageType::Pointer        OutImagePointerType;
    typedef typename OutImageType::RegionType     OutRegionType;
    typedef typename OutImageType::PixelType	  OutPixelType;
    typedef typename OutImageType::SizeType       OutSizeType;
    typedef typename OutImageType::IndexType      OutIndexType;

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
      m_NormalizeDistance = (2*hws+1)*(2*hws+1) * 
	m_CutoffDistance*m_CutoffDistance;
    }

    void SetHalfSearchSize(const unsigned int hss)
    {
      m_HalfSearchSize.Fill(hss);
    }
    void SetCutOffDistance(const float thresh)
    {
      m_CutoffDistance = thresh;
       // Update NormalizeDistance
      m_NormalizeDistance = (2*m_HalfPatchSize[0]+1)*
	(2*m_HalfPatchSize[1]+1) * 
	m_CutoffDistance*m_CutoffDistance;
    }

  protected:
    /** Constructor */
    NLMeansFilter();
    /** Destructor */
    ~NLMeansFilter() override = default;

    void ThreadedGenerateData(const OutRegionType& outputRegionForThread, 
			      itk::ThreadIdType itkNotUsed(threadId)) override;

    void GenerateInputRequestedRegion() override;

    InRegionType OutputRegionToInputRegion
      (const OutRegionType& outputRegion, 
       int& mirrorFirstRow, 
       int& mirrorFirstCol, 
       int& mirrorLastRow, 
       int& mirrorLastCol,
       bool& needMirror) const;

    /** PrintSelf method */
    void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  private:
    NLMeansFilter(const Self&) = delete; //purposely not implemented
    NLMeansFilter& operator=(const Self&) = delete; //purposely not implemented

    void ComputeIntegralImage(
       const std::vector<double> & dataInput, 
       std::vector<double> &imIntegral, 
       const OutIndexType shift, const InSizeType sizeIntegral, 
       const InSizeType sizeInput);

    OutPixelType ComputeDistance(const unsigned int row, 
				 const unsigned int col, 
				 const std::vector<double>& imIntegral, 
				 const unsigned int nbCols);

    // Define class attributes
    InSizeType m_HalfSearchSize;
    InSizeType m_HalfPatchSize;
    float m_Var;
    float m_CutoffDistance;
    float m_NormalizeDistance; //cutoff**2 * windowSize**2

    static const int m_ROW=1;
    static const int m_COL=0;
  };
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastNLMeansImageFilter.hxx"
#endif

#endif

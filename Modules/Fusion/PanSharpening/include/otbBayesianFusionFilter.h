/*
 * Copyright (C) 2007 Julien Radoux
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

#ifndef otbBayesianFusionFilter_h
#define otbBayesianFusionFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbFunctorImageFilter.h"
#include "otbStreamingMatrixTransposeMatrixImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{

namespace Functor
{
/** \class BayesianFunctor
 * \brief Functor for the bayesian fusion filter. Please refer to BayesianFusionFilter.
 *
 *
 * \ingroup OTBPanSharpening
 */
template <class TInputMultiSpectral, class TInputMultiSpectralInterp, class TInputPanchro, class TOutput>
class BayesianFunctor
{
public:
  BayesianFunctor()
  {
  }
  virtual ~BayesianFunctor()
  {
  }
  typedef typename TInputMultiSpectral::RealValueType RealType;
  typedef typename itk::VariableSizeMatrix<RealType>  MatrixType;

  void SetLambda(float lambda)
  {
    m_Lambda = lambda;
  }
  void SetS(float S)
  {
    m_S = S;
  }
  void SetAlpha(float alpha)
  {
    m_Alpha = alpha;
  }
  void SetBeta(MatrixType matrix)
  {
    m_Beta = matrix;
  }
  void SetCovarianceInvMatrix(MatrixType matrix)
  {
    m_CovarianceInvMatrix = matrix;
  }
  void SetVcondopt(MatrixType matrix)
  {
    m_Vcondopt = matrix;
  }
  float GetLambda()
  {
    return m_Lambda;
  }
  float GetAlpha()
  {
    return m_Alpha;
  }
  float GetS()
  {
    return m_S;
  }
  MatrixType GetBeta()
  {
    return m_Beta;
  }
  MatrixType GetCovarianceInvMatrix()
  {
    return m_CovarianceInvMatrix;
  }
  MatrixType GetVcondopt()
  {
    return m_Vcondopt;
  }

  void operator()(TOutput& obs, const TInputMultiSpectral& itkNotUsed(ms), const TInputMultiSpectralInterp& msi, const TInputPanchro& p)
  {
    MatrixType obsMat, msiVect;
    obsMat.SetSize(1, obs.GetSize());
    msiVect.SetSize(1, msi.GetSize());
    for (unsigned int i = 0; i < msi.GetSize(); ++i)
    {
      msiVect(0, i) = msi[i];
    }
    obsMat = msiVect * m_CovarianceInvMatrix;
    obsMat *= 2 * (1 - m_Lambda);
    MatrixType PanVect;
    PanVect = m_Beta.GetTranspose();
    PanVect *= (p - m_Alpha);
    PanVect /= m_S;
    PanVect *= 2 * m_Lambda;

    /** TODO
     *  To modify using + method operator. If we use it now -> exceptionmacro (no GetClassName...)
     * obsMat += PanVect;
     **/
    if ((obsMat.Cols() != PanVect.Cols()) || (obsMat.Rows() != PanVect.Rows()))
    {
      itkGenericExceptionMacro(<< "Matrix with size (" << obsMat.Rows() << "," << obsMat.Cols() << ") cannot be subtracted from matrix with size ("
                               << PanVect.Rows() << "," << PanVect.Cols() << " )");
    }

    for (unsigned int r = 0; r < obsMat.Rows(); ++r)
    {
      for (unsigned int c = 0; c < obsMat.Cols(); ++c)
      {
        obsMat(r, c) += PanVect(r, c);
      }
    }
    //**** END TODO ****/
    obsMat *= m_Vcondopt;
    for (unsigned int i = 0; i < obs.GetSize(); ++i)
    {
      obs[i] = static_cast<typename TOutput::ValueType>(obsMat(0U, i));
    }
  }

  constexpr size_t OutputSize(const std::array<size_t, 3> inputsNbBands) const
  {
    return inputsNbBands[1];
  }

private:
  float      m_Lambda;
  float      m_S;
  float      m_Alpha;
  MatrixType m_CovarianceInvMatrix;
  MatrixType m_Beta;
  MatrixType m_Vcondopt;
};
}

/***** TODO ***
 * Complete the description with J. Radoux text
 */
/***** END TODO ***/

/** \class BayesianFusionFilter
 * \brief Bayesian fusion filter. Contribution of Julien Radoux
 *
 * Please refer to D. Fasbender, J. Radoux and P. Bogaert,
 * Bayesian Data Fusion for Adaptable Image Pansharpening,
 * Transactions on Geoscience and Remote Sensing,
 * vol. 46, n. 6, 2007, pp 1847-1857
 *
 * \sa FusionImageBase
 * \sa MatrixTransposeMatrix
 * \sa StreamingStatisticsVectorImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup Fusion
 *
 *
 * \example Fusion/BayesianFusionImageFilter.cxx
 *
 *
 *
 * \ingroup OTBPanSharpening
 */

template <class TInputMultiSpectralImage, class TInputMultiSpectralInterpImage, class TInputPanchroImage, class TOutputImage>
class ITK_EXPORT BayesianFusionFilter
    : public FunctorImageFilter<Functor::BayesianFunctor<typename TInputMultiSpectralImage::PixelType, typename TInputMultiSpectralInterpImage::PixelType,
                                                         typename TInputPanchroImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputMultiSpectralImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputMultiSpectralImage       InputMultiSpectralImageType;
  typedef TInputMultiSpectralInterpImage InputMultiSpectralInterpImageType;
  typedef TInputPanchroImage             InputPanchroImageType;
  typedef TOutputImage                   OutputImageType;

  /** "typedef" for standard classes. */
  typedef BayesianFusionFilter Self;
  using BayesianFunctorType = Functor::BayesianFunctor<typename TInputMultiSpectralImage::PixelType, typename TInputMultiSpectralInterpImage::PixelType,
                                                       typename TInputPanchroImage::PixelType, typename TOutputImage::PixelType>;
  typedef FunctorImageFilter<BayesianFunctorType> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(BayesianFusionFilter, FunctorImageFilter);

  /** Supported images definition. */
  typedef typename InputMultiSpectralImageType::PixelType               InputMultiSpectralPixelType;
  typedef typename InputMultiSpectralImageType::InternalPixelType       InputMultiSpectralInternalPixelType;
  typedef typename InputMultiSpectralInterpImageType::PixelType         InputMultiSpectralInterpPixelType;
  typedef typename InputMultiSpectralInterpImageType::InternalPixelType InputMultiSpectralInterpInternalPixelType;
  typedef typename InputPanchroImageType::PixelType                     InputPanchroPixelType;
  typedef typename OutputImageType::PixelType                           OutputPixelType;
  typedef typename OutputImageType::InternalPixelType                   OutputInternalPixelType;

  /** Real class typedef definition. */
  typedef typename itk::NumericTraits<InputPanchroPixelType>::RealType                     InputPanchroRealType;
  typedef typename itk::NumericTraits<InputMultiSpectralInternalPixelType>::RealType       InputMultiSpectralRealType;
  typedef typename InputMultiSpectralImageType::RegionType                                 InputMultiSpectralImageRegionType;
  typedef typename itk::NumericTraits<InputMultiSpectralInterpInternalPixelType>::RealType InputMultiSpectralInterpRealType;
  typedef typename InputMultiSpectralInterpImageType::RegionType                           InputMultiSpectralInterpImageRegionType;
  typedef typename InputPanchroImageType::RegionType                                       InputPanchroImageRegionType;
  typedef typename OutputImageType::RegionType                                             OutputImageRegionType;

  /** Image size "typedef" definition. */
  typedef typename InputMultiSpectralImageType::SizeType SizeType;

  /** Typedef for statistic computing. */
  typedef StreamingStatisticsVectorImageFilter<InputMultiSpectralInterpImageType> StreamingStatisticsVectorImageFilterType;
  typedef typename StreamingStatisticsVectorImageFilterType::MatrixType           MatrixType;
  typedef StreamingMatrixTransposeMatrixImageFilter<InputMultiSpectralImageType, InputMultiSpectralImageType> MSTransposeMSType;
  typedef ImageToVectorImageCastFilter<InputPanchroImageType, InputMultiSpectralImageType>                    CasterType;

  void SetMultiSpect(const InputMultiSpectralImageType* multiSpect)
  {
    this->template SetInput<0>(multiSpect);
  }

  void SetMultiSpectInterp(const InputMultiSpectralInterpImageType* multiSpectInterp)
  {
    this->template SetInput<1>(multiSpectInterp);
  }

  void SetPanchro(const InputPanchroImageType* panchro)
  {
    this->template SetInput<2>(panchro);
  }

  const InputMultiSpectralImageType* GetMultiSpect()
  {
    return this->template GetInput<0>();
  }

  const InputMultiSpectralInterpImageType* GetMultiSpectInterp()
  {
    return this->template GetInput<1>();
  }

  const InputPanchroImageType* GetPanchro()
  {
    return this->template GetInput<2>();
  }

  /** Set the ponderation value. */
  itkSetMacro(Lambda, float);
  /** Give the ponderation value. */
  itkGetConstReferenceMacro(Lambda, float);

  /** Set the Beta matrix. */
  itkSetMacro(Beta, MatrixType);
  /** Give the Beta matrix. */
  itkGetConstReferenceMacro(Beta, MatrixType);

  /** Set the Covariance matrix. */
  itkSetMacro(CovarianceMatrix, MatrixType);
  /** Give the Covariance matrix. */
  itkGetConstReferenceMacro(CovarianceMatrix, MatrixType);

  /** Set the Covariance inverse matrix. */
  itkSetMacro(CovarianceInvMatrix, MatrixType);
  /** Give the Covariance inverse matrix. */
  itkGetConstReferenceMacro(CovarianceInvMatrix, MatrixType);

  /** Set the Bayesian Data Fusion matrix. */
  itkSetMacro(Vcondopt, MatrixType);
  /** Give the Bayesian Data Fusion matrix. */
  itkGetConstReferenceMacro(Vcondopt, MatrixType);

  /** Set the S coefficient. */
  itkSetMacro(S, float);
  /** Give the S coefficient. */
  itkGetConstReferenceMacro(S, float);

protected:
  BayesianFusionFilter() : Superclass(BayesianFunctorType{}, {{0, 0}})
  {
    m_Lambda                      = 0.9999;
    m_S                           = 1;
    m_StatisticsHaveBeenGenerated = false;
  };

  ~BayesianFusionFilter() override{};
  /** Check if internal statistics need to be computed, and do so */
  void BeforeThreadedGenerateData() override;
  /** Compute internal statistics required for fusion */
  void ComputeInternalStatistics(void);
  /** Call the superclass implementation and set the StatisticsHaveBeenGenerated
   * flag to false */
  void Modified(void) const override;

private:
  /** Ponderation declaration*/
  float m_Lambda;
  float m_S;
  /** Multispectral covariance matrix */
  MatrixType m_CovarianceMatrix;
  /** Multispectral inverse covariance matrix */
  MatrixType m_CovarianceInvMatrix;
  /** Regression coefficients matrix */
  MatrixType m_Beta;
  /** Optimisation matrix */
  MatrixType m_Vcondopt;
  /** True if internal statistics have been generated */
  mutable bool m_StatisticsHaveBeenGenerated;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBayesianFusionFilter.hxx"
#endif

#endif

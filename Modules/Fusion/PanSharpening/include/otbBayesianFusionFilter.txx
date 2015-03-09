/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBayesianFusionFilter_txx
#define __otbBayesianFusionFilter_txx

#include "otbBayesianFusionFilter.h"

namespace otb
{

template <class TInputMultiSpectralImage,
    class TInputMultiSpectralInterpImage,
    class TInputPanchroImage,
    class TOutputImage>
BayesianFusionFilter<TInputMultiSpectralImage,
    TInputMultiSpectralInterpImage,
    TInputPanchroImage,
    TOutputImage>
::BayesianFusionFilter()
{
  m_Lambda = 0.9999;
  m_S = 1;
  m_StatisticsHaveBeenGenerated = false;
}

template <class TInputMultiSpectralImage,
    class TInputMultiSpectralInterpImage,
    class TInputPanchroImage,
    class TOutputImage>
BayesianFusionFilter<TInputMultiSpectralImage,
    TInputMultiSpectralInterpImage,
    TInputPanchroImage,
    TOutputImage>
::~BayesianFusionFilter()
{

}
template <class TInputMultiSpectralImage,
    class TInputMultiSpectralInterpImage,
    class TInputPanchroImage,
    class TOutputImage>
void
BayesianFusionFilter<TInputMultiSpectralImage,
    TInputMultiSpectralInterpImage,
    TInputPanchroImage,
    TOutputImage>
::Modified() const
{
  Superclass::Modified();
  m_StatisticsHaveBeenGenerated = false;
}

template <class TInputMultiSpectralImage,
    class TInputMultiSpectralInterpImage,
    class TInputPanchroImage,
    class TOutputImage>
void
BayesianFusionFilter<TInputMultiSpectralImage,
    TInputMultiSpectralInterpImage,
    TInputPanchroImage,
    TOutputImage>
::BeforeThreadedGenerateData()
{
  if (!m_StatisticsHaveBeenGenerated)
    {
    this->ComputeInternalStatistics();
    m_StatisticsHaveBeenGenerated = true;
    }
}

template <class TInputMultiSpectralImage,
    class TInputMultiSpectralInterpImage,
    class TInputPanchroImage,
    class TOutputImage>
void
BayesianFusionFilter<TInputMultiSpectralImage,
    TInputMultiSpectralInterpImage,
    TInputPanchroImage,
    TOutputImage>
::ComputeInternalStatistics()
{
  OutputImageRegionType msiRequestedRegion = this->GetMultiSpectInterp()->GetRequestedRegion();
  OutputImageRegionType msRequestedRegion = this->GetMultiSpect()->GetRequestedRegion();
  OutputImageRegionType panchroRequestedRegion = this->GetPanchro()->GetRequestedRegion();

  // Allocate output
  typename OutputImageType::Pointer             output           = this->GetOutput();
  typename InputMultiSpectralImageType::Pointer multiSpec   = const_cast<InputMultiSpectralImageType *>(
    this->GetMultiSpect());
  typename InputMultiSpectralInterpImageType::Pointer multiSpecInterp  = const_cast<InputMultiSpectralInterpImageType *>(
    this->GetMultiSpectInterp());
  typename InputPanchroImageType::Pointer panchro          = const_cast<InputPanchroImageType *>(
    this->GetPanchro());

  /** Variable Initialisaton  */
  m_Beta.SetSize(multiSpecInterp->GetNumberOfComponentsPerPixel() + 1, 1);
  m_Beta.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);

  m_CovarianceMatrix.SetSize(
    multiSpecInterp->GetNumberOfComponentsPerPixel(), multiSpecInterp->GetNumberOfComponentsPerPixel());
  m_CovarianceMatrix.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);

  m_CovarianceInvMatrix.SetSize(
    multiSpecInterp->GetNumberOfComponentsPerPixel(), multiSpecInterp->GetNumberOfComponentsPerPixel());
  m_CovarianceInvMatrix.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);

  m_Vcondopt.SetSize(multiSpecInterp->GetNumberOfComponentsPerPixel(), multiSpecInterp->GetNumberOfComponentsPerPixel());
  m_Vcondopt.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);
  /** Compute the inverse of the multispectral interpolated image covariance matrix   */
  typename StreamingStatisticsVectorImageFilterType::Pointer covComputefilter =
    StreamingStatisticsVectorImageFilterType::New();

  covComputefilter->SetInput(multiSpecInterp);
  covComputefilter->Update();

  m_CovarianceMatrix = covComputefilter->GetCovariance();
  otbMsgDebugMacro(<< "Covariance: " << m_CovarianceMatrix);

  m_CovarianceInvMatrix = m_CovarianceMatrix.GetInverse();
  /** Beta computation : Regression model coefficient */
  // MatrixTransform only support vectorimage input
  typename CasterType::Pointer caster         = CasterType::New();
  caster->SetInput(panchro);
  // caster->Update();
  // Compute the transpose multispectral image multiplied by itself
  typename MSTransposeMSType::Pointer msTransposeMs  = MSTransposeMSType::New();
  // Compute the transpose multispectral image multiplied by the panchromatic one
  typename MSTransposeMSType::Pointer msTransposePan  = MSTransposeMSType::New();
  // Add a dimension filled with ones to the images
  msTransposeMs->SetUsePadFirstInput(true);
  msTransposeMs->SetUsePadSecondInput(true);
  msTransposePan->SetUsePadFirstInput(true);

  msTransposeMs->SetFirstInput(multiSpec);
  msTransposeMs->SetSecondInput(multiSpec);

  msTransposePan->SetFirstInput(multiSpec);
  msTransposePan->SetSecondInput(caster->GetOutput());

  msTransposeMs->Update();
  otbMsgDebugMacro(<< "MsTMs: " << msTransposeMs->GetResultOutput()->Get());
  msTransposePan->Update();
  otbMsgDebugMacro(<< "MsTPan: " << msTransposePan->GetResultOutput()->Get());

  MatrixType temp;
  temp = msTransposeMs->GetResultOutput()->Get().GetInverse();
  m_Beta = temp * msTransposePan->GetResultOutput()->Get();

  // S computation : quadratique mean of the regression residue
  // Compute the transpose panchromatic image multiplied by itself
  typename MSTransposeMSType::Pointer panTransposePan  = MSTransposeMSType::New();
  panTransposePan->SetFirstInput(caster->GetOutput());
  panTransposePan->SetSecondInput(caster->GetOutput());
  panTransposePan->Update();
  otbMsgDebugMacro(<< "PanTPan: " << msTransposePan->GetResultOutput()->Get());
  MatrixType S, tempS, tempS2;
  S = panTransposePan->GetResultOutput()->Get();
  tempS = msTransposePan->GetResultOutput()->Get().GetTranspose();
  tempS = tempS * m_Beta;

  /** TODO
   *  To modify using - method operator. If we use it now -> exceptionmacro (no GetClassName...)
   *  S = S-tempS;
   **/
  if ((S.Rows() != tempS.Rows()) || (S.Cols() != tempS.Cols()))
    {
    itkExceptionMacro(<< "Matrix with size (" << S.Rows() << "," <<
                      S.Cols() << ") cannot be subtracted from matrix with size (" <<
                      tempS.Rows() << "," << tempS.Cols() << " )");
    }
  for (unsigned int r = 0; r < S.Rows(); ++r)
    {
    for (unsigned int c = 0; c < S.Cols(); ++c)
      {
      S(r, c) -= tempS(r, c);
      }
    }
  //**** END TODO ****/

  tempS = m_Beta.GetTranspose();
  tempS2 =  msTransposePan->GetResultOutput()->Get();
  tempS = tempS * tempS2;
  /** TODO
   *  To modify using - method operator. If we use it now -> exceptionmacro (no GetClassName...)
   *  S = S-tempS;
   **/
  if ((S.Rows() != tempS.Rows()) || (S.Cols() != tempS.Cols()))
    {
    itkExceptionMacro(<< "Matrix with size (" << S.Rows() << "," <<
                      S.Cols() << ") cannot be subtracted from matrix with size (" <<
                      tempS.Rows() << "," << tempS.Cols() << " )");

    }
  for (unsigned int r = 0; r < S.Rows(); ++r)
    {
    for (unsigned int c = 0; c < S.Cols(); ++c)
      {
      S(r, c) -= tempS(r, c);
      }
    }
  //**** END TODO ****/

  MatrixType xxT, xxTb, xxTbT, xxTbTb;
  xxT = msTransposeMs->GetResultOutput()->Get().GetTranspose();
  xxTb = xxT * m_Beta;
  xxTbT = xxTb.GetTranspose();
  xxTbTb = xxTbT * m_Beta;
  /** TODO
   *  To modify using - method operator. If we use it now -> exceptionmacro (no GetClassName...)
   * S = S-xxTbTb;
   **/
  if ((S.Cols() != xxTbTb.Cols()) || (S.Cols() != xxTbTb.Cols()))
    {
    itkExceptionMacro(<< "Matrix with size (" << S.Rows() << "," <<
                      S.Cols() << ") cannot be subtracted from matrix with size (" <<
                      xxTbTb.Rows() << "," << xxTbTb.Cols() << " )");
    }

  for (unsigned int r = 0; r < S.Rows(); ++r)
    {
    for (unsigned int c = 0; c < S.Cols(); ++c)
      {
      S(r, c) += xxTbTb(r, c);
      }
    }
  //**** END TODO ****/

  unsigned int size1 = multiSpec->GetLargestPossibleRegion().GetSize()[0] *
                       multiSpec->GetLargestPossibleRegion().GetSize()[1];
  unsigned int size2 = multiSpec->GetNumberOfComponentsPerPixel() + 1;
  m_S = S(0, 0);
  m_S /= static_cast<float>(size1 - size2);

  // cutBeta is the N-1 last m_Beta element matrix.
  // varPan contains transpose(cutBeta)*cutBeta/S
  MatrixType varPan, cutBeta;
  varPan.SetSize(multiSpecInterp->GetNumberOfComponentsPerPixel(), 1);
  varPan.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);
  cutBeta.SetSize(multiSpecInterp->GetNumberOfComponentsPerPixel(), 1);
  cutBeta.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);
  // Take the N-1 m_Beta last elements
  for (unsigned int r = 1; r < m_Beta.Rows(); ++r)
    {
    cutBeta(r - 1, 0) = m_Beta(r, 0);
    }
  varPan = cutBeta;

  MatrixType tempvarPan;
  tempvarPan = varPan.GetTranspose();
  varPan *= tempvarPan;
  varPan /= m_S;
  // Compute the optimization matrix : m_Vcondopt
  // eye is the identical matrix which size is the number of components of the multispectral image
  MatrixType eye;
  eye.SetSize(multiSpecInterp->GetNumberOfComponentsPerPixel(), multiSpecInterp->GetNumberOfComponentsPerPixel());
  eye.Fill(itk::NumericTraits<InputMultiSpectralInterpRealType>::Zero);
  for (unsigned int r = 1; r < eye.Rows(); ++r)
    {
    eye(r, r) = vcl_pow(10., -12.);
    }

  /** TODO
   *  To modify using + method operator. If we use it now -> exceptionmacro (no GetClassName...)
   *  m_Vcondopt = 2 *m_Lambda*varPan+2*m_CovarianceInvMatrix*(1-m_Lambda)+eye;
   **/
  if ((m_Vcondopt.Cols() != varPan.Cols()) || (m_Vcondopt.Cols() != varPan.Cols())
      || (m_Vcondopt.Cols() != m_CovarianceInvMatrix.Cols()) || (m_Vcondopt.Cols() != m_CovarianceInvMatrix.Cols()))
    {
    itkExceptionMacro(<< "Matrix with size (" << m_Vcondopt.Rows() << "," <<
                      m_Vcondopt.Cols() << ") cannot be subtracted from matrix with size (" <<
                      varPan.Rows() << "," << varPan.Cols() << " ) or ( " <<
                      m_CovarianceInvMatrix.Rows() << "," << m_CovarianceInvMatrix.Cols() << ")");
    }
  for (unsigned int r = 0; r < m_Vcondopt.Rows(); ++r)
    {
    for (unsigned int c = 0; c < m_Vcondopt.Cols(); ++c)
      {
      m_Vcondopt(r, c) = 2 *m_Lambda*varPan(r, c)
                         + 2 * m_CovarianceInvMatrix(r, c) * (1 - m_Lambda)
                         + eye(r, c);
      }
    }
  //**** END TODO ****/
  m_Vcondopt = m_Vcondopt.GetInverse();
  // Functor initialization
  this->GetFunctor().SetVcondopt(m_Vcondopt);
  this->GetFunctor().SetBeta(cutBeta);
  this->GetFunctor().SetAlpha(m_Beta(0, 0));
  this->GetFunctor().SetCovarianceInvMatrix(m_CovarianceInvMatrix);
  this->GetFunctor().SetLambda(m_Lambda);
  this->GetFunctor().SetS(m_S);

  // Restore the previous buffered data
  multiSpecInterp->SetRequestedRegion(msiRequestedRegion);
  multiSpecInterp->PropagateRequestedRegion();
  multiSpecInterp->UpdateOutputData();

  multiSpec->SetRequestedRegion(msRequestedRegion);
  multiSpec->PropagateRequestedRegion();
  multiSpec->UpdateOutputData();

  panchro->SetRequestedRegion(panchroRequestedRegion);
  panchro->PropagateRequestedRegion();
  panchro->UpdateOutputData();

}
} // end namespace otb

#endif

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
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVCAImageFilter_txx
#define __otbVCAImageFilter_txx


#include "otbVcaImageFilter.h"
#include "otbStandardWriterWatcher.h"


#define otbMsgDevMacroVCA(p) std::cout << p << std::endl;
//#define otbMsgDevMacroVCA(p);

namespace otb {

template<class TImage>
VCAImageFilter<TImage>::VCAImageFilter()
  : m_NumberOfEndmembers(0)
{
}

template<class TImage>
VCAImageFilter<TImage>::~VCAImageFilter()
{
}

template<class TImage>
void VCAImageFilter<TImage>::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename VectorImageType::ConstPointer inputPtr = this->GetInput();
  typename VectorImageType::Pointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  const unsigned int numberOfBands = inputPtr->GetNumberOfComponentsPerPixel();
  RegionType   outputRegion;
  IndexType    outputOrigin;
  SizeType     outputSize;

  outputOrigin.Fill(0);
  outputSize[0] = m_NumberOfEndmembers;
  outputSize[1] = 1;
  outputRegion.SetIndex(outputOrigin);
  outputRegion.SetSize(outputSize);

  outputPtr->SetLargestPossibleRegion( outputRegion );
  outputPtr->SetNumberOfComponentsPerPixel( numberOfBands );
}

template<class TImage>
void VCAImageFilter<TImage>::GenerateData()
{
  typedef typename ForwardPCAImageFilterType::NormalizeFilterType NormalizeFilterType;

  VectorImageType* input = const_cast<VectorImageType*>(this->GetInput());
  const unsigned int nbBands = this->GetInput()->GetNumberOfComponentsPerPixel();


  otbMsgDevMacroVCA( "Computing image stats" )
  typename StreamingStatisticsVectorImageFilterType::Pointer statsInput =
      StreamingStatisticsVectorImageFilterType::New();

  statsInput->SetInput(input);
  statsInput->SetEnableMinMax(false);
  statsInput->Update();

  double SNR, SNRth;
  vnl_matrix<PrecisionType> Ud;

  // SNR computation
    {
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "statsInput->GetCorrelation().GetVnlMatrix()" << std::endl;
    vnl_matrix<PrecisionType> R = statsInput->GetCorrelation().GetVnlMatrix();
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "svd(R)" << std::endl;
    vnl_svd<PrecisionType> svd(R);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "U" << std::endl;
    vnl_matrix<PrecisionType> U = svd.U();
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "Ud" << std::endl;
    vnl_matrix<PrecisionType> Ud = U.get_n_columns(0, m_NumberOfEndmembers);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "Udt" << std::endl;
    vnl_matrix<PrecisionType> Udt = Ud.transpose();

    // To remove the mean
    typename NormalizeFilterType::Pointer normalize = NormalizeFilterType::New();
    normalize->SetInput(input);
    normalize->SetMean(statsInput->GetMean());
    normalize->SetUseMean(true);
    normalize->SetUseStdDev(false);


    typename MatrixImageFilterType::Pointer mulUd = MatrixImageFilterType::New();
    mulUd->MatrixByVectorOn();
    mulUd->SetInput(normalize->GetOutput());
    mulUd->SetMatrix(Udt);
    mulUd->UpdateOutputInformation();

    typename StreamingStatisticsVectorImageFilterType::Pointer transformedStats =
        StreamingStatisticsVectorImageFilterType::New();

    /*
    typename ForwardPCAImageFilterType::Pointer pca = ForwardPCAImageFilterType::New();
    pca->SetInput(input);
    pca->SetUseNormalization(true);
    pca->SetUseVarianceForNormalization(false);
    pca->SetNumberOfPrincipalComponentsRequired(m_NumberOfEndmembers);
    pca->UpdateOutputInformation();

    std::cout << "PCA output" << std::endl;
    pca->GetOutput()->Print(std::cout);
    //transformedStats->SetInput(pca->GetOutput());
    */


    transformedStats->SetInput(mulUd->GetOutput());
    transformedStats->SetEnableMinMax(false);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "transformedStats Update" << std::endl;
    transformedStats->Update();

    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "statsInput->GetComponentCorrelation()" << std::endl;
    double P_R = nbBands * statsInput->GetComponentCorrelation();
    std::cout << "P_R " << P_R << std::endl;
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    std::cout << "transformedStats->GetComponentCorrelation()" << std::endl;

    double P_Rp = m_NumberOfEndmembers * transformedStats->GetComponentCorrelation()
                  + statsInput->GetMean().GetSquaredNorm();
    std::cout << "P_Rp " << P_Rp << std::endl;
    const double qL = static_cast<double>(m_NumberOfEndmembers) / nbBands;
    SNR = vcl_abs(10*vcl_log10( (P_Rp - (m_NumberOfEndmembers/nbBands)*P_R) / (P_R - P_Rp) ));
  }


  SNRth = 15 + 10 * vcl_log(m_NumberOfEndmembers) + 8;
  std::cout << "SNR : "  << SNR << std::endl;
  std::cout << "SNRth : "  << SNRth << std::endl;

  typename VectorImageType::Pointer Xd;
  typename VectorImageType::Pointer Y;

  std::vector<itk::ProcessObject::Pointer> refHolder;
  typename ForwardPCAImageFilterType::Pointer pca = ForwardPCAImageFilterType::New();
  typename NormalizeFilterType::Pointer normalize = NormalizeFilterType::New();

  if (SNR > SNRth)
    {
    otbMsgDevMacroVCA( "Using projective projection for dimensionnality reduction" )

    otbMsgDevMacroVCA( "Computing SVD of correlation matrix" )
    // Take the correlation matrix
    vnl_matrix<PrecisionType> R = statsInput->GetCorrelation().GetVnlMatrix();

    // Apply SVD
    vnl_svd<PrecisionType> svd(R);
    vnl_matrix<PrecisionType> U = svd.U();
    Ud = U.get_n_columns(0, m_NumberOfEndmembers);
    vnl_matrix<PrecisionType> UdT = Ud.transpose();

    otbMsgDevMacroVCA( "Apply dimensionality reduction" )
    // Xd = Ud.'*M;
    typename MatrixImageFilterType::Pointer mulUd = MatrixImageFilterType::New();
    mulUd->MatrixByVectorOn();
    mulUd->SetInput(this->GetInput());
    mulUd->SetMatrix(UdT);
    mulUd->UpdateOutputInformation();
    refHolder.push_back(mulUd.GetPointer());

    Xd = mulUd->GetOutput();

    // Compute mean(Xd)
    otbMsgDevMacroVCA( "Compute mean(Xd)" )
    typename StreamingStatisticsVectorImageFilterType::Pointer
        statsXd = StreamingStatisticsVectorImageFilterType::New();
    statsXd->SetEnableSecondOrderStats(false);
    statsXd->SetInput(Xd);
    // statsXd->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(10);
    statsXd->Update();
    typename VectorImageType::PixelType Xdmean = statsXd->GetMean();
    otbMsgDevMacroVCA( "mean(Xd) = " << Xdmean)

    // Projective projection
    // Xd ./ repmat( sum( Xd .* repmat(u, [1 N]) ) , [d 1]);
    otbMsgDevMacroVCA( "Compute projective projection" )
    typename ProjectiveProjectionImageFilterType::Pointer proj = ProjectiveProjectionImageFilterType::New();
    proj->SetInput(Xd);
    proj->SetProjectionDirection(Xdmean);
    refHolder.push_back(proj.GetPointer());

    Xd = proj->GetOutput();
    Y = Xd;

    }
  else
    {
    otbMsgDevMacroVCA( "Using PCA for dimensionnality reduction" )

    normalize->SetInput(input);
    normalize->SetMean(statsInput->GetMean());
    normalize->SetUseMean(true);
    normalize->SetUseStdDev(false);

    // Take the correlation matrix
    vnl_matrix<PrecisionType> R = statsInput->GetCovariance().GetVnlMatrix();

    // Apply SVD
    vnl_svd<PrecisionType> svd(R);
    vnl_matrix<PrecisionType> U = svd.U();
    Ud = U.get_n_columns(0, m_NumberOfEndmembers - 1);
    vnl_matrix<PrecisionType> UdT = Ud.transpose();

    typename MatrixImageFilterType::Pointer mulUd = MatrixImageFilterType::New();
    mulUd->MatrixByVectorOn();
    mulUd->SetInput(normalize->GetOutput());
    mulUd->SetMatrix(UdT);
    mulUd->UpdateOutputInformation();
    refHolder.push_back(mulUd.GetPointer());

    Xd = mulUd->GetOutput();

    typename VectorImageToAmplitudeImageFilterType::Pointer normComputer = VectorImageToAmplitudeImageFilterType::New();
    normComputer->SetInput(Xd);

    typename StreamingMinMaxImageFilterType::Pointer maxNormComputer = StreamingMinMaxImageFilterType::New();
    maxNormComputer->SetInput(normComputer->GetOutput());
    maxNormComputer->Update();
    typename ImageType::PixelType maxNorm = maxNormComputer->GetMaximum();
    otbMsgDevMacroVCA( "maxNorm : "  << maxNorm)

    typename ConcatenateScalarValueImageFilterType::Pointer concat = ConcatenateScalarValueImageFilterType::New();
    concat->SetInput(Xd);
    concat->SetScalarValue(maxNorm);
    refHolder.push_back(concat.GetPointer());

    Y = concat->GetOutput();
    Y->UpdateOutputInformation();
    }

  // E : result, will contain the endmembers
  vnl_matrix<PrecisionType> E(nbBands, m_NumberOfEndmembers);

  // A = zeros(q, q)
  // A(q, 1) = 1
  vnl_matrix<PrecisionType> A(m_NumberOfEndmembers, m_NumberOfEndmembers);
  A.fill(0);
  A(m_NumberOfEndmembers - 1, 0) = 1;
  typename RandomVariateGeneratorType::Pointer randomGen = RandomVariateGeneratorType::New();

  for (unsigned int i = 0; i < m_NumberOfEndmembers; ++i)
    {
    otbMsgDevMacroVCA( "----------------------------------------" )
    otbMsgDevMacroVCA( "Iteration " << i )

    // w = rand(q, 1)
    otbMsgDevMacroVCA( "Random vector generation " )
    vnl_vector<PrecisionType> w(m_NumberOfEndmembers);
    for (unsigned int j = 0; j < w.size(); ++j)
      {
      w(j) = randomGen->GetVariateWithOpenRange();
      }

    // f = ((I - A*pinv(A))*w) / (norm(I - A*pinv(A))*w))
    otbMsgDevMacroVCA( "f = ((I - A*pinv(A))*w) /(norm(I - A*pinv(A))*w))" )
    vnl_matrix<PrecisionType> tmpMat(m_NumberOfEndmembers, m_NumberOfEndmembers);
    tmpMat.set_identity();
    otbMsgDevMacroVCA( "A" << std::endl << A )
    vnl_svd<PrecisionType> Asvd(A);
    tmpMat -= A * Asvd.inverse();

    vnl_vector<PrecisionType> tmpNumerator = tmpMat * w;
    vnl_vector<PrecisionType> f = tmpNumerator / tmpNumerator.two_norm();

    // v = f.'*Y
    otbMsgDevMacroVCA( "f = " << f );
    otbMsgDevMacroVCA( "v = f.'*Y" );
    typename DotProductImageFilterType::Pointer dotfY = DotProductImageFilterType::New();
    dotfY->SetInput(Y);

    typename VectorImageType::PixelType fV(f.data_block(), f.size());
    dotfY->SetVector(typename VectorImageType::PixelType(fV));
    typename ImageType::Pointer v = dotfY->GetOutput();

    // abs(v)
    otbMsgDevMacroVCA( "abs(v)" );
    typename AbsImageFilterType::Pointer absVFilter = AbsImageFilterType::New();
    absVFilter->SetInput(v);

    // max(abs(v))
    otbMsgDevMacroVCA( "max(abs(v))" );
    typename StreamingMinMaxImageFilterType::Pointer maxAbs = StreamingMinMaxImageFilterType::New();
    maxAbs->SetInput(absVFilter->GetOutput());
    maxAbs->Update();

    // k = arg_max( max(abs(v)) )
    otbMsgDevMacroVCA( "k = arg_max( max(abs(v)) )" )
    IndexType maxIdx = maxAbs->GetMaximumIndex();
    otbMsgDevMacroVCA( "max : " << maxAbs->GetMaximum() )
    otbMsgDevMacroVCA( "maxIdx : " << maxIdx )

    // extract Y(:, k)
    otbMsgDevMacroVCA( "Y(:, k)" )
    RegionType region;
    region.SetIndex( maxIdx );
    SizeType size;
    size.Fill(1);
    region.SetSize(size);
    Y->SetRequestedRegion(region);
    Y->Update();

    // store new endmember in A
    // A(:, i) = Y(:, k)
    otbMsgDevMacroVCA( "A(:, i) = Y(:, k)" )
    typename VectorImageType::PixelType e = Y->GetPixel(maxIdx);
    otbMsgDevMacroVCA( "e = " << e )
    A.set_column(i, e.GetDataPointer());

    otbMsgDevMacroVCA( "A" << std::endl << A )

    // reproject new endmember in original space
    vnl_vector<PrecisionType> u;
    if (SNR > SNRth)
      {
      // u = Ud * Xd(:, k)
      otbMsgDevMacroVCA( "u = Ud * Xd(:, k)" )
      Xd->SetRequestedRegion(region);
      Xd->Update();
      typename VectorImageType::PixelType xd = Xd->GetPixel(maxIdx);
      vnl_vector<PrecisionType> xdV(xd.GetDataPointer(), xd.GetSize());
      u = Ud * xdV;
      }
    else
      {
      // u = invPCA( Xd(:, k) )
      otbMsgDevMacroVCA( "u = Ud * Xd(:, k)" )
      Xd->SetRequestedRegion(region);
      Xd->Update();
      typename VectorImageType::PixelType xd = Xd->GetPixel(maxIdx);
      vnl_vector<PrecisionType> xdV(xd.GetDataPointer(), xd.GetSize());
      //Ud = pca->GetTransformationMatrix().GetVnlMatrix().transpose();

      vnl_vector<PrecisionType> mean(statsInput->GetMean().GetDataPointer(), statsInput->GetMean().GetSize());
      u = Ud * xdV + mean;
      }

    // E(:, i) = u
    otbMsgDevMacroVCA( "E(:, i) = u" )
    otbMsgDevMacroVCA( "u = " << u )
    E.set_column(i, u);
    }

  typename VectorImageType::Pointer output = this->GetOutput();
  output->SetRegions(output->GetLargestPossibleRegion());
  //output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
  output->Allocate();

  itk::ImageRegionIteratorWithIndex<VectorImageType> it(output, output->GetLargestPossibleRegion());
  unsigned int i;
  for (it.GoToBegin(), i = 0; !it.IsAtEnd(); ++it, ++i)
    {
    vnl_vector<PrecisionType> e = E.get_column(i);
    typename VectorImageType::PixelType pixel(input->GetNumberOfComponentsPerPixel());
    for (unsigned int j = 0; j < e.size(); ++j)
      {
      pixel[j] = E(j, i);
      }
    it.Set(pixel);
    }
}

/**
 * Standard "PrintSelf" method
 */
template<class TImage>
void VCAImageFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}

} //end namespace otb

#endif

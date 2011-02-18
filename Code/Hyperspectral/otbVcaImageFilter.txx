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
  VectorImageType* input = const_cast<VectorImageType*>(this->GetInput());
  const unsigned int nbBands = this->GetInput()->GetNumberOfComponentsPerPixel();

  otbMsgDevMacro( "Computing image stats" );
  typename StreamingStatisticsVectorImageFilterType::Pointer statsInput =
      StreamingStatisticsVectorImageFilterType::New();

  statsInput->SetInput(input);
  statsInput->Update();

  otbMsgDevMacro( "Computing SVD of correlation matrix" );
  // Take the correlation matrix
  vnl_matrix<PrecisionType> R = statsInput->GetCorrelation().GetVnlMatrix();

  // Apply SVD
  vnl_svd<PrecisionType>    svd(R);
  vnl_matrix<PrecisionType> U = svd.U();
  vnl_matrix<PrecisionType> Ud = U.get_n_columns(0, m_NumberOfEndmembers);
  vnl_matrix<PrecisionType> UdT = Ud.transpose();

  otbMsgDevMacro( "Apply dimensionality reduction" );
  // Xd = Ud.'*M;
  typename MatrixMultiplyImageFilterType::Pointer mulUd = MatrixMultiplyImageFilterType::New();
  mulUd->SetInput(this->GetInput());
  mulUd->SetMatrix(UdT);
  mulUd->UpdateOutputInformation();

  typename VectorImageType::Pointer Xd = mulUd->GetOutput();

  // Compute mean(Xd)
  otbMsgDevMacro( "Compute mean(Xd)" );
  typename StreamingStatisticsVectorImageFilterType::Pointer statsXd = \
      StreamingStatisticsVectorImageFilterType::New();
  statsXd->SetEnableCorrelation(false);
  statsXd->SetEnableCovariance(false);
  statsXd->SetInput(Xd);
  statsXd->Update();
  typename VectorImageType::PixelType Xdmean = statsXd->GetMean();

  // Projective projection
  // Xd ./ repmat( sum( Xd .* repmat(u,[1 N]) ) ,[d 1]);
  otbMsgDevMacro( "Compute projective projection" );
  typename ProjectiveProjectionImageFilterType::Pointer proj = ProjectiveProjectionImageFilterType::New();
  proj->SetInput(Xd);
  proj->SetProjectionDirection(Xdmean);
  typename  VectorImageType::Pointer Y = proj->GetOutput();

  // E : result, will contain the endmembers
  vnl_matrix<PrecisionType> E(nbBands, m_NumberOfEndmembers);

  // A = zeros(q,q)
  // A(q,1) = 1
  vnl_matrix<PrecisionType> A(m_NumberOfEndmembers, m_NumberOfEndmembers);
  A.fill(0);
  A(m_NumberOfEndmembers - 1, 0) = 1;
  typename RandomVariateGeneratorType::Pointer randomGen = RandomVariateGeneratorType::New();

  for (unsigned int i = 0; i < m_NumberOfEndmembers; ++i)
    {
    otbMsgDevMacro( "Iteration " << i );

    // w = rand(q,1)
    otbMsgDevMacro( "Random vector generation " );
    vnl_vector<PrecisionType> w(m_NumberOfEndmembers);
    for (unsigned int j = 0; j < m_NumberOfEndmembers; ++j)
      {
      w(j) = randomGen->GetVariateWithOpenRange();
      }

    // f = ((I - A*pinv(A))*w) / (norm(I - A*pinv(A))*w))
    otbMsgDevMacro( "f = ((I - A*pinv(A))*w) /(norm(I - A*pinv(A))*w))" );
    vnl_matrix<PrecisionType> tmpMat(m_NumberOfEndmembers,m_NumberOfEndmembers);
    tmpMat.set_identity();
    otbMsgDevMacro( "A" << std::endl << A );
    vnl_svd<PrecisionType> Asvd(A);
    tmpMat -= A * Asvd.inverse();

    vnl_vector<PrecisionType> tmpNumerator = tmpMat * w;
    vnl_vector<PrecisionType> f = tmpNumerator / tmpNumerator.two_norm();

    // v = f.'*Y
    otbMsgDevMacro( "v = f.'*Y" );
    typename DotProductImageFilterType::Pointer dotfY = DotProductImageFilterType::New();
    dotfY->SetInput(Y);
    typename VectorImageType::PixelType fV(f.data_block(), f.size());
    dotfY->SetVector(typename VectorImageType::PixelType(fV));
    typename ImageType::Pointer v = dotfY->GetOutput();

    // abs(v)
    otbMsgDevMacro( "abs(v)" );
    typename AbsImageFilterType::Pointer absVFilter = AbsImageFilterType::New();
    absVFilter->SetInput(v);

    // max(abs(v))
    otbMsgDevMacro( "max(abs(v))" );
    typename StreamingMinMaxImageFilterType::Pointer maxAbs = StreamingMinMaxImageFilterType::New();
    maxAbs->SetInput(absVFilter->GetOutput());
    maxAbs->Update();

    // k = arg_max( max(abs(v)) )
    otbMsgDevMacro( "k = arg_max( max(abs(v)) )" );
    IndexType maxIdx = maxAbs->GetMaximumIndex();
    otbMsgDevMacro( "maxIdx : " << maxIdx  );

    // extract Y(:,k)
    otbMsgDevMacro( "Y(:,k)" );
    RegionType region;
    region.SetIndex( maxIdx );
    SizeType size;
    size.Fill(1);
    region.SetSize(size);
    Y->SetRequestedRegion(region);
    Y->Update();

    // store new endmember in A
    // A(:,i) = Y(:,k)
    otbMsgDevMacro( "A(:,i) = Y(:,k)" );
    typename VectorImageType::PixelType e = Y->GetPixel(maxIdx);
    A.set_column(i, e.GetDataPointer());
    otbMsgDevMacro( "A" << std::endl << A );

    // reproject in original space
    // u = Ud * Xd(:,k)
    otbMsgDevMacro( "u = Ud * Xd(:,k)" );
    Xd->SetRequestedRegion(region);
    Xd->Update();
    typename VectorImageType::PixelType xd = Xd->GetPixel(maxIdx);
    vnl_vector<PrecisionType> xdV(xd.GetDataPointer(), xd.GetSize());
    vnl_vector<PrecisionType> u = Ud * xdV;

    // E(:, i) = u
    otbMsgDevMacro( "E(:, i) = u" );
    E.set_column(i, u);
    }

  typename VectorImageType::Pointer output = this->GetOutput();
  output->SetRegions(output->GetLargestPossibleRegion());
  output->Allocate();

  itk::ImageRegionIteratorWithIndex<VectorImageType> it(output, output->GetLargestPossibleRegion());
  unsigned int i;
  for (it.GoToBegin(), i = 0; !it.IsAtEnd(); ++it, ++i)
    {
    vnl_vector<PrecisionType> e = E.get_column(i);
    typename VectorImageType::PixelType pixel(e.data_block(), e.size());
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

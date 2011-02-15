/*
 * VCAFilter.txx
 * Date:     $Date$
 * Version:  $Revision$
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

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

  std::cout << "Computing image stats" << std::endl;
  typename StreamingStatisticsVectorImageFilterType::Pointer statsInput =
      StreamingStatisticsVectorImageFilterType::New();

  statsInput->SetInput(input);
  statsInput->Update();

  std::cout << "Computing SVD of correlation matrix" << std::endl;
  // Take the correlation matrix
  vnl_matrix<PrecisionType> R = statsInput->GetCorrelation().GetVnlMatrix();

  // Apply SVD
  vnl_svd<PrecisionType>    svd(R);
  vnl_matrix<PrecisionType> U = svd.U();
  vnl_matrix<PrecisionType> Ud = U.get_n_columns(0, m_NumberOfEndmembers);
  vnl_matrix<PrecisionType> UdT = Ud.transpose();

//  logmatrixsize(U);
//  logmatrixsize(Ud);

  std::cout << "Apply dimensionality reduction" << std::endl;
  // Xd = Ud.'*M;
  typename MatrixMultiplyImageFilterType::Pointer mulUd = MatrixMultiplyImageFilterType::New();
  mulUd->SetInput(this->GetInput());
  mulUd->SetMatrix(UdT);
  mulUd->UpdateOutputInformation();

  typename VectorImageType::Pointer Xd = mulUd->GetOutput();
  //  logImagesize(Xd);

  // Compute mean(Xd)
  std::cout << "Compute mean(Xd)" << std::endl;
  typename StreamingStatisticsVectorImageFilterType::Pointer statsXd = \
      StreamingStatisticsVectorImageFilterType::New();
  statsXd->SetInput(Xd);
  statsXd->Update();
  typename VectorImageType::PixelType Xdmean = statsXd->GetMean();

  // Projective projection
  // Xd ./ repmat( sum( Xd .* repmat(u,[1 N]) ) ,[d 1]);
  std::cout << "Compute projective projection" << std::endl;
  typename ProjectiveProjectionImageFilterType::Pointer proj = ProjectiveProjectionImageFilterType::New();
  proj->SetInput(Xd);
  proj->SetProjectionDirection(Xdmean);
  typename  VectorImageType::Pointer Y = proj->GetOutput();
  //  logImagesize(Y);

  // E : result, will contain the endmembers
  vnl_matrix<PrecisionType> E(nbBands, m_NumberOfEndmembers);
  //  logmatrixsize(E);

  // A = zeros(q,q)
  // A(q,1) = 1
  vnl_matrix<PrecisionType> A(m_NumberOfEndmembers, m_NumberOfEndmembers);
  A.fill(0);
  A(m_NumberOfEndmembers - 1, 0) = 1;
  typename RandomVariateGeneratorType::Pointer randomGen = RandomVariateGeneratorType::New();

  for (unsigned int i = 0; i < m_NumberOfEndmembers; ++i)
    {
    std::cout << "Iteration " << i << std::endl;

    // w = rand(q,1)
    std::cout << "Random vector generation " << std::endl;
    vnl_vector<PrecisionType> w(m_NumberOfEndmembers);
    for (unsigned int j = 0; j < m_NumberOfEndmembers; ++j)
      {
      w(j) = randomGen->GetVariateWithOpenRange();
      }

    // f = ((I - A*pinv(A))*w) / (norm(I - A*pinv(A))*w))
    std::cout << "f = ((I - A*pinv(A))*w) /(norm(I - A*pinv(A))*w))" << std::endl;
    vnl_matrix<PrecisionType> tmpMat(m_NumberOfEndmembers,m_NumberOfEndmembers);
    tmpMat.set_identity();
    std::cout << "A" << std::endl << A << std::endl;
    vnl_svd<PrecisionType> Asvd(A);
    tmpMat -= A * Asvd.inverse();

    vnl_vector<PrecisionType> tmpNumerator = tmpMat * w;
    vnl_vector<PrecisionType> f = tmpNumerator / tmpNumerator.two_norm();

    // v = f.'*Y
    std::cout << "v = f.'*Y" << std::endl;
    typename DotProductImageFilterType::Pointer dotfY = DotProductImageFilterType::New();
    dotfY->SetInput(Y);
    typename VectorImageType::PixelType fV(f.data_block(), f.size());
    dotfY->SetVector(typename VectorImageType::PixelType(fV));
    typename ImageType::Pointer v = dotfY->GetOutput();
    //    logImagesize(v);

    // abs(v)
    std::cout << "abs(v)" << std::endl;
    typename AbsImageFilterType::Pointer absVFilter = AbsImageFilterType::New();
    absVFilter->SetInput(v);

    // max(abs(v))
    std::cout << "max(abs(v))" << std::endl;
    typename StreamingMinMaxImageFilterType::Pointer maxAbs = StreamingMinMaxImageFilterType::New();
    maxAbs->SetInput(absVFilter->GetOutput());
    maxAbs->Update();

    // k = arg_max( max(abs(v)) )
    std::cout << "k = arg_max( max(abs(v)) )" << std::endl;
    IndexType maxIdx = maxAbs->GetMaximumIndex();
    std::cout << "maxIdx : " << maxIdx << std::endl;

    // extract Y(:,k)
    std::cout << "Y(:,k)" << std::endl;
    RegionType region;
    region.SetIndex( maxIdx );
    SizeType size;
    size.Fill(1);
    region.SetSize(size);
    Y->SetRequestedRegion(region);
    Y->Update();

    // store new endmember in A
    // A(:,i) = Y(:,k)
    std::cout << "A(:,i) = Y(:,k)" << std::endl;
    typename VectorImageType::PixelType e = Y->GetPixel(maxIdx);
    A.set_column(i, e.GetDataPointer());
    std::cout << "A" << std::endl << A << std::endl;

    // reproject in original space
    // u = Ud * Xd(:,k)
    std::cout << "u = Ud * Xd(:,k)" << std::endl;
    Xd->SetRequestedRegion(region);
    Xd->Update();
    typename VectorImageType::PixelType xd = Xd->GetPixel(maxIdx);
    vnl_vector<PrecisionType> xdV(xd.GetDataPointer(), xd.GetSize());
    //    logvectorsize(xdV);
    //    logmatrixsize(Ud);
    vnl_vector<PrecisionType> u = Ud * xdV;
    //    logvectorsize(u);

    // E(:, i) = u
    std::cout << "E(:, i) = u" << std::endl;
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

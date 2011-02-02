/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbDotProductImageFilter.h"
#include "otbProjectiveProjectionImageFilter.h"
#include "otbMatrixMultiplyImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStreamingMinMaxImageFilter2.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter2.h"
#include "itkAbsImageFilter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "vnl/algo/vnl_svd.h"

const unsigned int Dimension = 2;
typedef double PixelType;
typedef double PrecisionType;

typedef otb::Image<PixelType, Dimension> ImageType;
typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
typedef ImageType::IndexType IndexType;
typedef ImageType::SizeType SizeType;
typedef ImageType::RegionType RegionType;
typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef itk::AbsImageFilter<ImageType, ImageType> AbsImageFilterType;
typedef otb::ProjectiveProjectionImageFilter<VectorImageType,VectorImageType,PrecisionType> ProjectiveProjectionImageFilterType;
typedef otb::DotProductImageFilter<VectorImageType,ImageType> DotProductImageFilterType;
typedef otb::MatrixMultiplyImageFilter<VectorImageType,VectorImageType,PrecisionType> MatrixMultiplyImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<VectorImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<VectorImageType> WriterType;
typedef otb::StreamingMinMaxImageFilter2<ImageType> StreamingMinMaxImageFilterType;
typedef otb::StreamingStatisticsVectorImageFilter2<VectorImageType> StreamingStatisticsVectorImageFilterType;
typedef otb::StreamingStatisticsImageFilter<ImageType> StreamingStatisticsImageFilterType;

typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomVariateGeneratorType;

typedef StreamingStatisticsVectorImageFilterType::MatrixType MatrixType;

#define logmatrixsize(p) std::cout << #p << " rows : " << p.rows() << "  cols : " << p.cols() << std::endl;
#define logvectorsize(p) std::cout << #p << " n : " << p.size() << std::endl;
#define logImagesize(p) p->UpdateOutputInformation(); std::cout << #p << " size : " << p->GetLargestPossibleRegion().GetSize() <<  "  nbBands : " << p->GetNumberOfComponentsPerPixel() << std::endl;

int otbVCATestHighSNR(int argc, char * argv[])
{
  const char * inputImage = argv[1];
  const char * outputImage = argv[2];
  const unsigned int nbEndmembers = atoi(argv[3]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);
  readerImage->UpdateOutputInformation();

  const unsigned int nbBands = readerImage->GetOutput()->GetNumberOfComponentsPerPixel();

  std::cout << "Computing image stats" << std::endl;
  StreamingStatisticsVectorImageFilterType::Pointer statsInput = \
      StreamingStatisticsVectorImageFilterType::New();

  statsInput->SetInput(readerImage->GetOutput());
  statsInput->Update();

  std::cout << "Computing SVD of correlation matrix" << std::endl;
  // Take the correlation matrix
  vnl_matrix<PrecisionType> R = statsInput->GetCorrelation().GetVnlMatrix();

  // Apply SVD
  vnl_svd<PrecisionType>    svd(R);
  vnl_matrix<PrecisionType> U = svd.U();
  vnl_matrix<PrecisionType> Ud = U.get_n_columns(0, nbEndmembers);
  vnl_matrix<PrecisionType> UdT = Ud.transpose();

  logmatrixsize(U);
  logmatrixsize(Ud);


  std::cout << "Apply dimensionality reduction" << std::endl;
  // Xd = Ud.'*M;
  MatrixMultiplyImageFilterType::Pointer mulUd = MatrixMultiplyImageFilterType::New();
  mulUd->SetInput(readerImage->GetOutput());
  mulUd->SetMatrix(UdT);
  mulUd->UpdateOutputInformation();

  VectorImageType::Pointer Xd = mulUd->GetOutput();
  logImagesize(Xd);

  // Compute mean(Xd)
  std::cout << "Compute mean(Xd)" << std::endl;
  StreamingStatisticsVectorImageFilterType::Pointer statsXd = \
      StreamingStatisticsVectorImageFilterType::New();
  statsXd->SetInput(Xd);
  statsXd->Update();
  VectorImageType::PixelType Xdmean = statsXd->GetMean();

  // Projective projection
  // Xd ./ repmat( sum( Xd .* repmat(u,[1 N]) ) ,[d 1]);
  std::cout << "Compute projective projection" << std::endl;
  ProjectiveProjectionImageFilterType::Pointer proj = ProjectiveProjectionImageFilterType::New();
  proj->SetInput(Xd);
  proj->SetProjectionDirection(Xdmean);
  VectorImageType::Pointer Y = proj->GetOutput();
  logImagesize(Y);

  // E : result, will contain the endmembers
  vnl_matrix<PrecisionType> E(nbBands, nbEndmembers);
  logmatrixsize(E);

  // A = zeros(q,q)
  // A(q,1) = 1
  vnl_matrix<PrecisionType> A(nbEndmembers, nbEndmembers);
  A.fill(0);
  A(nbEndmembers - 1, 0) = 1;
  RandomVariateGeneratorType::Pointer randomGen = RandomVariateGeneratorType::New();

  for (unsigned int i = 0; i < nbEndmembers; ++i)
    {
    std::cout << "Iteration " << i << std::endl;

    // w = rand(q,1)
    std::cout << "Random vector generation " << std::endl;
    vnl_vector<PrecisionType> w(nbEndmembers);
    for (unsigned int j = 0; j < nbEndmembers; ++j)
      {
      w(j) = randomGen->GetVariateWithOpenRange();
      }

    // f = ((I - A*pinv(A))*w) / (norm(I - A*pinv(A))*w))
    std::cout << "f = ((I - A*pinv(A))*w) /(norm(I - A*pinv(A))*w))" << std::endl;
    vnl_matrix<PrecisionType> tmpMat(nbEndmembers,nbEndmembers);
    tmpMat.set_identity();
std::cout << "A" << std::endl << A << std::endl;
    vnl_svd<PrecisionType> Asvd(A);
    tmpMat -= A * Asvd.inverse();

    vnl_vector<PrecisionType> tmpNumerator = tmpMat * w;
    vnl_vector<PrecisionType> f = tmpNumerator / tmpNumerator.two_norm();

    // v = f.'*Y
    std::cout << "v = f.'*Y" << std::endl;
    DotProductImageFilterType::Pointer dotfY = DotProductImageFilterType::New();
    dotfY->SetInput(Y);
    VectorImageType::PixelType fV(f.data_block(), f.size());
    dotfY->SetVector(VectorImageType::PixelType(fV));
    ImageType::Pointer v = dotfY->GetOutput();
    logImagesize(v);

    // abs(v)
    std::cout << "abs(v)" << std::endl;
    AbsImageFilterType::Pointer absVFilter = AbsImageFilterType::New();
    absVFilter->SetInput(v);

    // max(abs(v))
    std::cout << "max(abs(v))" << std::endl;
    StreamingMinMaxImageFilterType::Pointer maxAbs = StreamingMinMaxImageFilterType::New();
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
    VectorImageType::PixelType e = Y->GetPixel(maxIdx);
    A.set_column(i, e.GetDataPointer());

    // reproject in original space
    // u = Ud * Xd(:,k)
    std::cout << "u = Ud * Xd(:,k)" << std::endl;
    Xd->SetRequestedRegion(region);
    Xd->Update();
    VectorImageType::PixelType xd = Xd->GetPixel(maxIdx);
    vnl_vector<PrecisionType> xdV(xd.GetDataPointer(), xd.GetSize());
    logvectorsize(xdV);
    logmatrixsize(Ud);
    vnl_vector<PrecisionType> u = Ud * xdV;
    logvectorsize(u);

    // E(:, i) = u
    std::cout << "E(:, i) = u" << std::endl;
    E.set_column(i, u);
    }

  VectorImageType::Pointer output = VectorImageType::New();
  RegionType region;
  IndexType  index;
  SizeType   size;
  index.Fill(0);
  size[0] = nbEndmembers;
  size[1] = 1;
  region.SetIndex(index);
  region.SetSize(size);
  output->SetRegions(region);
  output->SetNumberOfComponentsPerPixel(nbBands);
  output->Allocate();

  itk::ImageRegionIteratorWithIndex<VectorImageType> it(output, output->GetLargestPossibleRegion());
  unsigned int i;
  for (it.GoToBegin(), i = 0; !it.IsAtEnd(); ++it, ++i)
    {
    vnl_vector<PrecisionType> e = E.get_column(i);
    VectorImageType::PixelType pixel(e.data_block(), e.size());
    it.Set(pixel);
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}

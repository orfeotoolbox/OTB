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
#include "otbMatrixImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;
typedef double PrecisionType;

typedef otb::Image<PixelType, Dimension> ImageType;
typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::ProjectiveProjectionImageFilter<VectorImageType, VectorImageType, PrecisionType> ProjectiveProjectionImageFilterType;
typedef otb::DotProductImageFilter<VectorImageType, ImageType> DotProductImageFilterType;
typedef otb::MatrixImageFilter<VectorImageType, VectorImageType> MatrixImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<VectorImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<VectorImageType> WriterType;
typedef otb::StreamingStatisticsVectorImageFilter<VectorImageType> StreamingStatisticsVectorImageFilterType;
typedef otb::StreamingStatisticsImageFilter<ImageType> StreamingStatisticsImageFilterType;

typedef StreamingStatisticsVectorImageFilterType::MatrixType MatrixType;

int otbProjectiveProjectionNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  ProjectiveProjectionImageFilterType::Pointer filter = ProjectiveProjectionImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbProjectiveProjectionTestHighSNR(int itkNotUsed(argc), char * argv[])
{
  const char * inputImage = argv[1];
  const unsigned int nbEndmembers = atoi(argv[2]);
  const char * outputImage = argv[3];


  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

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
  vnl_matrix<PrecisionType> Ud = U.get_n_columns(0, nbEndmembers).transpose();

  std::cout << "Apply dimensionnality reduction" << std::endl;
  // Xd = Ud.'*M;
  MatrixImageFilterType::Pointer mulUd = MatrixImageFilterType::New();
  mulUd->SetInput(readerImage->GetOutput());
  mulUd->SetMatrix(Ud);
  mulUd->MatrixByVectorOn();
  mulUd->UpdateOutputInformation();

  VectorImageType::Pointer Xd = mulUd->GetOutput();

  // Compute mean(Xd)
  std::cout << "Compute mean(Xd)" << std::endl;
  StreamingStatisticsVectorImageFilterType::Pointer statsXd = \
      StreamingStatisticsVectorImageFilterType::New();
  statsXd->SetInput(Xd);
  statsXd->Update();
  VectorImageType::PixelType Xdmean = statsXd->GetMean();

  // Compute Xd ./ repmat( sum( Xd .* repmat(u, [1 N]) ) , [d 1]);
  // -> divides each pixel component by the dot product <Xd(i, j), mean(Xd)>
  std::cout << "Compute projective projection" << std::endl;
  ProjectiveProjectionImageFilterType::Pointer proj = ProjectiveProjectionImageFilterType::New();
  proj->SetInput(Xd);
  proj->SetProjectionDirection(Xdmean);

  std::cout << "Write output" << std::endl;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(proj->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

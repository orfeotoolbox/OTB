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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbVcaImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}
//    OUTPUTS: {PCAOutput.tif}, {InversePCAOutput.tif}, {InversePCAOutput1.png}, {PCAOutput1.png}, {PCAOutput2.png}, {PCAOutput3.png}
//    3
//  Software Guide : EndCommandLineArgs
int main(int argc, char * argv[])
{
  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "inputFileName outputFileName EstimatenumberOfEndmembers outputPrettyFilename1 outputPrettyFilename2 outputPrettyFilename3   " << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int estimateNumberOfEndmembers = atoi(argv[3]);
  const unsigned int Dimension = 2;

  typedef double                                  PixelType;
  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::VectorImage<unsigned char, 2>      OutputPrettyImageType;

  typedef otb::ImageFileReader<ImageType>         ReaderType;
  typedef otb::ImageFileWriter<ImageType>    WriterType;
  
  typedef otb::VectorRescaleIntensityImageFilter<ImageType,ImageType>             RescalerType;
  typedef otb::VectorRescaleIntensityImageFilter<ImageType,OutputPrettyImageType> RescalerType2;
  
  typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;
  typedef otb::VCAImageFilter<ImageType>                       VCAFilterType;

  typedef otb::UnConstrainedLeastSquareImageFilter<ImageType,ImageType,double> UCLSUnmixingFilterType;

  typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;

  typedef vnl_vector<double> VectorType;
  typedef vnl_matrix<double> MatrixType;

  /// / Noise filtering
  //  typedef otb::LocalActivityVectorImageFilter< ImageType, ImageType > NoiseFilterType;

  // // Image filtering
  //  typedef otb::MNFImageFilter< ImageType, ImageType,
  //    NoiseFilterType, otb::Transform::FORWARD > FilterType;

  //read image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  

  //rescale input image between 0 and 1
  RescalerType::Pointer         rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(1.);
  /*
  * Estimate Endmembers
  */
  std::cout << "Estimate Endmembers by VCA" << std::endl;

  VCAFilterType::Pointer vca = VCAFilterType::New();
  vca->SetNumberOfEndmembers(estimateNumberOfEndmembers);
  vca->SetInput(rescaler->GetOutput());

  ImageType::Pointer endmembersImage;
  endmembersImage = vca->GetOutput();

  /*
     * Transform Endmembers image to matrix representation
     */
  std::cout << "Endmembers extracted" << std::endl;
  std::cout << "Converting endmembers to matrix" << std::endl;
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(endmembersImage);
  endMember2Matrix->Update();

  MatrixType endMembersMatrix = endMember2Matrix->GetMatrix();
  std::cout << "Endmembers matrix : " << endMembersMatrix << std::endl;

  /*
  * Unmix
  */
  UCLSUnmixingFilterType::Pointer unmixer =
    UCLSUnmixingFilterType::New();

  unmixer->SetInput(rescaler->GetOutput());
  unmixer->SetMatrix(endMembersMatrix);
  unmixer->SetNumberOfThreads(1); // FIXME : currently buggy
      
  ImageType::Pointer abundanceMap;
  abundanceMap = unmixer->GetOutput();
      
  /*
  * Write abundance map
  */
  std::cout << "Write abundance map " << outfname << std::endl;

  WriterType::Pointer  writer = WriterType::New();
  writer->SetInput(abundanceMap);
  writer->SetFileName(outfname);
  
  writer->Update();
  
  typedef otb::Image<PixelType, Dimension>                        MonoImageType;
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType> ExtractROIFilterType;
  typedef otb::Image<unsigned char, 2>                            OutputImageType;
  typedef itk::RescaleIntensityImageFilter<MonoImageType,
                                           OutputImageType>       PrettyRescalerType;
  typedef otb::ImageFileWriter<OutputImageType>                   WriterType2;

  for (unsigned int cpt = 0; cpt < 3; ++cpt)
    {
    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    PrettyRescalerType::Pointer   prettyRescaler = PrettyRescalerType::New();
    WriterType2::Pointer          writer2 = WriterType2::New();

    extractROIFilter->SetInput(abundanceMap);
    extractROIFilter->SetChannel(cpt + 1);

    prettyRescaler->SetInput(extractROIFilter->GetOutput());
    prettyRescaler->SetOutputMinimum(0);
    prettyRescaler->SetOutputMaximum(255);

    writer2->SetInput(prettyRescaler->GetOutput());
    writer2->SetFileName(argv[cpt + 4]);
    writer2->Update();
    }

  return EXIT_SUCCESS;
} // end main

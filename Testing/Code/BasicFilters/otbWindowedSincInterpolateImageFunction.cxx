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
#include "itkExceptionObject.h"

#include "otbWindowedSincInterpolateImageFunction.h"
#include "itkConstantBoundaryCondition.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

#include "otbStreamingImageFileWriter.h"
#include "itkTranslationTransform.h"
#include "otbStreamingResampleImageFilter.h"
  
int otbWindowedSincInterpolateImageFunction(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef otb::Image<double,2>                                                ImageType;
  // Gaussian
  typedef otb::Function::GaussianWindowFunction<double, double>               GaussType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType, GaussType>     GaussInterpolatorType;
  typedef GaussInterpolatorType::ContinuousIndexType                          GaussContinuousIndexType;
  // Cosine
  typedef otb::Function::CosineWindowFunction<double, double>                 CosType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType, CosType>       CosInterpolatorType;
  typedef CosInterpolatorType::ContinuousIndexType                            CosContinuousIndexType;
  // Hamming
  typedef otb::Function::HammingWindowFunction<double, double>                HammingType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType, HammingType>   HammingInterpolatorType;
  typedef HammingInterpolatorType::ContinuousIndexType                        HammingContinuousIndexType;
  // Welch
  typedef otb::Function::WelchWindowFunction<double, double>                  WelchType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType,WelchType>      WelchInterpolatorType;
  typedef WelchInterpolatorType::ContinuousIndexType                          WelchContinuousIndexType;
  // Lanczos
  typedef otb::Function::LanczosWindowFunction<double, double>                LanczosType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType, LanczosType>   LanczosInterpolatorType;
  typedef LanczosInterpolatorType::ContinuousIndexType                        LanczosContinuousIndexType;
  // Blackman
  typedef otb::Function::BlackmanWindowFunction<double, double>               BlackmanType;
  typedef otb::WindowedSincInterpolateImageFunction<ImageType, BlackmanType>  BlackmanInterpolatorType;
  typedef BlackmanInterpolatorType::ContinuousIndexType                       BlackmanContinuousIndexType;
  
  typedef otb::ImageFileReader<ImageType>                                     ReaderType;
  
  // Instantiating objects
  GaussInterpolatorType::Pointer    gauss    = GaussInterpolatorType::New();
  CosInterpolatorType::Pointer      cos      = CosInterpolatorType::New();
  HammingInterpolatorType::Pointer  hamming  = HammingInterpolatorType::New();
  WelchInterpolatorType::Pointer    welch    = WelchInterpolatorType::New();
  LanczosInterpolatorType::Pointer  lanczos  = LanczosInterpolatorType::New();
  BlackmanInterpolatorType::Pointer blackman = BlackmanInterpolatorType::New();

  unsigned int i = 4;
  
  std::vector<GaussContinuousIndexType>    gaussindicesList;
  std::vector<CosContinuousIndexType>      cosindicesList;
  std::vector<HammingContinuousIndexType>  hammingindicesList;
  std::vector<WelchContinuousIndexType>    welchindicesList;
  std::vector<LanczosContinuousIndexType>  lanczosindicesList;
  std::vector<BlackmanContinuousIndexType> blackmanindicesList;
  while(i<static_cast<unsigned int>(argc) && (i+1)<static_cast<unsigned int>(argc))
    {
      GaussContinuousIndexType idx1;
      idx1[0]=atof(argv[i]);
      idx1[1]=atof(argv[i+1]);   
      gaussindicesList.push_back(idx1);
      CosContinuousIndexType idx2;
      idx2[0]=atof(argv[i]);
      idx2[1]=atof(argv[i+1]);   
      cosindicesList.push_back(idx2);
      HammingContinuousIndexType idx3;
      idx3[0]=atof(argv[i]);
      idx3[1]=atof(argv[i+1]);   
      hammingindicesList.push_back(idx3);
      WelchContinuousIndexType idx4;
      idx4[0]=atof(argv[i]);
      idx4[1]=atof(argv[i+1]);   
      welchindicesList.push_back(idx4);
      LanczosContinuousIndexType idx5;
      idx5[0]=atof(argv[i]);
      idx5[1]=atof(argv[i+1]);   
      lanczosindicesList.push_back(idx5);
      BlackmanContinuousIndexType idx6;
      idx6[0]=atof(argv[i]);
      idx6[1]=atof(argv[i+1]);   
      blackmanindicesList.push_back(idx6);
      i+=2;
    }


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  gauss->SetInputImage(reader->GetOutput());
  gauss->SetRadius(atoi(argv[3]));
  cos->SetInputImage(reader->GetOutput());
  cos->SetRadius(atoi(argv[3]));
  hamming->SetInputImage(reader->GetOutput());
  hamming->SetRadius(atoi(argv[3]));
  welch->SetInputImage(reader->GetOutput());
  welch->SetRadius(atoi(argv[3]));
  lanczos->SetInputImage(reader->GetOutput());
  lanczos->SetRadius(atoi(argv[3]));
  blackman->SetInputImage(reader->GetOutput());
  blackman->SetRadius(atoi(argv[3]));

  std::ofstream file;
  file.open(outfname);
  
  file << "Gauss Window Function"<<std::endl;
  for(std::vector<GaussContinuousIndexType>::iterator it = gaussindicesList.begin();it!=gaussindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<gauss->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file << std::endl;
  
  file << "Cosine Window Function"<<std::endl;
  for(std::vector<CosContinuousIndexType>::iterator it = cosindicesList.begin();it!=cosindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<cos->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file << std::endl;
  
  file << "Hamming Window Function"<<std::endl;
  for(std::vector<HammingContinuousIndexType>::iterator it = hammingindicesList.begin();it!=hammingindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<hamming->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file << std::endl;
  
  file << "Welch Window Function"<<std::endl;
  for(std::vector<WelchContinuousIndexType>::iterator it = welchindicesList.begin();it!=welchindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<welch->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file << std::endl;
  
  file << "Lanczos Window Function"<<std::endl;
  for(std::vector<LanczosContinuousIndexType>::iterator it = lanczosindicesList.begin();it!=lanczosindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<lanczos->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  file << std::endl;
  
  file << "Blackman Window Function"<<std::endl;
  for(std::vector<BlackmanContinuousIndexType>::iterator it = blackmanindicesList.begin();it!=blackmanindicesList.end();++it)
    {
      file<<(*it)<<" -> "<<blackman->EvaluateAtContinuousIndex((*it))<<std::endl;
    }
  
  file.close();
  


  /**********************************************/
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  typedef itk::TranslationTransform<double,2> TransformType;
  typedef otb::StreamingResampleImageFilter<ImageType,ImageType,double> StreamingResampleImageFilterType;
  
  // Instantiating object
  WriterType::Pointer writer = WriterType::New();
  StreamingResampleImageFilterType::Pointer resampler = StreamingResampleImageFilterType::New();
  TransformType::Pointer transform = TransformType::New();
  
  // Resampler connected to input image
  resampler->SetInput(reader->GetOutput());
  
  // Size of output resampler result
  StreamingResampleImageFilterType::SizeType size;
  size[0]=600;
  size[1]=600;
  resampler->SetSize(size);
  
  resampler->SetInterpolatorNeighborhoodRadius(atoi(argv[3]));
  resampler->SetInterpolator(gauss);

  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 100;
  translation[1] = 200;			
  transform->SetOffset(translation);
  
  // Resampler is updated with new transformation (default is identity)
  resampler->SetTransform(transform);
  
  // Result of resampler is written
  writer->SetInput(resampler->GetOutput());
  writer->SetNumberOfStreamDivisions(1);
  writer->SetFileName("tutu.tif");
  
  writer->Update();			




  return EXIT_SUCCESS;
}

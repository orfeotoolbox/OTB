/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImagePCAShapeModelEstimatorTest.cxx, v $
  Language:  C++
  Date:      $Date: 2008-02-03 04:05:34 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Insight classes
#include "otbImage.h"
#include "itkVector.h"
#include "vnl/vnl_matrix_fixed.h"
#include "vnl/vnl_math.h"
#include "itkLightProcessObject.h"
#include "itkTextOutput.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkImagePCAShapeModelEstimator.h"

//Data definitions
#define   IMGWIDTH            2
#define   IMGHEIGHT           2
#define   NDIMENSION          2
#define   NUMTRAINIMAGES      3
#define   NUMLARGESTPC        3

// class to support progress feeback

class ShowProgressObject
{
public:
  ShowProgressObject(itk::LightProcessObject * o)
  {
    m_Process = o;
  }
  void ShowProgress()
  {
    std::cout << "Progress " << m_Process->GetProgress() << std::endl;
  }
  itk::LightProcessObject::Pointer m_Process;
};

int otbImagePCAShapeModelEstimatorTest(int itkNotUsed(argc), char* argv[])
{
  /*    const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[1]));
      const unsigned int numberOfTrainingImages(atoi(argv[2]));
      std::vector<std::string> imagesfilenames;
      std::vector<std::string> outputimagesfilenames;
      int cpt(3);
      for(; cpt<(numberOfTrainingImages+3); ++cpt)
      {
          imagesfilenames.push_back(argv[cpt]);
      }
      int cpt2(cpt);
      for(; cpt<(numberOfPrincipalComponentsRequired+cpt2); ++cpt)
      {
          outputimagesfilenames.push_back(argv[cpt]);
      }
  */

  itk::OutputWindow::SetInstance(itk::TextOutput::New().GetPointer());

  //------------------------------------------------------
  //Create 3 simple test images with
  //------------------------------------------------------
  typedef otb::Image<double, NDIMENSION> InputImageType;
  typedef otb::Image<double, NDIMENSION> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  // Instantiating object
  ReaderType::Pointer reader1 = ReaderType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  ReaderType::Pointer reader3 = ReaderType::New();

  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  WriterType::Pointer writer3 = WriterType::New();
  WriterType::Pointer writer4 = WriterType::New();

  reader1->SetFileName(argv[1]);
  reader2->SetFileName(argv[2]);
  reader3->SetFileName(argv[3]);

  writer1->SetFileName(argv[4]);
  writer2->SetFileName(argv[5]);
  writer3->SetFileName(argv[6]);
  writer4->SetFileName(argv[7]);

  //----------------------------------------------------------------------
  // Test code for the Shape model estimator
  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  //Set the image model estimator
  //----------------------------------------------------------------------
  typedef itk::ImagePCAShapeModelEstimator<InputImageType, OutputImageType>
  ImagePCAShapeModelEstimatorType;

  ImagePCAShapeModelEstimatorType::Pointer
    applyPCAShapeEstimator = ImagePCAShapeModelEstimatorType::New();

  //----------------------------------------------------------------------
  //Set the parameters of the clusterer
  //----------------------------------------------------------------------
  applyPCAShapeEstimator->SetNumberOfTrainingImages(NUMTRAINIMAGES);
  applyPCAShapeEstimator->SetNumberOfPrincipalComponentsRequired(NUMLARGESTPC + 1);
  applyPCAShapeEstimator->SetNumberOfPrincipalComponentsRequired(NUMLARGESTPC);
  applyPCAShapeEstimator->SetInput(0, reader1->GetOutput());
  applyPCAShapeEstimator->SetInput(1, reader2->GetOutput());
  applyPCAShapeEstimator->SetInput(2, reader3->GetOutput());

  applyPCAShapeEstimator->Update();

  writer1->SetInput(applyPCAShapeEstimator->GetOutput(0));
  writer1->Update();
  writer2->SetInput(applyPCAShapeEstimator->GetOutput(1));
  writer2->Update();
  writer3->SetInput(applyPCAShapeEstimator->GetOutput(2));
  writer3->Update();
  writer4->SetInput(applyPCAShapeEstimator->GetOutput(3));
  writer4->Update();

  //Test the printself function to increase coverage
  applyPCAShapeEstimator->Print(std::cout);

  //Exercise TypeMacro in superclass
  typedef ImagePCAShapeModelEstimatorType::Superclass GenericEstimatorType;
  std::cout << applyPCAShapeEstimator->GenericEstimatorType::GetNameOfClass() << std::endl;

  //Print out the number of training images and the number of principal
  //components
  std::cout << "The number of training images are: " <<
  applyPCAShapeEstimator->GetNumberOfTrainingImages() << std::endl;

  std::cout << "The number of principal components desired are: " <<
  applyPCAShapeEstimator->GetNumberOfPrincipalComponentsRequired() << std::endl;

  //Print the eigen vectors
  vnl_vector<double> eigenValues =
    applyPCAShapeEstimator->GetEigenValues();
  unsigned int numEigVal =  eigenValues.size();
  std::cout << "Number of returned eign-values: " << numEigVal << std::endl;

  std::cout << "The " <<
  applyPCAShapeEstimator->GetNumberOfPrincipalComponentsRequired() <<
  " largest eigen values are:" << std::endl;

  for (unsigned int i = 0; i < vnl_math_min(numEigVal, (unsigned int) NUMLARGESTPC); ++i)
    {
    std::cout << eigenValues[i] << std::endl;
    }

  //Print the MeanImage
/*
  OutputImageType::Pointer outImage = applyPCAShapeEstimator->GetOutput( 0 );
  OutputImageIterator outImageIt( outImage, outImage->GetBufferedRegion() );
  outImageIt.GoToBegin();

  std::cout << "The mean image is:" << std::endl;
  while (!outImageIt.IsAtEnd() )
  {
    std::cout << (double)(outImageIt.Get()) << ";"  << std::endl;
    ++outImageIt;
  }
  std::cout << "  " << std::endl;
  //Print the largest two eigen vectors
  for (unsigned int j=1; j< NUMLARGESTPC + 1; ++j )
  {
    OutputImageType::Pointer outImage2 = applyPCAShapeEstimator->GetOutput( j );
    OutputImageIterator outImage2It( outImage2, outImage2->GetBufferedRegion() );
    outImage2It.GoToBegin();

    std::cout << "" << std::endl;
    std::cout << "The eigen vector number: " << j << " is:" << std::endl;
    while (!outImage2It.IsAtEnd() )
    {
      std::cout << (double) (outImage2It.Get()) << ";"  << std::endl;
      ++outImage2It;
    }
    std::cout << "  " << std::endl;

  }
*/

  return EXIT_SUCCESS;
}

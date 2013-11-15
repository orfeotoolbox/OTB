/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGreyLevelRunLengthMatrixTextureCoefficientsCalculatorTest.cxx,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "otbImageFileReader.h"

#include "itkScalarImageToGreyLevelRunLengthMatrixGenerator.h"
#include "itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator.h"

int ijGreyLevelRunLengthMatrixTextureCoefficientsCalculatorTest( int argc, char* argv[] )
{
  if (argc < 2)
    {
    std::cerr << "usage : " << argv[0] << " <input_image> " << std::endl;
    }

  typedef int PixelType;
  const unsigned int ImageDimension = 2;
  typedef float RealType;

  typedef otb::Image<PixelType, ImageDimension> ImageType;
  
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer imageReader = ReaderType::New();
  imageReader->SetFileName( argv[1] );
  imageReader->Update();

  ImageType::PointType origin = imageReader->GetOutput()->GetOrigin();
  ImageType::PointType antiorigin = imageReader->GetOutput()->GetOrigin();
  unsigned long numberOfPixelsInMask = 1;
  unsigned int numberOfDirections = 1;
  for ( unsigned int i = 0; i < ImageDimension; i++ )
    {
    antiorigin[i] += imageReader->GetOutput()->GetSpacing()[i] * 
      ( imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[i] - 1 ) ;
    
    numberOfPixelsInMask *= imageReader->GetOutput()->GetLargestPossibleRegion().GetSize()[i];
    numberOfDirections *= 3;
    } 
  numberOfDirections = static_cast<unsigned int>( vcl_floor( 0.5 * static_cast<double>(numberOfDirections) ) );
  unsigned int numberOfBins = 100;

  try
    {
				typedef itk::Statistics::ScalarImageToGreyLevelRunLengthMatrixGenerator<ImageType>
						RunLengthMatrixGeneratorType;
				RunLengthMatrixGeneratorType::Pointer generator = RunLengthMatrixGeneratorType::New();
				generator->SetInput( imageReader->GetOutput() );
				generator->SetNumberOfBinsPerAxis( numberOfBins );
				generator->SetPixelValueMinMax( 0, 255 );
				generator->SetDistanceValueMinMax( 0, origin.EuclideanDistanceTo( antiorigin ) );
				generator->Update();
		
				typedef itk::Statistics::GreyLevelRunLengthMatrixTextureCoefficientsCalculator
						<RunLengthMatrixGeneratorType::HistogramType> CalculatorType;
				CalculatorType::Pointer calculator = CalculatorType::New();
				calculator->SetInput( generator->GetOutput() );
				calculator->Update();
		
				RealType sre = calculator->GetShortRunEmphasis() 
						/ static_cast<RealType>( numberOfDirections );
				RealType lre = calculator->GetLongRunEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType gln = calculator->GetGreyLevelNonuniformity()
						/ static_cast<RealType>( numberOfDirections );
				RealType rln = calculator->GetRunLengthNonuniformity()
						/ static_cast<RealType>( numberOfDirections );
				RealType rp = static_cast<RealType>( calculator->GetTotalNumberOfRuns() ) 
						/ static_cast<RealType>( numberOfPixelsInMask )
						/ static_cast<RealType>( numberOfDirections );
				RealType lgre = calculator->GetLowGreyLevelRunEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType hgre = calculator->GetHighGreyLevelRunEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType srlge = calculator->GetShortRunLowGreyLevelEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType srhge = calculator->GetShortRunHighGreyLevelEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType lrlge = calculator->GetLongRunLowGreyLevelEmphasis()
						/ static_cast<RealType>( numberOfDirections );
				RealType lrhge = calculator->GetLongRunHighGreyLevelEmphasis()
						/ static_cast<RealType>( numberOfDirections );

				std::cout.setf(std::ios::fixed,std::ios::floatfield);
				std::cout.precision(6);

				std::cout << "SRE: " << sre << std::endl;
				std::cout << "LRE: " << lre << std::endl;
				std::cout << "GLN: " << gln << std::endl;
				std::cout << "RLN: " << rln << std::endl;
				std::cout << "RP : " << rp << std::endl;
				std::cout << "LGRE: " << lgre << std::endl;
				std::cout << "HGRE: " << hgre << std::endl;
				std::cout << "SRLGE: " << srlge << std::endl;
				std::cout << "SRHGE: " << srhge << std::endl;
				std::cout << "LRLGE: " << lrlge << std::endl;
				std::cout << "LRHGE: " << lrhge << std::endl;
                                
                                bool passed = true;
                                double trueSRE = 0.249999;
                                double trueRLN = 247518;
                                

                                if (vnl_math_abs(sre - trueSRE) > 0.001)
                                  {
                                  std::cerr << "Error:" << std::endl;
                                  std::cerr << "SRE calculated wrong. Expected: " << trueSRE << ", got: "
                                            << sre << std::endl;
                                  passed = false;
                                  }
                                if (vnl_math_abs(rln - trueRLN) > 0.001)
                                  {
                                  std::cerr << "Error:" << std::endl;
                                  std::cerr << "RLN calculated wrong. Expected: " << trueRLN << ", got: "
                                            << rln << std::endl;
                                  passed = false;
                                  }
                                
                                if (!passed)
                                  {
                                  std::cerr << "Test failed" << std::endl;
                                  return EXIT_FAILURE;
                                  }
                                else
                                  {
                                  std::cerr << "Test succeeded" << std::endl;
                                  return EXIT_SUCCESS;
                                  }
                                
                                
                                
    return EXIT_SUCCESS;
    }
  catch (...)
    {
    return EXIT_FAILURE;  
    }
}

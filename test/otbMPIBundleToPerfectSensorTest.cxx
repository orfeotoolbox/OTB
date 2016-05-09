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

// Includes
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbGridResampleImageFilter.h"
#include "otbImportGeoInformationImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "itkFixedArray.h"
// Elevation handler
//#include "otbWrapperElevationParametersHandler.h"
#include "otbPleiadesPToXSAffineTransformCalculator.h"

#include "otbMPIConfig.h"

#include <cstdlib>
#include <iostream>
#include <boost/chrono/thread_clock.hpp>

// Test
int otbMPIBundleToPerfectSensorTest(int argc, char* argv[])
{
  // Mono-thread execution
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);

  // Start chrono
  boost::chrono::thread_clock::time_point startTimer = boost::chrono::thread_clock::now();
  
  // MPI Initialization
  typedef otb::mpi::MPIConfig    MPIConfigType;
  MPIConfigType::Pointer config = MPIConfigType::New();
  config->Init(argc,argv,true);

  // Verify the number of parameters in the command line
  if (argc != 4)
  {
    std::stringstream message;
    message << "Usage: " << std::endl;
    message << argv[0] << " PANInputImageFile XSInputImageFile OutputImageFile " << std::endl;
    config->logError(message.str());
    config->abort(EXIT_FAILURE);
  }

  // Image types
  typedef float FloatPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<FloatPixelType,Dimension>  FloatVectorImageType;
  
  // Reader
  typedef otb::ImageFileReader<FloatVectorImageType>  ReaderType;
  
  // PAN Reader
  ReaderType::Pointer panReader = ReaderType::New();
  
  // PAN Reader configuration
  std::string panInputFilename = std::string(argv[1]);
  panReader->SetFileName(panInputFilename);
  FloatVectorImageType* panchroV = panReader->GetOutput();
  panReader->UpdateOutputInformation();
  if ( panchroV->GetNumberOfComponentsPerPixel() != 1 )
  {
    config->logError("The panchromatic image must be a single channel image");
	config->abort(EXIT_FAILURE);
  }

  // XS Reader
  ReaderType::Pointer xsReader = ReaderType::New();
  
  // XS Reader configuration
  std::string xsInputFilename = std::string(argv[2]);
  xsReader->SetFileName(xsInputFilename);
  FloatVectorImageType* xs = xsReader->GetOutput();
  xsReader->UpdateOutputInformation();
  
  // Transform the PAN image to otb::Image
  typedef otb::Image<FloatVectorImageType::InternalPixelType> InternalImageType;
  typedef otb::MultiToMonoChannelExtractROI<float,float> ExtractFilterType;
  ExtractFilterType::Pointer channelSelect = ExtractFilterType::New();
  channelSelect->SetChannel(1);
  channelSelect->SetInput(panchroV);
  channelSelect->UpdateOutputInformation();
  InternalImageType::Pointer panchro = channelSelect->GetOutput();

  typedef otb::BCOInterpolateImageFunction<FloatVectorImageType> InterpolatorType;
  typedef otb::GenericRSResampleImageFilter<FloatVectorImageType, FloatVectorImageType>  ResamplerType;
  typedef otb::GridResampleImageFilter<FloatVectorImageType, FloatVectorImageType>  BasicResamplerType;
  typedef otb::ImportGeoInformationImageFilter<FloatVectorImageType,InternalImageType> ImportGeoInformationFilterType;
  typedef otb::SimpleRcsPanSharpeningFusionImageFilter<InternalImageType, FloatVectorImageType, FloatVectorImageType> FusionFilterType;

  // Resample filter
  ResamplerType::Pointer    resampler = ResamplerType::New();
  BasicResamplerType::Pointer basicResampler = BasicResamplerType::New();
  ImportGeoInformationFilterType::Pointer geoImport = ImportGeoInformationFilterType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  resampler->SetInterpolator(interpolator);
  basicResampler->SetInterpolator(interpolator);

  // Fusion filter
  FusionFilterType::Pointer  fusionFilter = FusionFilterType::New();
  fusionFilter->SetPanInput(panchro);
  
  // Setup the DEM Handler
  //otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");
  
  // Set up output image information
  FloatVectorImageType::SpacingType spacing = panchro->GetSpacing();
  FloatVectorImageType::IndexType   start = panchro->GetLargestPossibleRegion().GetIndex();
  FloatVectorImageType::SizeType    size = panchro->GetLargestPossibleRegion().GetSize();
  FloatVectorImageType::PointType   origin = panchro->GetOrigin();
  FloatVectorImageType::PixelType defaultValue;
  itk::NumericTraits<FloatVectorImageType::PixelType>::SetLength(defaultValue, xs->GetNumberOfComponentsPerPixel());

  otb::PleiadesPToXSAffineTransformCalculator::TransformType::OffsetType offset
        = otb::PleiadesPToXSAffineTransformCalculator::ComputeOffset(panchroV,xs);

  origin+=offset;
  origin[0]=origin[0]/4;
  origin[1]=origin[1]/4;
      
  basicResampler->SetOutputOrigin(origin);
  basicResampler->SetInput(xs);
  basicResampler->SetOutputOrigin(origin);

  FloatVectorImageType::SpacingType xsSpacing = xs->GetSpacing();
  xsSpacing*=0.25;
      
  basicResampler->SetOutputSpacing(xsSpacing);
  basicResampler->SetOutputSize(size);
  basicResampler->SetOutputStartIndex(start);
  basicResampler->SetEdgePaddingValue(defaultValue);

  geoImport->SetInput(basicResampler->GetOutput());
  geoImport->SetSource(panchro);

  fusionFilter->SetXsInput(geoImport->GetOutput());

  // Update MPI Pipeline
  std::string outputFilename = std::string(argv[3]);
  config->UpdateMPI(fusionFilter->GetOutput(),outputFilename, false, true);
  
  // End chrono
  boost::chrono::thread_clock::time_point stopTimer = boost::chrono::thread_clock::now();
  std::stringstream message;
  message << "Duration = " << boost::chrono::duration_cast<boost::chrono::milliseconds>(stopTimer-startTimer).count() <<" ms\n";
  config->logInfo(message.str());
  
  return EXIT_SUCCESS;
}


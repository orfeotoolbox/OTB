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

// Software Guide : BeginLatex
//
// This example demonstrates the use of the
// \doxygen{otb}{KmzProductWriter}. This filter is intended to
// produce Kmz file (Google Earth Readable). In this example we will
// use a file with no meta-data, use the
// \doxygen{otb}{GGCPToRPCSensorModelImageFilter} in order to 
// approximate a rpc model, and then use our filter to produce a kmz
// file.
// Note that the \doxygen{otb}{KmzProductWriter} can only process
// inputs with a non empty keyword list.
//
// The first step toward the use of these filters is to include the
// proper header files: the one for the rpc sensor estimation filter and
// the one defining the procedure for creating kmz files.
//
// Software Guide : EndLatex

#include "otbMacro.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

// Software Guide : BeginCodeSnippet
#include "otbKmzProductWriter.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
// Software Guide : EndCodeSnippet
//

int main(int argc, char* argv[])
{

  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] << " infname outfname kmzFileName a1x a1y b1x b1y b1z ... aNx aNy aNz bNx bNy bNz" << std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 3) % 5 != 0)
    {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  
// Software Guide : BeginLatex
//
// We will start by defining the types for the image and the image file
// reader. 
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<float, 2>                      ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now proceed to declare the type for the rpc sensor model
// estimation filter. The class \doxygen{otb}{GGCPToRPCSensorModelImageFilter} is
// templated over the input image type. 
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType> GCPsToSensorModelFilterType;
  typedef GCPsToSensorModelFilterType::Point2DType        Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType        Point3DType;

  GCPsToSensorModelFilterType::Pointer rpcEstimator = GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());
// Software Guide : EndCodeSnippet

  unsigned int nbGCPs = (argc - 3) / 5;

  std::cout << "Receiving " << nbGCPs << " from command line." << std::endl;

  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
// Software Guide : BeginLatex
//
// Here, we set all the Ground Control Points associated to the image
// indexes. This is the entry of the rpc sensor model
// estimator. Everytime a GCP is added, the output image information
// or its keywordlist is updated. In general, a dozen of GCPs are
// needed to estimate an accurate sensor model. The points are added
// via the method AddGCP(PointType2D, PointType3D). The outpput image
// obtained have the needed meta-data informations for the rest of the
// process.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    Point2DType sensorPoint;
    sensorPoint[0] = atof(argv[3 + gcpId * 5]);
    sensorPoint[1] = atof(argv[4 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[5 + 5 * gcpId]);
    geoPoint[1] = atof(argv[6 + 5 * gcpId]);
    geoPoint[2] = atof(argv[7 + 5 * gcpId]);

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
// Software Guide : EndCodeSnippet

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " << geoPoint << std::endl;
    }

  rpcEstimator->GetOutput()->UpdateOutputInformation();
  std::cout << "Residual ground error: " << rpcEstimator->GetRMSGroundError() << std::endl;

  
// Software Guide : BeginLatex
//
// The last step of the chain, is to export the image to a Google 
// Earth understandable format using the KmzProductWriter. Note that 
// the writer can associate legend  via the method
// AddLegend(std::string description,ImageType * legend) and a logo  
// SetLogo(ImageType*) to the kmz. 
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::KmzProductWriter<ImageType>     KmzProductWriterType;
  KmzProductWriterType::Pointer    kmzWriter  = KmzProductWriterType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Finally, we trigger the kmz writting by calling the \code{Update()}
// method on the writer. 
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  kmzWriter->SetInput(rpcEstimator->GetOutput());
  kmzWriter->SetPath(argv[2]);
  
  kmzWriter->Update();
// Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}

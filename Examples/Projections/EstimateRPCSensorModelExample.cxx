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
// \index{otb::GCPsToRPCSensorModelImageFilter}
// \index{otb::GCPsToRPCSensorModelImageFilter!header}
//
//
// The following example illustrates the application of estimation
// of a sensor model to an image (limited to a RPC sensor model for now).

// The \doxygen{otb}{GCPsToRPCSensorModelImageFilter} estimates a RPC
// sensor model from a list of user defined GCPs. Internally, it uses
// an ossimRpcSolver, which performs the estimation using the well
// known least-square method.

// Let's look at the minimal code required to use this
// algorithm. First, the following header defining the
// \doxygen{otb}{GCPsToRPCSensorModelImageFilter} class must be
// included.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include <ios>
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGCPsToRPCSensorModelImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname outfname a1x a1y b1x b1y b1z ... aNx aNy bNx bNy bNz" <<
    std::endl;
    return EXIT_FAILURE;
    }
  else if ((argc - 3) % 5 != 0)
    {
    std::cerr << "Inconsistent GCPs description!" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname       = argv[1];
  const char * outfname      = argv[2];

  //  Software Guide : BeginLatex
  //
  //  We declare the image type based on a particular pixel type and
  //  dimension. In this case the \code{float} type is used for the pixels.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<float, 2>            ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::GCPsToRPCSensorModelImageFilter<ImageType>
  GCPsToSensorModelFilterType;

  typedef GCPsToSensorModelFilterType::Point2DType Point2DType;
  typedef GCPsToSensorModelFilterType::Point3DType Point3DType;
  // Software Guide : EndCodeSnippet

  // We instantiate reader and writer types
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{GCPsToRPCSensorModelImageFilter} is instantiated.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  GCPsToSensorModelFilterType::Pointer rpcEstimator =
    GCPsToSensorModelFilterType::New();
  rpcEstimator->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // We retrieve the command line parameters and put them in the
  // correct variables. Firstly, We determine the number of GCPs
  // set from the command line parameters and they are stored in:
  // \begin{itemize}
  // \item \doxygen{otb}{Point3DType} : Store the sensor point (3D ground point)
  // \item \doxygen{otb}{Point2DType} : Pixel assotiated in the image (2D physical coordinates)

  // Here we do not use DEM or MeanElevation. It is also possible to give a 2D
  // ground point and use the DEM or MeanElevation to get
  // the corresponding elevation.
  // \end{itemize}
  // Software Guide : EndLatex

  unsigned int nbGCPs = (argc - 3) / 5;

  std::cout << "Receiving " << nbGCPs << " from command line." << std::endl;

  // Software Guide : BeginCodeSnippet
  for (unsigned int gcpId = 0; gcpId < nbGCPs; ++gcpId)
    {
    Point2DType sensorPoint;
    sensorPoint[0] = atof(argv[3 + gcpId * 5]);
    sensorPoint[1] = atof(argv[4 + gcpId * 5]);

    Point3DType geoPoint;
    geoPoint[0] = atof(argv[5 + 5 * gcpId]);
    geoPoint[1] = atof(argv[6 + 5 * gcpId]);
    geoPoint[2] = atof(argv[7 + 5 * gcpId]);

    std::cout << "Adding GCP sensor: " << sensorPoint << " <-> geo: " <<
    geoPoint << std::endl;

    rpcEstimator->AddGCP(sensorPoint, geoPoint);
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // Note that the \doxygen{otb}{GCPsToRPCSensorModelImageFilter} needs
  // at least 20 GCPs to estimate a proper RPC sensor model,
  // although no warning will be reported to the user if
  // the number of GCPs is lower than 20.
  // Actual estimation of the sensor model takes place in the
  // \code{GenerateOutputInformation()} method.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rpcEstimator->GetOutput()->UpdateOutputInformation();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The result of the RPC model estimation and the residual ground
  // error is then save in a txt file. Note that This filter does
  // not modify the image buffer, but only the metadata.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::ofstream ofs;
  ofs.open(outfname);

  // Set floatfield to format properly
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  ofs.precision(10);

  ofs << (ImageType::Pointer) rpcEstimator->GetOutput() << std::endl;
  ofs << "Residual ground error: " << rpcEstimator->GetRMSGroundError() <<
  std::endl;
  ofs.close();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The output image can be now given to the \doxygen{otb}{orthorectificationFilter}.
  // Note that this filter allows also to import GCPs from the image
  // metadata, if any.
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}

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
// Let's assume that you have a KML file (hence in geographical coordinates)
// that you would like to superpose to some image with a specific map projection.
// Of course, you could use the handy ogr2ogr tool to do that, but it won't
// integrate so seamlessly into your OTB application.
//
// You can also suppose that the image on which you want to superpose
// the data is not in a specific map projection but a raw image from a
// particular sensor.  Thanks to OTB, the same code below will be able
// to do the appropriate conversion.
//
// This example demonstrates the use of the
// \doxygen{otb}{GeometriesProjectionFilter}.
//
// Software Guide : EndLatex

#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"
// #include "otbGeometriesFileReader.h"
// #include "otbGeometriesFileWriter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int argc, char* argv[])
{

  if (argc < 4)
    {
    std::cerr << argv[0] <<
    " <input vector filename> <input image name> <output vector filename>\n";

    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  //
  // Declare the vector data type that you would like to use in your
  // application.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Declare and instantiate the vector data reader:
  // \doxygen{otb}{GeometriesFileReader}. The call to the
  // \code{UpdateOutputInformation()} method fill up the header information.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
    argv[1], otb::ogr::DataSource::Modes::read);
  InputGeometriesType::Pointer in_set = InputGeometriesType::New(input);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We need the image only to retrieve its projection information,
  // i.e. map projection or sensor model parameters. Hence, the image
  // pixels won't be read, only the header information using the
  // \code{UpdateOutputInformation()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned short int, 2> ImageType;
  typedef otb::ImageFileReader<ImageType>   ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The \doxygen{otb}{GeometriesProjectionFilter} will do the work of
  // converting the vector data coordinates. It is usually a good idea
  // to use it when you design applications reading or saving vector
  // data.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GeometriesProjectionFilter GeometriesFilterType;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Information concerning the original projection of the vector data
  // will be automatically retrieved from the metadata. Nothing else
  // is needed from you:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(in_set);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Information about the target projection is retrieved directly from
  // the image:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetOutputKeywordList(imageReader->GetOutput()->GetImageKeywordlist()); // nec qd capteur
  filter->OutputImageReference.SetOrigin(imageReader->GetOutput()->GetOrigin()); // nec qd capteur
  filter->OutputImageReference.SetSpacing(imageReader->GetOutput()->GetSpacing()); // nec qd capteur
  filter->SetOutputProjectionRef( imageReader->GetOutput()->GetProjectionRef()); // ~ wkt
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finally, the result is saved into a new vector file.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
    argv[3], otb::ogr::DataSource::Modes::write);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  filter->SetOutput(out_set);
  filter->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // It is worth noting that none of this code is specific to the
  // vector data format. Whether you pass a shapefile, or a KML file,
  // the correct driver will be automatically instantiated.
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}

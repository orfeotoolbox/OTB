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
// Instead of using \doxygen{otb}{VectorData} to apply projections as
// explained in \ref{sec:VectorDataProjection}, we can also \emph{directly} work
// on OGR data types thanks to \doxygen{otb}{GeometriesProjectionFilter}.
//
// This example demonstrates how to proceed with this alternative set of vector
// data types.
//
// Software Guide : EndLatex

#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"

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
  // Declare the geometries type that you would like to use in your
  // application. Unlike \doxygen{otb}{VectorData}, \doxygen{otb}{GeometriesSet}
  // is a single type for any kind of geometries set (OGR data source, or OGR
  // layer).
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // First, declare and instantiate the data source
  // \subdoxygen{otb}{ogr}{DataSource}. Then, encapsulate this data source into
  // a \doxygen{otb}{GeometriesSet}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::ogr::DataSource::Pointer input = otb::ogr::DataSource::New(
    argv[1], otb::ogr::DataSource::Modes::Read);
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
  // converting the geometries coordinates. It is usually a good idea
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
  // necessary for sensors
  filter->SetOutputKeywordList(imageReader->GetOutput()->GetImageKeywordlist());
  // necessary for sensors
  filter->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  // necessary for sensors
  filter->SetOutputSpacing(imageReader->GetOutput()->GetSpacing());
  // ~ wkt
  filter->SetOutputProjectionRef( imageReader->GetOutput()->GetProjectionRef());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finally, the result is saved into a new vector file.
  // Unlike other OTB filters, \doxygen{otb}{GeometriesProjectionFilter} expects
  // to be given a valid output geometries set where to store the result of its
  // processing -- otherwise the result will be an in-memory data source, and
  // not stored in a file nor a data base.
  //
  // Then, the processing is started by calling \code{Update()}. The actual
  // serialization of the results is guaranteed to be completed when the output
  // geometries set object goes out of scope, or when \code{SyncToDisk} is
  // called.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(
    argv[3], otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  filter->SetOutput(out_set);
  filter->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once again, it is worth noting that none of this code is specific to the
  // vector data format. Whether you pass a shapefile, or a KML file, the
  // correct driver will be automatically instantiated.
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}

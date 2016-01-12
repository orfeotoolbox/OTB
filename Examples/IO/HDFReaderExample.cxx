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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {GSSTF_NCEP.2b.2008.12.31.he5}
//    OUTPUTS: {HDFReaderExample.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example describes how to read a MODIS image(Moderate-resolution Imaging Spectroradiometer)
// .This scientific instrument was launched into Earth orbit by NASA.
// The MODIS image format is HDF (Hierarchical Data Format). This
// format is widely use to store remote sensing images.
// HDF format is designed to store and organize large amounts of numerical data
// including multidimensional arrays, raster images, and tables.
// A specific feature of this format is that it works with container
// structures which can hold datasets and other groups. It implies that
// access to these structures in OTB presents some adaptations compare
// to other image formats.
// OTB is using GDAL to support HDF. HDF4 and HDF5 supports in Gdal is
// not activated by default. You need to download the HDF run-time
// libraries and compile Gdal by adding the support of these formats. You can
// find more information here : \url{http://trac.osgeo.org/gdal/wiki/HDF}
//
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

#include <sstream>
// Software Guide : BeginCodeSnippet
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int itkNotUsed(argc), char * argv[])
{
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We need now to declare the data types that we will be using and instanciate the
// reader (which is a \doxygen{otb}{PointSetFileReader}).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef unsigned char                                InputPixelType;
  const unsigned int Dimension                         = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>         HDFReaderType;
  typedef otb::ImageFileWriter<InputImageType>         HDFWriterType;

  HDFReaderType::Pointer reader = HDFReaderType::New();
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // If you specify the path name of the hdf file, by default the reader will open the first dataset in the container.
  // Otherwise, there are 2 ways to access HDF subdatasets in OTB. The basic idea is to concatenate the subdataset
  // name or number to the input filename. The ':' character is the standard separator in the input filename.
  // For example myfile.hdf:SubdatasetName or myfile.hdf:SubdatasetNumber
  // In this example we will access the first dataset of the MODIS MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf:0
  //
  // Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const std::string inputFilename (argv[1]);
  const std::string outputFilename (argv[2]);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// You can access to subdatasets' information available in the HDF file using
// the \code{GetSubDatasetInfo} method of \doxygen{otb}{GDALImageIO}.
// It allows storing HDF subdatasets names and descriptions in vector of string.
// You can find below how to print the name and the decription
// of all the subdatasets.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::GDALImageIO::Pointer readerGDAL = otb::GDALImageIO::New();

  readerGDAL->SetFileName(inputFilename.c_str());

  std::vector<std::string> vectSubdatasetNames;
  std::vector<std::string> vectorSubdatasetDescriptions;

  if (readerGDAL->CanReadFile(inputFilename.c_str()))
    {
    if (readerGDAL->GetSubDatasetInfo(vectSubdatasetNames, vectorSubdatasetDescriptions))
      {
      // Fill vDataset with subdataset descriptor info
      for( unsigned int itSubDataset = 0; itSubDataset < (unsigned int)
            vectorSubdatasetDescriptions.size(); itSubDataset++ )
        {
        std::cout << "Dataset Number " <<  itSubDataset << " name is: " << vectSubdatasetNames[itSubDataset].c_str()
            << " and description is: " << vectSubdatasetNames[itSubDataset].c_str() << std::endl;
        }
      }
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then we write the first dataset of the MODIS product. As explained above,
  // we instantiate the reader with the correct input filename.
  //
  // Software Guide : EndLatex
  HDFWriterType::Pointer writer = HDFWriterType::New();

  // Software Guide : BeginCodeSnippet
  std::ostringstream out;
  out << inputFilename;
  out << ":";
  out << 0;

  reader->SetFileName(out.str());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can then built the pipeline And execute it by calling the \code{Update} method of
  //  the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetFileName(outputFilename.c_str());
  writer->SetInput(reader->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{HDFReaderExample0.eps}
  // \itkcaption[HDF Dataset]{Quicklook of the first dataset extraction of MODIS product.}
  // \label{fig:IMAGERY_HDF_0}
  // \end{figure}
  //
  //
  //  Figures \ref{fig:IMAGERY_HDF_0} illustrates the extraction of MODIS
  //  subdataset through \doxygen{otb}{ImageFileReader}.
  //
  //  Software Guide : EndLatex
  return EXIT_SUCCESS;
}

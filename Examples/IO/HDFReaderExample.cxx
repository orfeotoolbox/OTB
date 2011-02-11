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
//    INPUTS: {srs.las}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example describes how to read a HDF (Hierarchical Data Format) file. This
// format is widely use to store remote sensing images especially use by the well-known MODIS (Moderate-resolution Imaging Spectroradiometer)
// scientific instrument launched into Earth orbit by NASA.
// HDF format is designed to store and organize large amounts of numerical data
// including multidimensional arrays, raster images, and tables.
// A specific feature of this format is that it works with container structures which can hold datasets and other groups
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int main(int argc, char * argv[])
{
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We need now to declare the data types that we will be using and instanciate the
// reader (which is a \doxygen{otb}{PointSetFileReader}).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> HDFReaderType;

  HDFReaderType::Pointer reader = HDFReaderType::New();
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // If you specify the path name of the hdf file, by default the reader will open the first dataset in the container.
  // Otherwise, there are 2 ways to access HDF subdatasets in OTB. The basic idea is to concatenate the subdataset
  // name or number to the input filename. For example myfile.hdf:SubdatasetName or myfile.hdf:SubdatasetNumber
  // In this example we will access the first dataset of the MODIS MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf:1
  //
  // Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const char * inputFilename  = argv[1];
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// You can access to subdatasets list in the HDF file
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  otb::GDALImageIO::Pointer readerGDAL = otb::GDALImageIO::New();

  readerGDAL->SetFileName(inputFilename);

  std::vector<std::string> vectSubdatasetNames;
  std::vector<std::string> vectorSubdatasetDescriptions;

  if (readerGDAL->CanReadFile(inputFilename))
    {
    if (readerGDAL->GetSubDatasetInfo(vectSubdatasetNames, vectorSubdatasetDescriptions))
      {
      // Fill vDataset with subdataset descriptor info
      for( unsigned int itSubDataset = 0; itSubDataset < (unsigned int) vectorSubdatasetDescriptions.size() ; itSubDataset++ )
        {
        std::cout << "Dataset Number " <<  itSubDataset+1 << " name is " << vectorSubdatasetDescriptions[itSubDataset].c_str() << std::endl;
        }
      }
    }

  // Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}	


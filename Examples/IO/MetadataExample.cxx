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


//  SoftwareGuide: BeginLatex
//
//  This example illustrates the access to metadata image information
//  with OTB. By metadata, we mean data which is typically stored with
//  remote sensing images, like geographical coordinates of pixels,
//  pixel spacing or resolution, etc. Of course, the availability of
//  these data depends on the image format used and on the fact that
//  the image producer must fill the available metadata fields. The
//  image formats which typically support metadata are for example
//  CEOS and GeoTiff.
//
//  The metadata support is embedded in OTB's IO functionnalities and
//  is accessible through the \doxygen{otb}{Image} and
//  \doxygen{otb}{VectorImage} classes. You should avoid using the
//  \doxygen{itk}{Image} class if you want to have metadata support.
//
//  SoftwareGuide: EndLatex

#include "itkMacro.h"
#include <iostream>
#include <fstream>
#include <string>

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int itkNotUsed(argc), char* argv[])
{

  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputAsciiFilename  = argv[2];

//  SoftwareGuide: BeginLatex
//
//  This simple example will consist on reading an image from a file
//  and writing the metadata to an output ASCII file. As usual we
//  start by defining the types needed for the image to be read.
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
// SoftwareGuide : EndCodeSnippet

//  SoftwareGuide: BeginLatex
//
//  We can now instantiate the reader and get a pointer to the input image.
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  ReaderType::Pointer     reader = ReaderType::New();
  InputImageType::Pointer image = InputImageType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  image = reader->GetOutput();
// SoftwareGuide : EndCodeSnippet

//  SoftwareGuide: BeginLatex
//
//  Once the image has been read, we can access the metadata
//  information. We will copy this information to an ASCII file, so we
//  create an output file stream for this purpose.
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  std::ofstream file;

  file.open(outputAsciiFilename);
// SoftwareGuide : EndCodeSnippet

//  SoftwareGuide: BeginLatex
//
//  We can now call the different available methods for accessing the
//  metadata. Useful methods are :
//  \begin{itemize}
//      \item \code{GetSpacing}: the sampling step;
//      \item \code{GetOrigin}: the coordinates of the origin of the image;
//      \item \code{GetProjectionRef}: the image projection reference;
//      \item \code{GetGCPProjection}: the projection for the eventual
//  ground control points;
//      \item \code{GetGCPCount}: the number of GCPs available;
//  \end{itemize}
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  file << "Spacing " << image->GetSpacing() << std::endl;
  file << "Origin " << image->GetOrigin() << std::endl;

  file << "Projection REF " << image->GetProjectionRef() << std::endl;

  file << "GCP Projection " << image->GetGCPProjection() << std::endl;

  unsigned int GCPCount = image->GetGCPCount();
  file << "GCP Count " << image->GetGCPCount() << std::endl;
// SoftwareGuide : EndCodeSnippet

//  SoftwareGuide: BeginLatex
//
//  One can also get the GCPs by number, as well as their coordinates
//  in image and geographical space.
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  for (unsigned int GCPnum = 0; GCPnum < GCPCount; GCPnum++)
    {
    file << "GCP[" << GCPnum << "] Id " << image->GetGCPId(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Info " << image->GetGCPInfo(GCPnum) <<
    std::endl;
    file << "GCP[" << GCPnum << "] Row " << image->GetGCPRow(GCPnum) <<
    std::endl;
    file << "GCP[" << GCPnum << "] Col " << image->GetGCPCol(GCPnum) <<
    std::endl;
    file << "GCP[" << GCPnum << "] X " << image->GetGCPX(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Y " << image->GetGCPY(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Z " << image->GetGCPZ(GCPnum) << std::endl;
    file << "----------------" << std::endl;
    }
// SoftwareGuide : EndCodeSnippet

//  SoftwareGuide: BeginLatex
//
//  If a geographical transformation is available, it can be recovered
//  as follows.
//
//  SoftwareGuide: EndLatex

// SoftwareGuide : BeginCodeSnippet
  InputImageType::VectorType tab = image->GetGeoTransform();

  file << "Geo Transform " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " " << i << " -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetUpperLeftCorner();
  file << "Corners " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " UL[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetUpperRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " UR[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetLowerLeftCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " LL[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetLowerRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " LR[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  file.close();
  // SoftwareGuide : EndCodeSnippet

  return EXIT_SUCCESS;
}

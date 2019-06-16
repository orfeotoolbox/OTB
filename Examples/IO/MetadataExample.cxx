/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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

#include "itkMacro.h"
#include <iostream>
#include <fstream>
#include <string>

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int itkNotUsed(argc), char* argv[])
{

  // Verify the number of parameters in the command line
  const char* inputFilename       = argv[1];
  const char* outputAsciiFilename = argv[2];

  //  This simple example will consist on reading an image from a file
  //  and writing the metadata to an output ASCII file. As usual we
  //  start by defining the types needed for the image to be read.

  using InputPixelType         = unsigned char;
  const unsigned int Dimension = 2;

  using InputImageType = otb::Image<InputPixelType, Dimension>;

  using ReaderType = otb::ImageFileReader<InputImageType>;

  //  We can now instantiate the reader and get a pointer to the input image.

  ReaderType::Pointer     reader = ReaderType::New();
  InputImageType::Pointer image  = InputImageType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  image = reader->GetOutput();

  //  Once the image has been read, we can access the metadata
  //  information. We will copy this information to an ASCII file, so we
  //  create an output file stream for this purpose.

  std::ofstream file;

  file.open(outputAsciiFilename);

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

  file << "Spacing " << image->GetSignedSpacing() << std::endl;
  file << "Origin " << image->GetOrigin() << std::endl;

  file << "Projection REF " << image->GetProjectionRef() << std::endl;

  file << "GCP Projection " << image->GetGCPProjection() << std::endl;

  unsigned int GCPCount = image->GetGCPCount();
  file << "GCP Count " << image->GetGCPCount() << std::endl;

  //  One can also get the GCPs by number, as well as their coordinates
  //  in image and geographical space.

  for (unsigned int GCPnum = 0; GCPnum < GCPCount; GCPnum++)
  {
    file << "GCP[" << GCPnum << "] Id " << image->GetGCPId(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Info " << image->GetGCPInfo(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Row " << image->GetGCPRow(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Col " << image->GetGCPCol(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] X " << image->GetGCPX(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Y " << image->GetGCPY(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Z " << image->GetGCPZ(GCPnum) << std::endl;
    file << "----------------" << std::endl;
  }

  //  If a geographical transformation is available, it can be recovered
  //  as follows.

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

  return EXIT_SUCCESS;
}

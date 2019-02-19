/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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



// This example illustrates how to import data into the \doxygen{otb}{Image}
// class. This is particularly useful for interfacing with other software
// systems. Many systems use a contiguous block of memory as a buffer
// for image pixel data. The current example assumes this is the case and
// feeds the buffer into an \doxygen{otb}{ImportImageFilter}, thereby producing an
// Image as output.

//
// For fun we create a synthetic image with a centered sphere in
// a locally allocated buffer and pass this block of memory to the
// ImportImageFilter. This example is set up so that on execution, the
// user must provide the name of an output file as a command-line argument.
//
// \index{otb::ImportImageFilter!Instantiation}
// \index{otb::ImportImageFilter!Header}
//
// First, the header file of the ImportImageFilter class must be
// included.

#include "otbImage.h"
#include "otbImportImageFilter.h"

#include "otbImageFileWriter.h"
#include "itkRGBPixel.h"

int main(int argc, char * argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  outputImageFile" << std::endl;
    return 1;
    }

  // Next, we select the data type to use to represent the image pixels.  We
  // assume that the external block of memory uses the same data type to
  // represent the pixels.
  //
  typedef unsigned char PixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<PixelType, Dimension> ImageType;

  // The type of the ImportImageFilter is instantiated in the
  // following line.
  //
  // \index{otb::ImportImageFilter!Instantiation}

  typedef otb::ImportImageFilter<ImageType> ImportFilterType;

  //  A filter object created using the \code{New()} method is then
  //  assigned to a \code{SmartPointer}.
  //
  // \index{otb::ImportImageFilter!Pointer}
  // \index{otb::ImportImageFilter!New()}
  //
  ImportFilterType::Pointer importFilter = ImportFilterType::New();

  // This filter requires the user to specify the size of the image to be
  // produced as output.  The \code{SetRegion()} method is used to this end.
  // The image size should exactly match the number of pixels available in the
  // locally allocated buffer.
  //
  // \index{otb::ImportImageFilter!SetRegion()}
  // \index{otb::ImportImageFilter!New()}
  // \index{otb::ImportImageFilter!New()}
  //
  ImportFilterType::SizeType size;

  size[0]  = 200;  // size along X
  size[1]  = 200;  // size along Y

  ImportFilterType::IndexType start;
  start.Fill(0);

  ImportFilterType::RegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  importFilter->SetRegion(region);

  //  The origin of the output image is specified with the \code{SetOrigin()}
  //  method.

  double origin[Dimension];
  origin[0] = 0.0;    // X coordinate
  origin[1] = 0.0;    // Y coordinate

  importFilter->SetOrigin(origin);

  //  The spacing of the image is passed with the \code{SetSpacing()} method.

  double spacing[Dimension];
  spacing[0] = 1.0;    // along X direction
  spacing[1] = 1.0;    // along Y direction

  importFilter->SetSpacing(spacing);

  //  Next we allocate the memory block containing the pixel data to be
  //  passed to the ImportImageFilter. Note that we use exactly the
  //  same size that was specified with the \code{SetRegion()} method.  In a
  //  practical application, you may get this buffer from some other library
  //  using a different data structure to represent the images.

  // MODIFIED
  const unsigned int numberOfPixels =  size[0] * size[1];
  PixelType *        localBuffer = new PixelType[numberOfPixels];

  const double radius = 80.0;

  //  Here we fill up the buffer with a binary sphere. We use simple
  //  \code{for()} loops here similar to those found in the C or FORTRAN
  //  programming languages. Note that otb
  //  does not use \code{for()} loops in its internal code to access
  //  pixels. All pixel access tasks are instead performed using
  //  \doxygen{otb}{ImageIterator}s that support the management of
  //  n-dimensional images.

  const double radius2 = radius * radius;
  PixelType *  it = localBuffer;

  for (unsigned int y = 0; y < size[1]; y++)
    {
    const double dy = static_cast<double>(y) - static_cast<double>(size[1]) /
                      2.0;
    for (unsigned int x = 0; x < size[0]; x++)
      {
      const double dx = static_cast<double>(x) - static_cast<double>(size[0]) /
                        2.0;
      const double d2 = dx * dx + dy * dy;
      *it++ = (d2 < radius2) ? 255 : 0;
      }
    }

  //  The buffer is passed to the ImportImageFilter with the
  //  \code{SetImportPointer()}. Note that the last argument of this method
  //  specifies who will be responsible for deleting the memory block once it
  //  is no longer in use. A \code{false} value indicates that the
  //  ImportImageFilter will not try to delete the buffer when its
  //  destructor is called. A \code{true} value, on the other hand, will allow the
  //  filter to delete the memory block upon destruction of the import filter.
  //
  //  For the ImportImageFilter to appropriately delete the
  //  memory block, the memory must be allocated with the C++
  //  \code{new()} operator.  Memory allocated with other memory
  //  allocation mechanisms, such as C \code{malloc} or \code{calloc}, will not
  //  be deleted properly by the ImportImageFilter.  In
  //  other words, it is the application programmer's responsibility
  //  to ensure that ImportImageFilter is only given
  //  permission to delete the C++ \code{new} operator-allocated memory.

  const bool importImageFilterWillOwnTheBuffer = true;
  importFilter->SetImportPointer(localBuffer, numberOfPixels,
                                 importImageFilterWillOwnTheBuffer);

  //  Finally, we can connect the output of this filter to a pipeline.
  //  For simplicity we just use a writer here, but it could be any other filter.

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[1]);

  writer->SetInput(dynamic_cast<ImageType*>(importFilter->GetOutput()));

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& exp)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << exp << std::endl;
    return -1;
    }

  //  Note that we do not call \code{delete} on the buffer since we pass
  //  \code{true} as the last argument of \code{SetImportPointer()}. Now the
  //  buffer is owned by the ImportImageFilter.

  return EXIT_SUCCESS;
}

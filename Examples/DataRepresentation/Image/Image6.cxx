/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// Software Guide : BeginLatex
//
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
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImportVectorImageFilter.h"
// Software Guide : EndCodeSnippet

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

  // Software Guide : BeginLatex
  //
  // Next, we select the data type to use to represent the image pixels.  We
  // assume that the external block of memory uses the same data type to
  // represent the pixels.
  //
  // Software Guide : EndLatex
  //
  // Software Guide : BeginCodeSnippet
  typedef unsigned char PixelType;
  const unsigned int Dimension = 2;
  unsigned int       numberOfBands = 5;

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The type of the ImportImageFilter is instantiated in the
  // following line.
  //
  // \index{otb::ImportImageFilter!Instantiation}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImportVectorImageFilter<ImageType> ImportFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  A filter object created using the \code{New()} method is then
  //  assigned to a \code{SmartPointer}.
  //
  // \index{otb::ImportImageFilter!Pointer}
  // \index{otb::ImportImageFilter!New()}
  //
  // Software Guide : EndLatex
  //
  // Software Guide : BeginCodeSnippet
  ImportFilterType::Pointer importFilter = ImportFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // This filter requires the user to specify the size of the image to be
  // produced as output.  The \code{SetRegion()} method is used to this end.
  // The image size should exactly match the number of pixels available in the
  // locally allocated buffer.
  //
  // \index{otb::ImportImageFilter!SetRegion()}
  // \index{otb::ImportImageFilter!New()}
  // \index{otb::ImportImageFilter!New()}
  //
  // Software Guide : EndLatex
  //
  // Software Guide : BeginCodeSnippet
  ImportFilterType::SizeType size;

  size[0]  = 200;  // size along X
  size[1]  = 200;  // size along Y

  ImportFilterType::IndexType start;
  start.Fill(0);

  ImportFilterType::RegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  importFilter->SetRegion(region);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The origin of the output image is specified with the \code{SetOrigin()}
  //  method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double origin[Dimension];
  origin[0] = 0.0;    // X coordinate
  origin[1] = 0.0;    // Y coordinate

  importFilter->SetOrigin(origin);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The spacing of the image is passed with the \code{SetSpacing()} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double spacing[Dimension];
  spacing[0] = 1.0;    // along X direction
  spacing[1] = 1.0;    // along Y direction

  importFilter->SetSpacing(spacing);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Next we allocate the memory block containing the pixel data to be
  //  passed to the ImportImageFilter. Note that we use exactly the
  //  same size that was specified with the \code{SetRegion()} method.  In a
  //  practical application, you may get this buffer from some other library
  //  using a different data structure to represent the images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  // MODIFIED

  const unsigned int numberOfPixels =  size[0] * size[1] * numberOfBands;

  PixelType * localBuffer = new PixelType[numberOfPixels];
  // Software Guide : EndCodeSnippet

  const double radius = 80.0;

  //  Software Guide : BeginLatex
  //
  //  Here we fill up the buffer with a binary sphere. We use simple
  //  \code{for()} loops here similar to those found in the C or FORTRAN
  //  programming languages. Note that otb
  //  does not use \code{for()} loops in its internal code to access
  //  pixels. All pixel access tasks are instead performed using
  //  \doxygen{otb}{ImageIterator}s that support the management of
  //  n-dimensional images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const double radius2 = radius * radius;
  PixelType *  it = localBuffer;
  int          length = 0;
  for (unsigned int y = 0; y < size[1]; y++)
    {
    const double dy = static_cast<double>(y) - static_cast<double>(size[1]) /
                      2.0;
    for (unsigned int x = 0; x < size[0]; x++)
      {
      const double dx = static_cast<double>(x) - static_cast<double>(size[0]) /
                        2.0;
      const double d2 = dx * dx + dy * dy;
      PixelType    pTmp = (d2 < radius2) ? 255 : 0;
      for (unsigned int nbBandsTmp = 0;
           nbBandsTmp < numberOfBands;
           nbBandsTmp++)
        *it++ = pTmp;
      length++;
      }
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
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
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const bool importImageFilterWillOwnTheBuffer = true;
  importFilter->SetImportPointer(localBuffer, numberOfPixels,
                                 importImageFilterWillOwnTheBuffer);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Finally, we can connect the output of this filter to a pipeline.
  //  For simplicity we just use a writer here, but it could be any other filter.
  //
  //  Software Guide : EndLatex

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[1]);

  ImageType* imTmp = dynamic_cast<ImageType*>(importFilter->GetOutput());

  // Software Guide : BeginCodeSnippet
  writer->SetInput(imTmp);
  // Software Guide : EndCodeSnippet

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

  //  Software Guide : BeginLatex
  //
  //  Note that we do not call \code{delete} on the buffer since we pass
  //  \code{true} as the last argument of \code{SetImportPointer()}. Now the
  //  buffer is owned by the ImportImageFilter.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}

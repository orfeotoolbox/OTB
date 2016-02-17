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
// Many image processing tasks require images of non-scalar pixel type. A
// typical example is a multispectral image.  The following code illustrates
// how to instantiate and use an image whose pixels are of vector type.
//
// We could use the \doxygen{itk}{Vector} class to define the pixel
// type.  The Vector class is intended to represent a geometrical vector in
// space. It is not intended to be used as an array container like the
// \href{http://www.sgi.com/tech/stl/Vector.html}{\code{std::vector}} in
// \href{http://www.sgi.com/tech/stl/}{STL}.  If you are interested in
// containers, the \doxygen{itk}{VectorContainer} class may provide the
// functionality you want.
//
// \index{itk::Vector}
// \index{itk::Vector!header}
//
// However, the \doxygen{itk}{Vector} is a fixed size array and it
// assumes that the number of channels of the image is known at
// compile time. Therefore, we prefer to use the
// \doxygen{otb}{VectorImage} class which allows choosing the number
// of channels of the image at runtime. The pixels will be of type
// \doxygen{itk}{VariableLengthVector}.
//
// The first step is to include the header file of the VectorImage class.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorImage.h"
// Software Guide : EndCodeSnippet

int main(int, char *[])
{
  // Software Guide : BeginLatex
  //
  // The VectorImage class is templated over the type used to represent
  // the coordinate in space and over the dimension of the space.  In
  // this example,
  // we want to represent Pl\'eiades images which have 4 bands.
  //
  // \index{otb::VectorImage!Instantiation}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned char                  PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  // Software Guide : EndCodeSnippet

  // Then the image object can be created
  ImageType::Pointer image = ImageType::New();

  // The image region should be initialized
  ImageType::IndexType start;
  ImageType::SizeType  size;

  size[0]  = 200;  // size along X
  size[1]  = 200;  // size along Y

  start[0] =   0;  // first index on X
  start[1] =   0;  // first index on Y

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  // Pixel data is allocated
  image->SetRegions(region);

  // Software Guide : BeginLatex
  // Since the pixel dimensionality is chosen at runtime, one has to
  // pass this parameter to the image before memory allocation.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  image->SetNumberOfComponentsPerPixel(4);
  image->Allocate();
  // Software Guide : EndCodeSnippet

  ImageType::IndexType pixelIndex;

  pixelIndex[0] = 27;   // x position
  pixelIndex[1] = 29;   // y position

  // Software Guide : BeginLatex
  //
  // The VariableLengthVector class overloads the operator
  // \code{[]}. This makes it possible to access the
  // Vector's components using index notation. The user must not
  // forget to allocate the memory for each individual pixel by using
  // the \code{Reserve} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::PixelType pixelValue;
  pixelValue.Reserve(4);

  pixelValue[0] =  1;   // Blue component
  pixelValue[1] =  6;   // Green component
  pixelValue[2] =  100; // Red component
  pixelValue[3] =  100; // NIR component
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now store this vector in one of the image pixels by defining an
  // index and invoking the \code{SetPixel()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  image->SetPixel(pixelIndex,   pixelValue);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex

  // The GetPixel method can also be used to read Vectors
  // pixels from the image
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::PixelType value = image->GetPixel(pixelIndex);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Lets repeat that both \code{SetPixel()} and \code{GetPixel()} are
  // inefficient and should only be used for debugging purposes or for
  // implementing interactions with a graphical user interface such as
  // querying pixel value by clicking with the mouse.
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}

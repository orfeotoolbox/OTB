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
// Even though OTB can be used to perform
// general image processing tasks, the primary purpose of the toolkit is the
// processing of remote sensing image data.  In that respect, additional
// information about the images is considered mandatory. In particular the
// information associated with the physical spacing between pixels and the
// position of the image in space with respect to some world coordinate
// system are extremely important.
//
// Image origin and spacing are fundamental to many
// applications. Registration, for example, is performed in physical
// coordinates. Improperly defined spacing and origins will result in
// inconsistent results in such processes. Remote sensing images with no spatial
// information should not be used for image analysis,
// feature extraction, GIS input, etc. In
// other words, remote sensing images lacking spatial information are not only
// useless but also hazardous.
//
// \begin{figure} \center
// \includegraphics[width=\textwidth]{ImageOriginAndSpacing.eps}
// \itkcaption[OTB Image Geometrical Concepts]{Geometrical concepts associated
// with the OTB image.}
// \label{fig:ImageOriginAndSpacing}
// \end{figure}
//
// Figure \ref{fig:ImageOriginAndSpacing} illustrates the main geometrical
// concepts associated with the \doxygen{otb}{Image}. In this figure,
// circles are
// used to represent the center of pixels. The value of the pixel is assumed
// to exist as a Dirac Delta Function located at the pixel center. Pixel
// spacing is measured between the pixel centers and can be different along
// each dimension. The image origin is associated with the coordinates of the
// first pixel in the image. A \emph{pixel} is considered to be the
// rectangular region surrounding the pixel center holding the data
// value. This can be viewed as the Voronoi region of the image grid, as
// illustrated in the right side of the figure.  Linear interpolation of
// image values is performed inside the Delaunay region whose corners
// are pixel centers.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "itkPoint.h"

int main(int, char *[])
{
  typedef otb::Image<unsigned short, 2> ImageType;

  ImageType::Pointer image = ImageType::New();

  ImageType::IndexType start;
  ImageType::SizeType  size;

  size[0]  = 200;  // size along X
  size[1]  = 200;  // size along Y

  start[0] =   0;  // first index on X
  start[1] =   0;  // first index on Y

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  image->FillBuffer(0);

  // Software Guide : BeginLatex
  //
  // Image spacing is represented in a \code{FixedArray}
  // whose size matches the dimension of the image. In order to manually set
  // the spacing of the image, an array of the corresponding type must be
  // created.  The elements of the array should then be initialized with the
  // spacing between the centers of adjacent pixels. The following code
  // illustrates the methods available in the Image class for dealing with
  // spacing and origin.
  //
  // \index{otb::Image!Spacing}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::SpacingType spacing;

  // Note: measurement units (e.g., meters, feet, etc.) are defined by the application.
  spacing[0] = 0.70; // spacing along X
  spacing[1] = 0.70; // spacing along Y
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The array can be assigned to the image using
  // the \code{SetSpacing()} method.
  //
  // \index{otb::Image!SetSpacing()}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  image->SetSpacing(spacing);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The spacing information can be retrieved from an image by using the
  // \code{GetSpacing()} method. This method returns a reference to a
  // \code{FixedArray}. The returned object can then be used to read the
  // contents of the array. Note the use of the \code{const} keyword to indicate
  // that the array will not be modified.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const ImageType::SpacingType& sp = image->GetSpacing();

  std::cout << "Spacing = ";
  std::cout << sp[0] << ", " << sp[1] << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The image origin is managed in a similar way to the spacing.  A
  // \code{Point} of the appropriate dimension must first be
  // allocated.  The coordinates of the origin can then be assigned to
  // every component.  These coordinates correspond to the position of
  // the first pixel of the image with respect to an arbitrary
  // reference system in physical space. It is the user's
  // responsibility to make sure that multiple images used in the same
  // application are using a consistent reference system. This is
  // extremely important in image registration applications.
  //
  // The following code illustrates the creation and assignment of a variable
  // suitable for initializing the image origin.
  //
  // \index{otb::Image!origin}
  // \index{otb::Image!SetOrigin()}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::PointType origin;

  origin[0] = 0.0;  // coordinates of the
  origin[1] = 0.0;  // first pixel in 2-D

  image->SetOrigin(origin);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The origin can also be retrieved from an image by using the
  //  \code{GetOrigin()} method. This will return a reference to a
  //  \code{Point}. The reference can be used to read the contents of
  //  the array. Note again the use of the \code{const} keyword to indicate
  //  that the array contents will not be modified.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const ImageType::PointType& orgn = image->GetOrigin();

  std::cout << "Origin = ";
  std::cout << orgn[0] << ", " << orgn[1] << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once the spacing and origin of the image have been initialized, the image
  // will correctly map pixel indices to and from physical space
  // coordinates. The following code illustrates how a point in physical
  // space can be mapped into an image index for the purpose of reading the
  // content of the closest pixel.
  //
  // First, a \doxygen{itk}{Point} type must be declared. The point type is
  // templated over the type used to represent coordinates and over the
  // dimension of the space. In this particular case, the dimension of the
  // point must match the dimension of the image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Point<double, ImageType::ImageDimension> PointType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The Point class, like an \doxygen{itk}{Index}, is a relatively small and
  // simple object.  For this reason, it is not reference-counted like the
  // large data objects in OTB.  Consequently, it is also not manipulated
  // with \doxygen{itk}{SmartPointer}s.  Point objects are simply declared as
  // instances of any other C++ class.  Once the point is declared, its
  // components can be accessed using traditional array notation. In
  // particular, the \code{[]} operator is available. For efficiency reasons,
  // no bounds checking is performed on the index used to access a particular
  // point component. It is the user's responsibility to make sure that the
  // index is in the range $\{0, Dimension-1\}$.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  PointType point;

  point[0] = 1.45;    // x coordinate
  point[1] = 7.21;    // y coordinate
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The image will map the point to an index using the values of the
  // current spacing and origin. An index object must be provided to
  // receive the results of the mapping. The index object can be
  // instantiated by using the \code{IndexType} defined in the Image
  // type.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::IndexType pixelIndex;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The \code{TransformPhysicalPointToIndex()} method of the image class
  // will compute the pixel index closest to the point provided. The method
  // checks for this index to be contained inside the current buffered pixel
  // data. The method returns a boolean indicating whether the resulting
  // index falls inside the buffered region or not. The output index should
  // not be used when the returned value of the method is \code{false}.
  //
  // The following lines illustrate the point to index mapping and the
  // subsequent use of the pixel index for accessing pixel data from the
  // image.
  //
  // \index{otb::Image!TransformPhysicalPointToIndex()}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  bool isInside = image->TransformPhysicalPointToIndex(point, pixelIndex);

  if (isInside)
    {
    ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);

    pixelValue += 5;

    image->SetPixel(pixelIndex, pixelValue);
    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Remember that \code{GetPixel()} and \code{SetPixel()} are very
  // inefficient methods for accessing pixel data. Image iterators should be
  // used when massive access to pixel data is required.
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}

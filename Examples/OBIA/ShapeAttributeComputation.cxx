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
//    INPUTS: {qb_RoadExtract2.tif}
//    OUTPUTS: {OBIAShapeAttribute.txt}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This basic example shows how compute shape attributes at the object level.
//  The input image is firstly converted into a set of regions (
// \doxygen{itk}{ShapeLabelObject}), some attribute values of each
//  object are computed and then saved to an ASCII file.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkShapeLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkShapeLabelMapFilter.h"
// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int main(int argc, char * argv[])
{

  if (argc != 3)
    {
    std::cerr << "usage: " << argv[0] << " input outputcentroidlist" <<
    std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  // The image types are defined using pixel types and
  // dimensions. The input image is defined as an \doxygen{otb}{Image}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const int dim = 2;
  typedef unsigned long                         PixelType;
  typedef otb::Image<PixelType, dim>            ImageType;
  typedef unsigned long                         LabelType;
  typedef itk::ShapeLabelObject<LabelType, dim> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>        LabelMapType;
  typedef itk::LabelImageToLabelMapFilter
  <ImageType, LabelMapType> ConverterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Firstly, the image reader is instantiated.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Here the \doxygen{itk}{ShapeLabelObject} type
  // is chosen in order to read some attributes related to the shape
  // of the objects, by opposition to the content of the object, with
  // the \doxygen{itk}{StatisticsLabelObject}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::ShapeLabelMapFilter<LabelMapType> ShapeFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The input image is converted in a collection of objects
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput(reader->GetOutput());
  converter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());

  ShapeFilterType::Pointer shape = ShapeFilterType::New();

  shape->SetInput(converter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Update the shape filter, so its output will be up to date.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  shape->Update();
  // Software Guide : EndCodeSnippet

  std::cout << "Nb. objects conv. " <<
  converter->GetOutput()->GetNumberOfLabelObjects() << std::endl;

  std::cout << "Nb. objects shape " <<
  shape->GetOutput()->GetNumberOfLabelObjects() << std::endl;

  //  Software Guide : BeginLatex
  //
  // Then, we can read the attribute values we're interested in. The \doxygen{itk}{BinaryImageToShapeLabelMapFilter}
  // produces consecutive labels, so we can use a for loop and \code{GetLabelObject()} method to retrieve
  // the label objects. If the labels are not consecutive, the \code{GetNthLabelObject()} method must be
  // use instead of \code{GetLabelObject()}, or an iterator on the label
  // object container of the label map.
  // In this example, we write 2 shape attributes of each object to a text file (the size and the centroid coordinates).
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::ofstream outfile(argv[2]);

  LabelMapType::Pointer labelMap = shape->GetOutput();
  for (unsigned long label = 1;
       label <= labelMap->GetNumberOfLabelObjects();
       label++)
    {
    // We don't need a SmartPointer of the label object here,
    // because the reference is kept in the label map.
    const LabelObjectType * labelObject = labelMap->GetLabelObject(label);
    outfile << label << "\t" << labelObject->GetPhysicalSize() << "\t"
            << labelObject->GetCentroid() << std::endl;
    }

  outfile.close();
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}

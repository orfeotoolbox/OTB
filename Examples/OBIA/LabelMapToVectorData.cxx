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
//    INPUTS: {rcc8_mire1.png}
//    OUTPUTS: {rcc8_mire2_vectorizer.shp}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::LabelMapToVectorDataFilter}
//
//
// This class vectorizes a LabelObject to a VectorData.
//
// The \doxygen{otb}{LabelMapToVectorDataFilter} converts an \doxygen{itk}{LabelMap} to an
// \doxygen{otb}{VectorData} where all the pixels get the attribute
// value of the label object they belong to.
// It uses the class \doxygen{otb}{LabelObjectToPolygonFunctor} wich
// follows a finite state machine described in \cite{Francis2000}.
//
//  Only polygon conversion is available yet.
// Software Guide : EndLatex

#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"

// Software Guide : BeginLatex
//
// These are the main header files which need to be included:
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
#include "otbLabelMapToVectorDataFilter.h"
#include "otbAttributesMapLabelObject.h"
// Software Guide : EndCodeSnippet
#include "itkLabelImageToLabelMapFilter.h"

int main(int argc, char * argv[])
{
  /** Use the labelObjecttopolygon functor (not thread safe) only polygon conversion is available yet*/
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
    }
  const char * infname = argv[1];
  const char * outfname = argv[2];

  //  Software Guide : BeginLatex
  //
  // The image types are defined using pixel types and
  // dimension. The input image is defined as an \doxygen{itk}{Image},
  // the output is a \doxygen{otb}{VectorData}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension               = 2;
  typedef unsigned short                   LabelType;
  typedef otb::Image<LabelType, Dimension> LabeledImageType;
  typedef otb::VectorData<double, 2>       VectorDataType;
  // Software Guide : EndCodeSnippet

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<LabeledImageType>    LabeledReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  // Label map typedef
  //  Software Guide : BeginLatex
  //
  // The Attribute Label Map is
  // instantiated using the image pixel types as template parameters.
  // The LabelObjectToPolygonFunctor is instantiated with LabelObjectType and PolygonType.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::AttributesMapLabelObject<LabelType, Dimension,
      double>
  LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>
  LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType,
      LabelMapType>
  LabelMapFilterType;
 // Software Guide : EndCodeSnippet

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  WriterType::Pointer        writer = WriterType::New();

  //  Software Guide : BeginLatex
  //  Now the reader and writer are instantiated and
  //  the input image is set and a name is given to the output image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  lreader->SetFileName(infname);
  writer->SetFileName(outfname);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Then, the input image is converted to a map of label objects.
  //  Here each white region connected regions are converted. So the background is define all zero pixels.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
  labelMapFilter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Then, the \doxygen{otb}{LabelMapToVectorDataFilter} is instantiated. This is
  // the main filter which performs the vectorization.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::LabelMapToVectorDataFilter<LabelMapType,
      VectorDataType>
  LabelMapToVectorDataFilterType;

  LabelMapToVectorDataFilterType::Pointer MyFilter =
    LabelMapToVectorDataFilterType::New();

  MyFilter->SetInput(labelMapFilter->GetOutput());
  MyFilter->Update();
  MyFilter->GetOutput()->SetProjectionRef(
    lreader->GetOutput()->GetProjectionRef());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The output can be passed to a writer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetInput(MyFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  As usual, it is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  try
    {
    writer->Update();
    return EXIT_SUCCESS;
    }
  catch (itk::ExceptionObject& excep)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet
  catch (...)
    {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}

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


/* Example usage:
./LabelMapToVectorData Input/labelImage_UnsignedChar.tif Output/LabelMapToVectorData.shp
*/


// \index{otb::LabelMapToVectorDataFilter}
//
//
// This class vectorizes a LabelObject to a VectorData.
//
// The \doxygen{otb}{LabelMapToVectorDataFilter} converts an \doxygen{itk}{LabelMap} to an
// \doxygen{otb}{VectorData} where all the pixels get the attribute
// value of the label object they belong to.
// It uses the class \doxygen{otb}{LabelObjectToPolygonFunctor} which
// follows a finite state machine described in \cite{Francis2000}.
//
//  Only polygon conversion is available yet.

#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"

#include <fstream>
#include <iostream>

#include "otbImage.h"

// These are the main header files which need to be included:
#include "otbLabelMapToVectorDataFilter.h"
#include "otbAttributesMapLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"

int main(int argc, char* argv[])
{
  /** Use the labelObjecttopolygon functor (not thread safe) only polygon conversion is available yet*/
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputVectorfile(shp)" << std::endl;
    return EXIT_FAILURE;
  }
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  // The image types are defined using pixel types and
  // dimension. The input image is defined as an \doxygen{itk}{Image},
  // the output is a \doxygen{otb}{VectorData}.

  const unsigned int                       Dimension = 2;
  typedef unsigned short                   LabelType;
  typedef otb::Image<LabelType, Dimension> LabeledImageType;
  typedef otb::VectorData<double, 2>       VectorDataType;

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<LabeledImageType>    LabeledReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  // Label map typedef
  // The Attribute Label Map is
  // instantiated using the image pixel types as template parameters.
  // The LabelObjectToPolygonFunctor is instantiated with LabelObjectType and PolygonType.

  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>     LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                  LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType> LabelMapFilterType;

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  WriterType::Pointer        writer  = WriterType::New();

  //  Now the reader and writer are instantiated and
  //  the input image is set and a name is given to the output image.

  lreader->SetFileName(infname);
  writer->SetFileName(outfname);

  //  Then, the input image is converted to a map of label objects.
  //  Here each white region connected regions are converted. So the background is define all zero pixels.

  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(lreader->GetOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());
  labelMapFilter->Update();

  //  Then, the \doxygen{otb}{LabelMapToVectorDataFilter} is instantiated. This is
  // the main filter which performs the vectorization.

  typedef otb::LabelMapToVectorDataFilter<LabelMapType, VectorDataType> LabelMapToVectorDataFilterType;

  LabelMapToVectorDataFilterType::Pointer MyFilter = LabelMapToVectorDataFilterType::New();

  MyFilter->SetInput(labelMapFilter->GetOutput());
  MyFilter->Update();
  MyFilter->GetOutput()->SetProjectionRef(lreader->GetOutput()->GetProjectionRef());

  //  The output can be passed to a writer.

  writer->SetInput(MyFilter->GetOutput());

  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  As usual, it is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
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
  catch (...)
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

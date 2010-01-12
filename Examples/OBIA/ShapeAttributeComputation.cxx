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
//  This example shows the
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkShapeLabelMapFilter.h"

// Software Guide : EndCodeSnippet
#include "otbImageFileReader.h"
#include <fstream>

int main(int argc, char * argv[])
{
  const int dim                           = 2;
  typedef unsigned long                   PixelType;
  typedef itk::Image< PixelType, dim >    ImageType;
  
  if( argc != 3)
    {
    std::cerr << "usage: " << argv[0] << " input outputcentroidlist" << std::endl;
    return EXIT_FAILURE;
    }

  // read the input image
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  // define the object type. Here the ShapeLabelObject type
  // is chosen in order to read some attribute related to the shape
  // of the objects (by opposition to the content of the object, with
  // the StatisticsLabelObejct).
  typedef unsigned long                           LabelType;
  typedef itk::ShapeLabelObject< LabelType, dim > LabelObjectType;
  typedef itk::LabelMap< LabelObjectType >        LabelMapType;

  // convert the image in a collection of objects
  typedef itk::LabelImageToLabelMapFilter< ImageType, LabelMapType > ConverterType;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( reader->GetOutput() );
  converter->SetBackgroundValue( itk::NumericTraits<LabelType>::min() );

  typedef itk::ShapeLabelMapFilter< LabelMapType > ShapeFilterType;
  ShapeFilterType::Pointer shape = ShapeFilterType::New();

  shape->SetInput( converter->GetOutput() );
  // update the shape filter, so its output will be up to date
  shape->Update();

  std::cout << "Nb. objects conv. " << converter->GetOutput()->GetNumberOfLabelObjects() << std::endl;

    std::cout << "Nb. objects shape " << shape->GetOutput()->GetNumberOfLabelObjects() << std::endl;
  // then we can read the attribute values we're interested in. The BinaryImageToShapeLabelMapFilter
  // produce consecutive labels, so we can use a for loop and GetLabelObject() method to retrieve
  // the label objects. If the labels are not consecutive, the GetNthLabelObject() method must be
  // use instead of GetLabelObject(), or an iterator on the label
  // object container of the label map.
    std::ofstream outfile( argv[2] );


  LabelMapType::Pointer labelMap = shape->GetOutput();
  for( unsigned long label=1; label<=labelMap->GetNumberOfLabelObjects(); label++ )
    {
    // we don't need a SmartPointer of the label object here, because the reference is kept in
    // in the label map.
    const LabelObjectType * labelObject = labelMap->GetLabelObject( label );
    outfile << label << "\t" << labelObject->GetPhysicalSize() << "\t" << labelObject->GetCentroid() << std::endl;
    }

  outfile.close();
  return EXIT_SUCCESS;
}       

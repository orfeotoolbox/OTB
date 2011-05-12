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
#include "otbVectorDataTransformFilterApplication.h"

#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataTransformFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataProjectionFilter.h"
#include "itkCenteredSimilarity2DTransform.h"
#include "otbMath.h"

namespace otb
{

int VectorDataTransformFilterApplication::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("VectorDataTransformFilterApplication");
  descriptor->SetDescription("Apply a transform to each vertex of the input VectorData");
  descriptor->AddOption("Input","Support image","in", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("InputVectorData","Input vector data","vd", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Output","Output vector data","out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("TanslationX","Translation in the X direction","tx", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("TanslationY","Translation in the Y direction","ty", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("Angle",
                        "Angle of the rotation to apply in degrees","rot", 
                        1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("CenterX","The first coordinate of the rotation center","cx", 1, 
                        false, ApplicationDescriptor::Real);
  descriptor->AddOption("CenterY","The second coordinate of the rotation center","cy", 1, 
                        false, ApplicationDescriptor::Real);
  descriptor->AddOption("Scale","The scale to apply ","sc", 1, false, ApplicationDescriptor::Real);
  
  return EXIT_SUCCESS;
}

int VectorDataTransformFilterApplication::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {
    typedef otb::VectorData<>                         VectorDataType;
    typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
    typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;
    typedef otb::VectorDataTransformFilter
      <VectorDataType, VectorDataType>                VectorDataTransformType;
    typedef otb::VectorDataProjectionFilter<VectorDataType,
      VectorDataType>                                 VDProjectionFilterType;

    typedef otb::VectorImage<double, 2>               ImageType;
    typedef otb::ImageFileReader<ImageType>           ReaderType;
    
    // Instanciate the image reader
    ReaderType::Pointer      reader = ReaderType::New();
    reader->SetFileName(parseResult->GetParameterString("Input"));
    reader->UpdateOutputInformation();

    VectorDataFileReaderType::Pointer  vdreader = VectorDataFileReaderType::New();
    vdreader->SetFileName(parseResult->GetParameterString("InputVectorData"));
    vdreader->Update();
  
    // Reproject the VectorData In the image coordinate system
    VDProjectionFilterType::Pointer  vdproj = VDProjectionFilterType::New();
    vdproj->SetInput(vdreader->GetOutput());
    vdproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
    vdproj->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    vdproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
    
    // Set up the transform (Apply a translation of 8 pixels in the y
    // direction)
    typedef itk::CenteredSimilarity2DTransform<double> TransformType;
    TransformType::Pointer transform = TransformType::New();
    
    TransformType::ParametersType parameters(6);
    parameters[0] = 1;                  // Scale
    parameters[1] = 0.;                 // Rotation Angle in radian
    parameters[2] = 0;          // Center of the rotation (X)
    parameters[3] = 0;          // Center of the rotation (Y)
    parameters[4] = 0;                  // Translation (X)
    parameters[5] = 0;                  // Translation (Y)

    // Get parameters if any
    if(parseResult->IsOptionPresent("Scale"))
      parameters[0] = parseResult->GetParameterFloat("Scale");

    if(parseResult->IsOptionPresent("Angle"))
      parameters[1] = CONST_PI*parseResult->GetParameterFloat("Angle")/180.;

    if(parseResult->IsOptionPresent("CenterX"))
      parameters[2] = parseResult->GetParameterFloat("CenterX");
    
    if(parseResult->IsOptionPresent("CenterY"))
      parameters[3] = parseResult->GetParameterFloat("CenterY");
    
    if(parseResult->IsOptionPresent("TanslationX"))
      parameters[4] = parseResult->GetParameterFloat("TanslationX");

    if(parseResult->IsOptionPresent("TanslationY"))
      parameters[5] = parseResult->GetParameterFloat("TanslationY");
    
    // Set the parameters to the transform
    transform->SetParameters(parameters);
    
    VectorDataTransformType::Pointer transformFilter = VectorDataTransformType::New();
    transformFilter->SetInput(vdproj->GetOutput());
    transformFilter->SetTransform(transform);
  
    // retransform int the input vector projection
    VDProjectionFilterType::Pointer  reverseVdProj = VDProjectionFilterType::New();
    reverseVdProj->SetInput(transformFilter->GetOutput());
    reverseVdProj->SetOutputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
    reverseVdProj->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    reverseVdProj->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());

    //Write the vectordata
    VectorDataFileWriterType::Pointer vdwriter = VectorDataFileWriterType::New();
    vdwriter->SetInput(reverseVdProj->GetOutput());
    vdwriter->SetFileName(parseResult->GetParameterString("Output"));
    vdwriter->Update();
  
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cout << "Following otbException caught :" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch ( std::bad_alloc & err )
    {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
    }
  catch ( ... )
    {
    std::cout << "Unknown Exception found !" << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
}

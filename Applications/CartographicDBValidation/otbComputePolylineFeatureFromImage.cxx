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
#include "otbComputePolylineFeatureFromImage.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "itkPreOrderTreeIterator.h"

#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"

#include "otbParserConditionDataNodeFeatureFunction.h"
#include "otbNDVIDataNodeFeatureFunction.h"

namespace otb
{

int ComputePolylineFeatureFromImage::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("ComputePolylineFeatureFromImage");
  descriptor->SetDescription("Compute a polyline feature descriptors from an input image which are part of the polyline pixels that verify the FeatureExpression");
  descriptor->AddOption("InputImage", "An image from which to compute description",
                        "img",   1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("InputVectorData", "Vector data containing the polylines onto which the feature will be computed",
                        "vdin",  1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("FeatureExpression", "The feature formula (b1 > 0.3)",
                        "expr",  1, true, ApplicationDescriptor::String);
  descriptor->AddOption("FieldName", "The feature name (NONDVI, ROADSA...)",
                        "field", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("OutputVectorData", "The output vector data containing the feature",
                        "out",   1, true, ApplicationDescriptor::FileName);

  descriptor->AddOption("DEMDirectory", "DEM directory",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);

  return EXIT_SUCCESS;
}

int ComputePolylineFeatureFromImage::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Images
  typedef float                                           PixelType;
  typedef VectorImage<PixelType, 2>                       ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;

  // VectorData
  typedef VectorData<>                                    VectorDataType;
  typedef VectorDataType::DataNodeType                    DataNodeType;
  typedef DataNodeType::ValuePrecisionType                PrecisionType;
  typedef DataNodeType::PrecisionType                     CoordRepType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType>
                                                          TreeIteratorType;
  typedef VectorDataFileReader<VectorDataType>            VectorDataReaderType;
  typedef VectorDataFileWriter<VectorDataType>            VectorDataWriterType;

  typedef VectorDataIntoImageProjectionFilter
    <VectorDataType, ImageType>                           VectorDataIntoImageProjType;
  typedef VectorDataProjectionFilter
    <VectorDataType, VectorDataType>                      VectorDataProjectionFilterType;

  typedef ParserConditionDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
                                                          ParserConditionFeatureFunctionType;

  // Vector Data into Image projection
  //// Read the Image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetParameterString("InputImage"));
  reader->UpdateOutputInformation();
  reader->Update();
  ImageType::Pointer inputImage = reader->GetOutput();
  //// Read the Vectordata
  VectorDataReaderType::Pointer vdreader = VectorDataReaderType::New();
  vdreader->SetFileName(parseResult->GetParameterString("InputVectorData"));
  vdreader->Update();
  //// Projection
  VectorDataIntoImageProjType::Pointer vprojIm = VectorDataIntoImageProjType::New();
  vprojIm->SetInputVectorData(vdreader->GetOutput());
  vprojIm->SetInputImage(inputImage);
  if( parseResult->IsOptionPresent("DEMDirectory") )
    {
      vprojIm->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
    }
  vprojIm->SetUseOutputSpacingAndOriginFromImage(true); // we want index as input;
  vprojIm->Update();

  // Add description
  ParserConditionFeatureFunctionType::Pointer vdescriptor = ParserConditionFeatureFunctionType::New();
  vdescriptor->SetExpression(parseResult->GetParameterString("FeatureExpression"));
  vdescriptor->SetInputImage(inputImage);

  VectorDataType::Pointer outVD = VectorDataType::New();
  // Retrieving root node
  DataNodeType::Pointer root = outVD->GetDataTree()->GetRoot()->Get();
  // Create the document node
  DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  outVD->GetDataTree()->Add(document, root);
  // Create the folder node
  DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  // Adding the layer to the data tree
  outVD->GetDataTree()->Add(folder, document);

  TreeIteratorType itVector(vprojIm->GetOutput()->GetDataTree());
  itVector.GoToBegin();

  while (!itVector.IsAtEnd())
    {
    vdescriptor->SetInputImage(reader->GetOutput());
    if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
      {
      DataNodeType::Pointer currentGeometry = itVector.Get();
      currentGeometry->SetFieldAsDouble(parseResult->GetParameterString("FieldName"),
                                        (double)(vdescriptor->Evaluate(*(currentGeometry.GetPointer()))[0]));

      outVD->GetDataTree()->Add(currentGeometry, folder);
      }
    ++itVector;
    }

  /*
   * Reprojection of the output VectorData
   *
   * The output of VectorDataToRoadDescription is in image index coordinates
   *
   * 3 cases :
   * - input image has no geo-information : pass through
   * - input image is in cartographic projection : apply image spacing and origin, and set the ProjectionRef
   * - input image is in sensor model geometry : reproject in WGS84
   *
   */

  std::string projRef = inputImage->GetProjectionRef();
  ImageKeywordlist kwl;
  itk::ExposeMetaData<ImageKeywordlist>(inputImage->GetMetaDataDictionary(),
                                        MetaDataKey::OSSIMKeywordlistKey,
                                        kwl);

  VectorDataType::Pointer projectedVD;

  if ( !projRef.empty() )
    {
    // image is in cartographic projection
    // apply spacing and origin + set projection WKT
    // The VectorData in output of the chain is in image index coordinate,
    // and the projection information is lost
    // Apply an affine transform to apply image origin and spacing,
    // and arbitrarily set the ProjectionRef to the input image ProjectionRef

    typedef itk::AffineTransform<VectorDataType::PrecisionType, 2> TransformType;
    typedef otb::VectorDataTransformFilter<VectorDataType, VectorDataType> VDTransformType;

    TransformType::ParametersType params;
    params.SetSize(6);
    params[0] = inputImage->GetSpacing()[0];
    params[1] = 0;
    params[2] = 0;
    params[3] = inputImage->GetSpacing()[1];
    params[4] = inputImage->GetOrigin()[0];
    params[5] = inputImage->GetOrigin()[1];

    TransformType::Pointer transform = TransformType::New();
    transform->SetParameters(params);

    VDTransformType::Pointer vdTransform = VDTransformType::New();
    vdTransform->SetTransform(transform);
    vdTransform->SetInput(outVD);
    vdTransform->Update();

    projectedVD = vdTransform->GetOutput();

    projectedVD->SetProjectionRef(inputImage->GetProjectionRef());
    }
    else if ( kwl.GetSize() > 0 )
    {
    // image is in sensor model geometry

    // Reproject VectorData in image projection
    VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(outVD);

    vproj->SetOutputKeywordList(inputImage->GetImageKeywordlist());
    vproj->SetOutputProjectionRef(inputImage->GetProjectionRef());
    vproj->SetOutputOrigin(inputImage->GetOrigin());
    vproj->SetOutputSpacing(inputImage->GetSpacing());

    if( parseResult->IsOptionPresent("DEMDirectory") )
      {
      vproj->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
      }

    vproj->Update();
    projectedVD = vproj->GetOutput();
    }
    else
    {
    // no georeferencing information

    projectedVD = outVD;

    }

   // Write vectordata with description
   VectorDataWriterType::Pointer vdwriter = VectorDataWriterType::New();
   vdwriter->SetFileName(parseResult->GetParameterString("OutputVectorData"));
   vdwriter->SetInput(projectedVD);
   vdwriter->Update();

  return EXIT_SUCCESS;
}

}

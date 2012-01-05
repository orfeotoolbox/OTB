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
#include <iostream>

#include "otbVectorImage.h"

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"


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

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class ComputePolylineFeatureFromImage: public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputePolylineFeatureFromImage Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  // Images
  typedef FloatVectorImageType::PixelType PixelType;
  typedef FloatVectorImageType ImageType;

  // VectorData
  typedef VectorData<> VectorDataType;
  typedef VectorDataType::DataNodeType DataNodeType;
  typedef DataNodeType::ValuePrecisionType PrecisionType;
  typedef DataNodeType::PrecisionType CoordRepType;
  typedef itk::PreOrderTreeIterator<VectorDataType::DataTreeType> TreeIteratorType;
  typedef VectorDataIntoImageProjectionFilter<VectorDataType, ImageType> VectorDataIntoImageProjType;
  typedef VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;

  typedef ParserConditionDataNodeFeatureFunction<ImageType, CoordRepType, PrecisionType>
      ParserConditionFeatureFunctionType;

  /** Standard macro */
  itkNewMacro(Self)
;

  itkTypeMacro(ComputePolylineFeatureFromImage, otb::Application)
;

private:
  void DoInit()
  {
    SetName("ComputePolylineFeatureFromImage");
    SetDescription("Compute a polyline feature descriptors from an input image which are part of the polyline pixels that verify the FeatureExpression");

    SetDocName("Compute Polyline Feature From Image");
    SetDocLongDescription("This application computes a polyline feature descriptors from an input image which are part of the polyline pixels that verify the FeatureExpression.");
    SetDocLimitations("Since it do not rely on streaming process, take care of the size of input image before launching application.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "An image from which to compute description.");

    AddParameter(ParameterType_InputVectorData, "vd", "Vector Data");
    SetParameterDescription("vd", "Vector data containing the polylines onto which the feature will be computed.");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_String, "expr", "Feature expression");
    SetParameterDescription("expr", "The feature formula (b1 > 0.3)");

    AddParameter(ParameterType_String, "field", "Feature name");
    SetParameterDescription("field", "The feature name (NONDVI, ROADSA...)");

    AddParameter(ParameterType_OutputVectorData, "out", "Output Vector Data");
    SetParameterDescription("out", "The output vector data containing the features");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "NDVI.TIF");
    SetDocExampleParameterValue("vd", "roads_ground_truth.shp");
    SetDocExampleParameterValue("expr", "\"(b1 > 0.4)\"");
    SetDocExampleParameterValue("field", "NONDVI");
    SetDocExampleParameterValue("out", "PolylineFeatureFromImage_LI_NONDVI_gt.shp");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    // Vector Data into Image projection
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    inImage->UpdateOutputInformation();
    try
      {
      inImage->Update();
      }
    catch (...)
      {
      itkGenericExceptionMacro(<< "Error during image update, maybe your image is to big.");
      }

    otbAppLogDEBUG( << "Starting PolylineFeature extraction process" )

    // Vector Data into Image projection
    //// Read the Vectordata

    VectorDataType* inVectorData = GetParameterVectorData("vd");
    inVectorData->Update();
    //// Projection
    VectorDataIntoImageProjType::Pointer vprojIm = VectorDataIntoImageProjType::New();
    vprojIm->SetInputVectorData(inVectorData);
    vprojIm->SetInputImage(inImage);

    // Elevation through the elevation handler
    if (ElevationParametersHandler::IsElevationEnabled(this, "elev"))
      {
      switch(ElevationParametersHandler::GetElevationType(this, "elev"))
        {
        case Elevation_DEM:
        {
        vprojIm->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
        vprojIm->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
        }
        break;
        case Elevation_Average:
        {
        vprojIm->SetAverageElevation(ElevationParametersHandler::GetAverageElevation(this, "elev"));
        }
        break;
        //   Commented cause using a tiff file is not implemented yet
        //  case Elevation_Tiff:
        //  {
        //  }
        //  break;
        }
      }

    vprojIm->SetUseOutputSpacingAndOriginFromImage(true); // we want index as input;
    vprojIm->Update();

    // Add description
    ParserConditionFeatureFunctionType::Pointer vdescriptor = ParserConditionFeatureFunctionType::New();
    vdescriptor->SetExpression(GetParameterString("expr"));
    vdescriptor->SetInputImage(inImage);

    m_OutVectorData = VectorDataType::New();
    // Retrieving root node
    DataNodeType::Pointer root = m_OutVectorData->GetDataTree()->GetRoot()->Get();
    // Create the document node
    DataNodeType::Pointer document = DataNodeType::New();
    document->SetNodeType(otb::DOCUMENT);
    // Adding the layer to the data tree
    m_OutVectorData->GetDataTree()->Add(document, root);
    // Create the folder node
    DataNodeType::Pointer folder = DataNodeType::New();
    folder->SetNodeType(otb::FOLDER);
    // Adding the layer to the data tree
    m_OutVectorData->GetDataTree()->Add(folder, document);

    TreeIteratorType itVector(vprojIm->GetOutput()->GetDataTree());
    itVector.GoToBegin();

    while (!itVector.IsAtEnd())
      {
      vdescriptor->SetInputImage(inImage);
      if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
        {
        DataNodeType::Pointer currentGeometry = itVector.Get();
        currentGeometry->SetFieldAsDouble(GetParameterString("field"),
                                          (double) (vdescriptor->Evaluate(*(currentGeometry.GetPointer()))[0]));
        m_OutVectorData->GetDataTree()->Add(currentGeometry, folder);
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

    std::string projRef = inImage->GetProjectionRef();
    ImageKeywordlist kwl;
    itk::ExposeMetaData<ImageKeywordlist>(inImage->GetMetaDataDictionary(), MetaDataKey::OSSIMKeywordlistKey, kwl);

    if (!projRef.empty())
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
      params[0] = inImage->GetSpacing()[0];
      params[1] = 0;
      params[2] = 0;
      params[3] = inImage->GetSpacing()[1];
      params[4] = inImage->GetOrigin()[0];
      params[5] = inImage->GetOrigin()[1];

      TransformType::Pointer transform = TransformType::New();
      transform->SetParameters(params);

      VDTransformType::Pointer vdTransform = VDTransformType::New();
      vdTransform->SetTransform(transform);
      vdTransform->SetInput(m_OutVectorData);
      vdTransform->Update();

      m_ProjectedVectorData = vdTransform->GetOutput();

      m_ProjectedVectorData->SetProjectionRef(inImage->GetProjectionRef());
      }
    else
      if (kwl.GetSize() > 0)
        {
        // image is in sensor model geometry

        // Reproject VectorData in image projection
        VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
        vproj->SetInput(m_OutVectorData);

        vproj->SetOutputKeywordList(inImage->GetImageKeywordlist());
        vproj->SetOutputProjectionRef(inImage->GetProjectionRef());
        vproj->SetOutputOrigin(inImage->GetOrigin());
        vproj->SetOutputSpacing(inImage->GetSpacing());

        // Elevation through the elevation handler
        switch(ElevationParametersHandler::GetElevationType(this, "elev"))
          {
          case Elevation_DEM:
          {
          vproj->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
          vproj->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
          }
          break;
          case Elevation_Average:
          {
          vproj->SetAverageElevation(ElevationParametersHandler::GetAverageElevation(this, "elev"));
          }
          break;
          //   Commented cause using a tiff file is not implemented yet
          //  case Elevation_Tiff:
          //  {
          //  }
          //  break;
          }

        vproj->Update();
        m_ProjectedVectorData = vproj->GetOutput();
        }
      else
        {
        // no georeferencing information
        m_ProjectedVectorData = m_OutVectorData;
        }

    // Set the output vectorData
    SetParameterOutputVectorData("out", m_ProjectedVectorData);

  };

  VectorDataType::Pointer m_ProjectedVectorData;
  VectorDataType::Pointer m_OutVectorData;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolylineFeatureFromImage)



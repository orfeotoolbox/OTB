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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMultiChannelExtractROI.h"
#include "otbStandardFilterWatcher.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperTypes.h"

#include "otbWrapperElevationParametersHandler.h"

#include "otbGenericRSTransform.h"

namespace otb
{
namespace Wrapper
{

class ExtractROI : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI                          Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef otb::GenericRSTransform<>           RSTransformType;
  typedef RSTransformType::InputPointType     Point3DType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractROI, otb::Application);

  /** Filters typedef */
  typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                      FloatVectorImageType::InternalPixelType> ExtractROIFilterType;

private:
  void DoInit()
  {
    SetName("ExtractROI");
    SetDescription("Extract a ROI defined by the user.");

    // Documentation
    SetDocName("Extract ROI");
    SetDocLongDescription("This application extracts a Region Of Interest with user defined size, or reference image.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,"mode","Extraction mode");
    AddChoice("mode.standard","Standard");
    SetParameterDescription("mode.standard","In standard mode, extract is done according the coordinates entered by the user");
    AddChoice("mode.fit","Fit");
    SetParameterDescription("mode.fit","In fit mode, extract is made to best fit a reference image.");

    AddParameter(ParameterType_InputImage,"mode.fit.ref","Reference image");
    SetParameterDescription("mode.fit.ref","Reference image to define the ROI");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this,"mode.fit.elev");

    AddParameter(ParameterType_Int,  "startx", "Start X");
    SetParameterDescription("startx", "ROI start x position.");
    AddParameter(ParameterType_Int,  "starty", "Start Y");
    SetParameterDescription("starty", "ROI start y position.");
    AddParameter(ParameterType_Int,  "sizex",  "Size X");
    SetParameterDescription("sizex","size along x in pixels.");
    AddParameter(ParameterType_Int,  "sizey",  "Size Y");
    SetParameterDescription("sizey","size along y in pixels.");

    // Default values
    SetDefaultParameterInt("startx", 0);
    SetDefaultParameterInt("starty", 0);
    SetDefaultParameterInt("sizex",  0);
    SetDefaultParameterInt("sizey",  0);

    // Channelist Parameters
    AddParameter(ParameterType_ListView,  "cl", "Output Image channels");
    SetParameterDescription("cl","Channels to write in the output image.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "VegetationIndex.hd");
    SetDocExampleParameterValue("startx", "40");
    SetDocExampleParameterValue("starty", "250");
    SetDocExampleParameterValue("sizex", "150");
    SetDocExampleParameterValue("sizey", "150");
    SetDocExampleParameterValue("out", "ExtractROI.tif");
  }

  void DoUpdateParameters()
  {
    // Update the sizes only if the user has not defined a size
    if ( HasValue("in") )
      {
      ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
      ExtractROIFilterType::InputImageType::RegionType  largestRegion = inImage->GetLargestPossibleRegion();

      if (!HasUserValue("sizex")  && !HasUserValue("sizey") )
        {
        SetParameterInt("sizex", largestRegion.GetSize()[0]);
        SetParameterInt("sizey", largestRegion.GetSize()[1]);
        }

      // Update the values of the channels to be selected
      unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ClearChoices("cl");
      for (unsigned int idx = 0; idx < nbComponents; ++idx)
        {
        std::ostringstream key, item;
        key<<"cl.channel"<<idx+1;
        item<<"Channel"<<idx+1;
        AddChoice(key.str(), item.str());
        }

      // Put the limit of the index and the size relative the image
      SetMinimumParameterIntValue("sizex", 0);
      SetMaximumParameterIntValue("sizex", largestRegion.GetSize(0));

      SetMinimumParameterIntValue("sizey", 0);
      SetMaximumParameterIntValue("sizey", largestRegion.GetSize(1));

      SetMinimumParameterIntValue("startx", 0);
      SetMaximumParameterIntValue("startx", largestRegion.GetSize(0));

      SetMinimumParameterIntValue("starty", 0);
      SetMaximumParameterIntValue("starty", largestRegion.GetSize(1));

      // Crop the roi region to be included in the largest possible
      // region
      if(!this->CropRegionOfInterest())
        {
        // Put the index of the ROI to origin and try to crop again
        SetParameterInt("startx", 0);
        SetParameterInt("starty", 0);
        this->CropRegionOfInterest();
        }
      }

    if(GetParameterString("mode")=="fit")
      {
      this->SetParameterRole("startx",Role_Output);
      this->SetParameterRole("starty",Role_Output);
      this->SetParameterRole("sizex",Role_Output);
      this->SetParameterRole("sizey",Role_Output);

      this->DisableParameter("startx");
      this->DisableParameter("starty");
      this->DisableParameter("sizex");
      this->DisableParameter("sizey");

      }
    else if(GetParameterString("mode")=="standard")
      {
      this->SetParameterRole("startx",Role_Input);
      this->SetParameterRole("starty",Role_Input);
      this->SetParameterRole("sizex",Role_Input);
      this->SetParameterRole("sizey",Role_Input);

      this->EnableParameter("startx");
      this->EnableParameter("starty");
      this->EnableParameter("sizex");
      this->EnableParameter("sizey");
      }
  }

  bool CropRegionOfInterest()
  {
    FloatVectorImageType::RegionType region;
    region.SetSize(0,  GetParameterInt("sizex"));
    region.SetSize(1,  GetParameterInt("sizey"));
    region.SetIndex(0, GetParameterInt("startx"));
    region.SetIndex(1, GetParameterInt("starty"));

    if ( HasValue("in") )
      {
        if (region.Crop(GetParameterImage("in")->GetLargestPossibleRegion()))
          {
            SetParameterInt("sizex", region.GetSize(0));
            SetParameterInt("sizey", region.GetSize(1));
            SetParameterInt("startx", region.GetIndex(0));
            SetParameterInt("starty", region.GetIndex(1));
            return true;
          }
      }
    return false;
  }

  void DoExecute()
  {
    ExtractROIFilterType::InputImageType* inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();


    if(GetParameterString("mode")=="fit")
      {
      // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"mode.fit.elev");

      FloatVectorImageType::Pointer referencePtr = this->GetParameterImage<FloatVectorImageType>("mode.fit.ref");
      referencePtr->UpdateOutputInformation();

      RSTransformType::Pointer rsTransform = RSTransformType::New();
      rsTransform->SetInputKeywordList(referencePtr->GetImageKeywordlist());
      rsTransform->SetInputProjectionRef(referencePtr->GetProjectionRef());
      rsTransform->SetOutputKeywordList(inImage->GetImageKeywordlist());
      rsTransform->SetOutputProjectionRef(inImage->GetProjectionRef());
      rsTransform->InstanciateTransform();

      FloatVectorImageType::IndexType uli_ref,uri_ref,lli_ref,lri_ref;

      uli_ref = referencePtr->GetLargestPossibleRegion().GetIndex();
      uri_ref = uli_ref;
      uri_ref[0]+=referencePtr->GetLargestPossibleRegion().GetSize()[0];
      lli_ref = uli_ref;
      lli_ref[1]+=referencePtr->GetLargestPossibleRegion().GetSize()[1];
      lri_ref = lli_ref;
      lri_ref[0]+=referencePtr->GetLargestPossibleRegion().GetSize()[0];

      FloatVectorImageType::PointType ulp_ref,urp_ref,llp_ref,lrp_ref;

      referencePtr->TransformIndexToPhysicalPoint(uli_ref,ulp_ref);
      referencePtr->TransformIndexToPhysicalPoint(uri_ref,urp_ref);
      referencePtr->TransformIndexToPhysicalPoint(lli_ref,llp_ref);
      referencePtr->TransformIndexToPhysicalPoint(lri_ref,lrp_ref);

      FloatVectorImageType::PointType ulp_out, urp_out, llp_out,lrp_out;

      ulp_out = rsTransform->TransformPoint(ulp_ref);
      urp_out = rsTransform->TransformPoint(urp_ref);
      llp_out = rsTransform->TransformPoint(llp_ref);
      lrp_out = rsTransform->TransformPoint(lrp_ref);

      FloatVectorImageType::IndexType uli_out, uri_out, lli_out, lri_out;

      inImage->TransformPhysicalPointToIndex(ulp_out,uli_out);
      inImage->TransformPhysicalPointToIndex(urp_out,uri_out);
      inImage->TransformPhysicalPointToIndex(llp_out,lli_out);
      inImage->TransformPhysicalPointToIndex(lrp_out,lri_out);

      FloatVectorImageType::IndexType uli, lri;

      uli[0] = std::min(std::min(uli_out[0],uri_out[0]),std::min(lli_out[0],lri_out[0]));
      uli[1] = std::min(std::min(uli_out[1],uri_out[1]),std::min(lli_out[1],lri_out[1]));

      lri[0] = std::max(std::max(uli_out[0],uri_out[0]),std::max(lli_out[0],lri_out[0]));
      lri[1] = std::max(std::max(uli_out[1],uri_out[1]),std::max(lli_out[1],lri_out[1]));

      SetParameterInt("startx",uli[0]);
      SetParameterInt("starty",uli[1]);
      SetParameterInt("sizex",lri[0]-uli[0]);
      SetParameterInt("sizey",lri[1]-uli[1]);

      std::cout<<"ul: "<<uli<<", lr: "<<lri<<std::endl;

      this->CropRegionOfInterest();
      }


    m_ExtractROIFilter = ExtractROIFilterType::New();
    m_ExtractROIFilter->SetInput(inImage);
    m_ExtractROIFilter->SetStartX(GetParameterInt("startx"));
    m_ExtractROIFilter->SetStartY(GetParameterInt("starty"));
    m_ExtractROIFilter->SetSizeX(GetParameterInt("sizex"));
    m_ExtractROIFilter->SetSizeY(GetParameterInt("sizey"));

    for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
      {
      m_ExtractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1 );
      }

    SetParameterOutputImage("out", m_ExtractROIFilter->GetOutput());
  }

  ExtractROIFilterType::Pointer   m_ExtractROIFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ExtractROI)

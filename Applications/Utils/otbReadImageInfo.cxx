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
#include "otbCoordinateToName.h"

namespace otb
{
namespace Wrapper
{

class ReadImageInfo : public Application
{
public:
  /** Standard class typedefs. */
  typedef ReadImageInfo                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ReadImageInfo, otb::Application);

private:
  ReadImageInfo()
  {
    SetName("ReadImageInfo");
    SetDescription("Get information about the image");

    // Documentation
    SetDocName("Read image information Application");
    SetDocLongDescription("Display informations about the input image like: image size, metadata, projections...");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("Utilities");
    AddDocTag(Tags::Meta);
  }

  virtual ~ReadImageInfo()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    
    //Create output parameters to store image informations
    AddParameter(ParameterType_Int,"sizex","Size X");
    GetParameterByKey("sizex")->SetRole(Role_Output);
    AddParameter(ParameterType_Int,"sizey","Size Y");
    GetParameterByKey("sizey")->SetRole(Role_Output);
    
    AddParameter(ParameterType_Int,"spacingx","Pixel Size X");
    GetParameterByKey("spacingx")->SetRole(Role_Output);
    AddParameter(ParameterType_Int,"spacingy","Pixel Size Y");
    GetParameterByKey("spacingy")->SetRole(Role_Output);
    AddParameter(ParameterType_Int,"numberbands","Number Of Bands");
    GetParameterByKey("numberbands")->SetRole(Role_Output);
    
    
    AddParameter(ParameterType_String,"sensor","Sensor id");
    GetParameterByKey("sensor")->SetRole(Role_Output);
    MandatoryOff("sensor");
   
    AddParameter(ParameterType_String,"id","Id of the image");
    GetParameterByKey("id")->SetRole(Role_Output);
    
    AddParameter(ParameterType_String,"time","Acquisition time");
    GetParameterByKey("time")->SetRole(Role_Output);
    
    AddParameter(ParameterType_Float,"ullat","Upper left lattitude");
    GetParameterByKey("ullat")->SetRole(Role_Output);
    SetDefaultParameterFloat("ullat", 0);
    AddParameter(ParameterType_Float,"ullon","Upper left longitude");
    GetParameterByKey("ullon")->SetRole(Role_Output);
    SetDefaultParameterFloat("ullon", 0);
    AddParameter(ParameterType_Float,"urlat","Upper right lattitude");
    GetParameterByKey("urlat")->SetRole(Role_Output);
    SetDefaultParameterFloat("urlat", 0);
    AddParameter(ParameterType_Float,"urlon","Upper right longitude");
    GetParameterByKey("urlon")->SetRole(Role_Output);
    SetDefaultParameterFloat("urlon", 0);
    AddParameter(ParameterType_Float,"lrlat","Lower right lattitude");
    GetParameterByKey("lrlat")->SetRole(Role_Output);
    SetDefaultParameterFloat("lrlat", 0);
    AddParameter(ParameterType_Float,"lrlon","Lower right longitude");
    GetParameterByKey("lrlon")->SetRole(Role_Output);
    SetDefaultParameterFloat("lrlon", 0);
    AddParameter(ParameterType_Float,"lllat","Lower left lattitude");
    GetParameterByKey("lllat")->SetRole(Role_Output);
    SetDefaultParameterFloat("lllat", 0);
    AddParameter(ParameterType_Float,"lllon","Lower left longitude");
    GetParameterByKey("lllon")->SetRole(Role_Output);
    SetDefaultParameterFloat("lllon", 0);
    
    AddParameter(ParameterType_String,"town","Main town near center of image");
    GetParameterByKey("town")->SetRole(Role_Output);
    
    AddParameter(ParameterType_String,"country","Country of the image");
    GetParameterByKey("country")->SetRole(Role_Output);

    AddParameter(ParameterType_Group, "rgb", "Default RGB Display");
    SetParameterDescription("rgb","This group of parameters allows to access to the default rgb composition.");
    GetParameterByKey("rgb")->SetRole(Role_Output);
    
    AddParameter(ParameterType_Int, "rgb.r", "Red Band");
    SetParameterDescription("rgb.r","Red band Number");
    SetDefaultParameterInt("rgb.r", 1);
    GetParameterByKey("rgb.r")->SetRole(Role_Output);
    
    AddParameter(ParameterType_Int, "rgb.g", "Green Band");
    SetParameterDescription("rgb.g","Green band Number");
    SetDefaultParameterInt("rgb.g", 2);
    GetParameterByKey("rgb.g")->SetRole(Role_Output);

    AddParameter(ParameterType_Int, "rgb.b", "Blue Band");
    SetParameterDescription("rgb.b","Blue band Number");
    SetDefaultParameterInt("rgb.b", 3);
    GetParameterByKey("rgb.b")->SetRole(Role_Output);

    AddParameter(ParameterType_String,"projectionref","Projection Coordinate System");
    GetParameterByKey("projectionref")->SetRole(Role_Output);
    
    AddParameter(ParameterType_String,"keywordlist","Image Keywordlist");
    GetParameterByKey("keywordlist")->SetRole(Role_Output);

    AddParameter(ParameterType_Group, "gcp", "Ground Control Points informations");
    SetParameterDescription("gcp","This group of parameters allows to access to the GCPs informations.");
    GetParameterByKey("gcp")->SetRole(Role_Output);

    AddParameter(ParameterType_Int, "gcp.count", "GCPs Number");
    SetParameterDescription("gcp.count","Number of GCPs");
    SetDefaultParameterInt("gcp.count", 0);
    GetParameterByKey("gcp.count")->SetRole(Role_Output);

    AddParameter(ParameterType_String,"gcp.proj","GCP Projection System");
    GetParameterByKey("gcp.proj")->SetRole(Role_Output);

    AddParameter(ParameterType_StringList,"gcp.ids","GCPs Id");
    //GetParameterByKey("gcp.ids")->SetRole(Role_Output);
    AddParameter(ParameterType_StringList,"gcp.info","GCPs Info");
    //GetParameterByKey("gcp.info")->SetRole(Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    try
      {
      FloatVectorImageType::Pointer inImage = GetParameterImage("in");
      // Read informations
      typedef otb::ImageMetadataInterfaceBase ImageMetadataInterfaceType;
      ImageMetadataInterfaceType::Pointer metadataInterface = ImageMetadataInterfaceFactory::CreateIMI(inImage->GetMetaDataDictionary());

      //Get image size
      SetParameterInt("sizex", inImage->GetLargestPossibleRegion().GetSize()[0]);
      SetParameterInt("sizey", inImage->GetLargestPossibleRegion().GetSize()[1]);

      //Get image spacing
      SetParameterInt("spacingx", inImage->GetSpacing()[0]);
      SetParameterInt("spacingy", inImage->GetSpacing()[1]);
    
      SetParameterInt("numberbands", inImage->GetNumberOfComponentsPerPixel());

      //std::cout << "metadata " << metadataInterface << std::endl;
      SetParameterString("sensor", metadataInterface->GetSensorID());
      SetParameterString("id", metadataInterface->GetImageKeywordlist().GetMetadataByKey("image_id"));
      SetParameterString("projectionref", metadataInterface->GetProjectionRef());
      
      // Format acquisition time
      itk::OStringStream osstime;
      osstime<<metadataInterface->GetYear()<<"-";
      if(metadataInterface->GetMonth()<10)
        osstime<<"0";
      osstime<<metadataInterface->GetMonth()<<"-";
      if(metadataInterface->GetDay()<10)
        osstime<<"0";
      osstime<<metadataInterface->GetDay()<<"T";
      if(metadataInterface->GetHour()<10)
        osstime<<"0";
      osstime<<metadataInterface->GetHour()<<":";
      if(metadataInterface->GetMinute()<10)
        osstime<<"0";
      osstime<<metadataInterface->GetMinute();
      osstime<<":00";
      SetParameterString("time", osstime.str());

      itk::OStringStream osskeywordlist;
      osskeywordlist<<metadataInterface->GetImageKeywordlist() << std::endl;
      SetParameterString("keywordlist", osskeywordlist.str());

      double ullat = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ul_lat").c_str());
      double ullon = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ul_lon").c_str());
      double urlat = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ur_lat").c_str());
      double urlon = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ur_lon").c_str());
      double lrlat = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("lr_lat").c_str());
      double lrlon = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("lr_lon").c_str());
      double lllat = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ll_lat").c_str());
      double lllon = atof(metadataInterface->GetImageKeywordlist().GetMetadataByKey("ll_lon").c_str());
      
      SetParameterInt("rgb.r", metadataInterface->GetDefaultDisplay()[0]);
      SetParameterInt("rgb.g", metadataInterface->GetDefaultDisplay()[1]);
      SetParameterInt("rgb.b", metadataInterface->GetDefaultDisplay()[2]);
 
      SetParameterInt("gcp.count", metadataInterface->GetGCPCount());
      SetParameterString("gcp.proj", metadataInterface->GetGCPProjection());
      
      std::vector<std::string> gcp_ids;
      std::vector<std::string> gcp_infos;

      for(int gcpIdx = 0; gcpIdx  < GetParameterInt("gcp.count"); ++ gcpIdx)
        {
        gcp_ids.push_back(metadataInterface->GetGCPId(gcpIdx));
        gcp_infos.push_back(metadataInterface->GetGCPInfo(gcpIdx));
        }
      
      SetParameterStringList("gcp.ids", gcp_ids);
      SetParameterStringList("gcp.info", gcp_infos);
      
      // Retrieve footprint
      SetParameterFloat("ullat", ullat);
      SetParameterFloat("ullon", ullon);
      SetParameterFloat("urlat", urlat);
      SetParameterFloat("urlon", urlon);
      SetParameterFloat("lrlat", lrlat);
      SetParameterFloat("lrlon", lrlon);
      SetParameterFloat("lllat", lllat);
      SetParameterFloat("lllon", lllon);

      double centerlat = 0.25*(ullat+urlat+lrlat+lllat);
      double centerlon = 0.25*(ullon+urlon+lrlon+lllon);

      CoordinateToName::Pointer coord2name = CoordinateToName::New();
      coord2name->SetLat(centerlat);
      coord2name->SetLon(centerlon);
      coord2name->Evaluate();

      SetParameterString("town", coord2name->GetPlaceName());
      SetParameterString("country", coord2name->GetCountryName());
      }
    catch ( itk::ExceptionObject & err )
      {
      //Do nothing at all
      }
    // Show result
    otbAppLogINFO( << "Image informations:" << std::endl);
    //otbAppLogINFO( << "General: ");

    typedef std::vector< std::pair<std::string, std::string> > ParametersListType;
    std::vector< std::pair<std::string, std::string> > appList = GetOutputParametersSumUp();
    
    for (ParametersListType::const_iterator it = appList.begin(); it != appList.end(); ++it)
      {
      otbAppLogINFO( << it->first << "   " << it->second);
      }
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ReadImageInfo)

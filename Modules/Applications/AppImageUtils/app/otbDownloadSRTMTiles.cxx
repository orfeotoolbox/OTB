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
#include "otbGenericRSTransform.h"
#include "otbCurlHelper.h"

namespace otb
{

enum
{
  Mode_Download,
  Mode_List
};

const std::string SRTMServerPath = "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/";

const char* USGSContinentDir[] = {"Africa",
                                  "Australia",
                                  "Eurasia",
                                  "Islands",
                                  "North_America",
                                  "South_America"};

const std::vector<std::string> USGSContinentList(USGSContinentDir, USGSContinentDir + sizeof(USGSContinentDir)/sizeof(USGSContinentDir[0]));

const std::string HGTExtension = ".hgt.zip";
const std::string HGTExtensionSimulation = ".hgt";
namespace Wrapper
{

class DownloadSRTMTiles : public Application
{
public:
  /** Standard class typedefs. */
  typedef DownloadSRTMTiles             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::GenericRSTransform<>     RSTransformType;

  typedef FloatVectorImageType::IndexType    IndexType;
  typedef FloatVectorImageType::PointType    PointType;
  typedef FloatVectorImageType::SizeType     SizeType;
  typedef FloatVectorImageType::SpacingType  SpacingType;
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(DownloadSRTMTiles, otb::Application);

private:

  int m_Mode;

  bool SRTMTileExists(const std::string & url) const
  {
    switch ( m_Mode )
      {
      case Mode_Download:
      {
      CurlHelper::Pointer curl = CurlHelper::New();
      curl->SetTimeout(0);
      return !curl->IsCurlReturnHttpError(url);
      }
      break;
      case Mode_List:
      {
      return itksys::SystemTools::FileExists(url.c_str());
      }
      default :
      break;
      }
    return false;
  }
  void DoInit()
  {
    SetName("DownloadSRTMTiles");
    SetDescription("Download or list SRTM tiles related to a set of images");

    // Documentation
    SetDocName("Download or list SRTM tiles related to a set of images");
    SetDocLongDescription("This application allows selecting the appropriate SRTM tiles that covers a list of images. It builds a list of the required tiles. Two modes are available: the first one download those tiles from the USGS SRTM3 website (http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/), the second one list those tiles in a local directory. In both cases, you need to indicate the directory in which directory  tiles will be download or the location of local SRTM files.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("Utilities");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images on which you want to determine corresponding SRTM tiles.");

    // UserDefined values
    AddParameter(ParameterType_Choice, "mode", "Download/List corresponding SRTM tiles.");
    //MandatoryOn("mode");

    AddChoice("mode.download", "Download");
    SetParameterDescription("mode.download","Download corresponding tiles on USGE server.");

    AddParameter(ParameterType_Directory, "mode.download.outdir", "Output directory");
    SetParameterDescription("mode.download.outdir", "Directory where zipped tiles will be save. You'll need to unzip all tile files before using them in your application.");

    AddChoice("mode.list", "List tiles");
    SetParameterDescription("mode.list","List tiles in an existing local directory.");

    AddParameter(ParameterType_Directory, "mode.list.indir", "Input directory");
    SetParameterDescription("mode.list.indir", "Input directory where SRTM tiles can are located.");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("mode", "list");
    SetDocExampleParameterValue("mode.list.indir", "/home/user/srtm_dir/");

  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }


  void DoExecute()
  {

    //Get the mode
    m_Mode = GetParameterInt("mode");

    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    if( inList->Size() == 0 )
      {
      itkExceptionMacro("No input Image set...");
      }

    inList->GetNthElement(0)->UpdateOutputInformation();

    std::set<std::string> listTilesVector;
    std::set<std::string> tiles;

    for( unsigned int i=0; i<inList->Size(); i++ )
      {
      FloatVectorImageType::Pointer inImage = inList->GetNthElement(i);
      inImage->UpdateOutputInformation();

      RSTransformType::Pointer rsTransformToWGS84 = RSTransformType::New();
       rsTransformToWGS84->SetInputKeywordList(inImage->GetImageKeywordlist());
       rsTransformToWGS84->SetInputProjectionRef(inImage->GetProjectionRef());
       rsTransformToWGS84->SetOutputProjectionRef(static_cast<std::string> (otb::GeoInformationConversion::ToWKT(4326)));

       rsTransformToWGS84->InstanciateTransform();

       const SizeType size = inImage->GetLargestPossibleRegion().GetSize();
       const PointType origin=inImage->GetOrigin();
       const SpacingType spacing=inImage->GetSpacing();
       PointType upperLeft;
       upperLeft[0] = origin[0];
       upperLeft[1] = origin[1];
       PointType upperLeftWGS84 = rsTransformToWGS84->TransformPoint(upperLeft);
       otbAppLogDEBUG(<< "upperLeftWGS84 " << upperLeftWGS84);

       PointType upperRight;
       upperRight[0] = origin[0]+ (size[0] - 1) * spacing[0];
       upperRight[1] = origin[1];
       PointType upperRightWGS84 = rsTransformToWGS84->TransformPoint(upperRight);
       otbAppLogDEBUG(<< "upperRightWGS84 " << upperRightWGS84);

       PointType lowerLeft;
       lowerLeft[0] = origin[0];
       lowerLeft[1] = origin[1]+ (size[1] - 1) * spacing[1];
       PointType lowerLeftWGS84 = rsTransformToWGS84->TransformPoint(lowerLeft);
       otbAppLogDEBUG(<< "lowerLeftWGS84 " << lowerLeftWGS84);

       PointType lowerRight;
       lowerRight[0] = origin[0] + (size[0] - 1) * spacing[0];
       lowerRight[1] = origin[1] + (size[1] - 1) * spacing[1];
       PointType lowerRightWGS84 = rsTransformToWGS84->TransformPoint(lowerRight);
       otbAppLogDEBUG(<< "lowerRightWGS84 " << lowerRightWGS84);

      // the iterator constructor can also be used to construct from arrays:
      const double Longitude[] = {upperLeftWGS84[0],upperRightWGS84[0],lowerLeftWGS84[0],lowerRightWGS84[0]};
      const double Latitude[] = {upperLeftWGS84[1],upperRightWGS84[1],lowerLeftWGS84[1],lowerRightWGS84[1]};

      std::vector<double> vecLong (Longitude, Longitude + sizeof(Longitude) / sizeof(double) );
      std::vector<double> vecLat (Latitude, Latitude + sizeof(Latitude) / sizeof(double) );

      std::vector<double> absVecLong;
      absVecLong.resize(vecLong.size());                         // allocate space

      std::vector<double> absVecLat;
      absVecLat.resize(vecLat.size());                         // allocate space

      std::vector<double>::iterator AbsLongit=absVecLong.begin();

      for (std::vector<double>::const_iterator it=vecLong.begin(); it!=vecLong.end(); ++it)
        {
        *AbsLongit = vcl_abs(*it);
        ++AbsLongit;
        }

      std::vector<double>::iterator AbsLatit=absVecLat.begin();

      for (std::vector<double>::const_iterator it=vecLat.begin(); it!=vecLat.end(); ++it)
        {
        *AbsLatit = vcl_abs(*it);
        ++AbsLatit;
        }

      const unsigned int distMinLong =
        std::distance(absVecLong.begin(), min_element (absVecLong.begin(),absVecLong.end()));
      const unsigned int distMaxLong =
        std::distance(absVecLong.begin(), max_element (absVecLong.begin(),absVecLong.end()));

      const unsigned int distMinLat =
        std::distance(absVecLat.begin(), min_element (absVecLat.begin(),absVecLat.end()));
      const unsigned int distMaxLat =
        std::distance(absVecLat.begin(), max_element (absVecLat.begin(),absVecLat.end()));

      //find corresponding tiled  for initialization
      //FIXME recode this properly

      int floorMinLong = 0;
      int floorMaxLong = 0;
      int floorMinLat = 0;
      int floorMaxLat = 0;

      if (vecLong.at(distMinLong) <= vecLong.at(distMaxLong))
        {
        floorMinLong = std::floor(vecLong.at(distMinLong));
        floorMaxLong = std::floor(vecLong.at(distMaxLong));
        }
      else
        {
        floorMinLong = std::floor(vecLong.at(distMaxLong));
        floorMaxLong = std::floor(vecLong.at(distMinLong));
        }

      if (vecLat.at(distMinLat) <= vecLat.at(distMaxLat))
        {
        floorMinLat = std::floor(vecLat.at(distMinLat));
        floorMaxLat = std::floor(vecLat.at(distMaxLat));
        }
      else
        {
        floorMinLat = std::floor(vecLat.at(distMaxLat));
        floorMaxLat = std::floor(vecLat.at(distMinLat));
        }

      //Construct SRTM tile filename based on min/max lat/long
      for (int k = floorMinLat; k <= floorMaxLat; ++k)
        {
        for (int j = floorMinLong; j <= floorMaxLong; ++j)
          {
          std::ostringstream ossOutput;
          if (k < 0)
            {
            ossOutput << "S";
            }
          else
            {
            ossOutput << "N";
            }

          if (vcl_abs(k) <= 9)
            {
            ossOutput << "0";
            }
          ossOutput << vcl_abs(k);

          if (j < 0)
            {
            ossOutput << "W";
            }
          else
            {
            ossOutput << "E";
            }
          if (vcl_abs(j) >= 10 && vcl_abs(j) < 100)
            {
            ossOutput << "0";
            }
          else if (vcl_abs(j) <= 9)
            {
            ossOutput << "00";
            }

          ossOutput << vcl_abs(j);

          tiles.insert(ossOutput.str());
          }
        }

      }

    //iterate over all tiles to build URLs
    for(std::set<std::string>::const_iterator it= tiles.begin(); it!=tiles.end(); ++it)
      {
      switch ( m_Mode )
        {
        case Mode_Download:
        {
        //Build URL
        bool findURL = false;
        std::string url;
        for(std::vector<std::string>::const_iterator contIt= USGSContinentList.begin(); contIt!=USGSContinentList.end(); ++contIt)
          {
          std::ostringstream urlStream;
          CurlHelper::Pointer curl = CurlHelper::New();
          curl->SetTimeout(0);
          urlStream << SRTMServerPath;
          urlStream << *contIt;
          urlStream << "/";
          urlStream << *it;
          urlStream << HGTExtension;

          url = urlStream.str();

          if (!curl->IsCurlReturnHttpError( urlStream.str()))
            {
            findURL = true;
            break;
            }
          else
            {
            //try down casing the url
            std::string lowerIt = *it;
            std::transform(it->begin(), it->end(), lowerIt.begin(), ::tolower);

            urlStream.clear();
            urlStream << SRTMServerPath;
            urlStream << *contIt;
            urlStream << "/";
            urlStream << lowerIt;
            urlStream << HGTExtension;
            if (!curl->IsCurlReturnHttpError( urlStream.str()))
              {
              tiles.erase(*it);
              tiles.insert(lowerIt);
              //urls.insert(urlStream.str());
              findURL = true;
              break;
              }
            else
              {
              //upcase all
              std::string upperIt = *it;
              std::transform(it->begin(), it->end(), upperIt.begin(), ::toupper);

              urlStream.clear();
              urlStream << SRTMServerPath;
              urlStream << *contIt;
              urlStream << "/";
              urlStream << upperIt;
              urlStream << HGTExtension;

              if (!curl->IsCurlReturnHttpError( urlStream.str()))
                {
                tiles.erase(*it);
                tiles.insert(upperIt);
                //urls.insert(urlStream.str());
                findURL = true;
                break;
                }
              }
            }

          }

        if (!findURL)
          {
          itkExceptionMacro(<< url  <<" not found!");
          }

        otbAppLogINFO(<< "Found Tile on USGS server at URL: " << url);
        // TODO use the RetrieveUrlInMemory? In this case need to adapt the
        // timeout

        const std::string outDir = GetParameterString("mode.download.outdir");

        std::ostringstream oss;
        oss<<outDir<<"/foo";

        if( itksys::SystemTools::Touch( oss.str().c_str(), true ) == false )
          {
          itkExceptionMacro( "Error, no write permission in given output directory "<< outDir <<".");
          }
        else
          {
          itksys::SystemTools::RemoveFile( oss.str().c_str() );
          }

        CurlHelper::Pointer curlReq = CurlHelper::New();
        curlReq->SetTimeout(0);
        curlReq->RetrieveFile(url, GetParameterString("mode.download.outdir") + "/" + *it + HGTExtension);
        //TODO unzip here (can do this in memory?)
        }
        break;
        case Mode_List:
        {
        bool findURL = false;
        std::ostringstream listStream;
        listStream << "Corresponding SRTM tiles: ";
        listStream << GetParameterString("mode.list.indir") + "/";
        if ( this->SRTMTileExists(GetParameterString("mode.list.indir") + "/" + *it + HGTExtensionSimulation) )
          {
          listStream << *it + HGTExtensionSimulation << " ";
          findURL = true;
          }
        else
          {
          //try down casing the url
          std::string lowerIt = *it;
          std::transform(it->begin(), it->end(), lowerIt.begin(), ::tolower);

          if ( this->SRTMTileExists(GetParameterString("mode.list.indir") + "/" + lowerIt + HGTExtensionSimulation) )
            {
            tiles.erase(*it);
            tiles.insert(lowerIt);
            findURL = true;
            }
          else
            {
            //upcase all
            std::string upperIt = *it;
            std::transform(it->begin(), it->end(), upperIt.begin(), ::toupper);

            if (this->SRTMTileExists(GetParameterString("mode.list.indir") + "/" + lowerIt + HGTExtensionSimulation) )
              {
              tiles.erase(*it);
              tiles.insert(upperIt);
              findURL = true;
              }
            }
          }

        if (!findURL)
          {
          itkExceptionMacro(<< "Tile " <<  *it + HGTExtensionSimulation  <<" not found in " << GetParameterString("mode.list.indir") << " !");
          }
        otbAppLogINFO( << listStream.str());
        }
        break;
        }
      }
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DownloadSRTMTiles)

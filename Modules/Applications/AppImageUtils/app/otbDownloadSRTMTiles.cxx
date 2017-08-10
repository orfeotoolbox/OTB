/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

const std::string HGTZIPExtension = ".hgt.zip";
const std::string HGTExtension = ".hgt";
const std::string ZIPExtension = ".zip";
#ifdef _WIN32
const char Sep = '\\';
#else
const char Sep = '/';
#endif
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

  typedef enum {
    SRTM_Unknown = 0,
    SRTM_Africa,
    SRTM_Australia,
    SRTM_Eurasia,
    SRTM_Islands,
    SRTM_North_America,
    SRTM_South_America
  } SRTMContinent;

  typedef struct {
    int Lon : 9;
    int Lat : 8;
    unsigned int Conti : 3;
  } SRTMTileId;

  class TileIdComparator
    {
    public:
      bool operator() (const SRTMTileId & a, const SRTMTileId & b) const
        {
        if (a.Lat < b.Lat)
          return true;
        if (a.Lat == b.Lat && a.Lon < b.Lon)
          return true;
        return false;
        }
    };

  typedef std::set<SRTMTileId,TileIdComparator> SRTMTileSet;

  DownloadSRTMTiles()
    {
    static SRTMTileId staticTileList[] = {
    {0,72,2},{0,73,2},{0,97,2},{0,98,2},{0,99,2}
      };
    // initialize the default set of SRTM tiles
    m_SRTMTileList = SRTMTileSet(staticTileList,staticTileList+5);
    }

  std::string SRTMIdToName(const SRTMTileId &id) const
    {
    char name[8];
    sprintf(name,"%c%02d%c%03d",
      (id.Lat < 0 ? 'S' : 'N'),
      vcl_abs(id.Lat),
      (id.Lon < 0 ? 'W' : 'E'),
      vcl_abs(id.Lon));
    return std::string(name);
    }

  std::string SRTMIdToContinent(const SRTMTileId &id) const
    {
    switch (id.Conti)
      {
      case SRTM_Africa:
        return std::string("Africa");
      case SRTM_Australia:
        return std::string("Australia");
      case SRTM_Eurasia:
        return std::string("Eurasia");
      case SRTM_Islands:
        return std::string("Islands");
      case SRTM_North_America:
        return std::string("North_America");
      case SRTM_South_America:
        return std::string("South_America");
      default:
        break;
      }
    return std::string();
    }

  bool SRTMTileExists(const SRTMTileId & tile, std::string & continent) const
  {
    SRTMTileSet::const_iterator pos = m_SRTMTileList.find(tile);
    if (pos != m_SRTMTileList.end())
      {
      continent = this->SRTMIdToContinent(*pos);
      return true;
      }
    return false;
  }

  bool SRTMTileDownloaded(const std::string & name, const std::string & tileDir) const
  {
    std::string path(tileDir);
    if (!path.empty() && path.back() != Sep)
      {
      path += Sep;
      }
    // try different filenames
    std::string filepath(path+name+HGTExtension);
    bool exists = itksys::SystemTools::FileExists(filepath.c_str());
    if (!exists)
      {
      filepath += ZIPExtension;
      exists = itksys::SystemTools::FileExists(filepath.c_str());
      }

    if (!exists)
      {
      std::string lowerName(name);
      std::transform(name.begin(), name.end(), lowerName.begin(), ::tolower);
      filepath = path + lowerName + HGTExtension;
      exists = itksys::SystemTools::FileExists(filepath.c_str());
      if (!exists)
        {
        filepath += ZIPExtension;
        exists = itksys::SystemTools::FileExists(filepath.c_str());
        }
      }
    return exists;
  }

  bool CheckPermissions(const std::string &dir) const
    {
    std::string path(dir);
    if (!path.empty() && path.back() != Sep)
      {
      path += Sep;
      }
    path += "foo";
    if( itksys::SystemTools::Touch( path.c_str(), true ) )
      {
      itksys::SystemTools::RemoveFile( path.c_str() );
      }
    else
      {
      return false;
      }
    return true;
    }

  void DoInit() ITK_OVERRIDE
  {
    SetName("DownloadSRTMTiles");
    SetDescription("Download or list SRTM tiles related to a set of images");

    // Documentation
    SetDocName("Download or list SRTM tiles related to a set of images");
    SetDocLongDescription("This application allows selecting the appropriate SRTM tiles that covers a list of images. It builds a list of the required tiles. Two modes are available: the first one download those tiles from the USGS SRTM3 website (http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/), the second one list those tiles in a local directory. In both cases, you need to indicate the directory in which directory  tiles will be download or the location of local SRTM files.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);
	  AddDocTag("Utilities");

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images on which you want to determine corresponding SRTM tiles.");

    AddParameter(ParameterType_Directory, "tiledir", "Tiles directory");
    SetParameterDescription("tiledir", "Directory where SRTM tiles "
      "are stored. In download mode, the zipped archives will be downloaded to "
      "this directory. You'll need to unzip all tile files before using them in"
      " your application. In any case, this directory will be inspected to "
      "check which tiles are already downloaded.");

    // UserDefined values
    AddParameter(ParameterType_Choice, "mode", "Download/List corresponding SRTM tiles.");
    //MandatoryOn("mode");

    AddChoice("mode.download", "Download");
    SetParameterDescription("mode.download","Download corresponding tiles on USGE server.");

    AddChoice("mode.list", "List tiles");
    SetParameterDescription("mode.list","List tiles in an existing local directory.");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("mode", "list");
    SetDocExampleParameterValue("tiledir", "/home/user/srtm_dir/");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }


  void DoExecute() ITK_OVERRIDE
  {
    //Get the mode
    int mode = GetParameterInt("mode");

    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    const std::string tileDir = this->GetParameterString("tiledir");

    if( inList->Size() == 0 )
      {
      itkExceptionMacro("No input Image set...");
      }

    // Should already come with Output information updated
    //inList->GetNthElement(0)->UpdateOutputInformation();

    SRTMTileSet tiles;
    for( unsigned int i=0; i<inList->Size(); i++ )
      {
      FloatVectorImageType::Pointer inImage = inList->GetNthElement(i);
      // Should already come with Output information updated
      //inImage->UpdateOutputInformation();

      RSTransformType::Pointer rsTransformToWGS84 = RSTransformType::New();
      rsTransformToWGS84->SetInputKeywordList(inImage->GetImageKeywordlist());
      rsTransformToWGS84->SetInputProjectionRef(inImage->GetProjectionRef());
      rsTransformToWGS84->SetOutputProjectionRef(static_cast<std::string> (otb::GeoInformationConversion::ToWKT(4326)));
      
      rsTransformToWGS84->InstantiateTransform();
      
      const SizeType size = inImage->GetLargestPossibleRegion().GetSize();
      const IndexType start = inImage->GetLargestPossibleRegion().GetIndex();
      PointType tmpPoint;
      itk::ContinuousIndex<double,2> index(start);
      index[0] += -0.5;
      index[1] += -0.5;
      inImage->TransformContinuousIndexToPhysicalPoint(index,tmpPoint);
      PointType ul = rsTransformToWGS84->TransformPoint(tmpPoint);
      index[0] += size[0];
      inImage->TransformContinuousIndexToPhysicalPoint(index,tmpPoint);
      PointType ur = rsTransformToWGS84->TransformPoint(tmpPoint);
      index[1] += size[1];
      inImage->TransformContinuousIndexToPhysicalPoint(index,tmpPoint);
      PointType lr = rsTransformToWGS84->TransformPoint(tmpPoint);
      index[0] -= (double)size[0];
      inImage->TransformContinuousIndexToPhysicalPoint(index,tmpPoint);
      PointType ll = rsTransformToWGS84->TransformPoint(tmpPoint);
      
      int floorMinLong = std::floor(std::min(
        std::min(ul[0],ur[0]),
        std::min(ll[0],lr[0])));
      int floorMaxLong = std::floor(std::max(
        std::max(ul[0],ur[0]),
        std::max(ll[0],lr[0])));
      int floorMinLat = std::floor(std::min(
        std::min(ul[1],ur[1]),
        std::min(ll[1],lr[1])));
      int floorMaxLat = std::floor(std::max(
        std::max(ul[1],ur[1]),
        std::max(ll[1],lr[1])));

      otbAppLogDEBUG(<< "upperLeftWGS84 "  << ul);
      otbAppLogDEBUG(<< "upperRightWGS84 " << ur);
      otbAppLogDEBUG(<< "lowerLeftWGS84 "  << ll);
      otbAppLogDEBUG(<< "lowerRightWGS84 " << lr);

      //Construct SRTM tile filename based on min/max lat/long
      for (int k = floorMinLat; k <= floorMaxLat; ++k)
        {
        for (int j = floorMinLong; j <= floorMaxLong; ++j)
          {
          tiles.insert(SRTMTileId({j,k,0}));
          }
        }
      }

    if (tiles.empty())
      {
      otbAppLogWARNING(<< "No tile found for the given image(s)!");
      return;
      }
    otbAppLogINFO(<<"Input image(s) cover "<< tiles.size() << " potential tiles.");

    //iterate over all tiles to build URLs
    std::vector<std::string> nonSRTMTiles;
    std::vector<std::string> localTiles;
    std::vector<std::string> missingTiles;
    std::vector<std::string> continentList;
    for(SRTMTileSet::iterator it= tiles.begin(); it!=tiles.end(); ++it)
      {
      std::string curName(this->SRTMIdToName(*it));
      std::string continent;
      // Check 1 : does the tile exists in SRTM ? If yes, get the continent
      if (SRTMTileExists(*it,continent))
        {
        // Check 2 : is the tile already downloaded
        if (SRTMTileDownloaded(curName,tileDir))
          {
          localTiles.push_back(curName);
          }
        else
          {
          missingTiles.push_back(curName);
          continentList.push_back(continent);
          }
        }
      else
        {
        nonSRTMTiles.push_back(curName);
        }
      }

    // If download mode : try to get missing tiles
    int srtmHitRatio = ( 100 * (tiles.size() - nonSRTMTiles.size() ) ) /
      tiles.size();
    otbAppLogINFO(<< "Percentage of tiles present in SRTM: " << srtmHitRatio);
    if (mode == Mode_List)
      {
      std::ostringstream oss;
      for (unsigned int i=0 ; i<nonSRTMTiles.size() ; ++i)
        {
        oss << nonSRTMTiles[i] << " ";
        }
      otbAppLogINFO(<< "Tiles not present in SRTM: " << oss.str());
      }
    otbAppLogINFO(<< "Number of tiles already available: "<< localTiles.size());
    if (mode == Mode_List)
      {
      std::ostringstream oss;
      for (unsigned int i=0 ; i<localTiles.size() ; ++i)
        {
        oss << localTiles[i] << " ";
        }
      otbAppLogINFO(<< "  --> " << oss.str());
      }
    otbAppLogINFO(<< "Number of tiles to be downloaded: "<< missingTiles.size());
    if (mode == Mode_List)
      {
      std::ostringstream oss;
      for (unsigned int i=0 ; i<missingTiles.size() ; ++i)
        {
        oss << missingTiles[i] << " ";
        }
      otbAppLogINFO(<< "  --> " << oss.str());
      }
    if (mode == Mode_Download)
      {
      // Check permissions first
      if ( !CheckPermissions(tileDir) )
        {
        otbAppLogFATAL(<< "Can't write into directory : '"<< tileDir <<"'");
        }
      std::vector<std::string>::const_iterator it,itConti;
      CurlHelper::Pointer curl = CurlHelper::New();
      curl->SetTimeout(0);
      std::string request;
      for (it=missingTiles.begin(), itConti=continentList.begin() ;
           it!=missingTiles.end() && itConti!=continentList.end() ;
           ++it, ++itConti)
        {
        otbAppLogINFO(<< " - downloading tile "<<*it << " ...");
        request = SRTMServerPath + *itConti + "/" + *it + HGTZIPExtension;
        if (curl->IsCurlReturnHttpError( request ))
          {
          otbAppLogWARNING(<< "Can't access tile : "<< request);
          continue;
          }
        std::string outputFile(tileDir);
        if (!outputFile.empty() && outputFile.back() != Sep)
          {
          outputFile += Sep;
          }
        outputFile += *it + HGTZIPExtension;
        curl->RetrieveFile(request, outputFile);
        }
      }
  }

  SRTMTileSet m_SRTMTileList;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DownloadSRTMTiles)

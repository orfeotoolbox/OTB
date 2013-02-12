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

namespace otb
{

enum
{
  Mode_Download,
  Mode_Simulate
};

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

  typedef FloatVectorImageType::IndexType IndexType;
  typedef FloatVectorImageType::PointType PointType;
  typedef FloatVectorImageType::SizeType  SizeType;
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(DownloadSRTMTiles, otb::Application);

private:

  void DoInit()
  {
    SetName("DownloadSRTMTiles");
    SetDescription("Download or list SRTM tiles related to images");

    // Documentation
    SetDocName("Download or list SRTM tiles related to images");
    SetDocLongDescription("Download or list SRTM tiles related to images...");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("Utilities");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImageList,  "il",   "Input images list");
    SetParameterDescription("il", "The list of images on which you want to determine corresponding SRTM tiles.");

    // UserDefined values
    AddParameter(ParameterType_Choice, "mode", "download or simulate");
    MandatoryOn("mode");

    AddChoice("mode.download", "Download");
    SetParameterDescription("mode.download","Download.");

    AddParameter(ParameterType_OutputFilename, "mode.download.outdir", "ouput dir");
    SetParameterDescription("mode.download.outdir", "out dir");

    AddChoice("mode.simulate", "List tiles");
    SetParameterDescription("mode.simulate","List tiles");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("mode", "simulate");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }


  void DoExecute()
  {
    // Get the input image list
    FloatVectorImageListType::Pointer inList = this->GetParameterImageList("il");

    if( inList->Size() == 0 )
      {
      itkExceptionMacro("No input Image set...");
      }

    inList->GetNthElement(0)->UpdateOutputInformation();

    std::set<std::string> simulateTilesVector;

    for( unsigned int i=0; i<inList->Size(); i++ )
      {
      FloatVectorImageType::Pointer inImage = inList->GetNthElement(i);
      inImage->UpdateOutputInformation();

      RSTransformType::Pointer rsTransformToWGS84 = RSTransformType::New();
      rsTransformToWGS84->SetInputKeywordList(inImage->GetImageKeywordlist());
      rsTransformToWGS84->SetInputProjectionRef(inImage->GetProjectionRef());

      rsTransformToWGS84->InstanciateTransform();

      const SizeType size = inImage->GetLargestPossibleRegion().GetSize();

      PointType upperLeft;
      upperLeft[0] = 0;
      upperLeft[1] = 0;
      PointType upperLeftWGS84 = rsTransformToWGS84->TransformPoint(upperLeft);
      otbAppLogINFO(<< "upperLeftWGS84 " << upperLeftWGS84);

      PointType upperRight;
      upperRight[0] = size[0] - 1;
      upperRight[1] = 0;
      PointType upperRightWGS84 = rsTransformToWGS84->TransformPoint(upperRight);
      otbAppLogINFO(<< "upperRightWGS84 " << upperRightWGS84);

      PointType lowerLeft;
      lowerLeft[0] = 0;
      lowerLeft[1] = size[1] - 1;
      PointType lowerLeftWGS84 = rsTransformToWGS84->TransformPoint(lowerLeft);
      otbAppLogINFO(<< "lowerLeftWGS84 " << lowerLeftWGS84);

      PointType lowerRight;
      lowerRight[0] = size[0] - 1;
      lowerRight[1] = size[1] - 1;
      PointType lowerRightWGS84 = rsTransformToWGS84->TransformPoint(lowerRight);
      otbAppLogINFO(<< "lowerRightWGS84 " << lowerRightWGS84);

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

      std::set<std::string> tiles;

      //Construct SRTM tile filename based on min/max lat/long
      for (int i = floorMinLat; i <= floorMaxLat; ++i)
        {
        for (int j = floorMinLong; j <= floorMaxLong; ++j)
          {
          std::ostringstream ossOutput;
          if (i < 0)
            {
            ossOutput << "S";
            }
          else
            {
            ossOutput << "N";
            }

          if (vcl_abs(i) <= 9)
            {
            ossOutput << "0";
            }
          ossOutput << vcl_abs(i);

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

      const std::string SRTMServerPath = "http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/";

      const char* args[] = {"Africa",
                            "Australia",
                            "Eurasia",
                            "Islands",
                            "North_America",
                            "South_America"};

      const std::vector<std::string> continent(args, args + sizeof(args)/sizeof(args[0]));

      const std::string extension = ".hgt.zip";
      const std::string extensionSimulation = ".hgt";

      //iterate over all tiles to build URLs
      for(std::set<std::string>::const_iterator it= tiles.begin();it!=tiles.end();++it)
        {
        switch ( GetParameterInt("mode") )
          {
          case Mode_Download:
          {
          //Build URL
          bool findURL = false;
          std::string url;
          for(std::vector<std::string>::const_iterator contIt= continent.begin(); contIt!=continent.end(); ++contIt)
            {
            std::ostringstream urlStream;
            CurlHelper::Pointer curl = CurlHelper::New();
            curl->SetTimeout(0);
            urlStream << SRTMServerPath;
            urlStream << *contIt;
            urlStream << "/";
            urlStream << *it;
            urlStream << extension;

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
            urlStream << extension;
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
              urlStream << extension;

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
          // TODO use the RetrieveUrlInMemory? In this case need to adapt the timeout
          CurlHelper::Pointer curlReq = CurlHelper::New();
          curlReq->SetTimeout(0);
          curlReq->RetrieveFile(url, GetParameterString("mode.download.outdir") + "/" + *it + extension);
          //TODO unzip here (can do this in memory?)
          }
          break;
          case Mode_Simulate:
          {
          simulateTilesVector.insert(*it + extensionSimulation);
          }
          break;
          }
        }
      }
    switch ( GetParameterInt("mode") )
      {
      case Mode_Simulate:
      {
      std::ostringstream listStream;
      listStream << "Corresponding SRTM tiles: ";
      for(std::set<std::string>::const_iterator it= simulateTilesVector.begin();it!=simulateTilesVector.end();++it)
        {
        listStream << *it << " ";
        }
      otbAppLogINFO( << listStream.str());
      }
      break;
      }
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DownloadSRTMTiles)

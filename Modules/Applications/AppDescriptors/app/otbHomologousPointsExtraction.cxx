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
#include "otbWrapperApplicationFactory.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#ifdef OTB_USE_SIFTFAST
#include "otbSiftFastImageFilter.h"
#else
#include "otbImageToSIFTKeyPointSetFilter.h"
#endif
#include "otbImageToSURFKeyPointSetFilter.h"
#include "otbKeyPointSetsMatchingFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbKeyPointSetsMatchingFilter.h"

#include "otbGenericRSTransform.h"
#include "otbOGRDataSourceWrapper.h"
#include "ogrsf_frmts.h"

namespace otb
{
namespace Wrapper
{

class HomologousPointsExtraction: public Application
{
public:
  /** Standard class typedefs. */
  typedef HomologousPointsExtraction      Self;
  typedef Application                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef FloatVectorImageType::PixelType                    RealVectorType;
  typedef FloatVectorImageType::InternalPixelType            ValueType;
  typedef itk::PointSet<RealVectorType,2>                    PointSetType;

  // This is ugly ...
  #ifdef OTB_USE_SIFTFAST
  typedef SiftFastImageFilter<FloatImageType,PointSetType>   SiftFilterType;
  #else
  typedef ImageToSIFTKeyPointSetFilter<FloatImageType,PointSetType> SiftFilterType;
  #endif

  typedef ImageToSURFKeyPointSetFilter<FloatImageType,PointSetType> SurfFilterType;

  typedef itk::Statistics::EuclideanDistanceMetric<RealVectorType> DistanceType;
  typedef otb::KeyPointSetsMatchingFilter<PointSetType,
                                          DistanceType>      MatchingFilterType;

  typedef MatchingFilterType::LandmarkListType               LandmarkListType;
  typedef PointSetType::PointType                            PointType;
  typedef otb::MultiToMonoChannelExtractROI<ValueType,
                                            ValueType>      ExtractChannelFilterType;
  typedef otb::GenericRSTransform<>                         RSTransformType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(HomologousPointsExtraction, otb::Wrapper::Application);

private:
  void DoInit()
  {
    SetName("HomologousPointsExtraction");
    SetDocName("Homologous points extraction");
    SetDescription("Compute homologous points between images using keypoints");
    SetDocLongDescription("This application allows computing homologous points between images using keypoints. "
      " SIFT or SURF keypoints can be used and the band on which keypoints are computed can be set independantly for both images."
      " The application offers two modes :"
      " the first is the full mode where keypoints are extracted from the full extent of both images"
      " (please note that in this mode large image file are not supported). "
      "The second mode, called geobins, allows one to set-up spatial binning to get fewer points"
      " spread accross the entire image. "
      "In this mode, the corresponding spatial bin in the second image is estimated using geographical"
      " transform or sensor modelling, and is padded according to the user defined precision. Last, in"
      " both modes the application can filter matches whose colocalisation in first image exceed this precision. "
      "The elevation parameters are to deal more precisely with sensor modelling in case of sensor geometry data. "
      "The outvector option allows creating a vector file with segments corresponding to the localisation error between the matches."
      " It can be useful to assess the precision of a registration for instance."
      " The vector file is always reprojected to EPSG:4326 to allow display in a GIS."
      " This is done via reprojection or by applying the image sensor models.");
    // Documentation
    SetDocName("Homologous Points Extraction");
    SetDocLimitations("Full mode does not handle large images.");
    SetDocSeeAlso("RefineSensorModel");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in1", "Input Image 1");
    SetParameterDescription("in1"," First input image");

    AddParameter(ParameterType_Int,"band1","Input band 1");
    SetParameterDescription("band1","Index of the band from input image 1 to use for keypoints extraction");
    SetMinimumParameterIntValue("band1",1);
    SetDefaultParameterInt("band1",1);

    AddParameter(ParameterType_InputImage, "in2", "Input Image 2");
    SetParameterDescription("in2"," Second input image");

    AddParameter(ParameterType_Int,"band2","Input band 2");
    SetParameterDescription("band2","Index of the band from input image 1 to use for keypoints extraction");
    SetMinimumParameterIntValue("band2",1);
    SetDefaultParameterInt("band2",1);

    AddParameter(ParameterType_Choice,"algorithm","Keypoints detection algorithm");
    SetParameterDescription("algorithm","Choice of the detection algorithm to use");

    AddChoice("algorithm.surf","SURF algorithm");
    AddChoice("algorithm.sift","SIFT algorithm");

    AddParameter(ParameterType_Float,"threshold","Distance threshold for matching");
    SetParameterDescription("threshold","The distance threshold for matching.");
    SetMinimumParameterFloatValue("threshold",0.0);
    SetDefaultParameterFloat("threshold",0.6);

    AddParameter(ParameterType_Empty,"backmatching","Use back-matching to filter matches.");
    SetParameterDescription("backmatching","If set to true, matches should be consistent in both ways.");
    MandatoryOff("backmatching");
    DisableParameter("backmatching");

    AddParameter(ParameterType_Choice,"mode","Keypoints search mode");

    AddChoice("mode.full","Extract and match all keypoints (no streaming)");
    SetParameterDescription("mode.full","Extract and match all keypoints, loading both images entirely into memory");

    AddChoice("mode.geobins","Search keypoints in small spatial bins regularly spread accross first image");
    SetParameterDescription("mode.geobins","This method allows retrieving a set of tie points regulary spread accross image 1. Corresponding bins in image 2 are retrieved using sensor and geographical information if available. The first bin position takes into account the margin parameter. Bins are cropped to the largest image region shrinked by the margin parameter for both in1 and in2 images.");

    AddParameter(ParameterType_Int,"mode.geobins.binsize","Size of bin");
    SetParameterDescription("mode.geobins.binsize","Radius of the spatial bin in pixels");
    SetDefaultParameterInt("mode.geobins.binsize",256);
    SetMinimumParameterIntValue("mode.geobins.binsize",1);

    AddParameter(ParameterType_Int,"mode.geobins.binsizey","Size of bin (y direction)");
    SetParameterDescription("mode.geobins.binsizey","Radius of the spatial bin in pixels (y direction). If not set, the mode.geobins.binsize value is used.");
    SetMinimumParameterIntValue("mode.geobins.binsizey",1);
    MandatoryOff("mode.geobins.binsizey");

    AddParameter(ParameterType_Int,"mode.geobins.binstep","Steps between bins");
    SetParameterDescription("mode.geobins.binstep","Steps between bins in pixels");
    SetDefaultParameterInt("mode.geobins.binstep",256);
    SetMinimumParameterIntValue("mode.geobins.binstep",1);

    AddParameter(ParameterType_Int,"mode.geobins.binstepy","Steps between bins (y direction)");
    SetParameterDescription("mode.geobins.binstepy","Steps between bins in pixels (y direction). If not set, the mode.geobins.binstep value is used.");
    SetMinimumParameterIntValue("mode.geobins.binstepy",1);
    MandatoryOff("mode.geobins.binstepy");

    AddParameter(ParameterType_Int,"mode.geobins.margin","Margin from image border to start/end bins (in pixels)");
    SetParameterDescription("mode.geobins.margin","Margin from image border to start/end bins (in pixels)");
    SetMinimumParameterIntValue("mode.geobins.margin",0);
    SetDefaultParameterInt("mode.geobins.margin",10);

    AddParameter(ParameterType_Float,"precision","Estimated precision of the colocalisation function (in pixels).");
    SetParameterDescription("precision","Estimated precision of the colocalisation function in pixels");
    SetDefaultParameterFloat("precision",0.);

    AddParameter(ParameterType_Empty,"mfilter","Filter points according to geographical or sensor based colocalisation");
    SetParameterDescription("mfilter","If enabled, this option allows one to filter matches according to colocalisation from sensor or geographical information, using the given tolerancy expressed in pixels");

    AddParameter(ParameterType_Empty,"2wgs84","If enabled, points from second image will be exported in WGS84");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_OutputFilename,"out","Output file with tie points");
    SetParameterDescription("out","File containing the list of tie points");

    AddParameter(ParameterType_OutputFilename,"outvector","Output vector file with tie points");
    SetParameterDescription("outvector","File containing segments representing matches ");
    MandatoryOff("outvector");
    DisableParameter("outvector");

    // Doc example parameter settings
    SetDocExampleParameterValue("in1", "sensor_stereo_left.tif");
    SetDocExampleParameterValue("in2","sensor_stereo_right.tif");
    SetDocExampleParameterValue("mode","full");
    SetDocExampleParameterValue("out","homologous.txt");
  }

  void DoUpdateParameters()
  {

  }

  void Match(FloatImageType * im1, FloatImageType * im2, RSTransformType * rsTransform, RSTransformType * rsTransform1ToWGS84,RSTransformType * rsTransform2ToWGS84, std::ofstream & file, OGRMultiLineString * mls = NULL)
  {
    MatchingFilterType::Pointer matchingFilter = MatchingFilterType::New();

    if(GetParameterString("algorithm")=="sift")
      {
      otbAppLogINFO("Using SIFT points");
      SiftFilterType::Pointer sift1 = SiftFilterType::New();
      sift1->SetInput(im1);

      SiftFilterType::Pointer sift2 = SiftFilterType::New();
      sift2->SetInput(im2);

      sift1->Update();

      otbAppLogINFO("Found " << sift1->GetOutput()->GetNumberOfPoints()<<" sift points in image 1.");

      sift2->Update();

      otbAppLogINFO("Found " << sift2->GetOutput()->GetNumberOfPoints()<<" sift points in image 2.");

      matchingFilter->SetInput1(sift1->GetOutput());
      matchingFilter->SetInput2(sift2->GetOutput());
      }
    else if(GetParameterString("algorithm")=="surf")
      {
      SurfFilterType::Pointer surf1 = SurfFilterType::New();
      surf1->SetInput(im1);

      SurfFilterType::Pointer surf2 = SurfFilterType::New();
      surf2->SetInput(im2);

      otbAppLogINFO("Doing update");
      surf1->Update();

      otbAppLogINFO("Found " << surf1->GetOutput()->GetNumberOfPoints()<<" surf points in image 1.");

      surf2->Update();

      otbAppLogINFO("Found " << surf2->GetOutput()->GetNumberOfPoints()<<" surf points in image 2.");

      matchingFilter->SetInput1(surf1->GetOutput());
      matchingFilter->SetInput2(surf2->GetOutput());
      matchingFilter->SetDistanceThreshold(GetParameterFloat("threshold"));
      matchingFilter->SetUseBackMatching(IsParameterEnabled("backmatching"));
      }

    try
      {

      matchingFilter->Update();

      LandmarkListType::Pointer landmarks = matchingFilter->GetOutput();

      otbAppLogINFO("Found " << landmarks->Size() <<" homologous points.");

      unsigned int discarded  = 0;

      for (LandmarkListType::Iterator it = landmarks->Begin();
           it != landmarks->End(); ++it)
        {
        PointType point1 = it.Get()->GetPoint1();
        PointType point2 = it.Get()->GetPoint2();

        double error = 0;
        PointType pprime1,pprime2;

        bool filtered = false;

        if(IsParameterEnabled("mfilter"))
          {
          pprime1 = rsTransform->TransformPoint(point1);
          error = vcl_sqrt((point2[0]-pprime1[0])*(point2[0]-pprime1[0])+(point2[1]-pprime1[1])*(point2[1]-pprime1[1]));

          if(error>GetParameterFloat("precision")*vcl_sqrt(vcl_abs(im2->GetSpacing()[0]*im2->GetSpacing()[1])))
            {
            filtered = true;
            }
          }

        if(!filtered)
          {
          if(IsParameterEnabled("2wgs84"))
            {
            pprime2 = rsTransform2ToWGS84->TransformPoint(point2);

            file<<point1[0]<<"\t"<<point1[1]<<"\t"<<pprime2[0]<<"\t"<<pprime2[1]<<std::endl;
            }
          else
            {
            file<<point1[0]<<"\t"<<point1[1]<<"\t"<<point2[0]<<"\t"<<point2[1]<<std::endl;
            }

          if(mls)
            {
             pprime1 = rsTransform1ToWGS84->TransformPoint(point1);
             pprime2 = rsTransform2ToWGS84->TransformPoint(point2);

             OGRLineString ls;
             ls.addPoint(pprime1[0],pprime1[1]);
             ls.addPoint(pprime2[0],pprime2[1]);
             mls->addGeometry(&ls);
            }

          }
        else
          {
          ++discarded;
          }
        }
      otbAppLogINFO(<<discarded<<" points discarded");
      }
    catch(itk::ExceptionObject &)
      {
      // silent catch
      }
  }


  void DoExecute()
  {
    OGRMultiLineString mls;

    FloatVectorImageType::Pointer image1 = this->GetParameterImage("in1");
    FloatVectorImageType::Pointer image2 = this->GetParameterImage("in2");

    // Setting up RS Transform
    RSTransformType::Pointer rsTransform = RSTransformType::New();
    rsTransform->SetInputKeywordList(image1->GetImageKeywordlist());
    rsTransform->SetInputProjectionRef(image1->GetProjectionRef());
    rsTransform->SetOutputKeywordList(image2->GetImageKeywordlist());
    rsTransform->SetOutputProjectionRef(image2->GetProjectionRef());

    RSTransformType::Pointer rsTransform1ToWGS84 = RSTransformType::New();
    rsTransform1ToWGS84->SetInputKeywordList(image1->GetImageKeywordlist());
    rsTransform1ToWGS84->SetInputProjectionRef(image1->GetProjectionRef());

    RSTransformType::Pointer rsTransform2ToWGS84 = RSTransformType::New();
    rsTransform2ToWGS84->SetInputKeywordList(image2->GetImageKeywordlist());
    rsTransform2ToWGS84->SetInputProjectionRef(image2->GetProjectionRef());

    // Setting up output file
    std::ofstream file;
    file.open(GetParameterString("out").c_str());
    file<<std::fixed;
    file.precision(12);

    // Setting up channel extractors
    ExtractChannelFilterType::Pointer extractChannel1 = ExtractChannelFilterType::New();
    extractChannel1->SetInput(image1);
    extractChannel1->SetChannel(GetParameterInt("band1"));

    ExtractChannelFilterType::Pointer extractChannel2 = ExtractChannelFilterType::New();
    extractChannel2->SetInput(image2);
    extractChannel2->SetChannel(GetParameterInt("band2"));

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    rsTransform->InstanciateTransform();
    rsTransform1ToWGS84->InstanciateTransform();
    rsTransform2ToWGS84->InstanciateTransform();


    if(GetParameterString("mode")=="full")
      {
      // Launch detection on whole images
      Match(extractChannel1->GetOutput(),extractChannel2->GetOutput(),rsTransform,rsTransform1ToWGS84,rsTransform2ToWGS84,file,&mls);

      }
    else if(GetParameterString("mode")=="geobins")
      {
      // Compute binning on first image
      FloatImageType::SizeType size = image1->GetLargestPossibleRegion().GetSize();
      unsigned int bin_size_x = GetParameterInt("mode.geobins.binsize");
      unsigned int bin_size_y = bin_size_x;

      unsigned int image_border_margin = GetParameterInt("mode.geobins.margin");

      if(IsParameterEnabled("mode.geobins.binsizey"))
        {
        bin_size_y = GetParameterInt("mode.geobins.binsizey");
        }

      unsigned int bin_step_x = GetParameterInt("mode.geobins.binstep");
      unsigned int bin_step_y = bin_step_x;

      if(IsParameterEnabled("mode.geobins.binstepy"))
        {
        bin_step_y = GetParameterInt("mode.geobins.binstepy");
        }

      unsigned int nb_bins_x = static_cast<unsigned int>(vcl_ceil(static_cast<float>(size[0]-2*image_border_margin)/(bin_size_x + bin_step_x)));
      unsigned int nb_bins_y = static_cast<unsigned int>(vcl_ceil(static_cast<float>(size[1]-2*image_border_margin)/(bin_size_y + bin_step_y)));

      for(unsigned int i = 0; i<nb_bins_x; ++i)
        {
        for(unsigned int j = 0; j<nb_bins_y; ++j)
          {
          unsigned int startx = image_border_margin + i*(bin_size_x + bin_step_x);
          unsigned int starty = image_border_margin + j*(bin_size_y + bin_step_y);


          FloatImageType::SizeType size1;
          FloatImageType::IndexType index1;
          FloatImageType::RegionType region1;

          index1[0]=startx;
          index1[1]=starty;
          size1[0] = bin_size_x;
          size1[1] = bin_size_y;

          region1.SetIndex(index1);
          region1.SetSize(size1);

          FloatImageType::RegionType largestRegion = image1->GetLargestPossibleRegion();

          largestRegion.ShrinkByRadius(image_border_margin);
          region1.Crop(largestRegion);

          otbAppLogINFO("("<<i+1<<"/"<<nb_bins_x<<", "<<j+1<<"/"<<nb_bins_y<<") Considering region1 : "<<region1.GetIndex()<<", "<<region1.GetSize());


          extractChannel1->SetExtractionRegion(region1);


          // We need to find the corresponding region in image 2
          itk::ContinuousIndex<double,2> ul_i, ur_i, lr_i, ll_i;
          FloatImageType::PointType ul1, ur1, ll1, lr1, p1, p2, p3, p4;
          itk::ContinuousIndex<double,2> i1, i2, i3, i4, i_min, i_max;

          ul_i[0] = static_cast<double>(startx) - 0.5;
          ul_i[1] = static_cast<double>(starty) - 0.5;
          ur_i = ul_i;
          lr_i = ul_i;
          ll_i = ul_i;
          ur_i[0] += static_cast<double>(bin_size_x);
          lr_i[0] += static_cast<double>(bin_size_x);
          lr_i[1] += static_cast<double>(bin_size_y);
          ll_i[1] += static_cast<double>(bin_size_y);

          image1->TransformContinuousIndexToPhysicalPoint(ul_i,ul1);
          image1->TransformContinuousIndexToPhysicalPoint(ur_i,ur1);
          image1->TransformContinuousIndexToPhysicalPoint(lr_i,lr1);
          image1->TransformContinuousIndexToPhysicalPoint(ll_i,ll1);

          p1 = rsTransform->TransformPoint(ul1);
          p2 = rsTransform->TransformPoint(ur1);
          p3 = rsTransform->TransformPoint(lr1);
          p4 = rsTransform->TransformPoint(ll1);

          image2->TransformPhysicalPointToContinuousIndex(p1,i1);
          image2->TransformPhysicalPointToContinuousIndex(p2,i2);
          image2->TransformPhysicalPointToContinuousIndex(p3,i3);
          image2->TransformPhysicalPointToContinuousIndex(p4,i4);

          i_min[0] = std::min(std::min(i1[0],i2[0]),std::min(i3[0],i4[0]));
          i_min[1] = std::min(std::min(i1[1],i2[1]),std::min(i3[1],i4[1]));

          i_max[0] = std::max(std::max(i1[0],i2[0]),std::max(i3[0],i4[0]));
          i_max[1] = std::max(std::max(i1[1],i2[1]),std::max(i3[1],i4[1]));

          FloatImageType::IndexType index2;
          FloatImageType::SizeType size2;

          index2[0] = vcl_floor(i_min[0]);
          index2[1] = vcl_floor(i_min[1]);

          size2[0] = vcl_ceil(i_max[0]-i_min[0]);
          size2[1] = vcl_ceil(i_max[1]-i_min[1]);

          FloatImageType::RegionType region2;
          region2.SetIndex(index2);
          region2.SetSize(size2);
          region2.PadByRadius(static_cast<unsigned int>(GetParameterInt("precision")));

          FloatImageType::RegionType largestRegion2 = image2->GetLargestPossibleRegion();
          largestRegion2.ShrinkByRadius(image_border_margin);

          if(region2.Crop(largestRegion2))
            {
            otbAppLogINFO("Corresponding region2 is "<<region2.GetIndex()<<", "<<region2.GetSize());

            extractChannel2->SetExtractionRegion(region2);

            Match(extractChannel1->GetOutput(),extractChannel2->GetOutput(),rsTransform,rsTransform1ToWGS84,rsTransform2ToWGS84,file,&mls);
            }
          else
            {
            otbAppLogINFO("Corresponding region2 is out of range: "<<region2.GetIndex()<<", "<<region2.GetSize());
            }
          }
        }
      }
      file.close();

      if(IsParameterEnabled("outvector"))
        {
        // Create the datasource (for matches export)
        otb::ogr::Layer layer(NULL, false);
        otb::ogr::DataSource::Pointer ogrDS;

        ogrDS = otb::ogr::DataSource::New(GetParameterString("outvector"), otb::ogr::DataSource::Modes::Overwrite);
        std::string projref = "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9122\"]],AUTHORITY[\"EPSG\",\"4326\"]]";
        OGRSpatialReference oSRS(projref.c_str());

        // and create the layer
        layer = ogrDS->CreateLayer("matches", &oSRS, wkbMultiLineString);
        OGRFeatureDefn & defn = layer.GetLayerDefn();
        ogr::Feature feature(defn);

        feature.SetGeometry(&mls);
        layer.CreateFeature(feature);
        }
  }
};
}
}


  OTB_APPLICATION_EXPORT(otb::Wrapper::HomologousPointsExtraction)

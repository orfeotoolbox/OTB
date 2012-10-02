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

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#include "otbSiftFastImageFilter.h"
#include "otbImageToSURFKeyPointSetFilter.h"
#include "otbKeyPointSetsMatchingFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbKeyPointSetsMatchingFilter.h"

#include "itkPointSet.h"
#include "itkEuclideanDistance.h"
#include "otbGenericRSTransform.h"

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
  typedef SiftFastImageFilter<FloatImageType,PointSetType>   SiftFilterType;
  typedef ImageToSURFKeyPointSetFilter<FloatImageType,PointSetType> SurfFilterType;

  typedef itk::Statistics::EuclideanDistance<RealVectorType> DistanceType;
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
    SetDescription("TODO");

    // Documentation
    SetDocName("Homologous Points Extraction");
    SetDocLongDescription("TODO");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");

    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in1", "Input Image 1");
    SetParameterDescription("in1"," First input image");

    AddParameter(ParameterType_InputImage, "in2", "Input Image 2");
    SetParameterDescription("in2"," Second input image");

    AddParameter(ParameterType_Int,"binsize","Size of bin");
    AddParameter(ParameterType_Int,"binstep","Step between bins");

    AddParameter(ParameterType_Empty,"2wgs84","Export points from image 2 in wgs84");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_OutputFilename,"out","Output file with tie points");
    SetParameterDescription("out","File containing the list of tie points");
  }

  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    FloatImageType::SizeType size = this->GetParameterImage("in1")->GetLargestPossibleRegion().GetSize();

    unsigned int bin_size = GetParameterInt("binsize");
    unsigned int bin_step = GetParameterInt("binstep");
    unsigned int nb_bins_x = size[0]/(bin_size + bin_step);
    unsigned int nb_bins_y = size[1]/(bin_size + bin_step);

    FloatImageType::SpacingType spacing1 = this->GetParameterImage("in1")->GetSpacing();
    FloatImageType::SpacingType spacing2 = this->GetParameterImage("in2")->GetSpacing();
    
    FloatImageType::PointType origin1 = this->GetParameterImage("in1")->GetOrigin();
    FloatImageType::PointType origin2 = this->GetParameterImage("in2")->GetOrigin();

    RSTransformType::Pointer rsTransform = RSTransformType::New();
    rsTransform->SetInputKeywordList(this->GetParameterImage("in1")->GetImageKeywordlist());
    rsTransform->SetInputProjectionRef(this->GetParameterImage("in1")->GetProjectionRef());
    rsTransform->SetOutputKeywordList(this->GetParameterImage("in2")->GetImageKeywordlist());
    rsTransform->SetOutputProjectionRef(this->GetParameterImage("in2")->GetProjectionRef());

    RSTransformType::Pointer rsTransform2ToWGS84 = RSTransformType::New();
    rsTransform2ToWGS84->SetInputKeywordList(this->GetParameterImage("in2")->GetImageKeywordlist());
    rsTransform2ToWGS84->SetInputProjectionRef(this->GetParameterImage("in2")->GetProjectionRef());
    

    // Elevation through the elevation handler
    if (ElevationParametersHandler::IsElevationEnabled(this, "elev"))
      {
      switch(ElevationParametersHandler::GetElevationType(this, "elev"))
        {
        case Elevation_DEM:
        {
        rsTransform->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
        rsTransform->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
        rsTransform2ToWGS84->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
        rsTransform2ToWGS84->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
        }
        break;
        case Elevation_Average:
        {
        rsTransform->SetAverageElevation(ElevationParametersHandler::GetAverageElevation(this, "elev"));
        rsTransform2ToWGS84->SetAverageElevation(ElevationParametersHandler::GetAverageElevation(this, "elev"));
        }
        break;
        }
      }

    rsTransform->InstanciateTransform();
    rsTransform2ToWGS84->InstanciateTransform();

    unsigned int precision = 30;

    std::ofstream file;
    file.open(GetParameterString("out").c_str());
    file<<std::fixed;
    file.precision(12);

    for(unsigned int i = 0; i<nb_bins_x;++i)
      {
      for(unsigned int j = 0; j<nb_bins_y; ++j)
        {
        unsigned int startx = bin_step/2 + i*(bin_size + bin_step);
        unsigned int starty = bin_step/2 + j*(bin_size + bin_step);
        

        FloatImageType::SizeType size1;
        FloatImageType::IndexType index1;
        FloatImageType::RegionType region1;

        index1[0]=startx;
        index1[1]=starty;
        size1[0] = bin_size;
        size1[1] = bin_size;

        region1.SetIndex(index1);
        region1.SetSize(size1);

        region1.Crop(this->GetParameterImage("in1")->GetLargestPossibleRegion());

        otbAppLogINFO("("<<i<<"/"<<nb_bins_x<<", "<<j<<"/"<<nb_bins_y<<") Considering region1 : "<<region1);

        ExtractChannelFilterType::Pointer extractChannel1 = ExtractChannelFilterType::New();
        extractChannel1->SetInput(this->GetParameterImage("in1"));
        extractChannel1->SetChannel(1);
        extractChannel1->SetExtractionRegion(region1);
        
        SiftFilterType::Pointer sift1 = SiftFilterType::New();
        sift1->SetInput(extractChannel1->GetOutput());

        // We need to find the corresponding region in image 2
        FloatImageType::PointType ul1, ur1, ll1, lr1, p1, p2, p3, p4, ul2, lr2;

        ul1[0] = origin1[0] + startx * spacing1[0];
        ul1[1] = origin1[1] + starty * spacing1[1];

        ur1[0] = origin1[0] + (startx+bin_size) * spacing1[0];
        ur1[1] = origin1[1] + starty * spacing1[1];

        lr1[0] = origin1[0] + (startx+bin_size) * spacing1[0];
        lr1[1] = origin1[1] + (starty+bin_size) * spacing1[1];

        ll1[0] = origin1[0] + (startx) * spacing1[0];
        ll1[1] = origin1[1] + (starty+bin_size) * spacing1[1];

        p1 = rsTransform->TransformPoint(ul1);
        p2 = rsTransform->TransformPoint(ur1);
        p3 = rsTransform->TransformPoint(lr1);
        p4 = rsTransform->TransformPoint(ll1);

        ul2[0] = std::min(std::min(p1[0],p2[0]),std::min(p3[0],p4[0]));
        ul2[1] = std::min(std::min(p1[1],p2[1]),std::min(p3[1],p4[1]));

        lr2[0] = std::max(std::max(p1[0],p2[0]),std::max(p3[0],p4[0]));
        lr2[1] = std::max(std::max(p1[1],p2[1]),std::max(p3[1],p4[1]));

        FloatImageType::IndexType index2;
        FloatImageType::SizeType size2;

        index2[0] = vcl_floor((ul2[0]-origin2[0])/spacing2[0]);
        index2[1] = vcl_floor((ul2[1]-origin2[1])/spacing2[1]);

        size2[0] = vcl_ceil((lr2[0]-ul2[0])/vcl_abs(spacing2[0]));
        size2[1] = vcl_ceil((lr2[1]-ul2[1])/vcl_abs(spacing2[1]));

        FloatImageType::RegionType region2;
        region2.SetIndex(index2);
        region2.SetSize(size2);
        region2.PadByRadius(2*precision);

        if(region2.Crop(this->GetParameterImage("in2")->GetLargestPossibleRegion()))
          {        
          otbAppLogINFO("Corresponding region2 is "<<region2);

          ExtractChannelFilterType::Pointer extractChannel2 = ExtractChannelFilterType::New();
          extractChannel2->SetInput(this->GetParameterImage("in2"));
          extractChannel2->SetChannel(1);
          extractChannel2->SetExtractionRegion(region2);

          SiftFilterType::Pointer sift2 = SiftFilterType::New();
          sift2->SetInput(extractChannel2->GetOutput());

          sift1->Update();

          otbAppLogINFO("Found " << sift1->GetOutput()->GetNumberOfPoints()<<" points in region 1.");

          sift2->Update();

          otbAppLogINFO("Found " << sift2->GetOutput()->GetNumberOfPoints()<<" points in region 2.");

          MatchingFilterType::Pointer matchingFilter = MatchingFilterType::New();
          matchingFilter->SetInput1(sift1->GetOutput());
          matchingFilter->SetInput2(sift2->GetOutput());
          matchingFilter->Update();
          
          LandmarkListType::Pointer landmarks = matchingFilter->GetOutput();
          
          otbAppLogINFO("Found " << landmarks->Size() <<" homologous points.");
          
          unsigned int discarded  = 0;

          for (LandmarkListType::Iterator it = landmarks->Begin();
               it != landmarks->End(); ++it)
            {
            PointType point1 = it.Get()->GetPoint1();
            PointType point2 = it.Get()->GetPoint2();
          
            PointType pprime = rsTransform->TransformPoint(point1);

            double error = vcl_sqrt((point2[0]-pprime[0])*(point2[0]-pprime[0])+(point2[1]-pprime[1])*(point2[1]-pprime[1]));

            std::cout<<point1<<" "<<point2<<" "<<pprime<<" "<<error<<std::endl;

            if(error<2*precision)
              {
              if(IsParameterEnabled("2wgs84"))
                {
                pprime = rsTransform2ToWGS84->TransformPoint(point2);

                file<<point1[0]<<"\t"<<point1[1]<<"\t"<<pprime[0]<<"\t"<<pprime[1]<<std::endl;
                }
              else
                {
                file<<point1[0]<<"\t"<<point1[1]<<"\t"<<point2[0]<<"\t"<<point2[1]<<std::endl;
                }
              }
            else
              {
              ++discarded;
              }
            }

          otbAppLogINFO(<<discarded<<" points discarded");
          }
        }
      }
    file.close();
  }
};
}
}


OTB_APPLICATION_EXPORT(otb::Wrapper::HomologousPointsExtraction)

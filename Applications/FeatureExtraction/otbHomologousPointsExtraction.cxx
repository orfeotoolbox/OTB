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

    AddParameter(ParameterType_OutputFilename,"out","Output file with tie points");
    SetParameterDescription("out","File containing the list of tie points");
  }

  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    ExtractChannelFilterType::Pointer extractChannel1 = ExtractChannelFilterType::New();
    extractChannel1->SetInput(this->GetParameterImage("in1"));
    extractChannel1->SetChannel(1);

    SiftFilterType::Pointer sift1 = SiftFilterType::New();
    sift1->SetInput(extractChannel1->GetOutput());
//    sift1->SetScalesNumber(1);

    ExtractChannelFilterType::Pointer extractChannel2 = ExtractChannelFilterType::New();
    extractChannel2->SetInput(this->GetParameterImage("in2"));
    extractChannel2->SetChannel(1);
    
    SiftFilterType::Pointer sift2 = SiftFilterType::New();
    sift2->SetInput(extractChannel2->GetOutput());
//    sift2->SetScalesNumber(1);

    sift1->Update();

    otbAppLogINFO("Found " << sift1->GetOutput()->GetNumberOfPoints()<<" points in image 1.");

    sift2->Update();

    otbAppLogINFO("Found " << sift2->GetOutput()->GetNumberOfPoints()<<" points in image 2.");

    MatchingFilterType::Pointer matchingFilter = MatchingFilterType::New();
    matchingFilter->SetInput1(sift1->GetOutput());
    matchingFilter->SetInput2(sift2->GetOutput());
    matchingFilter->Update();

    LandmarkListType::Pointer landmarks = matchingFilter->GetOutput();

    otbAppLogINFO("Found " << landmarks->Size() <<" homologous points.");

    std::ofstream file;
    file.open(GetParameterString("out").c_str());
    
    for (LandmarkListType::Iterator it = landmarks->Begin();
         it != landmarks->End(); ++it)
      {
      PointType point1 = it.Get()->GetPoint1();
      PointType point2 = it.Get()->GetPoint2();
      
      file<<point1[0]<<"\t"<<point1[1]<<"\t"<<point2[0]<<"\t"<<point2[1]<<std::endl;
    }
    file.close();

  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::HomologousPointsExtraction)

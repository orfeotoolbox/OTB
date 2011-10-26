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

#include "otbStreamingLineSegmentDetector.h"

#include "otbVectorImageToAmplitudeImageFilter.h"
#include "otbVectorData.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataTransformFilter.h"
#include "itkAffineTransform.h"


namespace otb
{
namespace Wrapper
{

class LineSegmentDetection: public Application
{
public:
  /** Standard class typedefs. */
  typedef LineSegmentDetection          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LineSegmentDetection, otb::Wrapper::Application);

private:
  LineSegmentDetection()
  {
    SetName("LineSegmentDetection");
    SetDescription("Detect line segments in raster");

    // Documentation
    SetDocName("Line segment detection application");
    SetDocLongDescription("This application detects line segments in raster. Detected lines are saved in a shape file.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine BundleToPerfectSensor ${OTB-BIN}/bin "
      "--in ${OTB-Data}/Input/QB_Suburb.png --out linesDetected.shp ");
     AddDocTag("Feature extraction");
  }

  virtual ~LineSegmentDetection()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage, "in", "Input Image");


    AddParameter(ParameterType_OutputVectorData, "out", "Output detected");
    SetParameterDescription("out"," Output detected line segments raster.");
    AddParameter(ParameterType_Directory, "dem", "DEM directory");
    MandatoryOff("dem");

    AddParameter(ParameterType_Empty, "norescale", "No rescaling in [0, 255]");
    MandatoryOff("norescale");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::VectorImageToAmplitudeImageFilter<FloatVectorImageType, FloatImageType>
      VectorImageToAmplitudeImageFilterType;

    typedef otb::StreamingStatisticsImageFilter<FloatImageType>
      StreamingStatisticsImageFilterType;

    typedef itk::ShiftScaleImageFilter<FloatImageType, FloatImageType>
      ShiftScaleImageFilterType;

    typedef otb::StreamingLineSegmentDetector
      < FloatImageType >::FilterType LSDFilterType;

    VectorImageToAmplitudeImageFilterType::Pointer amplitudeConverter
      = VectorImageToAmplitudeImageFilterType::New();

    amplitudeConverter->SetInput( GetParameterImage("in") );

    FloatImageType::Pointer image = amplitudeConverter->GetOutput();

    StreamingStatisticsImageFilterType::Pointer stats
       = StreamingStatisticsImageFilterType::New();

    ShiftScaleImageFilterType::Pointer shiftScale
      = ShiftScaleImageFilterType::New();

    // Default behavior is to do the rescaling
    if ( !IsParameterEnabled("norescale") )
      {
      stats->SetInput(amplitudeConverter->GetOutput());

      AddProcess(stats->GetStreamer(), "Image statistics");
      stats->Update();
      FloatImageType::PixelType min = stats->GetMinimum();
      FloatImageType::PixelType max = stats->GetMaximum();

      shiftScale->SetInput(amplitudeConverter->GetOutput());
      shiftScale->SetShift( -min );
      shiftScale->SetScale( 255.0 / (max - min) );

      image = shiftScale->GetOutput();
      }

    LSDFilterType::Pointer lsd
      = LSDFilterType::New();
    lsd->GetFilter()->SetInput(image);

    AddProcess(lsd->GetStreamer(), "Running Line Segment Detector");
    lsd->Update();

    /*
     * Reprojection of the output VectorData
     *
     * The output of LSDFilterType is in image physical coordinates,
     * projection WKT applied if the input image has one
     *
     * We need to reproject in WGS84 if the input image is in sensor model geometry
     */

    std::string projRef = GetParameterImage("in")->GetProjectionRef();
    ImageKeywordlist kwl = GetParameterImage("in")->GetImageKeywordlist();

    VectorDataType::Pointer vd = lsd->GetFilter()->GetOutputVectorData();
    VectorDataType::Pointer projectedVD = vd;

    if ( projRef.empty() && kwl.GetSize() > 0 )
      {
      // image is in sensor model geometry

      // Reproject VectorData in image projection
      typedef otb::VectorDataProjectionFilter
        <VectorDataType, VectorDataType>                     VectorDataProjectionFilterType;

      VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
      vproj->SetInput(vd);
      vproj->SetInputKeywordList(GetParameterImage("in")->GetImageKeywordlist());
      vproj->SetInputOrigin(GetParameterImage("in")->GetOrigin());
      vproj->SetInputSpacing(GetParameterImage("in")->GetSpacing());

      // Configure DEM directory
      if(IsParameterEnabled("dem"))
        {
        vproj->SetDEMDirectory( GetParameterString("dem") );
        }
      else
        {
        if ( otb::ConfigurationFile::GetInstance()->IsValid() )
          {
          vproj->SetDEMDirectory(otb::ConfigurationFile::GetInstance()->GetDEMDirectory());
          }
        }

      AddProcess(vproj, "Reprojecting output vector data");
      vproj->Update();

      projectedVD = vproj->GetOutput();
      }

    SetParameterOutputVectorData("out", projectedVD);
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LineSegmentDetection)

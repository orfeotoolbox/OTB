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

#include "otbMeanShiftVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{


class MeanShiftSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef MeanShiftSegmentation         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MeanShiftSegmentation, otb::Application);

private:

  MeanShiftSegmentation()
  {
    SetName("MeanShiftSegmentation");
    SetDescription("Perform mean shift clustering");

    // Documentation
    SetDocName("Mean Shift segmentation application");
    SetDocLongDescription("This application performs mean shift clustering.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine MeanShiftClustering ${OTB-BIN}/bin "
      "--in ${OTB-Data}/Input/qb_RoadExtract2sub200x200.tif --cout MeanShift_ClusteredOutput.tif"
        "--fout MeanShift_FilterOutput.tif --lout MeanShift_LabeledClusteredOutput.tif"
        "--cbout MeanShift_ClusterBoundariesOutput.tif ---spatialr 16 --ranger 16 --minsize 10 --scale 1.0 ");
    AddDocTag("Segmentation");
    }

  virtual ~MeanShiftSegmentation()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,   "in",     "Input Image");
    AddParameter(ParameterType_OutputImage,  "fout",    "Filtered output");
    AddParameter(ParameterType_OutputImage,  "cout",    "Clustered output");
    AddParameter(ParameterType_OutputImage,  "lout",    "Label output");
    AddParameter(ParameterType_OutputImage,  "cbout",   "Cluster Boundaries output");
    MandatoryOff("fout");
    MandatoryOff("cout");
    MandatoryOff("lout");
    MandatoryOff("cbout");

    AddParameter(ParameterType_Int,          "spatialr",   "Spatial radius");
    AddParameter(ParameterType_Float,        "ranger",   "Range radius");
    AddParameter(ParameterType_Int,          "minsize",   "Min region size");
    AddParameter(ParameterType_Float,        "scale",   "Scale");
    SetParameterInt("spatialr",   5);
    SetParameterFloat("ranger",   15.0);
    SetParameterInt("minsize",    100);
    SetParameterFloat("scale",    100000.);
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType* input = GetParameterImage("in");

    typedef otb::MeanShiftVectorImageFilter<FloatVectorImageType, FloatVectorImageType> MSFilterType;
    MSFilterType::Pointer filter = MSFilterType::New();

    filter->SetInput(input);
    filter->SetSpatialRadius( GetParameterInt("spatialr") );
    filter->SetRangeRadius( GetParameterFloat("ranger") );
    filter->SetMinimumRegionSize( GetParameterInt("minsize") );
    filter->SetScale( GetParameterFloat("scale") );

    m_Ref = filter;

    if (IsParameterEnabled("fout") && HasValue("fout"))
      SetParameterOutputImage("fout", filter->GetOutput());
    if (IsParameterEnabled("cout") && HasValue("cout"))
      SetParameterOutputImage("cout", filter->GetClusteredOutput());
    if (IsParameterEnabled("lout") && HasValue("lout"))
      SetParameterOutputImage("lout", filter->GetLabeledClusteredOutput());
    if (IsParameterEnabled("cbout") && HasValue("cbout"))
      SetParameterOutputImage("cbout", filter->GetClusterBoundariesOutput());
  }

  itk::LightObject::Pointer m_Ref;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MeanShiftSegmentation)

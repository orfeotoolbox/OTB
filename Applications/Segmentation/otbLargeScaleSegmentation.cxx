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


class LargeScaleSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef LargeScaleSegmentation         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::MeanShiftVectorImageFilter<FloatVectorImageType, FloatVectorImageType> MSFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LargeScaleSegmentation, otb::Application);

private:
  void DoInit()
  {
    SetName("LargeScaleSegmentation");
    SetDescription("Perform Large scale segmentation");

    // Documentation
    SetDocName("Large Scale segmentation");
    SetDocLongDescription(".");
    SetDocLimitations(" .");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,   "in",     "Input Image");
    SetParameterDescription( "in", "The input image." );

    AddParameter(ParameterType_InputImage,   "inmask",     "Mask Image");
    SetParameterDescription( "inmask", "Mask image. (Pixel with 0 will not be processed)." );
    MandatoryOff("inmask");

    AddParameter(ParameterType_OutputVectorData,"outvd", "Output VectorData");
    SetParameterDescription( "outvd", "The name of output Vector Data." );

    //AddParameter(ParameterType_OutputImage,  "lout",    "Labeled output");
    //SetParameterDescription( "lout", "The labeled output image." );
    //MandatoryOff("lout");

    //
    AddParameter(ParameterType_Choice, "filter", "Segmentation Filter");
    SetParameterDescription("filter", "Choose your segmentation filter method.");

    AddChoice("filter.meanshiftedison", "EDISON MeanShift");
    SetParameterDescription("filter.meanshiftedison", "EDISON based MeanShift filter. (is going to be replaced by newframework).");
    // EDISON Meanshift Parameters

    AddChoice("filter.meanshift", "MeanShift");
    SetParameterDescription("filter.meanshiftedison", "MeanShift filter.");
    // MeanShift Parameters

    AddChoice("filter.connectedcomponent", "ConnectedComponentMuParser");
    SetParameterDescription("filter.connectedcomponent", "connectedComponant muparser filter.");
    // CC parameters


    AddParameter(ParameterType_Group, "relabel", "Relabel Output.");
    SetParameterDescription("relabel", "Relabel output and eliminates to small objects. Background value will be assigned instead.");

    MandatoryOff("relabel");

     AddParameter(ParameterType_Int, "relabel.minsize", "Minimum object size");
     SetParameterDescription("relabel.minsize", "object with size under this threshold (area in pixels) will be replaced by the background value.");
     SetDefaultParameterInt("relabel.minsize", 1);
     SetMinimumParameterIntValue("relabel.minsize", 0);
     MandatoryOff("relabel.minsize");

     AddParameter(ParameterType_Int, "relabel.backgroundvalue", "Background value");
     SetParameterDescription("relabel.backgroundvalue", "Background value.");
     SetDefaultParameterInt("relabel.backgroundvalue", 0);
     MandatoryOff("relabel.backgroundvalue");

    //
     AddParameter(ParameterType_String, "layername", "Layer Name");
     SetParameterDescription("layername", "Layer Name.(by default : Layer )");
     SetParameterString("layername", "layer");
     MandatoryOff("layername");

     AddParameter(ParameterType_String, "fieldname", "Field Name");
     SetParameterDescription("fieldname", "field Name.(by default : DN )");
     SetParameterString("fieldname", "DN");
     MandatoryOff("fieldname");

     AddParameter(ParameterType_Int, "tilesize", "tile size");
     SetParameterDescription("tilesize", "streaming to dimension for computing vectorization.(automatic tile dimension is set "
         "   if tile size set to 0 (by default).)");
     SetDefaultParameterInt("tilesize", 0);
     MandatoryOff("tilesize");

     AddParameter(ParameterType_Int, "startlabel", "start label");
     SetParameterDescription("startlabel", "Start label.(1 by default)");
     SetDefaultParameterInt("startlabel", 1);
     MandatoryOff("startlabel");

     AddParameter(ParameterType_Empty, "neighbor", "use 8connected");
     SetParameterDescription("neighbor", "Pixel neighborhood vectorization strategy. 4 or 8 neighborhood .(4 neighborhood by default.)");
     MandatoryOff("neighbor");



    // Doc example parameter settings

    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("outvd", "SegmentationVectorData.sqlite");
    SetDocExampleParameterValue("filter", "meanshift");


  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
   }



};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleSegmentation)

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

// Segmentation filters includes
#include "otbMeanShiftVectorImageFilter.h"

#include "otbMeanShiftImageFilter2.h"

#include "otbStreamingVectorizedSegmentationOGR.h"

//#include "otbPersistentImageToOGRDataFilter.h"
//#include "otbPersistentFilterStreamingDecorator.h"

#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbMaskMuParserFilter.h"


#include "otbParser.h"

#include "otbOGRDataSourceWrapper.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"


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

  typedef UInt32ImageType               LabelImageType;
  typedef UInt32ImageType               MaskImageType;


  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                        MaskImageType::PixelType>                ExtractROIFilterType;
   typedef ObjectList<ExtractROIFilterType>                                      ExtractROIFilterListType;

   // typedef itk::ImageToImageFilter<FloatVectorImageType,UInt32ImageType> SegmentationFilterType;
  typedef otb::MeanShiftVectorImageFilter<FloatVectorImageType,FloatVectorImageType,LabelImageType> MSEDISONSegmentationFilterType;


  // TODO replace by new meanshift segmentation scheme
  //typedef otb::MeanShiftConnectedComponentSegmentationFilter< FloatVectorImageType, MaskImageType,LabelImageType >  MeanShiftConnectedComponentSegmentationFilterType;
  // typedef otb::MeanShiftImageFilter2<FloatVectorImageType, LabelImageType> MeanShiftFilterType;


  // Mask generation
  typedef otb::Functor::ConnectedComponentMuParserFunctor<FloatVectorImageType::PixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<
      FloatVectorImageType,
      LabelImageType,
      FunctorType,
      MaskImageType > ConnectedComponentSegmentationFilterType;


  typedef otb::MeanShiftImageFilter2
       <FloatVectorImageType, FloatVectorImageType>
   MeanShiftFilterType;
  // mask typedef
  typedef otb::MaskMuParserFilter<FloatVectorImageType, MaskImageType> MaskMuParserFilterType;


  typedef otb::StreamingVectorizedSegmentationOGR<FloatVectorImageType, ConnectedComponentSegmentationFilterType> ConnectedComponentStreamingVectorizedSegmentationOGRType;

  typedef otb::StreamingVectorizedSegmentationOGR<FloatVectorImageType, MSEDISONSegmentationFilterType> MSEDIONStreamingVectorizedSegmentationOGRType;
 // typedef otb::StreamingVectorizedSegmentationOGR<FloatVectorImageType, ConnectedComponentSegmentationFilterType> ConnectedComponentStreamingVectorizedSegmentationOGRType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LargeScaleSegmentation, otb::Application);

private:
  void DoInit()
  {
    SetName("LargeScaleSegmentation");
    SetDescription("Performs Large scale segmentation");

    // Documentation
    SetDocName("Large Scale segmentation");
    SetDocLongDescription("This application is dedicated to image segmentation. "
      "By handling streaming and OGR framework, it has been optimized for processing large scale data ."
      " Numerous segmentations algorithms are available. Output are vector data type.");
    SetDocLimitations(" .");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image.");

    AddParameter(ParameterType_InputImage, "inmask", "Mask Image");
    SetParameterDescription("inmask", "Mask image. (Pixel with 0 will not be processed).");
    MandatoryOff("inmask");

    AddParameter(ParameterType_OutputFilename, "outvd", "Output VectorData");
    SetParameterDescription("outvd", "The name of output Vector Data.");

    AddParameter(ParameterType_OutputImage, "lout", "Labeled output");
    SetParameterDescription("lout", "The labeled output image.");
    MandatoryOff("lout");

    //
    AddParameter(ParameterType_Choice, "filter", "Segmentation Filter");
    SetParameterDescription("filter", "Choose your segmentation filter method.");

    AddChoice("filter.meanshiftedison", "EDISON MeanShift");
    SetParameterDescription("filter.meanshiftedison",
                            "EDISON based MeanShift filter. (is going to be replaced by newframework).");
    // EDISON Meanshift Parameters
    AddParameter(ParameterType_Int, "filter.meanshiftedison.spatialr", "Spatial radius");
    SetParameterDescription("filter.meanshiftedison.spatialr", "Spatial radius defining neighborhood.");
    AddParameter(ParameterType_Float, "filter.meanshiftedison.ranger", "Range radius");
    SetParameterDescription("filter.meanshiftedison.ranger", "Range radius defining the interval in the color space.");
    AddParameter(ParameterType_Int, "filter.meanshiftedison.minsize", "Min region size");
    SetParameterDescription("filter.meanshiftedison.minsize", "Minimun size of a region to be kept after clustering.");
    AddParameter(ParameterType_Float, "filter.meanshiftedison.scale", "Scale");
    SetParameterDescription("filter.meanshiftedison.scale", "Scale to stretch the image before processing.");
    SetDefaultParameterInt("filter.meanshiftedison.spatialr", 5);
    SetDefaultParameterFloat("filter.meanshiftedison.ranger", 15.0);
    SetDefaultParameterInt("filter.meanshiftedison.minsize", 100);
    SetDefaultParameterFloat("filter.meanshiftedison.scale", 100000.);

    AddChoice("filter.meanshift", "MeanShift");
    SetParameterDescription(
                            "filter.meanshift",
                            "MeanShift filter (MeanShift filtered output is segmented by connected component segmentation, this step will be replaced by a dedicated scheme (WIP)).");
    // MeanShift Parameters
    AddParameter(ParameterType_Int, "filter.meanshift.spatialr", "Spatial radius");
    SetParameterDescription("filter.meanshift.spatialr", "Spatial radius defining neighborhood.");
    AddParameter(ParameterType_Float, "filter.meanshift.ranger", "Range radius");
    SetParameterDescription("filter.meanshift.ranger", "Range radius defining the interval in the color space.");
    AddParameter(ParameterType_Float, "filter.meanshift.thres", "convergence threshold");
    SetParameterDescription("filter.meanshift.thres", "convergence threshold. iterative scheme will stop if MeanShift "
      "vector is below this threshold (1e-3 by default) or iteration number reached maximum iteration number.");
    AddParameter(ParameterType_Int, "filter.meanshift.maxiter", "maximum iteration number");
    SetParameterDescription("filter.meanshift.maxiter",
                            "iteration process is stopped if convergence hasn't been reached after this number of iteration (10 by default).");
    AddParameter(ParameterType_Empty, "filter.meanshift.useoptim", "use optimization");
    SetParameterDescription("filter.meanshift.useoptim", "Use mode optimization.");
    MandatoryOff("filter.meanshift.useoptim");
    AddParameter(ParameterType_String, "filter.meanshift.expr", "Connected Component Expression");
    SetParameterDescription("filter.meanshift.expr", "Formula used for connected component segmentation");
    SetDefaultParameterInt("filter.meanshift.spatialr", 5);

    SetDefaultParameterInt("filter.meanshift.spatialr", 5);
    SetDefaultParameterFloat("filter.meanshift.ranger", 15.0);
    SetDefaultParameterFloat("filter.meanshift.thres", 1e-3);
    SetMinimumParameterFloatValue("filter.meanshift.thres", 0.);
    SetDefaultParameterInt("filter.meanshift.maxiter", 10);
    SetMinimumParameterIntValue("filter.meanshift.maxiter", 1);

    AddChoice("filter.connectedcomponent", "ConnectedComponentMuParser");
    SetParameterDescription("filter.connectedcomponent", "connectedComponant muparser filter.");

    AddParameter(ParameterType_String, "filter.connectedcomponent.expr", "Connected Component Expression");
    SetParameterDescription("filter.connectedcomponent.expr", "Formula used for connected component segmentation");

    AddParameter(ParameterType_Int, "minsize", "Minimum object size");
    SetParameterDescription("minsize",
                            "object with size under this threshold (area in pixels) will be replaced by the background value.");
    SetDefaultParameterInt("minsize", 1);
    SetMinimumParameterIntValue("minsize", 1);
    MandatoryOff("minsize");

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
    SetParameterDescription("tilesize",
                            "streaming to dimension for computing vectorization.(automatic tile dimension is set "
                              "   if tile size set to 0 (by default).)");
    SetDefaultParameterInt("tilesize", 0);
    MandatoryOff("tilesize");

    AddParameter(ParameterType_Int, "startlabel", "start label");
    SetParameterDescription("startlabel", "Start label.(1 by default)");
    SetDefaultParameterInt("startlabel", 1);
    MandatoryOff("startlabel");

    AddParameter(ParameterType_Empty, "neighbor", "use 8 connected");
    SetParameterDescription("neighbor",
                            "Pixel neighborhood vectorization strategy. 4 or 8 neighborhood .(4 neighborhood by default.)");
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
    //
    std::string dataSourceName = GetParameterString("outvd");
    otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::write);

    //mask image transform vector image to LabelImage
    if (HasValue("inmask"))
      {
      m_MaskImage = this->GetParameterUInt32Image("inmask");
      m_MaskImage->UpdateOutputInformation();
      }

    const unsigned int minSize = static_cast<unsigned int> (this->GetParameterInt("minsize"));
    const unsigned int tileSize = static_cast<unsigned int> (this->GetParameterInt("tilesize"));

    std::string layerName = this->GetParameterString("layername");
    std::string fieldName = this->GetParameterString("fieldname");

    const unsigned int startLabel = this->GetParameterInt("startlabel");
    bool use8connected = IsParameterEnabled("neighbor");

    switch (GetParameterInt("filter"))
      {
      // MeanShiftEDISON based filtering
      case 0:
        {

        MSEDIONStreamingVectorizedSegmentationOGRType::Pointer
            EDISONVectorizationFilter = MSEDIONStreamingVectorizedSegmentationOGRType::New();
        m_VectorizationFilter = EDISONVectorizationFilter;

        EDISONVectorizationFilter->SetInput(GetParameterFloatVectorImage("in"));
        if (HasValue("inmask"))
          {
          EDISONVectorizationFilter->SetInputMask(this->GetParameterUInt32Image("inmask"));
          }
        EDISONVectorizationFilter->SetOGRDataSource(ogrDS);

        //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
        if (tileSize != 0)
          EDISONVectorizationFilter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
        else EDISONVectorizationFilter->GetStreamer()->SetAutomaticTiledStreaming();

        EDISONVectorizationFilter->SetLayerName(layerName);
        EDISONVectorizationFilter->SetFieldName(fieldName);
        EDISONVectorizationFilter->SetStartLabel(startLabel);
        if (use8connected)
        otbAppLogINFO(<<"Use 8 connected neighborhood."<<std::endl);
        EDISONVectorizationFilter->SetUse8Connected(use8connected);

        //segmentation paramters
        const unsigned int
            spatialRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.spatialr"));
        const unsigned int
            rangeRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.ranger"));
        const unsigned int
            minimumObjectSize = static_cast<unsigned int> (this->GetParameterInt("filter.meanshiftedison.minsize"));
        const float scale = this->GetParameterFloat("filter.meanshiftedison.scale");

        EDISONVectorizationFilter->GetSegmentationFilter()->SetSpatialRadius(spatialRadius);
        EDISONVectorizationFilter->GetSegmentationFilter()->SetRangeRadius(rangeRadius);
        EDISONVectorizationFilter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSize);
        EDISONVectorizationFilter->GetSegmentationFilter()->SetScale(scale);
        if (minSize > 1)
          {
          otbAppLogINFO(<<"Object with size under "<<minSize<<" will be suppressed."<<std::endl);
          EDISONVectorizationFilter->SetFilterSmallObject(true);
          EDISONVectorizationFilter->SetMinimumObjectSize(minSize);
          }
        EDISONVectorizationFilter->Initialize(); //must be called !
        EDISONVectorizationFilter->Update(); //must be called !
        //m_VectorizationFilter->Update();
        m_LabelImage = EDISONVectorizationFilter->GetSegmentationFilter()->GetLabeledClusteredOutput();

        break;

        }
        //MeanShift filter
      case 1:
        {

        ConnectedComponentStreamingVectorizedSegmentationOGRType::Pointer
            MSVectorizationFilter = ConnectedComponentStreamingVectorizedSegmentationOGRType::New();
        m_VectorizationFilter = MSVectorizationFilter;

        MeanShiftFilterType::Pointer MSFilter = MeanShiftFilterType::New();
        // MeanShift filtering
        // Set filter parameters

        //segmentation paramters
        const unsigned int
            spatialRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.spatialr"));
        const unsigned int rangeRadius = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.ranger"));
        const unsigned int maxIter = static_cast<unsigned int> (this->GetParameterInt("filter.meanshift.maxiter"));
        const float threshold = this->GetParameterFloat("filter.meanshift.thres");

        MSFilter->SetSpatialBandwidth(spatialRadius);
        MSFilter->SetRangeBandwidth(rangeRadius);
        MSFilter->SetThreshold(threshold);
        MSFilter->SetMaxIterationNumber(maxIter);
        MSFilter->SetInput(GetParameterFloatVectorImage("in"));
        MSFilter->SetModeSearchOptimization(IsParameterEnabled("filter.meanshift.useoptim"));

        MSVectorizationFilter->SetInput(GetParameterFloatVectorImage("in"));
        AddProcess(MSVectorizationFilter, "Computing Filtering");

        MSVectorizationFilter->SetInput(MSFilter->GetRangeOutput());
        if (HasValue("inmask"))
          {
          MSVectorizationFilter->SetInputMask(this->GetParameterUInt32Image("inmask"));
          MSVectorizationFilter->GetSegmentationFilter()->SetMaskImage(this->GetParameterUInt32Image("inmask"));

          }
        MSVectorizationFilter->SetOGRDataSource(ogrDS);

        //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
        if (tileSize != 0)
          MSVectorizationFilter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
        else MSVectorizationFilter->GetStreamer()->SetAutomaticTiledStreaming();

        MSVectorizationFilter->SetLayerName(layerName);
        MSVectorizationFilter->SetFieldName(fieldName);
        MSVectorizationFilter->SetStartLabel(startLabel);
        MSVectorizationFilter->SetUse8Connected(use8connected);

        MSVectorizationFilter->GetSegmentationFilter()->GetFunctor().SetExpression(
                                                                                   GetParameterString(
                                                                                                      "filter.meanshift.expr"));
        AddProcess(MSVectorizationFilter->GetSegmentationFilter(), "Computing segmentation");

        MSVectorizationFilter->Initialize(); //must be called !
        m_VectorizationFilter->Update();
        m_LabelImage = MSVectorizationFilter->GetSegmentationFilter()->GetOutput();

        break;
        }
        // CC filter
      case 2:
        {
        ConnectedComponentStreamingVectorizedSegmentationOGRType::Pointer
            CCVectorizationFilter = ConnectedComponentStreamingVectorizedSegmentationOGRType::New();
        m_VectorizationFilter = CCVectorizationFilter;

        CCVectorizationFilter->SetInput(GetParameterFloatVectorImage("in"));
        if (HasValue("inmask"))
          {
          CCVectorizationFilter->SetInputMask(this->GetParameterUInt32Image("inmask"));
          CCVectorizationFilter->GetSegmentationFilter()->SetMaskImage(this->GetParameterUInt32Image("inmask"));

          }
        CCVectorizationFilter->SetOGRDataSource(ogrDS);

        //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
        if (tileSize != 0)
          CCVectorizationFilter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
        else CCVectorizationFilter->GetStreamer()->SetAutomaticTiledStreaming();

        CCVectorizationFilter->SetLayerName(layerName);
        CCVectorizationFilter->SetFieldName(fieldName);
        CCVectorizationFilter->SetStartLabel(startLabel);
        CCVectorizationFilter->SetUse8Connected(use8connected);

        CCVectorizationFilter->GetSegmentationFilter()->GetFunctor().SetExpression(
                                                                                   GetParameterString(
                                                                                                      "filter.connectedcomponent.expr"));
        AddProcess(CCVectorizationFilter->GetSegmentationFilter(), "Computing segmentation");

        CCVectorizationFilter->Initialize(); //must be called !
        m_VectorizationFilter->Update();
        m_LabelImage = CCVectorizationFilter->GetSegmentationFilter()->GetOutput();

        break;
        }

      default:
        {
        otbAppLogFATAL(<<"non defined filtering method "<<GetParameterInt("filter")<<std::endl);
        break;

        }
        return;
      }

    ogrDS->SyncToDisk();
    SetParameterOutputImage<LabelImageType> ("lout", m_LabelImage);

  }

    itk::ProcessObject::Pointer m_SegmentationFilter;
    itk::ProcessObject::Pointer m_VectorizationFilter;
    MaskImageType::Pointer      m_MaskImage;
    LabelImageType::Pointer     m_LabelImage;
};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LargeScaleSegmentation)

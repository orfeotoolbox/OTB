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

#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"

#include "otbStreamingStatisticsImageFilter.h"
#include "otbSystem.h"
#include "itkChangeLabelImageFilter.h"

#include "otbTileImageFilter.h"

#include <time.h>
#include <vcl_algorithm.h>
#include <climits>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStandardWriterWatcher.h"


namespace otb
{
namespace Wrapper
{
class LSMSSmallRegionsMerging : public Application
{
public:
  typedef LSMSSmallRegionsMerging Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef FloatVectorImageType              ImageType;
  typedef ImageType::InternalPixelType      ImagePixelType;
  typedef UInt32ImageType                   LabelImageType;
  typedef LabelImageType::InternalPixelType LabelImagePixelType;

  typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
  typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;

  typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;

  typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
  typedef itk::ImageRegionConstIterator<ImageType> ImageIterator;

  typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> ChangeLabelImageFilterType;
  typedef otb::TileImageFilter<LabelImageType> TileImageFilterType;

  itkNewMacro(Self);
  itkTypeMacro(Merging, otb::Application);

private:
  ChangeLabelImageFilterType::Pointer m_ChangeLabelFilter;

  void DoInit()
  {
    SetName("LSMSSmallRegionsMerging");
    SetDescription("Third (optional) step of the exact Large-Scale Mean-Shift segmentation workflow.");

    SetDocName("Exact Large-Scale Mean-Shift segmentation, step 3 (optional)");
    SetDocLongDescription("This application performs the third step of the exact Large-Scale Mean-Shift segmentation workflow (LSMS). Given a segmentation result (label image) and the original image, it will merge regions whose size in pixels is lower than minsize parameter with the adjacent regions with the adjacent region with closest radiometry and acceptable size. Small regions will be processed by size: first all regions of area, which is equal to 1 pixel will be merged with adjacent region, then all regions of area equal to 2 pixels, until regions of area minsize. For large images one can use the nbtilesx and nbtilesy parameters for tile-wise processing, with the guarantees of identical results.");
    SetDocLimitations("This application is part of the Large-Scale Mean-Shift segmentation workflow (LSMS) and may not be suited for any other purpose.");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso("LSMSSegmentation, LSMSVectorization, MeanShiftSmoothing");
    AddDocTag(Tags::Segmentation);
    AddDocTag("LSMS");

    AddParameter(ParameterType_InputImage,  "in",    "Input image");
    SetParameterDescription( "in", "The input image." );
    AddParameter(ParameterType_InputImage,  "inseg",    "Segmented image");
    SetParameterDescription( "inseg", " The segmented image input. Segmented image input is the segmentation of the input image." );

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the input image where the minimal regions have been merged." );

    AddParameter(ParameterType_Int, "minsize", "Minimum Region Size");
    SetParameterDescription("minsize", "Minimum Region Size. If, after the segmentation, a region is of size lower than this criterion, the region is merged with the \"nearest\" region (radiometrically).");
    SetDefaultParameterInt("minsize", 50);
    SetMinimumParameterIntValue("minsize", 0);
    MandatoryOff("minsize");

    AddParameter(ParameterType_Int, "tilesizex", "Size of tiles in pixel (X-axis)");
    SetParameterDescription("tilesizex", "Size of tiles along the X-axis.");
    SetDefaultParameterInt("tilesizex", 500);
    SetMinimumParameterIntValue("tilesizex", 1);

    AddParameter(ParameterType_Int, "tilesizey", "Size of tiles in pixel (Y-axis)");
    SetParameterDescription("tilesizey", "Size of tiles along the Y-axis.");
    SetDefaultParameterInt("tilesizey", 500);
    SetMinimumParameterIntValue("tilesizey", 1);

    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("inseg","segmentation.tif");
    SetDocExampleParameterValue("out","merged.tif");
    SetDocExampleParameterValue("minsize","20");
    SetDocExampleParameterValue("tilesizex","256");
    SetDocExampleParameterValue("tilesizey","256");

  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    clock_t tic = clock();

    unsigned int minSize     = GetParameterInt("minsize");

    unsigned long sizeTilesX = GetParameterInt("tilesizex");
    unsigned long sizeTilesY = GetParameterInt("tilesizey");

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();
    unsigned long sizeImageX = imageIn->GetLargestPossibleRegion().GetSize()[0],
      sizeImageY = imageIn->GetLargestPossibleRegion().GetSize()[1];
    unsigned int numberOfComponentsPerPixel = imageIn->GetNumberOfComponentsPerPixel();

    LabelImageType::Pointer labelIn = GetParameterUInt32Image("inseg");

    StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
    stats->SetInput(labelIn);
    stats->Update();
    unsigned int regionCount=stats->GetMaximum();

    std::vector<unsigned int>nbPixels;
    nbPixels.clear();
    nbPixels.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      nbPixels[curLabel] = 0;

    ImageType::PixelType defaultValue(numberOfComponentsPerPixel);
    defaultValue.Fill(0);

    std::vector<ImageType::PixelType>sum(regionCount+1,defaultValue);

    unsigned int nbTilesX = sizeImageX/sizeTilesX + (sizeImageX%sizeTilesX > 0 ? 1 : 0);
    unsigned int nbTilesY = sizeImageY/sizeTilesY + (sizeImageY%sizeTilesY > 0 ? 1 : 0);

    otbAppLogINFO(<<"Number of tiles: "<<nbTilesX<<" x "<<nbTilesY);

    //Sums calculation per label
    otbAppLogINFO(<<"Sums calculation ...");

    for(unsigned int row = 0; row < nbTilesY; row++)
      for(unsigned int column = 0; column < nbTilesX; column++)
       {
        unsigned long startX = column*sizeTilesX;
        unsigned long startY = row*sizeTilesY;
        unsigned long sizeX = vcl_min(sizeTilesX,sizeImageX-startX);
        unsigned long sizeY = vcl_min(sizeTilesY,sizeImageY-startY);

        //Tiles extraction of the input image
        MultiChannelExtractROIFilterType::Pointer imageROI = MultiChannelExtractROIFilterType::New();
        imageROI->SetInput(imageIn);
        imageROI->SetStartX(startX);
        imageROI->SetStartY(startY);
        imageROI->SetSizeX(sizeX);
        imageROI->SetSizeY(sizeY);
        imageROI->Update();

        //Tiles extraction of the segmented image
        ExtractROIFilterType::Pointer labelImageROI = ExtractROIFilterType::New();
        labelImageROI->SetInput(labelIn);
        labelImageROI->SetStartX(startX);
        labelImageROI->SetStartY(startY);
        labelImageROI->SetSizeX(sizeX);
        labelImageROI->SetSizeY(sizeY);
        labelImageROI->Update();

        //Sums calculation for the mean calculation per label
        LabelImageIterator itLabel( labelImageROI->GetOutput(), labelImageROI->GetOutput()->GetLargestPossibleRegion());
        ImageIterator itImage( imageROI->GetOutput(), imageROI->GetOutput()->GetLargestPossibleRegion());

        for (itLabel.GoToBegin(), itImage.GoToBegin(); !itLabel.IsAtEnd(); ++itLabel, ++itImage)
          {
          nbPixels[itLabel.Value()]++;
          for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
            {
            sum[itLabel.Value()][comp]+=itImage.Get()[comp];
            }
          }
        }

    //LUT creation for the final relabelling
    std::vector<LabelImagePixelType> LUT;
    LUT.clear();
    LUT.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      {
      LUT[curLabel] = curLabel;
      }

    //Minimal size region suppression
    otbAppLogINFO(<<"Building LUT for small regions merging ...");

    for (unsigned int size=1; size<minSize; size++)
      {
      // LUTtmp creation in order to modify the LUT only at the end of the pass
      std::vector<LabelImagePixelType> LUTtmp;
      LUTtmp.clear();
      LUTtmp.resize(regionCount+1);
      for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
        {
        LUTtmp[curLabel] = LUT[curLabel];
        }

      for(unsigned int row = 0; row < nbTilesY; row++)
        {
        for(unsigned int column = 0; column < nbTilesX; column++)
          {
          std::set<int> minLabel, edgeLabel, labelMerged;
          std::map<int,std::set<int> > adjMap;

          unsigned long startX = column*sizeTilesX, startY = row*sizeTilesY;
          unsigned long sizeX = vcl_min(sizeTilesX+size+1,sizeImageX-startX),
            sizeY = vcl_min(sizeTilesY+size+1,sizeImageY-startY);

          ExtractROIFilterType::Pointer labelImageROI = ExtractROIFilterType::New();
          labelImageROI->SetInput(labelIn);
          labelImageROI->SetStartX(startX);
          labelImageROI->SetStartY(startY);
          labelImageROI->SetSizeX(sizeX);
          labelImageROI->SetSizeY(sizeY);
          labelImageROI->Update();

          LabelImageType::IndexType pixelIndex;

          //"Adjacency map" creation for the region with nbPixels=="size"
          for(pixelIndex[0]=0; pixelIndex[0]<static_cast<long>(sizeX); ++pixelIndex[0])
            for(pixelIndex[1]=0; pixelIndex[1]<static_cast<long>(sizeY); ++pixelIndex[1])
              {
              LabelImagePixelType curLabel = labelImageROI->GetOutput()->GetPixel(pixelIndex);

              if(labelMerged.count(LUT[curLabel]))
                {
                edgeLabel.insert(LUT[curLabel]);
                }
              if((pixelIndex[0]==0)&&(startX!=0))
                {
                edgeLabel.insert(LUT[curLabel]);
                }
              if((pixelIndex[1]==0)&&(startY!=0))
                {
                edgeLabel.insert(LUT[curLabel]);
                }

              if(pixelIndex[0]==static_cast<long>(sizeX)-1)
                {
                if(startX!=(nbTilesX-1)*sizeTilesX) edgeLabel.insert(LUT[curLabel]);
                }
              else
                {
                ++pixelIndex[0];
                LabelImagePixelType adjLabelX = labelImageROI->GetOutput()->GetPixel(pixelIndex);
                --pixelIndex[0];

                if(LUT[adjLabelX]!=LUT[curLabel])
                  {
                  if((nbPixels[LUT[curLabel]]>0)&&(nbPixels[LUT[curLabel]]==size))
                    {
                    adjMap[LUT[curLabel]].insert(LUT[adjLabelX]);
                    minLabel.insert(LUT[curLabel]);
                    }
                  if((nbPixels[LUT[adjLabelX]]>0)&&(nbPixels[LUT[adjLabelX]]==size))
                    {
                    adjMap[LUT[adjLabelX]].insert(LUT[curLabel]);
                    minLabel.insert(LUT[adjLabelX]);
                    }
                  }
                }
              if(pixelIndex[1]==static_cast<long>(sizeY)-1)
                {
                if(startY!=(nbTilesY-1)*sizeTilesY) edgeLabel.insert(LUT[curLabel]);
                }
              else
                {
                ++pixelIndex[1];
                LabelImagePixelType adjLabelY = labelImageROI->GetOutput()->GetPixel(pixelIndex);
                --pixelIndex[1];
                if(LUT[adjLabelY]!=LUT[curLabel])
                  {
                  if((nbPixels[LUT[curLabel]]>0)&&(nbPixels[LUT[curLabel]]==size))
                    {
                    adjMap[LUT[curLabel]].insert(LUT[adjLabelY]);
                    minLabel.insert(LUT[curLabel]);
                    }
                  if((nbPixels[LUT[adjLabelY]]>0)&&(nbPixels[LUT[adjLabelY]]==size))
                    {
                    adjMap[LUT[adjLabelY]].insert(LUT[curLabel]);
                    minLabel.insert(LUT[adjLabelY]); }
                  }
                }
              }

          //Searching the "nearest" region
          for(std::set<int>::iterator itMinLabel=minLabel.begin(); itMinLabel!=minLabel.end(); ++itMinLabel)
            {
            LabelImagePixelType curLabel = *itMinLabel, adjLabel(0);
            double err = itk::NumericTraits<double>::max();
            if(edgeLabel.count(curLabel)==0)
              {
              if(nbPixels[curLabel]==size)
                {
                edgeLabel.insert(curLabel);
                for(std::set<int>::iterator itAdjLabel=adjMap[curLabel].begin();
                    itAdjLabel!=adjMap[curLabel].end(); ++itAdjLabel)
                  {
                  double tmpError = 0;
                  LabelImagePixelType tmpLabel = *itAdjLabel;
                  if(tmpLabel!=curLabel)
                    {
                    for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
                      {
                      double curComp = static_cast<double>(sum[curLabel][comp])/nbPixels[curLabel];
                      int tmpComp = static_cast<double>(sum[tmpLabel][comp])/nbPixels[tmpLabel];
                      tmpError += (curComp-tmpComp)*(curComp-tmpComp);
                      }
                    if(tmpError<err)
                      {
                      err = tmpError;
                      adjLabel = tmpLabel;
                      }
                    }
                  }

                //Fusion of the two regions
                if(adjLabel!=curLabel)
                  {
                  unsigned int curLabelLUT = curLabel, adjLabelLUT = adjLabel;
                  while(LUTtmp[curLabelLUT] != curLabelLUT)
                    {
                    curLabelLUT = LUTtmp[curLabelLUT];
                    }
                  while(LUTtmp[adjLabelLUT] != adjLabelLUT)
                    {
                    adjLabelLUT = LUTtmp[adjLabelLUT];
                    }
                  if(curLabelLUT < adjLabelLUT)
                    {
                    LUTtmp[adjLabelLUT] = curLabelLUT;
                    }
                  else
                    {
                    LUTtmp[LUTtmp[curLabelLUT]] = adjLabelLUT; LUTtmp[curLabelLUT] = adjLabelLUT;
                    }
                  }
                }
              }
            }

          for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
            {
            LabelImagePixelType can = label;
            while(LUTtmp[can] != can)
              {
              can = LUTtmp[can];
              }
            LUTtmp[label] = can;
            }
          }
        }

      for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
        {
        LUT[label]=LUTtmp[label];
        if((nbPixels[label]!=0)&&(LUT[label]!=label))
          {
          nbPixels[LUT[label]]+=nbPixels[label];
          nbPixels[label]=0;
          for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
            {
            sum[LUT[label]][comp]+=sum[label][comp];
            }
          }
        }
      }
    //Relabelling
    m_ChangeLabelFilter = ChangeLabelImageFilterType::New();
    m_ChangeLabelFilter->SetInput(labelIn);
    for(LabelImagePixelType label = 1; label<regionCount+1; ++label)
      {
      if(label!=LUT[label])
        {
        m_ChangeLabelFilter->SetChange(label,LUT[label]);
        }
      }

    SetParameterOutputImage("out", m_ChangeLabelFilter->GetOutput());

    clock_t toc = clock();

    otbAppLogINFO(<<"Elapsed time: "<<(double)(toc - tic) / CLOCKS_PER_SEC<<" seconds");
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSSmallRegionsMerging)



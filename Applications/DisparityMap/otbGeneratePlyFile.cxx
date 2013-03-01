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

#include "itkImageRegionIteratorWithIndex.h"
#include "otbGenericRSTransform.h"
#include "otbBCOInterpolateImageFunction.h"

// MapProjection handler
#include "otbWrapperMapProjectionParametersHandler.h"

namespace otb
{
namespace Wrapper
{
class GeneratePlyFile : public Application
{
public:
  /** Standard class typedefs. */
  typedef GeneratePlyFile                     Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(GeneratePlyFile, otb::Application);

  typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>  InterpolatorType;
  typedef otb::GenericRSTransform<double,3,3>                     RSTransformType;
  typedef itk::ImageRegionIteratorWithIndex<FloatVectorImageType> IteratorType;

private:
  GeneratePlyFile(){}

  void DoInit()
  {
    SetName("GeneratePlyFile");
    SetDescription("Generate a 3D Ply file from a DEM and a color image.");

    SetDocName("Ply 3D files generation");
    SetDocLongDescription("Generate a 3D Ply file from a DEM and a color image.");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_InputImage,"indem","The input DEM");
    SetParameterDescription("indem", "The input DEM");

    AddParameter(ParameterType_Choice,"mode", "Conversion Mode");
    AddChoice("mode.dem","DEM");
    SetParameterDescription("mode.dem","DEM conversion mode");

    AddChoice("mode.3dgrid","3D grid");
    SetParameterDescription("mode.3dgrid","3D grid conversion mode");

    // Build the Output Map Projection
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "map");

    AddParameter(ParameterType_InputImage,"incolor","The input color image");
    SetParameterDescription("incolor", "The input color image");

    AddParameter(ParameterType_OutputFilename,"out","The output Ply file");
    SetParameterDescription("out","The output Ply file");

    // Doc example
    SetDocExampleParameterValue("indem","image_dem.tif");
    SetDocExampleParameterValue("out","out.ply");
    SetDocExampleParameterValue("incolor","image_color.tif");
  }

  void DoUpdateParameters()
  {
    // Update the UTM zone params
    MapProjectionParametersHandler::InitializeUTMParameters(this, "incolor", "map");
  }

  void DoExecute()
  {
    std::string outfname = GetParameterString("out");

    FloatVectorImageType::Pointer demPtr = this->GetParameterImage<FloatVectorImageType>("indem");
    demPtr->Update();

    FloatVectorImageType::Pointer colorPtr = this->GetParameterImage<FloatVectorImageType>("incolor");

    // First, find the footprint in the color image

    IteratorType it(demPtr,demPtr->GetLargestPossibleRegion());
    it.GoToBegin();

    FloatImageType::IndexType lr, ul;

    bool firstLoop = true;

    RSTransformType::Pointer rsTransform = RSTransformType::New();
    RSTransformType::Pointer toMap = RSTransformType::New();

    toMap->SetOutputProjectionRef(MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map"));


    if(GetParameterString("mode")=="dem")
        {
        otbAppLogINFO("DEM mode");
        rsTransform->SetInputProjectionRef(demPtr->GetProjectionRef());
        toMap->SetInputProjectionRef(demPtr->GetProjectionRef());
        }

    rsTransform->SetOutputProjectionRef(colorPtr->GetProjectionRef());
    rsTransform->SetOutputKeywordList(colorPtr->GetImageKeywordlist());
    rsTransform->InstanciateTransform();
    toMap->InstanciateTransform();

    unsigned long nbValidPoints = 0;

    // First pass is to find the color footprint
    while(!it.IsAtEnd())
      {
      RSTransformType::InputPointType dem3dPoint;

      bool valid = true;

      if(GetParameterString("mode")=="dem")
        {
        FloatImageType::PointType demPoint;
        demPtr->TransformIndexToPhysicalPoint(it.GetIndex(),demPoint);
        dem3dPoint[0]=demPoint[0];
        dem3dPoint[1]=demPoint[1];
        dem3dPoint[2]=it.Get()[0];
        }
      else
        {
        dem3dPoint[0]=it.Get()[0];
        dem3dPoint[1]=it.Get()[1];
        dem3dPoint[2]=it.Get()[2];

        if(it.Get()[4] < 1)
          {
          valid = false;
          }
        }

      if(valid)
        {
        ++nbValidPoints;

        RSTransformType::InputPointType color3dPoint = rsTransform->TransformPoint(dem3dPoint);



        FloatVectorImageType::PointType color2dPoint;
        color2dPoint[0] = color3dPoint[0];
        color2dPoint[1] = color3dPoint[1];

        FloatVectorImageType::IndexType color2dIndex;

        colorPtr->TransformPhysicalPointToIndex(color2dPoint,color2dIndex);

        // std::cout<<"DEM point: "<<dem3dPoint<<std::endl;
        // std::cout<<"Color point: "<<color3dPoint<<std::endl;
        // std::cout<<"Color index: "<<color2dIndex<<std::endl;
        // std::cout<<"Valid: "<<valid<<std::endl;
        // std::cout<<"Flag: "<<it.Get()[4]<<std::endl;

        if(colorPtr->GetLargestPossibleRegion().IsInside(color2dIndex))
          {

          if(firstLoop)
            {
            lr = color2dIndex;
            ul = color2dIndex;

            firstLoop = false;
            }
          else
            {
            ul[0] = std::min(ul[0],color2dIndex[0]);
            ul[1] = std::min(ul[1],color2dIndex[1]);
            lr[0] = std::max(lr[0],color2dIndex[0]);
            lr[1] = std::max(lr[1],color2dIndex[1]);
            }
          }
        }
      ++it;
      }

    FloatVectorImageType::RegionType region;
    region.SetIndex(ul);
    FloatVectorImageType::SizeType size;
    size[0] = static_cast<unsigned int>(lr[0]-ul[0]);
    size[1] = static_cast<unsigned int>(lr[1]-ul[1]);
    region.SetSize(size);

    otbAppLogINFO(<<"Number of valid points: "<<nbValidPoints);
    otbAppLogINFO(<<"Color region estimated: "<<region);

    // Now read the appropriate color region
    colorPtr->SetRequestedRegion(region);
    colorPtr->Update();

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    interpolator->SetInputImage(colorPtr);

    // Start writing ply file
    std::ofstream ofs(outfname.c_str());
    std::ostringstream oss;
    oss<<std::fixed;
    oss.precision(12);

    ofs<<"ply"<<std::endl;
    ofs<<"format ascii 1.0"<<std::endl;
    ofs<<"element vertex "<<nbValidPoints<<std::endl;
    ofs<<"property float x"<<std::endl;
    ofs<<"property float y"<<std::endl;
    ofs<<"property float z"<<std::endl;
    ofs<<"property uchar red"<<std::endl;
    ofs<<"property uchar green"<<std::endl;
    ofs<<"property uchar blue"<<std::endl;
    ofs<<"end_header"<<std::endl;


    // And loop again to generate the ply file
    it.GoToBegin();

     while(!it.IsAtEnd())
      {

      RSTransformType::InputPointType dem3dPoint;

      bool valid = true;

      if(GetParameterString("mode")=="dem")
        {
        FloatImageType::PointType demPoint;
        demPtr->TransformIndexToPhysicalPoint(it.GetIndex(),demPoint);
        dem3dPoint[0]=demPoint[0];
        dem3dPoint[1]=demPoint[1];
        dem3dPoint[2]=it.Get()[0];
        }
      else
        {
        dem3dPoint[0]=it.Get()[0];
        dem3dPoint[1]=it.Get()[1];
        dem3dPoint[2]=it.Get()[2];
        valid = (it.Get()[4]>0);
        }

      if(valid)
        {
        RSTransformType::InputPointType color3dPoint = rsTransform->TransformPoint(dem3dPoint);

        FloatVectorImageType::PointType color2dPoint;
        color2dPoint[0] = color3dPoint[0];
        color2dPoint[1] = color3dPoint[1];

        FloatVectorImageType::PixelType color = interpolator->Evaluate(color2dPoint);

        double red,green,blue;

        if(color.Size() == 4)
          {
          red = color[0];
          green = (0.9 * color[1] + 0.1 * color[3]);
          blue  = color[2];
          }
        else
          {
          red = color[0];
          green = red;
          blue = red;
          }

        // Clamp
        red = (red>255?255:(red<0?0:red));
        green = (green>255?255:(green<0?0:green));
        blue = (blue>255?255:(blue<0?0:blue));

        RSTransformType::InputPointType map3dPoint;

        map3dPoint = toMap->TransformPoint(dem3dPoint);

        oss.str("");
        oss<<map3dPoint[0]<<" "<<map3dPoint[1]<<" "<<map3dPoint[2]<<" "<<(int)red<<" "<<(int)green<<" " <<(int)blue<<std::endl;

        std::string tmp = oss.str();
        // std::replace(tmp.begin(),tmp.end(),'.',',');
        ofs<<tmp;
        }

      ++it;
      }
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::GeneratePlyFile)

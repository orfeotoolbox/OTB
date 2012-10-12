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

#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbMultiChannelGAndRIndexImageFilter.h"
#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "otbMultiChannelRAndBAndNIRIndexImageFilter.h"
#include "otbMultiChannelRAndGAndNIRIndexImageFilter.h"
#include "otbWaterIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"
#include "otbBuiltUpIndicesFunctor.h"


#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

class RadiometricIndices: public Application
{
public:
  /** Standard class typedefs. */
  typedef RadiometricIndices       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RadiometricIndices, otb::Wrapper::Application);

  /** Output  containers typedef */
  typedef ObjectList<itk::ProcessObject>                                    FilterListType;
  typedef ImageList<FloatImageType>                                         ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType> ImageListToVectorImageFilterType;

  /** Radiometric water indices functors typedef */
  typedef Functor::SRWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType>  SRWIFunctorType;
  typedef Functor::NDWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType>  NDWIFunctorType;
  typedef Functor::NDWI2<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDWI2FunctorType;
  typedef Functor::MNDWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> MNDWIFunctorType;
  typedef Functor::NDPI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType>  NDPIFunctorType;
  typedef Functor::NDTI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType>  NDTIFunctorType;
  //typedef Functor::WaterSqrtSpectralAngleFunctor<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> WaterSqrtSpectralAngleFunctor;

  /** Radiometric vegetation indices functors typedef */
  typedef Functor::NDVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDVIFunctor;
  typedef Functor::TNDVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> TNDVIFunctor;
  typedef Functor::RVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> RVIFunctor;
  typedef Functor::SAVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> SAVIFunctor;
  typedef Functor::TSAVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> TSAVIFunctor;
  typedef Functor::MSAVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> MSAVIFunctor;
  typedef Functor::MSAVI2<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> MSAVI2Functor;
  typedef Functor::GEMI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> GEMIFunctor;
  typedef Functor::IPVI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> IPVIFunctor;
  typedef Functor::LAIFromNDVILogarithmic<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> LAIFromNDVILogFunctor;
  typedef Functor::LAIFromReflectancesLinear<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> LAIFromReflLinearFunctor;
  typedef Functor::LAIFromNDVIFormosat2Functor<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> LAIFromNDVIFormoFunctor;

  /** Radiometric soil indices functors typedef */
  typedef Functor::IR<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> IRFunctor;
  typedef Functor::IC<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> ICFunctor;
  typedef Functor::IB<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> IBFunctor;
  typedef Functor::IB2<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType, FloatImageType::PixelType> IB2Functor;

  /** Radiometric built up indices functors typedef */
  typedef Functor::NDBI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDBIFunctor;

  /** Radiometric indices filters typedef */
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDWIFunctorType>  NDWIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDWI2FunctorType> NDWI2FilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, MNDWIFunctorType> MNDWIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDPIFunctorType>  NDPIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDTIFunctorType>  NDTIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, SRWIFunctorType>  SRWIFilterType;

  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, NDVIFunctor>              NDVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, TNDVIFunctor>             TNDVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, RVIFunctor>               RVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, SAVIFunctor>              SAVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, TSAVIFunctor>             TSAVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, MSAVIFunctor>             MSAVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, MSAVI2Functor>            MSAVI2FilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, GEMIFunctor>              GEMIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, IPVIFunctor>              IPVIFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromNDVILogFunctor>    LAIFromNDVILogFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromReflLinearFunctor> LAIFromReflLinearFilterType;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromNDVIFormoFunctor>  LAIFromNDVIFormoFilterType;

  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, IRFunctor>                  RIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, ICFunctor>                  CIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, IBFunctor>                  BIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, IB2Functor>                 BI2FilterType;

private:
  void DoInit()
  {
    SetName("RadiometricIndices");
    SetDescription("Compute radiometric indices.");

    // Documentation
    SetDocName("Radiometric Indices");
    SetDocLongDescription("This application computes radiometric indices using input image channels of the input image. The output image is a multi channel one which each channel is one of the selected index.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbVegetationIndicesFunctor, otbWaterIndicesFunctor and otbSoilIndicesFunctor classes");
 
    AddDocTag("Radiometric Indices");
    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Radiometric indices output image");

    AddRAMParameter();

    AddParameter(ParameterType_Group, "channels", "Channels selection");
    SetParameterDescription("channels", "Channels selection");
   
    AddParameter(ParameterType_Int,  "channels.blue",  "Blue Channel");
    SetParameterDescription("channels.blue", "Blue channel index");
    SetDefaultParameterInt("channels.blue", 1);
    AddParameter(ParameterType_Int,  "channels.green",  "Green Channel");
    SetParameterDescription("channels.green", "Green channel index");
    SetDefaultParameterInt("channels.green", 2);
    AddParameter(ParameterType_Int,  "channels.red",  "Red Channel");
    SetParameterDescription("channels.red", "Red channel index");
    SetDefaultParameterInt("channels.red", 3);
    AddParameter(ParameterType_Int,  "channels.nir",  "NIR Channel");
    SetParameterDescription("channels.nir", "NIR channel index");
    SetDefaultParameterInt("channels.nir", 4);
    AddParameter(ParameterType_Int,  "channels.mir",  "Mir Channel");
    SetParameterDescription("channels.mir", "Mir channel index");
    SetDefaultParameterInt("channels.mir", 5);
    //AddParameter(ParameterType_Int,  "channels.rho860",  "Rho860 Channel");
    //SetParameterDescription("channels.rho860", "860nm band channel index");
    //SetDefaultParameterInt("channels.rho860", 2);
    //AddParameter(ParameterType_Int,  "channels.rho1240",  "Rho1240 Channel");
    //SetParameterDescription("channels.rho1240", "1240nm band channel index");
    //SetDefaultParameterInt("channels.rho1240", 5);

    //AddParameter(ParameterType_Group, "index", "Indices selection");
    AddParameter(ParameterType_ListView,  "list", "Output Radiometric Indices");
    SetParameterDescription("list","List of wanted radiometric indices.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "RadiometricIndicesImages.tif");

    // Add Available choices
    m_Map.resize(21);
    for (unsigned int i=0; i<m_Map.size(); i++)
      {
      //key/item/#filter#Type/chan1/chan2
      m_Map[i].resize(6);
      }

    m_Map[0][0]="list.ndvi";
    m_Map[0][1]="Vegetation:NDVI";
    m_Map[0][2]="NDVI";
    m_Map[0][3]="red";
    m_Map[0][4]="nir";
    m_Map[0][5]="";

    m_Map[1][0]="list.tndvi";
    m_Map[1][1]="Vegetation:TNDVI";
    m_Map[1][2]="TNDVI";
    m_Map[1][3]="red";
    m_Map[1][4]="nir";
    m_Map[1][5]="";

    m_Map[2][0]="list.rvi";
    m_Map[2][1]="Vegetation:RVI";
    m_Map[2][2]="RVI";
    m_Map[2][3]="red";
    m_Map[2][4]="nir";
    m_Map[2][5]="";

    m_Map[3][0]="list.savi";
    m_Map[3][1]="Vegetation:SAVI";
    m_Map[3][2]="SAVI";
    m_Map[3][3]="red";
    m_Map[3][4]="nir";
    m_Map[3][5]="";

    m_Map[4][0]="list.tsavi";
    m_Map[4][1]="Vegetation:TSAVI";
    m_Map[4][2]="TSAVI";
    m_Map[4][3]="red";
    m_Map[4][4]="nir";
    m_Map[4][5]="";

    m_Map[5][0]="list.msavi";
    m_Map[5][1]="Vegetation:MSAVI";
    m_Map[5][2]="MSAVI";
    m_Map[5][3]="red";
    m_Map[5][4]="nir";
    m_Map[5][5]="";

    m_Map[6][0]="list.msavi2";
    m_Map[6][1]="Vegetation:MSAVI2";
    m_Map[6][2]="MSAVI2";
    m_Map[6][3]="red";
    m_Map[6][4]="nir";
    m_Map[6][5]="";

    m_Map[7][0]="list.gemi";
    m_Map[7][1]="Vegetation:GEMI";
    m_Map[7][2]="GEMI";
    m_Map[7][3]="red";
    m_Map[7][4]="nir";
    m_Map[7][5]="";

    m_Map[8][0]="list.ipvi";
    m_Map[8][1]="Vegetation:IPVI";
    m_Map[8][2]="IPVI";
    m_Map[8][3]="red";
    m_Map[8][4]="nir";
    m_Map[8][5]="";

    m_Map[9][0]="list.laindvilog";
    m_Map[9][1]="Vegetation:LAIFromNDVILog";
    m_Map[9][2]="LAIFromNDVILog";
    m_Map[9][3]="red";
    m_Map[9][4]="nir";
    m_Map[9][5]="";

    m_Map[10][0]="list.lairefl";
    m_Map[10][1]="Vegetation:LAIFromReflLinear";
    m_Map[10][2]="LAIFromReflLinear";
    m_Map[10][3]="red";
    m_Map[10][4]="nir";
    m_Map[10][5]="";

    m_Map[11][0]="list.laindviformo";
    m_Map[11][1]="Vegetation:LAIFromNDVIFormo";
    m_Map[11][2]="LAIFromNDVIFormo";
    m_Map[11][3]="red";
    m_Map[11][4]="nir";
    m_Map[11][5]="";

    m_Map[12][0]="list.ndwi";
    m_Map[12][1]="Water:NDWI";
    m_Map[12][2]="NDWI";
    m_Map[12][3]="nir";
    m_Map[12][4]="mir";
    m_Map[12][5]="";

    m_Map[13][0]="list.ndwi2";
    m_Map[13][1]="Water:NDWI2";
    m_Map[13][2]="NDWI2";
    m_Map[13][3]="green";
    m_Map[13][4]="nir";
    m_Map[13][5]="";

    m_Map[14][0]="list.mndwi";
    m_Map[14][1]="Water:MNDWI";
    m_Map[14][2]="MNDWI";
    m_Map[14][3]="green";
    m_Map[14][4]="mir";
    m_Map[14][5]="";

    m_Map[15][0]="list.ndpi";
    m_Map[15][1]="Water:NDPI";
    m_Map[15][2]="NDPI";
    m_Map[15][3]="mir";
    m_Map[15][4]="green";
    m_Map[15][5]="";

    m_Map[16][0]="list.ndti";
    m_Map[16][1]="Water:NDTI";
    m_Map[16][2]="NDPI";
    m_Map[16][3]="red";
    m_Map[16][4]="green";
    m_Map[16][5]="";
/*
    m_Map[17][0]="list.srwi";
    m_Map[17][1]="Water:SRWI";
    m_Map[17][2]="SRWI";
    m_Map[17][3]="rho860";
    m_Map[17][4]="rho1240";
    m_Map[17][5]="";
*/
    m_Map[17][0]="list.ri";
    m_Map[17][1]="Soil:RI";
    m_Map[17][2]="RI";
    m_Map[17][3]="red";
    m_Map[17][4]="green";
    m_Map[17][5]="";

    m_Map[18][0]="list.ci";
    m_Map[18][1]="Soil:CI";
    m_Map[18][2]="CI";
    m_Map[18][3]="red";
    m_Map[18][4]="green";
    m_Map[18][5]="";

    m_Map[19][0]="list.bi";
    m_Map[19][1]="Soil:BI";
    m_Map[19][2]="BI";
    m_Map[19][3]="red";
    m_Map[19][4]="green";
    m_Map[19][5]="";

    m_Map[20][0]="list.bi2";
    m_Map[20][1]="Soil:BI2";
    m_Map[20][2]="BI2";
    m_Map[20][3]="nir";
    m_Map[20][4]="red";
    m_Map[20][5]="green";

    ClearChoices("list");
    for ( unsigned int i=0 ; i<m_Map.size(); i++ )
      {
      AddChoice(m_Map[i][0],m_Map[i][1]);
      }
  }

  void DoUpdateParameters()
  {
    //Nothing to do here
    if ( HasValue("in") )
      {
      // Put the limit of the channel indices
      SetMinimumParameterIntValue("channels.blue", 1);
      SetMaximumParameterIntValue("channels.blue", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.green", 1);
      SetMaximumParameterIntValue("channels.green", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.red", 1);
      SetMaximumParameterIntValue("channels.red", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.nir", 1);
      SetMaximumParameterIntValue("channels.nir", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.mir", 1);
      SetMaximumParameterIntValue("channels.mir", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      //SetMinimumParameterIntValue("channels.rho860", 1);
      //SetMaximumParameterIntValue("channels.rho860", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      //SetMinimumParameterIntValue("channels.rho1240", 1);
      //SetMaximumParameterIntValue("channels.rho1240", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      }
  }

#define otbRadiometricWaterIndicesMacro( type )                         \
    {                                                                   \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New();\
    std::ostringstream oss;                                             \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][3];          \
    l_##type##Filter->GetFunctor().SetIndex1(this->GetParameterInt(oss.str()));\
    oss.str("");                                                        \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][4];          \
    l_##type##Filter->GetFunctor().SetIndex2(this->GetParameterInt(oss.str()));\
    l_##type##Filter->SetInput(inImage);                                \
    m_FilterList->PushBack( l_##type##Filter );                         \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );             \
    otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]][1] << " added.");\
    }
 
#define otbRadiometricVegetationIndicesMacro( type )                      \
    {                                                                     \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                               \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][3];            \
    l_##type##Filter->SetRedIndex(this->GetParameterInt(oss.str()));      \
    oss.str("");                                                          \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][4];            \
    l_##type##Filter->SetNIRIndex(this->GetParameterInt(oss.str()));      \
    l_##type##Filter->SetInput(inImage);                                  \
    m_FilterList->PushBack( l_##type##Filter );                           \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );               \
    otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]][1] << " added.");\
    }

#define otbRadiometricSoilIndicesMacro( type )                      \
    {                                                                     \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                               \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][3];            \
    l_##type##Filter->GetFunctor().SetRedIndex(this->GetParameterInt(oss.str()));      \
    oss.str("");                                                          \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][4];            \
    l_##type##Filter->GetFunctor().SetGreenIndex(this->GetParameterInt(oss.str()));      \
    l_##type##Filter->SetInput(inImage);                                  \
    m_FilterList->PushBack( l_##type##Filter );                           \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );               \
    otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]][1] << " added.");\
    }

  void DoExecute()
  {

    m_FilterList = FilterListType::New();
    m_ImageList  = ImageListType::New();
    m_Concatener = ImageListToVectorImageFilterType::New();

    FloatVectorImageType* inImage = GetParameterImage("in");

   for (unsigned int idx = 0; idx < GetSelectedItems("list").size(); ++idx)
      {
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:NDVI")
        otbRadiometricVegetationIndicesMacro(NDVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:TNDVI")
        otbRadiometricVegetationIndicesMacro(TNDVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:RVI")
        otbRadiometricVegetationIndicesMacro(RVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:SAVI")
        otbRadiometricVegetationIndicesMacro(SAVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:TSAVI")
        otbRadiometricVegetationIndicesMacro(TSAVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:MSAVI")
        otbRadiometricVegetationIndicesMacro(MSAVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:MSAVI2")
        otbRadiometricVegetationIndicesMacro(MSAVI2);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:GEMI")
        otbRadiometricVegetationIndicesMacro(GEMI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:IPVI")
        otbRadiometricVegetationIndicesMacro(IPVI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:LAIFromNDVILog")
        otbRadiometricVegetationIndicesMacro(LAIFromNDVILog);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:LAIFromReflLinear")
        otbRadiometricVegetationIndicesMacro(LAIFromReflLinear);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Vegetation:LAIFromNDVIFormo")
        otbRadiometricVegetationIndicesMacro(LAIFromNDVIFormo);

      if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:NDWI")
        otbRadiometricWaterIndicesMacro(NDWI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:NDWI2")
              otbRadiometricWaterIndicesMacro(NDWI2);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:MNDWI")
              otbRadiometricWaterIndicesMacro(MNDWI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:NDPI")
              otbRadiometricWaterIndicesMacro(NDPI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:NDTI")
              otbRadiometricWaterIndicesMacro(NDTI);
      //if (m_Map[GetSelectedItems("list")[idx]][1] == "Water:SRWI")
      //        otbRadiometricWaterIndicesMacro(SRWI);

      if (m_Map[GetSelectedItems("list")[idx]][1] == "Soil:RI")
        otbRadiometricSoilIndicesMacro(RI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Soil:CI")
        otbRadiometricSoilIndicesMacro(CI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Soil:BI")
        otbRadiometricSoilIndicesMacro(BI);
      if (m_Map[GetSelectedItems("list")[idx]][1] == "Soil:BI2")
        {
        BI2FilterType::Pointer l_BI2Filter = BI2FilterType::New();
        std::ostringstream oss;
        oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][3];
        l_BI2Filter->GetFunctor().SetNIRIndex(this->GetParameterInt(oss.str()));
        oss.str("");
        oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][4];
        l_BI2Filter->GetFunctor().SetRedIndex(this->GetParameterInt(oss.str()));
        oss.str("");
        oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]][5];
        l_BI2Filter->GetFunctor().SetGreenIndex(this->GetParameterInt(oss.str()));
        l_BI2Filter->SetInput(inImage);
        m_FilterList->PushBack( l_BI2Filter );
        m_ImageList->PushBack( l_BI2Filter->GetOutput() );
        otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]][1] << " added.");
        }
      }

    if( m_ImageList->Size() == 0 )
      {
      otbAppLogCRITICAL("No indices selected...");
      return;
      }
    
    m_Concatener->SetInput( m_ImageList );
    m_Concatener->UpdateOutputInformation();
    SetParameterOutputImage("out", m_Concatener->GetOutput());

  }
  
  FilterListType::Pointer                   m_FilterList;
  ImageListType::Pointer                    m_ImageList;
  ImageListToVectorImageFilterType::Pointer m_Concatener;
  std::vector<std::vector<std::string> >    m_Map;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RadiometricIndices)

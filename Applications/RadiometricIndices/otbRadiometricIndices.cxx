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

struct indiceSpec
{
    std::string key;
    std::string item;
    std::string description;
    std::string type;
    std::string chan1;
    std::string chan2;
    std::string chan3;
};


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
    SetDefaultParameterInt("channels.green", 1);
    AddParameter(ParameterType_Int,  "channels.red",  "Red Channel");
    SetParameterDescription("channels.red", "Red channel index");
    SetDefaultParameterInt("channels.red", 1);
    AddParameter(ParameterType_Int,  "channels.nir",  "NIR Channel");
    SetParameterDescription("channels.nir", "NIR channel index");
    SetDefaultParameterInt("channels.nir", 1);
    AddParameter(ParameterType_Int,  "channels.mir",  "Mir Channel");
    SetParameterDescription("channels.mir", "Mir channel index");
    SetDefaultParameterInt("channels.mir", 1);
    //AddParameter(ParameterType_Int,  "channels.rho860",  "Rho860 Channel");
    //SetParameterDescription("channels.rho860", "860nm band channel index");
    //SetDefaultParameterInt("channels.rho860", 1);
    //AddParameter(ParameterType_Int,  "channels.rho1240",  "Rho1240 Channel");
    //SetParameterDescription("channels.rho1240", "1240nm band channel index");
    //SetDefaultParameterInt("channels.rho1240", 1);

    //AddParameter(ParameterType_Group, "index", "Indices selection");
    AddParameter(ParameterType_ListView,  "list", "Available Radiometric Indices");
    SetParameterDescription("list","List of available radiometric indices.");


    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "RadiometricIndicesImages.tif");

    // Add Available choices
    struct indiceSpec s_NDVI;
    s_NDVI.key   = "list.ndvi";
    s_NDVI.item  = "Vegetation:NDVI";
    s_NDVI.description = "";
    s_NDVI.type  = "NDVI";
    s_NDVI.chan1 = "red";
    s_NDVI.chan2 = "nir";
    s_NDVI.chan3 = "";
    m_Map.push_back(s_NDVI);

    struct indiceSpec s_TNDVI;
    s_TNDVI.key   = "list.tndvi";
    s_TNDVI.item  = "Vegetation:TNDVI";
    s_TNDVI.description = "";
    s_TNDVI.type  = "TNDVI";
    s_TNDVI.chan1 = "red";
    s_TNDVI.chan2 = "nir";
    s_TNDVI.chan3 = "";
    m_Map.push_back(s_TNDVI);

    struct indiceSpec s_RVI;
    s_RVI.key   = "list.rvi";
    s_RVI.item  = "Vegetation:RVI";
    s_RVI.description = "";
    s_RVI.type  = "RVI";
    s_RVI.chan1 = "red";
    s_RVI.chan2 = "nir";
    s_RVI.chan3 = "";
    m_Map.push_back(s_RVI);

    struct indiceSpec s_SAVI;
    s_SAVI.key   = "list.savi";
    s_SAVI.item  = "Vegetation:SAVI";
    s_SAVI.description = "";
    s_SAVI.type  = "SAVI";
    s_SAVI.chan1 = "red";
    s_SAVI.chan2 = "nir";
    s_SAVI.chan3 = "";
    m_Map.push_back(s_SAVI);

    struct indiceSpec s_TSAVI;
    s_TSAVI.key   = "list.tsavi";
    s_TSAVI.item  = "Vegetation:TSAVI";
    s_TSAVI.description = "";
    s_TSAVI.type  = "TSAVI";
    s_TSAVI.chan1 = "red";
    s_TSAVI.chan2 = "nir";
    s_TSAVI.chan3 = "";
    m_Map.push_back(s_TSAVI);

    struct indiceSpec s_MSAVI;
    s_MSAVI.key   = "list.msavi";
    s_MSAVI.item  = "Vegetation:MSAVI";
    s_MSAVI.description = "";
    s_MSAVI.type  = "MSAVI";
    s_MSAVI.chan1 = "red";
    s_MSAVI.chan2 = "nir";
    s_MSAVI.chan3 = "";
    m_Map.push_back(s_MSAVI);

    struct indiceSpec s_MSAVI2;
    s_MSAVI2.key   = "list.msavi2";
    s_MSAVI2.item  = "Vegetation:MSAVI2";
    s_MSAVI2.description = "";
    s_MSAVI2.type  = "MSAVI2";
    s_MSAVI2.chan1 = "red";
    s_MSAVI2.chan2 = "nir";
    s_MSAVI2.chan3 = "";
    m_Map.push_back(s_MSAVI2);

    struct indiceSpec s_GEMI;
    s_GEMI.key   = "list.gemi";
    s_GEMI.item  = "Vegetation:GEMI";
    s_GEMI.description = "";
    s_GEMI.type  = "GEMI";
    s_GEMI.chan1 = "red";
    s_GEMI.chan2 = "nir";
    s_GEMI.chan3 = "";
    m_Map.push_back(s_GEMI);

    struct indiceSpec s_IPVI;
    s_IPVI.key   = "list.ipvi";
    s_IPVI.item  = "Vegetation:IPVI";
    s_IPVI.description = "";
    s_IPVI.type  = "IPVI";
    s_IPVI.chan1 = "red";
    s_IPVI.chan2 = "nir";
    s_IPVI.chan3 = "";
    m_Map.push_back(s_IPVI);

    struct indiceSpec s_LAIFromNDVILog;
    s_LAIFromNDVILog.key   = "list.laindvilog";
    s_LAIFromNDVILog.item  = "Vegetation:LAIFromNDVILog";
    s_LAIFromNDVILog.description = "";
    s_LAIFromNDVILog.type  = "LAIFromNDVILog";
    s_LAIFromNDVILog.chan1 = "red";
    s_LAIFromNDVILog.chan2 = "nir";
    s_LAIFromNDVILog.chan3 = "";
    m_Map.push_back(s_LAIFromNDVILog);

    struct indiceSpec s_LAIFromReflLinear;
    s_LAIFromReflLinear.key   = "list.lairefl";
    s_LAIFromReflLinear.item  = "Vegetation:LAIFromReflLinear";
    s_LAIFromReflLinear.description = "";
    s_LAIFromReflLinear.type  = "LAIFromReflLinear";
    s_LAIFromReflLinear.chan1 = "red";
    s_LAIFromReflLinear.chan2 = "nir";
    s_LAIFromReflLinear.chan3 = "";
    m_Map.push_back(s_LAIFromReflLinear);

    struct indiceSpec s_LAIFromNDVIFormo;
    s_LAIFromNDVIFormo.key   = "list.laindviformo";
    s_LAIFromNDVIFormo.item  = "Vegetation:LAIFromNDVIFormo";
    s_LAIFromNDVIFormo.description = "";
    s_LAIFromNDVIFormo.type  = "LAIFromNDVIFormo";
    s_LAIFromNDVIFormo.chan1 = "red";
    s_LAIFromNDVIFormo.chan2 = "nir";
    s_LAIFromNDVIFormo.chan3 = "";
    m_Map.push_back(s_LAIFromNDVIFormo);

    struct indiceSpec s_NDWI;
    s_NDWI.key   = "list.ndwi";
    s_NDWI.item  = "Water:NDWI";
    s_NDWI.description = "";
    s_NDWI.type  = "NDWI";
    s_NDWI.chan1 = "nir";
    s_NDWI.chan2 = "mir";
    s_NDWI.chan3 = "";
    m_Map.push_back(s_NDWI);

    struct indiceSpec s_NDWI2;
    s_NDWI2.key   = "list.ndwi2";
    s_NDWI2.item  = "Water:NDWI2";
    s_NDWI2.description = "";
    s_NDWI2.type  = "NDWI2";
    s_NDWI2.chan1 = "green";
    s_NDWI2.chan2 = "nir";
    s_NDWI2.chan3 = "";
    m_Map.push_back(s_NDWI2);

    struct indiceSpec s_MNDWI;
    s_MNDWI.key   = "list.mndwi";
    s_MNDWI.item  = "Water:MNDWI";
    s_MNDWI.description = "";
    s_MNDWI.type  = "MNDWI";
    s_MNDWI.chan1 = "green";
    s_MNDWI.chan2 = "mir";
    s_MNDWI.chan3 = "";
    m_Map.push_back(s_MNDWI);

    struct indiceSpec s_NDPI;
    s_NDPI.key   = "list.ndpi";
    s_NDPI.item  = "Water:NDPI";
    s_NDPI.description = "";
    s_NDPI.type  = "NDPI";
    s_NDPI.chan1 = "mir";
    s_NDPI.chan2 = "green";
    s_NDPI.chan3 = "";
    m_Map.push_back(s_NDPI);

    struct indiceSpec s_NDTI;
    s_NDTI.key   = "list.ndti";
    s_NDTI.item  = "Water:NDTI";
    s_NDTI.description = "";
    s_NDTI.type  = "NDTI";
    s_NDTI.chan1 = "red";
    s_NDTI.chan2 = "green";
    s_NDTI.chan3 = "";
    m_Map.push_back(s_NDTI);

    struct indiceSpec s_SRWI;
    s_SRWI.key   = "list.srwi";
    s_SRWI.item  = "Water:SRWI";
    s_SRWI.description = "";
    s_SRWI.type  = "SRWI";
    s_SRWI.chan1 = "rho860";
    s_SRWI.chan2 = "rho1240";
    s_SRWI.chan3 = "";
    //m_Map.push_back(s_SRWI);

    struct indiceSpec s_RI;
    s_RI.key   = "list.ri";
    s_RI.item  = "Soil:RI";
    s_RI.description = "";
    s_RI.type  = "RI";
    s_RI.chan1 = "red";
    s_RI.chan2 = "green";
    s_RI.chan3 = "";
    m_Map.push_back(s_RI);

    struct indiceSpec s_CI;
    s_CI.key   = "list.ci";
    s_CI.item  = "Soil:CI";
    s_CI.description = "";
    s_CI.type  = "CI";
    s_CI.chan1 = "red";
    s_CI.chan2 = "green";
    s_CI.chan3 = "";
    m_Map.push_back(s_CI);

    struct indiceSpec s_BI;
    s_BI.key   = "list.bi";
    s_BI.item  = "Soil:BI";
    s_BI.description = "";
    s_BI.type  = "BI";
    s_BI.chan1 = "red";
    s_BI.chan2 = "green";
    s_BI.chan3 = "";
    m_Map.push_back(s_BI);

    struct indiceSpec s_BI2;
    s_BI2.key   = "list.bi2";
    s_BI2.item  = "Soil:BI2";
    s_BI2.description = "";
    s_BI2.type  = "BI2";
    s_BI2.chan1 = "nir";
    s_BI2.chan2 = "red";
    s_BI2.chan3 = "green";
    m_Map.push_back(s_BI2);

    ClearChoices("list");
    for ( unsigned int i=0; i<m_Map.size(); i++ )
      {
      AddChoice(m_Map[i].key, m_Map[i].item);
      //SetParameterDescription(m_Map[i].item, m_Map[i].description);
      }
  }

  void DoUpdateParameters()
  {
    //Nothing to do here
  }

#define otbRadiometricWaterIndicesMacro( type )                           \
    {                                                                     \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                               \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan1;         \
    l_##type##Filter->GetFunctor().SetIndex1(this->GetParameterInt(oss.str()));\
    oss.str("");                                                          \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan2;         \
    l_##type##Filter->GetFunctor().SetIndex2(this->GetParameterInt(oss.str()));\
    l_##type##Filter->SetInput(inImage);                                  \
    m_FilterList->PushBack( l_##type##Filter );                           \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );               \
    otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]].item << " added.");\
    }
 
#define otbRadiometricVegetationIndicesMacro( type )                      \
    {                                                                     \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                               \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan1;         \
    l_##type##Filter->SetRedIndex(this->GetParameterInt(oss.str()));      \
    oss.str("");                                                          \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan2;         \
    l_##type##Filter->SetNIRIndex(this->GetParameterInt(oss.str()));      \
    l_##type##Filter->SetInput(inImage);                                  \
    m_FilterList->PushBack( l_##type##Filter );                           \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );               \
    otbAppLogINFO(<<m_Map[GetSelectedItems("list")[idx]].item<<" added.");\
    }

#define otbRadiometricSoilIndicesMacro( type )                            \
    {                                                                     \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                               \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan1;         \
    l_##type##Filter->GetFunctor().SetRedIndex(this->GetParameterInt(oss.str()));\
    oss.str("");                                                          \
    oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan2;         \
    l_##type##Filter->GetFunctor().SetGreenIndex(this->GetParameterInt(oss.str()));\
    l_##type##Filter->SetInput(inImage);                                  \
    m_FilterList->PushBack( l_##type##Filter );                           \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );               \
    otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]].item << " added.");\
    }

  void DoExecute()
  {

    unsigned int nbChan = GetParameterImage("in")->GetNumberOfComponentsPerPixel();

    if (   (this->GetParameterInt("channels.blue")  <= nbChan)
        && (this->GetParameterInt("channels.green") <= nbChan)
        && (this->GetParameterInt("channels.red")   <= nbChan)
        && (this->GetParameterInt("channels.nir")   <= nbChan)
        && (this->GetParameterInt("channels.mir")   <= nbChan))
      {

      m_FilterList = FilterListType::New();
      m_ImageList  = ImageListType::New();
      m_Concatener = ImageListToVectorImageFilterType::New();

      FloatVectorImageType* inImage = GetParameterImage("in");

      for (unsigned int idx = 0; idx < GetSelectedItems("list").size(); ++idx)
        {

        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:NDVI")
          otbRadiometricVegetationIndicesMacro(NDVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:TNDVI")
          otbRadiometricVegetationIndicesMacro(TNDVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:RVI")
          otbRadiometricVegetationIndicesMacro(RVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:SAVI")
          otbRadiometricVegetationIndicesMacro(SAVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:TSAVI")
          otbRadiometricVegetationIndicesMacro(TSAVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:MSAVI")
          otbRadiometricVegetationIndicesMacro(MSAVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:MSAVI2")
          otbRadiometricVegetationIndicesMacro(MSAVI2);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:GEMI")
          otbRadiometricVegetationIndicesMacro(GEMI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:IPVI")
          otbRadiometricVegetationIndicesMacro(IPVI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:LAIFromNDVILog")
          otbRadiometricVegetationIndicesMacro(LAIFromNDVILog);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:LAIFromReflLinear")
          otbRadiometricVegetationIndicesMacro(LAIFromReflLinear);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Vegetation:LAIFromNDVIFormo")
          otbRadiometricVegetationIndicesMacro(LAIFromNDVIFormo);

        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:NDWI")
          otbRadiometricWaterIndicesMacro(NDWI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:NDWI2")
          otbRadiometricWaterIndicesMacro(NDWI2);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:MNDWI")
          otbRadiometricWaterIndicesMacro(MNDWI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:NDPI")
          otbRadiometricWaterIndicesMacro(NDPI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:NDTI")
          otbRadiometricWaterIndicesMacro(NDTI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Water:SRWI")
          otbRadiometricWaterIndicesMacro(SRWI);

        if (m_Map[GetSelectedItems("list")[idx]].item == "Soil:RI")
          otbRadiometricSoilIndicesMacro(RI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Soil:CI")
          otbRadiometricSoilIndicesMacro(CI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Soil:BI")
          otbRadiometricSoilIndicesMacro(BI);
        if (m_Map[GetSelectedItems("list")[idx]].item == "Soil:BI2")
          {
          BI2FilterType::Pointer l_BI2Filter = BI2FilterType::New();
          std::ostringstream oss;
          oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan1;
          std::cout << "flag" << std::endl;
          l_BI2Filter->GetFunctor().SetNIRIndex(this->GetParameterInt(oss.str()));
          oss.str("");
          oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan2;
          l_BI2Filter->GetFunctor().SetRedIndex(this->GetParameterInt(oss.str()));
          oss.str("");
          oss<<"channels."<<m_Map[GetSelectedItems("list")[idx]].chan3;
          l_BI2Filter->GetFunctor().SetGreenIndex(this->GetParameterInt(oss.str()));
          l_BI2Filter->SetInput(inImage);
          m_FilterList->PushBack( l_BI2Filter );
          m_ImageList->PushBack( l_BI2Filter->GetOutput() );
          otbAppLogINFO(<< m_Map[GetSelectedItems("list")[idx]].item << " added.");
          }

        }

      if( m_ImageList->Size() == 0 )
        {
        otbAppLogCRITICAL("No indices selected...");
        }

      m_Concatener->SetInput( m_ImageList );
      m_Concatener->UpdateOutputInformation();

      SetParameterOutputImage("out", m_Concatener->GetOutput());
      }
    else
      {
      otbAppLogINFO("Wrong Band Number...");
      }

  }
  
  FilterListType::Pointer                   m_FilterList;
  ImageListType::Pointer                    m_ImageList;
  ImageListToVectorImageFilterType::Pointer m_Concatener;
  std::vector<struct indiceSpec>            m_Map;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RadiometricIndices)

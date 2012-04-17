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

#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

class RadiometricWaterIndices: public Application
{
public:
  /** Standard class typedefs. */
  typedef RadiometricWaterIndices  Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RadiometricWaterIndices, otb::Wrapper::Application);

  /** Output  containers typedef */
  typedef ObjectList<itk::ProcessObject>  FilterListType;
  typedef ImageList<FloatImageType> ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType> ImageListToVectorImageFilterType;

  /** Radiometric indices functors typedef */
  typedef Functor::SRWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> SRWIFunctorType;
  typedef Functor::NDWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDWIFunctorType;
  typedef Functor::NDWI2<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDWI2FunctorType;
  typedef Functor::MNDWI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> MNDWIFunctorType;
  typedef Functor::NDPI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDPIFunctorType;
  typedef Functor::NDTI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> NDTIFunctorType;
  //typedef Functor::WaterSqrtSpectralAngleFunctor<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType, FloatImageType::PixelType> WaterSqrtSpectralAngleFunctor;

  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, SRWIFunctorType>  SRWIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDWIFunctorType>  NDWIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDWI2FunctorType> NDWI2FilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, MNDWIFunctorType> MNDWIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDPIFunctorType>  NDPIFilterType;
  typedef itk::UnaryFunctorImageFilter<FloatVectorImageType, FloatImageType, NDTIFunctorType>  NDTIFilterType;

private:
  void DoInit()
  {
    SetName("RadiometricWaterIndices");
    SetDescription("Compute radiometric water indices.");

    // Documentation
    SetDocName("Radiometric Water");
    SetDocLongDescription("This application computes radiometric water indices using input image channels of the input image. The output image is a multi channel one which each channel is one of the selected index. The channel order is (if selected): NDWI, NDWI2, MNDWI, NDPI, NDTI, SRWI.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbWaterndices class");
 
    AddDocTag("Water Indices");
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
    SetDefaultParameterInt("channels.mir", 4);
    AddParameter(ParameterType_Int,  "channels.rho860",  "Rho860 Channel");
    SetParameterDescription("channels.rho860", "860nm band channel index");
    SetDefaultParameterInt("channels.rho860", 2);
    AddParameter(ParameterType_Int,  "channels.rho1240",  "Rho1240 Channel");
    SetParameterDescription("channels.rho1240", "1240nm band channel index");
    SetDefaultParameterInt("channels.rho1240", 5);

    AddParameter(ParameterType_Group, "index", "Indices selection");
      
    AddParameter(ParameterType_Empty, "index.ndwi", "NDWI");
    MandatoryOff("index.ndwi");
    SetParameterDescription("index.ndwi", " Normalized Difference Water Index");
    
    AddParameter(ParameterType_Empty, "index.ndwi2", "NDWI2");
    MandatoryOff("index.ndwi2");
    SetParameterDescription("index.ndwi2", " Normalized Difference Water Index 2");
 
    AddParameter(ParameterType_Empty, "index.mndwi", "MNDWI");
    MandatoryOff("index.mndwi");
    SetParameterDescription("index.mndwi", "Modified Normalized Difference Water Index");

    AddParameter(ParameterType_Empty, "index.ndpi", "NDPI");
    MandatoryOff("index.ndpi");
    SetParameterDescription("index.ndpi", "Normalized Difference Pond Index");

    AddParameter(ParameterType_Empty, "index.ndti", "NDTI");
    MandatoryOff("index.ndti");
    SetParameterDescription("index.ndti", "Normalized Difference Turbidity Index");

    AddParameter(ParameterType_Empty, "index.srwi", "SRWI");
    MandatoryOff("index.srwi");
      SetParameterDescription("index.srwi", "List of available radiometric indices, using Modis 860 and 1240 nm bands");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    /*SetDocExampleParameterValue("index.ndvi", "true");
    SetDocExampleParameterValue("index.rvi", "true");
    SetDocExampleParameterValue("index.ipvi", "true"); */
    SetDocExampleParameterValue("out", "RadiometricWaterIndicesImages.tif");
  }

  void DoUpdateParameters()
  {
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
      SetMinimumParameterIntValue("channels.rho860", 1);
      SetMaximumParameterIntValue("channels.rho860", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.rho1240", 1);
      SetMaximumParameterIntValue("channels.rho1240", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      }
  }

#define otbRadiometricWaterIndicesMacro( key, type, chan1, chan2  )     \
  if( this->IsParameterEnabled(key) )                                   \
    {                                                                   \
    type##FilterType::Pointer l_##type##Filter = type##FilterType::New(); \
    std::ostringstream oss;                                             \
    oss<<"channels."<<chan1;                                            \
    l_##type##Filter->GetFunctor().SetIndex1(this->GetParameterInt(oss.str())); \
    oss.str("");                                                        \
    oss<<"channels."<<chan2;                                            \
    l_##type##Filter->GetFunctor().SetIndex2(this->GetParameterInt(oss.str())); \
    l_##type##Filter->SetInput(inImage);                                \
    m_FilterList->PushBack( l_##type##Filter );                         \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );             \
    otbAppLogINFO(key << " indice added.");                             \
    }
 
  void DoExecute()
  {
    m_FilterList = FilterListType::New();
    m_ImageList = ImageListType::New();
    m_Concatener = ImageListToVectorImageFilterType::New();

    unsigned int redChannel = this->GetParameterInt("channels.red");
    unsigned int nirChannel = this->GetParameterInt("channels.nir");
    FloatVectorImageType* inImage = GetParameterImage("in");

    otbRadiometricWaterIndicesMacro("index.ndwi", NDWI, "nir", "mir");
    otbRadiometricWaterIndicesMacro("index.ndwi2", NDWI2, "green", "nir");
    otbRadiometricWaterIndicesMacro("index.mndwi", MNDWI, "green", "mir");
    otbRadiometricWaterIndicesMacro("index.ndpi", NDPI, "mir", "green");
    otbRadiometricWaterIndicesMacro("index.ndti", NDTI, "red", "green");
    otbRadiometricWaterIndicesMacro("index.srwi", SRWI, "rho860", "rho1240");
   
    if( m_ImageList->Size() == 0 )
      {
      otbAppLogCRITICAL("No indices selected...");
      return;
      }
    
    m_Concatener->SetInput( m_ImageList );
    m_Concatener->UpdateOutputInformation();
    SetParameterOutputImage("out", m_Concatener->GetOutput());
  }
  
  FilterListType::Pointer  m_FilterList;
  ImageListType::Pointer m_ImageList;
  ImageListToVectorImageFilterType::Pointer m_Concatener;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RadiometricWaterIndices)

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
#include "otbVegetationIndicesFunctor.h"

#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

class RadiometricVegetationIndices: public Application
{
public:
  /** Standard class typedefs. */
  typedef RadiometricVegetationIndices  Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RadiometricVegetationIndices, otb::Wrapper::Application);

  /** Output  containers typedef */
  typedef ObjectList<itk::ProcessObject>  FilterListType;
  typedef ImageList<FloatImageType> ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType> ImageListToVectorImageFilterType;

  /** Radiometric indices functors typedef */
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

  /** Radiometric indices filters typedef */
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, NDVIFunctor> NDVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, TNDVIFunctor> TNDVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, RVIFunctor> RVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, SAVIFunctor> SAVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, TSAVIFunctor> TSAVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, MSAVIFunctor> MSAVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, MSAVI2Functor> MSAVI2Filter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, GEMIFunctor> GEMIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, IPVIFunctor> IPVIFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromNDVILogFunctor> LAIFromNDVILogFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromReflLinearFunctor> LAIFromReflLinearFilter;
  typedef MultiChannelRAndNIRIndexImageFilter<FloatVectorImageType, FloatImageType, LAIFromNDVIFormoFunctor> LAIFromNDVIFormoFilter;

private:
  void DoInit()
  {
    SetName("RadiometricVegetationIndices");
    SetDescription("Compute radiometric indices based on Red and NIT channels.");

    // Documentation
    SetDocName("Radiometric Vegetation Application");
    SetDocLongDescription("This application computes radiometric indices that uses red and NIR channels of the input image. The output image is a multi channel one which each channel is one of the selected index. The channel order is the one of the selected indices.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbVecgetationIndices class");
 
    AddDocTag("Vegetation Indices");
    AddDocTag(Tags::FeatureExtraction);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image, with at least red and NIR channels");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Radiometric indices output image");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    AddParameter(ParameterType_Group, "channels", "Channels selection");
    SetParameterDescription("channels", "Channels selection");
    AddParameter(ParameterType_Int,  "channels.red",  "Red Channel");
    SetParameterDescription("channels.red", "Red channel index");
    SetDefaultParameterInt("channels.red", 3);
    AddParameter(ParameterType_Int,  "channels.nir",  "NIR Channel");
    SetParameterDescription("channels.nir", "NIR channel index");
    SetDefaultParameterInt("channels.nir", 4);
    
    AddParameter(ParameterType_Group, "index", "Indices selection");
    SetParameterDescription("index", "List of available radiometric indices");
    AddParameter(ParameterType_Empty, "index.ndvi", "NDVI");
    MandatoryOff("index.ndvi");
    SetParameterDescription("index.ndvi", "Normalized Difference Vegetation Index");
    
    AddParameter(ParameterType_Empty, "index.tndvi", "TNDVI");
    MandatoryOff("index.tndvi");
    SetParameterDescription("index.tndvi", " Transformed Normalized Difference Vegetation Index");
    
    AddParameter(ParameterType_Empty, "index.rvi", "RVI");
    MandatoryOff("index.rvi");
    SetParameterDescription("index.rvi", "Ratio Vegetation Index");

    AddParameter(ParameterType_Empty, "index.savi", "SAVI");
    MandatoryOff("index.savi");
    SetParameterDescription("index.savi", "Soil Adjusted Vegetation Index");

    AddParameter(ParameterType_Empty, "index.tsavi", "TSAVI");
    MandatoryOff("index.tsavi");
    SetParameterDescription("index.tsavi", "Transformed Soil Adjusted Vegetation Index");

    AddParameter(ParameterType_Empty, "index.msavi", "MSAVI");
    MandatoryOff("index.msavi");
    SetParameterDescription("index.msavi", "Modified Soil Adjusted Vegetation Index");

    AddParameter(ParameterType_Empty, "index.msavi2", "MSAVI2");
    MandatoryOff("index.msavi2");
    SetParameterDescription("index.msavi2", "Modified Soil Adjusted Vegetation Index 2");

    AddParameter(ParameterType_Empty, "index.gemi", "GEMI");
    MandatoryOff("index.gemi");
    SetParameterDescription("index.gemi", "Global Environment Monitoring Index");

    AddParameter(ParameterType_Empty, "index.ipvi", "IPVI");
    MandatoryOff("index.ipvi");
    SetParameterDescription("index.ipvi", "Infrared Percentage Vegetation Index");

    AddParameter(ParameterType_Empty, "index.laindvilog", "LAIFromNDVILog");
    MandatoryOff("index.laindvilog");
    SetParameterDescription("index.laindvilog", "Leaf Area Index from NDVI using a logarithmic relationship");

    AddParameter(ParameterType_Empty, "index.lairefl", "LAIFromReflLinear");
    MandatoryOff("index.lairefl");
    SetParameterDescription("index.lairefl", "Leaf Area Index from reflectance using a linear relationship");

    AddParameter(ParameterType_Empty, "index.laindviformo", "LAIFromNDVIFormo");
    MandatoryOff("index.laindviformo");
    SetParameterDescription("index.laindviformo", "Leaf Area Index from reflectance using using formula a*(exp(nir-red)/((red+nir)*b)-exp(c*b)), with a = 0.1519 b = 3.9443 c = 0.13. This formula is only valid for Formosat 2 reflectance TOCa linear relationship.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("index.ndvi", "true");
    SetDocExampleParameterValue("index.rvi", "true");
    SetDocExampleParameterValue("index.ipvi", "true");
    SetDocExampleParameterValue("out", "RadiometricVegetationIndicesImages.tif");
  }

  void DoUpdateParameters()
  {
    if ( HasValue("in") )
      {
       // Put the limit of the channel indices
      SetMinimumParameterIntValue("channels.red", 1);
      SetMaximumParameterIntValue("channels.red", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      SetMinimumParameterIntValue("channels.nir", 1);
      SetMaximumParameterIntValue("channels.nir", GetParameterImage("in")->GetNumberOfComponentsPerPixel());
      }
  }

#define otbRadiometricVegetationIndicesMacro( key, type )              \
  if( this->IsParameterEnabled(key) )                                  \
    {                                                                  \
    type##Filter::Pointer l_##type##Filter = type##Filter::New();      \
    l_##type##Filter->SetRedIndex(redChannel);                         \
    l_##type##Filter->SetNIRIndex(nirChannel);                         \
    l_##type##Filter->SetInput(inImage);                               \
    m_FilterList->PushBack( l_##type##Filter );                        \
    m_ImageList->PushBack( l_##type##Filter->GetOutput() );            \
    otbAppLogINFO(key << " indice added.");                            \
    }

  void DoExecute()
  {
    m_FilterList = FilterListType::New();
    m_ImageList = ImageListType::New();
    m_Concatener = ImageListToVectorImageFilterType::New();

    unsigned int redChannel = this->GetParameterInt("channels.red");
    unsigned int nirChannel = this->GetParameterInt("channels.nir");
    FloatVectorImageType* inImage = GetParameterImage("in");

    otbRadiometricVegetationIndicesMacro("index.ndvi", NDVI);
    otbRadiometricVegetationIndicesMacro("index.tndvi", TNDVI);
    otbRadiometricVegetationIndicesMacro("index.rvi", RVI);
    otbRadiometricVegetationIndicesMacro("index.savi", SAVI);
    otbRadiometricVegetationIndicesMacro("index.tsavi", TSAVI);
    otbRadiometricVegetationIndicesMacro("index.msavi", MSAVI);
    otbRadiometricVegetationIndicesMacro("index.msavi2", MSAVI2);
    otbRadiometricVegetationIndicesMacro("index.gemi", GEMI);
    otbRadiometricVegetationIndicesMacro("index.ipvi", IPVI);
    otbRadiometricVegetationIndicesMacro("index.laindvilog", LAIFromNDVILog);
    otbRadiometricVegetationIndicesMacro("index.lairefl", LAIFromReflLinear);
    otbRadiometricVegetationIndicesMacro("index.laindviformo", LAIFromNDVIFormo);
    
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

OTB_APPLICATION_EXPORT(otb::Wrapper::RadiometricVegetationIndices)

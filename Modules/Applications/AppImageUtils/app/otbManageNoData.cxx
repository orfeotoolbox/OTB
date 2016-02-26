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

#include "otbImageToNoDataMaskFilter.h"
#include "otbChangeNoDataValueFilter.h"
#include "itkMaskImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbChangeInformationImageFilter.h"

namespace otb
{
namespace Wrapper
{

class ManageNoData : public Application
{
public:
  /** Standard class typedefs. */
  typedef ManageNoData                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ManageNoData, otb::Application);

  /** Filters typedef */
  typedef otb::ImageToNoDataMaskFilter<FloatVectorImageType,UInt8ImageType> FilterType;
  typedef otb::ChangeNoDataValueFilter<FloatVectorImageType,FloatVectorImageType> ChangeNoDataFilterType;
  
  typedef otb::ImageList<FloatImageType> ImageListType;
  typedef otb::VectorImageToImageListFilter<FloatVectorImageType,ImageListType> VectorToListFilterType;
  typedef otb::ImageListToVectorImageFilter<ImageListType,FloatVectorImageType> ListToVectorFilterType;
  typedef itk::MaskImageFilter<FloatImageType,UInt8ImageType,FloatImageType> MaskFilterType;
  typedef otb::ChangeInformationImageFilter<FloatVectorImageType> ChangeInfoFilterType;

private:
  void DoInit()
  {
    SetName("ManageNoData");
    SetDescription("Manage No-Data");
    // Documentation
    SetDocName("No Data management");
    SetDocLongDescription("This application has two modes. The first allows building a mask of no-data pixels from the no-data flags read from the image file. The second allows updating the change the no-data value of an image (pixels value and metadata). This last mode also allows replacing NaN in images with a proper no-data value. To do so, one should activate the NaN is no-data option.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("BanMath");
    AddDocTag("Conversion");
    AddDocTag("Image Dynamic");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");

    AddParameter(ParameterType_Empty,"usenan", "Consider NaN as no-data");
    SetParameterDescription("usenan","If active, the application will consider NaN as no-data values as well");
    MandatoryOff("usenan");
    DisableParameter("usenan");
   
    AddParameter(ParameterType_Choice,"mode","No-data handling mode");
    SetParameterDescription("mode","Allows choosing between different no-data handling options");

    AddChoice("mode.buildmask","Build a no-data Mask");
    
    AddParameter(ParameterType_Float,"mode.buildmask.inv","Inside Value");
    SetParameterDescription("mode.buildmask.inv","Value given in the output mask to pixels that are not no data pixels");
    SetDefaultParameterInt("mode.buildmask.inv",1);

    AddParameter(ParameterType_Float,"mode.buildmask.outv","Outside Value");
    SetParameterDescription("mode.buildmask.outv","Value given in the output mask to pixels that are no data pixels");
    SetDefaultParameterInt("mode.buildmask.outv",0);

    AddChoice("mode.changevalue","Change the no-data value");

    AddParameter(ParameterType_Float,"mode.changevalue.newv","The new no-data value");
    SetParameterDescription("mode.changevalue.newv","The new no-data value");
    SetDefaultParameterInt("mode.changevalue.newv",0);

    AddChoice("mode.apply","Apply a mask as no-data");

    SetParameterDescription("mode.apply","Apply an external mask to an image using the no-data value of the input image");
    AddParameter(ParameterType_InputImage, "mode.apply.mask", "Mask image");
    SetParameterDescription("mode.apply.mask","Mask to be applied on input image (valid pixels have non null values)");

    SetParameterString("mode","buildmask");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "QB_Toulouse_Ortho_XS_nodatamask.tif uint8");
    SetDocExampleParameterValue("mode.buildmask.inv", "255");
    SetDocExampleParameterValue("mode.buildmask.outv", "0");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }


 void DoExecute()
  {
    FloatVectorImageType::Pointer inputPtr = this->GetParameterImage("in");
    
    m_Filter = FilterType::New();
    m_Filter->SetInsideValue(this->GetParameterFloat("mode.buildmask.inv"));
    m_Filter->SetOutsideValue(this->GetParameterFloat("mode.buildmask.outv"));
    m_Filter->SetNaNIsNoData(IsParameterEnabled("usenan"));
    m_Filter->SetInput(inputPtr);

    m_ChangeNoDataFilter = ChangeNoDataFilterType::New();
    m_ChangeNoDataFilter->SetInput(inputPtr);
    m_ChangeNoDataFilter->SetNaNIsNoData(IsParameterEnabled("usenan"));

    std::vector<double> newNoData(inputPtr->GetNumberOfComponentsPerPixel(),GetParameterFloat("mode.changevalue.newv"));

    m_ChangeNoDataFilter->SetNewNoDataValues(newNoData);
    
    if(GetParameterString("mode") == "buildmask")
      {
      SetParameterOutputImage("out",m_Filter->GetOutput());
      }
    else if(GetParameterString("mode") == "changevalue")
      {
      SetParameterOutputImage("out",m_ChangeNoDataFilter->GetOutput());
      }
    else if (GetParameterString("mode") == "apply")
      {
      m_MaskFilters.clear();
      UInt8ImageType::Pointer maskPtr = this->GetParameterImage<UInt8ImageType>("mode.apply.mask");
      unsigned int nbBands = inputPtr->GetNumberOfComponentsPerPixel();
      itk::MetaDataDictionary &dict = inputPtr->GetMetaDataDictionary();
      std::vector<bool> flags;
      std::vector<double> values;
      bool ret = otb::ReadNoDataFlags(dict,flags,values);
      if (!ret)
        {
        flags.resize(nbBands,true);
        values.resize(nbBands,0.0);
        }

      m_V2L = VectorToListFilterType::New();
      m_V2L->SetInput(inputPtr);
      ImageListType::Pointer inputList = m_V2L->GetOutput();
      inputList->UpdateOutputInformation();
      ImageListType::Pointer outputList = ImageListType::New();
      for (unsigned int i=0 ; i<nbBands ; ++i)
        {
        if (flags[i])
          {
          MaskFilterType::Pointer masker = MaskFilterType::New();
          masker->SetInput(inputList->GetNthElement(i));
          masker->SetMaskImage(maskPtr);
          masker->SetMaskingValue(0);
          masker->SetOutsideValue(values[i]);
          outputList->PushBack(masker->GetOutput());
          m_MaskFilters.push_back(masker);
          }
        else
          {
          outputList->PushBack(inputList->GetNthElement(i));
          }
        }
      m_L2V = ListToVectorFilterType::New();
      m_L2V->SetInput(outputList);
      if (!ret)
        {
        m_MetaDataChanger = ChangeInfoFilterType::New();
        m_MetaDataChanger->SetInput(m_L2V->GetOutput());
        m_MetaDataChanger->SetOutputMetaData<std::vector<bool> >(otb::MetaDataKey::NoDataValueAvailable,&flags);
        m_MetaDataChanger->SetOutputMetaData<std::vector<double> >(otb::MetaDataKey::NoDataValue,&values);
        SetParameterOutputImage("out",m_MetaDataChanger->GetOutput());
        }
      else
        {
        SetParameterOutputImage("out",m_L2V->GetOutput());
        }
      }
  }

  FilterType::Pointer m_Filter;
  ChangeNoDataFilterType::Pointer m_ChangeNoDataFilter;
  std::vector<MaskFilterType::Pointer> m_MaskFilters;
  VectorToListFilterType::Pointer m_V2L;
  ListToVectorFilterType::Pointer m_L2V;
  ChangeInfoFilterType::Pointer m_MetaDataChanger;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ManageNoData)


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

#include "otbFrostImageFilter.h"
#include "otbLeeImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class Despeckle : public Application
{
public:
  /** Standard class typedefs. */
  typedef Despeckle                           Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef itk::ImageToImageFilter<FloatImageType, FloatImageType> SpeckleFilterType;

  typedef LeeImageFilter<FloatImageType, FloatImageType>   LeeFilterType;
  typedef FrostImageFilter<FloatImageType, FloatImageType> FrostFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Despeckle, otb::Application);

private:
  void DoInit()
  {
    SetName("Despeckle");
    SetDescription("Perform speckle noise reduction on SAR image.");

    // Documentation
    SetDocName("Despeckle");
    SetDocLongDescription("This application reduce speckle noise. Two methods are available: Lee and Frost.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,"filter","speckle filtering method");
    AddChoice("filter.lee","Lee");
    SetParameterDescription("filter.lee","Lee filter");
    AddChoice("filter.frost","Frost");
    SetParameterDescription("filter.frost","Frost filter");

    AddParameter(ParameterType_Int,"filter.lee.rad","Radius");
    SetParameterDescription("filter.lee.rad","Radius for lee filter");

    AddParameter(ParameterType_Float,"filter.lee.nblooks","nb looks");
    SetParameterDescription("filter.lee.nblooks","Nb looks for lee filter");

    AddParameter(ParameterType_Int,"filter.frost.rad","Radius");
    SetParameterDescription("filter.frost.rad","Radius for frost filter");

    AddParameter(ParameterType_Float,"filter.frost.deramp","deramp");
    SetParameterDescription("filter.frost.deramp","Decrease factor declaration");

    // Default values
    SetDefaultParameterInt("filter.lee.rad", 1);
    SetDefaultParameterFloat("filter.lee.nblooks", 1.);
    SetDefaultParameterInt("filter.frost.rad",  1);
    SetDefaultParameterFloat("filter.frost.deramp",  0.1);


    // Doc example parameter settings
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("filter","lee");
    SetDocExampleParameterValue("filter.lee.rad", "5");
    SetDocExampleParameterValue("out", "despeckle.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType* inVImage = GetParameterImage("in");

    // Transform to otb::Image
    typedef otb::Image<FloatVectorImageType::InternalPixelType> FloatImageType;
    typedef otb::MultiToMonoChannelExtractROI<float,float> ExtractFilterType;

    ExtractFilterType::Pointer channelSelect = ExtractFilterType::New();
    m_Ref.push_back(channelSelect.GetPointer());
    channelSelect->SetChannel(1);
    channelSelect->SetInput(inVImage);
    channelSelect->UpdateOutputInformation();
    FloatImageType::Pointer inImage = channelSelect->GetOutput();
    inImage->UpdateOutputInformation();

    switch (GetParameterInt("filter"))
      {
      case 0:
      {
      LeeFilterType::Pointer  filter = LeeFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      LeeFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.lee.rad"));

      filter->SetRadius(lradius);
      filter->SetNbLooks(GetParameterFloat("filter.lee.nblooks"));

      otbAppLogINFO( << "Lee filter" );
      m_SpeckleFilter = filter;
      break;
      }
      case 1:
      {
      FrostFilterType::Pointer  filter = FrostFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      FrostFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.frost.rad"));

      filter->SetRadius(lradius);
      filter->SetDeramp(GetParameterFloat("filter.frost.deramp"));

      otbAppLogINFO( << "Frost filter" );
      m_SpeckleFilter = filter;
      break;
      }
      default:
      {
      otbAppLogFATAL(<<"non defined speckle reduction filter "<<GetParameterInt("filter")<<std::endl);
      break;
      }
      return;
      }

    SetParameterOutputImage("out", m_SpeckleFilter->GetOutput());
  }
  std::vector<itk::ProcessObject::Pointer> m_Ref;
  SpeckleFilterType::Pointer m_SpeckleFilter;
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::Despeckle)

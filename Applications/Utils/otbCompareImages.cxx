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

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbStreamingCompareImageFilter.h"

namespace otb
{
namespace Wrapper
{


class CompareImages : public Application
{
public:
  /** Standard class typedefs. */
  typedef CompareImages             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(CompareImages, otb::Application);

  /** Filters typedef */
  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType> ExtractROIMonoFilterType;
  typedef otb::StreamingCompareImageFilter<FloatImageType> StreamingCompareImageFilterType;

private:
  void DoInit()
  {
    SetName("CompareImages");
    SetDescription("Estimator between 2 images.");

    // Documentation
    SetDocName("Images comparaison");
    SetDocLongDescription("This application computes MSE (Mean Squared Error), MAE (Mean Absolute Error) and PSNR(Peak Signal to Noise Ratio) between the channel of two images (reference and measurement). The user has to set the used channel and can specified an ROI.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("BandMath application, ImageStatistics");
  
    AddDocTag("Statistics");

    AddParameter(ParameterType_Group, "ref", "Reference image properties");
    AddParameter(ParameterType_InputImage,  "ref.in",   "Reference image");
    SetParameterDescription("ref.in", "Image used as reference in the comparaison");
    AddParameter(ParameterType_Int,  "ref.channel",   "Reference image channel");
    SetParameterDescription("ref.channel", "Used channel for the reference image");
    SetDefaultParameterInt("ref.channel", 1);
    SetMinimumParameterIntValue("ref.channel", 1);

    AddParameter(ParameterType_Group, "meas", "Measured image properties");
    AddParameter(ParameterType_InputImage,  "meas.in",   "Measured image");
    SetParameterDescription("meas.in", "Image used as measured in the comparaison");
    AddParameter(ParameterType_Int,  "meas.channel",   "Measured image channel");
    SetParameterDescription("meas.channel", "Used channel for the measured image");
    SetDefaultParameterInt("meas.channel", 1);
    SetMinimumParameterIntValue("meas.channel", 1);

    AddParameter(ParameterType_Group, "roi", "Region Of Interest");
    AddParameter(ParameterType_Int,  "roi.startx", "Start X");
    SetDefaultParameterInt("roi.startx", 0);
    SetMinimumParameterIntValue("roi.startx", 0);

    SetParameterDescription("roi.startx", "ROI start x position.");
    AddParameter(ParameterType_Int,  "roi.starty", "Start Y");
    SetDefaultParameterInt("roi.starty", 0);
    SetMinimumParameterIntValue("roi.starty", 0);
    
    SetParameterDescription("roi.starty", "ROI start y position.");
    AddParameter(ParameterType_Int,  "roi.sizex",  "Size X");
    SetDefaultParameterInt("roi.sizex",  0);
    SetMinimumParameterIntValue("roi.sizex",  1);

    SetParameterDescription("roi.sizex","size along x in pixels.");
    AddParameter(ParameterType_Int,  "roi.sizey",  "Size Y");
    SetParameterDescription("roi.sizey","size along y in pixels.");
    SetDefaultParameterInt("roi.sizey", 0);
    SetMinimumParameterIntValue("roi.sizey",  1);

    AddParameter(ParameterType_Float, "mse",  "MSE");
    SetParameterDescription("mse", "Mean Squared Error value");
    SetParameterRole("mse", Role_Output );

    AddParameter(ParameterType_Float, "mae",  "MAE");
    SetParameterDescription("mae", "Mean Absolute Error value");
    SetParameterRole("mae", Role_Output );

    AddParameter(ParameterType_Float, "psnr",  "PSNR");
    SetParameterDescription("psnr", "Peak Signal to Noise Ratio value");
    SetParameterRole("psnr", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("ref.in", "GomaApres.png");
    SetDocExampleParameterValue("ref.channel", "1");
    SetDocExampleParameterValue("meas.in", "GomaAvant.png");
    SetDocExampleParameterValue("meas.channel", "2");
    SetDocExampleParameterValue("roi.startx", "20");
    SetDocExampleParameterValue("roi.starty", "30");
    SetDocExampleParameterValue("roi.sizex", "150");
    SetDocExampleParameterValue("roi.sizey", "200");
  }

  void DoUpdateParameters()
  {
    // Set channel interval
    if( HasValue("ref.in") )
      {
      SetMaximumParameterIntValue("ref.channel", this->GetParameterImage("ref.in")->GetNumberOfComponentsPerPixel());
      }
    if( HasValue("meas.in") )
      {
      SetMaximumParameterIntValue("meas.channel", this->GetParameterImage("meas.in")->GetNumberOfComponentsPerPixel());
      }

    // ROI
    FloatVectorImageType::RegionType region, userRegion;
    FloatVectorImageType::SizeType size;
    FloatVectorImageType::IndexType id;
    id[0] = this->GetParameterInt("roi.startx");
    id[1] = this->GetParameterInt("roi.starty");
    size[0] = this->GetParameterInt("roi.sizex");
    size[1] = this->GetParameterInt("roi.sizey");
    userRegion.SetSize(size);
    userRegion.SetIndex(id);
      
    if ( HasValue("ref.in") &&  HasValue("meas.in") )
      {
      region = this->GetParameterImage("ref.in")->GetLargestPossibleRegion();
      }
    else if ( HasValue("ref.in") &&  HasValue("meas.in") )
      {
      region = this->GetParameterImage("ref.in")->GetLargestPossibleRegion();
      }
    else if ( HasValue("meas.in") )
      {
      region = this->GetParameterImage("meas.in")->GetLargestPossibleRegion();
      }

    SetMaximumParameterIntValue("roi.startx", region.GetSize()[0]);
    SetMaximumParameterIntValue("roi.starty", region.GetSize()[1]);
    SetMaximumParameterIntValue("roi.sizex", region.GetSize()[0]-userRegion.GetIndex()[0]);
    SetMaximumParameterIntValue("roi.sizey", region.GetSize()[1]-userRegion.GetIndex()[1]);
  }

  void DoExecute()
  {
    // Init filters
    m_ExtractRefFilter = ExtractROIMonoFilterType::New();
    m_ExtractMeasFilter = ExtractROIMonoFilterType::New();
    m_CompareFilter = StreamingCompareImageFilterType::New();

    // Get input image pointers
    FloatVectorImageType::Pointer refIm = this->GetParameterImage("ref.in");
    FloatVectorImageType::Pointer measIm = this->GetParameterImage("meas.in");

    // Get the region of interest
    FloatVectorImageType::RegionType region;
    FloatVectorImageType::SizeType size;
    FloatVectorImageType::IndexType id;
    id[0] = this->GetParameterInt("roi.startx");
    id[1] = this->GetParameterInt("roi.starty");
    size[0] = this->GetParameterInt("roi.sizex");
    size[1] = this->GetParameterInt("roi.sizey");
    region.SetSize(size);
    region.SetIndex(id);

    if( !region.Crop(refIm->GetLargestPossibleRegion()) || !region.Crop(measIm->GetLargestPossibleRegion()) )
      {
      otbAppLogFATAL( << "ROI is not contained in the images");
      }

    m_ExtractRefFilter->SetInput( refIm );
    m_ExtractMeasFilter->SetInput( measIm );

    m_ExtractRefFilter->SetExtractionRegion(region);
    m_ExtractMeasFilter->SetExtractionRegion(region);

    // Set channels to extract
    m_ExtractRefFilter->SetChannel( this->GetParameterInt("ref.channel") );
    m_ExtractMeasFilter->SetChannel( this->GetParameterInt("meas.channel") );

    // Compute comparaison
    m_CompareFilter->SetInput1(m_ExtractRefFilter->GetOutput());
    m_CompareFilter->SetInput2(m_ExtractMeasFilter->GetOutput());
    m_CompareFilter->Update();

    // Show result
    otbAppLogINFO( << "MSE: " << m_CompareFilter->GetMSE() );
    otbAppLogINFO( << "MAE: " << m_CompareFilter->GetMAE() );
    otbAppLogINFO( << "PSNR: " << m_CompareFilter->GetPSNR() );

    SetParameterFloat( "mse", m_CompareFilter->GetMSE() );
    SetParameterFloat( "mae", m_CompareFilter->GetMAE() );
    SetParameterFloat( "psnr", m_CompareFilter->GetPSNR() );
  }
  
  
  ExtractROIMonoFilterType::Pointer m_ExtractRefFilter;
  ExtractROIMonoFilterType::Pointer m_ExtractMeasFilter;
  StreamingCompareImageFilterType::Pointer m_CompareFilter;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CompareImages)

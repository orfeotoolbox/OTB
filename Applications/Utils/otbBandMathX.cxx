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

#include "otbObjectList.h"

#include "otbBandMathImageFilterX.h"

namespace otb
{

namespace Wrapper
{

class BandMathX : public Application
{

public:
  /** Standard class typedefs. */
  typedef BandMathX                      Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BandMathX, otb::Application);


  typedef otb::BandMathImageFilterX<FloatVectorImageType>                             BandMathImageFilterType;


private:
  void DoInit()
  {
    SetName("BandMathX");
    SetDescription("Perform a mathematical operation on multiband images");

    SetDocName("Band Math X");
    SetDocLongDescription("This application performs a mathematical operation on multiband images."
      " Mathematical formula interpretation is done via MuParserX libraries https://code.google.com/p/muparserx/");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    AddDocTag("Util");

    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    SetParameterDescription("il", "Image list to perform computation on.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_String, "exp", "Expression");
    SetParameterDescription("exp",
                            "The mathematical expression to apply.");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif");
    SetDocExampleParameterValue("out", "apTvUtBandMathOutput.tif");
    SetDocExampleParameterValue("exp", "\"cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3, im3b4)\"");
  }

  void DoUpdateParameters()
  {

  }

  void DoExecute()
  {
    // Get the input image list
    FloatVectorImageListType::Pointer inList = GetParameterImageList("il");

    // checking the input images list validity
    const unsigned int nbImages = inList->Size();

    if (nbImages == 0)
      {
       itkExceptionMacro("No input Image set...; please set at least one input image");
      }

    //m_ChannelExtractorList = ExtractROIFilterListType::New();
    m_Filter               = BandMathImageFilterType::New();


    for (unsigned int i = 0; i < nbImages; i++)
      {
        FloatVectorImageType::Pointer currentImage = inList->GetNthElement(i);
        currentImage->UpdateOutputInformation();

        otbAppLogINFO( << "Image #" << i + 1 << " has "
                       << currentImage->GetNumberOfComponentsPerPixel()
                       << " components" << std::endl );

        m_Filter->SetNthInput(i,currentImage);

      }

    m_Filter->SetExpression(GetParameterString("exp"));

    // Set the output image
    SetParameterOutputImage("out", m_Filter->GetOutput());
  }

  BandMathImageFilterType::Pointer  m_Filter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::BandMathX)



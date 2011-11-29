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

namespace otb
{
namespace Wrapper
{

class ReadImageInfo : public Application
{
public:
  /** Standard class typedefs. */
  typedef ReadImageInfo                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ReadImageInfo, otb::Application);

private:
  ReadImageInfo()
  {
    SetName("ReadImageInfo");
    SetDescription("Get information about the image");

    // Documentation
    SetDocName("read image information Application");
    SetDocLongDescription("Display informations about the image in the logger tab.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

     AddDocTag("Util");
     AddDocTag(Tags::Meta);
  }

  virtual ~ReadImageInfo()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    std::ostringstream oss;
    oss << std::setprecision(15);
    oss << inImage;
    oss << std::endl;

    this->GetLogger()->Info( oss.str() );
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ReadImageInfo)


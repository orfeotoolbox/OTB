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

#include "otbImageFileReader.h"

namespace otb
{
namespace Wrapper
{


class InternalReader : public Application
{
public:
  /** Standard class typedefs. */
  typedef InternalReader                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(InternalReader, otb::Application);

  /** Filters typedef */
  typedef otb::ImageFileReader<VectorImageType>  ReaderFilterType;

private:
   InternalReader()
  {
    SetName("InternalReader");
    SetDescription("Read an image.");
    m_Reader = ReaderFilterType::New();
  }

  virtual ~InternalReader()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");

    AddParameter(ParameterType_String, "inname", "Input file name");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  { 
    m_Reader->SetFileName( GetParameterString("inname") );
    
    SetParameterOutputImage("out", m_Reader->GetOutput());
  }
  
  ReaderFilterType::Pointer m_Reader;
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::InternalReader)


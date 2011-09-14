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


class AllocateOutput : public Application
{
public:
  /** Standard class typedefs. */
  typedef AllocateOutput                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(AllocateOutput, otb::Application);


private:
   AllocateOutput()
  {
    SetName("AllocateOutput");
    m_Image = FloatVectorImageType::New();
    SetDescription("Allocate output image inside the process.");
  }

  virtual ~AllocateOutput()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  { 
    FloatVectorImageType::Pointer inImage = FloatVectorImageType::New();
    
    FloatVectorImageType::IndexType start;
    start[0] =   0;  // first index on X
    start[1] =   0;  // first index on Y

    FloatVectorImageType::SizeType size;
    size[0]  = 10;  // size along X
    size[1]  = 10;

    FloatVectorImageType::RegionType region;
    
    region.SetSize(size);
    region.SetIndex(start);
    
    m_Image->SetRegions(region);
    m_Image->SetNumberOfComponentsPerPixel( 3 );
    m_Image->Allocate();

    FloatVectorImageType::PixelType initialValue;
    initialValue.SetSize(3);
    initialValue.Fill(0);
    m_Image->FillBuffer(initialValue);

    SetParameterOutputImage("out", m_Image);
  }

  FloatVectorImageType::Pointer m_Image;
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::AllocateOutput)


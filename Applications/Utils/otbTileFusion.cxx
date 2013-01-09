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

#include "otbTileImageFilter.h"

namespace otb
{
namespace Wrapper
{

class TileFusion : public Application
{
public:
  /** Standard class typedefs. */
  typedef TileFusion                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(TileFusion, otb::Application);

  typedef otb::TileImageFilter<FloatVectorImageType> TileFilterType;

private:
  void DoInit()
  {
    SetName("TileFusion");
    SetDescription("Fusion of an image made of several tile files.");

    // Documentation
    SetDocName("Image TileFusion");
    SetDocLongDescription("Concatenate several tile files into a single image file.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImageList,  "il",   "Input Tile Images");
    SetParameterDescription("il", "Input tiles to concatenate (in lexicographic order : (0,0) (1,0) (0,1) (1,1)).");
    
    AddParameter(ParameterType_Int, "cols", "Number of tile columns");
    SetParameterDescription("cols", "Number of columns in the tile array");
    
    AddParameter(ParameterType_Int, "rows", "Number of tile rows");
    SetParameterDescription("rows", "Number of rows in the tile array");
    
    AddParameter(ParameterType_OutputImage,  "out",   "Output Image");
    SetParameterDescription("out", "Output entire image");
    
   // Doc example parameter settings
    SetDocExampleParameterValue("il", "Scene_R1C1.tif Scene_R1C2.tif Scene_R2C1.tif Scene_R2C2.tif");
    SetDocExampleParameterValue("cols","2");
    SetDocExampleParameterValue("rows","2");
    SetDocExampleParameterValue("out", "EntireImage.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to be done
  }

  void DoExecute()
  {
    // Get the input image list
    FloatVectorImageListType::Pointer tileList = this->GetParameterImageList("il");

    if( tileList->Size() == 0 )
      {
      itkExceptionMacro("No input Image set...");
      }
    
    m_FusionFilter = TileFilterType::New();

    TileFilterType::SizeType layout;
    layout[0] = this->GetParameterInt("cols");
    layout[1] = this->GetParameterInt("rows");
    m_FusionFilter->SetLayout(layout);
    
    for (unsigned int i=0; i<(layout[0]*layout[1]); i++)
      {
      m_FusionFilter->SetInput(i,tileList->GetNthElement(i));
      }
    
    SetParameterOutputImage("out", m_FusionFilter->GetOutput());
  }
  
  TileFilterType::Pointer m_FusionFilter;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TileFusion)

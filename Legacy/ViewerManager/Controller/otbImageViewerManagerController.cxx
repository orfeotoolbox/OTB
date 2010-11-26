
/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageViewerManagerController.h"
#include "otbMsgReporter.h"

namespace otb
{

ImageViewerManagerController
::ImageViewerManagerController()
{

/** NewVisu */
  // Build a "visu"controller
  m_VisuController = VisuControllerType::New();
  m_PreviewVisuController = VisuControllerType::New();
}

ImageViewerManagerController
::~ImageViewerManagerController()
{
  MsgReporter::GetInstance()->Hide();
}



unsigned int
ImageViewerManagerController
::OpenInputImage(const char * filename)
{
  unsigned int numberOfOpenedImages = 0;
  try
  {
    std::string strFilename = filename;
    numberOfOpenedImages = m_Model->OpenImage( strFilename );
  }
  catch ( ... )
  {
    try
    {
      std::string strFilename = filename;
      numberOfOpenedImages = m_Model->OpenImageList( strFilename );
    }
    catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
  }

  return numberOfOpenedImages;
}

/**
*
*/
void
ImageViewerManagerController
::CloseImage(unsigned int selectedItem)
{
  try
  {
    m_Model->CloseImage( selectedItem );
  }
  catch (itk::ExceptionObject & err)
  {
    MsgReporter::GetInstance()->SendError(err.GetDescription());
  }
}

void
ImageViewerManagerController
::UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem)
{
  try
    {

    m_Model->UpdateRGBChannelOrder(redChoice,greenChoice,BlueChoice, selectedItem);
  }
  catch (itk::ExceptionObject & err)
  {
    MsgReporter::GetInstance()->SendError(err.GetDescription());
  }
}

void
ImageViewerManagerController
::UpdateGrayScaleChannelOrder(int choice, unsigned int selectedItem)
{
  try
    {

    m_Model->UpdateGrayScaleChannelOrder(choice, selectedItem);
  }
  catch (itk::ExceptionObject & err)
  {
    MsgReporter::GetInstance()->SendError(err.GetDescription());
  }
}

void
ImageViewerManagerController
::UpdateAmplitudeChannelOrder(int realChoice, int imChoice, unsigned int selectedItem)
{
  try
    {
      m_Model->UpdateAmplitudeChannelOrder(realChoice,imChoice, selectedItem);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ImageViewerManagerController
::UpdatePhaseChannelOrder(int realChoice, int imChoice, unsigned int selectedItem)
{
  try
    {
      m_Model->UpdatePhaseChannelOrder(realChoice,imChoice, selectedItem);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ImageViewerManagerController
::Link(unsigned int leftChoice, unsigned int rightChoice, OffsetType offset)
{
  try
    {
      m_Model->Link(leftChoice,rightChoice,offset);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ImageViewerManagerController
::UpdateImageViewController(unsigned int selectedItem)
{
  try
    {
      m_Model->InitializeImageViewController(selectedItem);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}


} // end namespace otb


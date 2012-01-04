
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
#include "otbViewerController.h"
#include "otbMsgReporter.h"

#include "otbSystem.h"

namespace otb
{

ViewerController
::ViewerController()
{

/** NewVisu */
  // Build a "visu"controller
  m_VisuController = VisuControllerType::New();
  m_PreviewVisuController = VisuControllerType::New();
}

ViewerController
::~ViewerController()
{
  MsgReporter::GetInstance()->Hide();
}


unsigned int
ViewerController
::OpenInputImage(const char * filename, const unsigned int & id)
{
  unsigned int numberOfOpenedImages = 0;
  
  // Try to distinguish a jpeg2000 file whith the resoltion: filename:resId
  std::string realFile(filename);
  unsigned int addNum(0);
  System::ParseFileNameForAdditonalInfo( filename, realFile, addNum );
  
  if( filename != realFile )
    {
    if( m_Model->IsJPEG2000File( realFile ) )
      {
      try
        {
        numberOfOpenedImages = m_Model->OpenImage( realFile, addNum );
        return numberOfOpenedImages;
        }
      catch (itk::ExceptionObject & err)
        {
        MsgReporter::GetInstance()->SendError(err.GetDescription());
        }
      }
    }

  try
  {
    std::string strFilename = filename;
    numberOfOpenedImages = m_Model->OpenImage( strFilename, id );
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
ViewerController
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
ViewerController
::UpdateRGBChannelOrder(int redChoice , int greenChoice, int BlueChoice, unsigned int selectedItem)
{
  try
    {

    m_Model->UpdateRGBChannelOrder(redChoice, greenChoice, BlueChoice, selectedItem);
  }
  catch (itk::ExceptionObject & err)
  {
    MsgReporter::GetInstance()->SendError(err.GetDescription());
  }
}

void
ViewerController
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
ViewerController
::UpdateAmplitudeChannelOrder(int realChoice, int imChoice, unsigned int selectedItem)
{
  try
    {
      m_Model->UpdateAmplitudeChannelOrder(realChoice, imChoice, selectedItem);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ViewerController
::UpdatePhaseChannelOrder(int realChoice, int imChoice, unsigned int selectedItem)
{
  try
    {
      m_Model->UpdatePhaseChannelOrder(realChoice, imChoice, selectedItem);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ViewerController
::Link(unsigned int leftChoice, unsigned int rightChoice, OffsetType offset)
{
  try
    {
      m_Model->Link(leftChoice, rightChoice, offset);
    }
  catch (itk::ExceptionObject & err)
    {
      MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

void
ViewerController
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

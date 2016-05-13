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
#include "otbImageSettings.h"
#include "otbFragmentShaderRegistry.h"
#include "otbGlVersionChecker.h"
#include <GL/glew.h>

namespace otb
{

ImageSettings::ImageSettings()
  : m_MinRed( 0 ),
    m_MaxRed( 255 ),
    m_MinGreen( 0 ),
    m_MaxGreen( 255 ),
    m_MinBlue( 0 ),
    m_MaxBlue( 255 ),
    m_UseNoData( true ),
    m_NoData( 0 ),
    m_Gamma( 1. ),
    m_Alpha( 1. ),
    m_CurrentRed( 0 ),
    m_CurrentGreen( 0 ),
    m_CurrentBlue( 0 )
{
}


ImageSettings::~ImageSettings()
{
}


} // End namespace otb

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

#include "otbSarSensorModelAdapter.h"

#include <cassert>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "ossim/ossimSarSensorModel.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimSarSensorModel.h"
#endif


namespace otb
{

SarSensorModelAdapter::SarSensorModelAdapter():
  m_SensorModel(ITK_NULLPTR)
{
}

SarSensorModelAdapter::~SarSensorModelAdapter()
{
  if(m_SensorModel != ITK_NULLPTR)
    delete m_SensorModel;
}

bool SarSensorModelAdapter::LoadState(const ImageKeywordlist& image_kwl)
{
  InternalModelPointer model = new ossimplugins::ossimSarSensorModel();

   ossimKeywordlist geom;
   image_kwl.convertToOSSIMKeywordlist(geom);
  
   bool success = model->loadState(geom);

   if(success)
     {
     m_SensorModel = model;
     return true;
     }

   delete model;
   return false;
}

bool SarSensorModelAdapter::SaveState(ImageKeywordlist & image_kwl)
{
  if(m_SensorModel != ITK_NULLPTR)
    {
    ossimKeywordlist geom;

    bool success = m_SensorModel->saveState(geom);

    if(success)
      {
      image_kwl.Clear();
      image_kwl.SetKeywordlist(geom);
      
      }
    return success;
    }

  return false;

  
}

bool SarSensorModelAdapter::IsValidSensorModel() const
{
  return m_SensorModel != ITK_NULLPTR;
}

bool SarSensorModelAdapter::Deburst(std::vector<std::pair<unsigned long, unsigned long>> & lines)
{
  if(m_SensorModel != ITK_NULLPTR)
    {
    return m_SensorModel->deburst(lines);
    }
  
  return false;
}

} // namespace otb

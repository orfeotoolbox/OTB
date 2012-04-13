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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRLayerWrapper.h"
#include <cassert>
#include <boost/bind.hpp> 
#include "ogrsf_frmts.h" // OGRDataSource & OGRLayer

/*===========================================================================*/
/*==============================[ other stuff ]==============================*/
/*===========================================================================*/

namespace  { // Anonymous namespace
  /**\ingroup Geometry
   * Deleter for \c boost::shared_ptr<> that doesn't delete.
   * This is required for \c OGRLayer s that belong to \c OGRDataSource.
   * \internal
   */
  struct LeaveAloneDeleter
    {
    void operator()(OGRLayer*) const {}
    };
} // Anonymous namespace 


otb::ogr::Layer::Layer(OGRLayer* layer)
: m_Layer(layer, LeaveAloneDeleter())
{
}

otb::ogr::Layer::Layer(OGRLayer* layer, OGRDataSource* sourceInChargeOfLifeTime)
: m_Layer(layer,  boost::bind(&OGRDataSource::ReleaseResultSet, sourceInChargeOfLifeTime, _1))
{
  assert(layer && "A null OGRlayer cannot belong to an OGRDataSource" );
  // OGR always refuses "delete 0". *sigh*
}

int otb::ogr::Layer::GetFeatureCount(bool doForceComputation) const
{
  assert(m_Layer);
  return m_Layer->GetFeatureCount(doForceComputation);
}

std::string otb::ogr::Layer::GetName() const
{
  assert(m_Layer && "null layer");
  return m_Layer->GetName();
}

OGRLayer & otb::ogr::Layer::ogr()
{
  assert(m_Layer && "OGRLayer not initialized");
  return *m_Layer;
}

void otb::ogr::Layer::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "+";
  if (m_Layer) // in case for odd reason the layer that should exist can't be found
    {
    os << "Layer <" << m_Layer->GetName() << "\n";
    // + for each feature
    }
  else
    {
    os << "null Layer\n";
    }
}


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
#ifndef __otbOGRFeatureWrapper_txx
#define __otbOGRFeatureWrapper_txx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/

/*===========================================================================*/
/*================================[ Feature ]================================*/
/*===========================================================================*/
inline
OGRFeature & otb::ogr::Feature::ogr() const
{ // not returning a OGRFeature const& because OGR is not const-correct
  CheckInvariants();
  return *m_Feature;
}

inline
OGRFeature & otb::ogr::Feature::ogr()
{
  CheckInvariants();
  return *m_Feature;
}

inline
void otb::ogr::Feature::CheckInvariants() const
{
  assert(m_Feature && "OGRFeature can't be null");
}

#endif // __otbOGRFeatureWrapper_txx

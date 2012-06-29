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
#include "otbGeometriesProjectionFilter.h"


/*===========================================================================*/
/*====================[ ReprojectTransformationFunctor ]=====================*/
/*===========================================================================*/

template <typename TGeometry>
inline
otb::ogr::UniqueGeometryPtr
otb::internal::ReprojectTransformationFunctor::ByCopy::operator()(TGeometry const* in) const
{
  boost::interprocess::unique_ptr<TGeometry, ogr::internal::GeometryDeleter>
    out(in ? static_cast <TGeometry*>(in->clone()) : 0); // OGR clone doesn't use covariant return ...
  if (out)
    m_Reprojector.do_transform(*out);
  ogr::UniqueGeometryPtr res(out.release());
  return otb::move(res);
}

template <typename TGeometry>
inline
void
otb::internal::ReprojectTransformationFunctor::InPlace::operator()(TGeometry * inout) const
{
  if (inout)
    m_Reprojector.do_transform(*inout);
}

inline
void otb::internal::ReprojectTransformationFunctor::SetOnePointTransformation(InternalTransformPointerType transform)
{
  m_Transform = transform;
}


/*===========================================================================*/
/*======================[ GeometriesProjectionFilter ]=======================*/
/*===========================================================================*/

inline
void otb::GeometriesProjectionFilter::SetInputSpacing(ImageReference::SpacingType const& spacing)
{
  m_InputImageReference.SetSpacing(spacing);
}

inline
void otb::GeometriesProjectionFilter::SetOutputSpacing(ImageReference::SpacingType const& spacing)
{
  m_OutputImageReference.SetSpacing(spacing);
}

inline
void otb::GeometriesProjectionFilter::SetInputOrigin(ImageReference::OriginType const& origin)
{
  m_InputImageReference.SetOrigin(origin);
}

inline
void otb::GeometriesProjectionFilter::SetOutputOrigin(ImageReference::OriginType const& origin)
{
  m_OutputImageReference.SetOrigin(origin);
}


inline
void otb::GeometriesProjectionFilter::SetInputKeywordList(const ImageKeywordlist& kwl)
{
  this->m_InputKeywordList = kwl;
  this->Modified();
}

inline
void otb::GeometriesProjectionFilter::SetOutputKeywordList(const ImageKeywordlist& kwl)
{
  this->m_OutputKeywordList = kwl;
  this->Modified();
}

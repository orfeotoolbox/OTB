/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGeometriesProjectionFilter_hxx
#define otbGeometriesProjectionFilter_hxx

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
    out(in ? static_cast <TGeometry*>(in->clone()) : nullptr); // OGR clone doesn't use covariant return ...
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

#endif

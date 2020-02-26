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

#ifndef otbGeometriesToGeometriesFilter_hxx
#define otbGeometriesToGeometriesFilter_hxx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesToGeometriesFilter.h"

/*===========================================================================*/
/*==================[ DefaultGeometriesToGeometriesFilter ]==================*/
/*===========================================================================*/
template <class TransformationFunctor, class FieldTransformationPolicy>
inline otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::DefaultGeometriesToGeometriesFilter()
{
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
    /*virtual*/
    otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::~DefaultGeometriesToGeometriesFilter()
{
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
    /*virtual*/
    void
    otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::DoProcessLayer(otb::ogr::Layer const& source,
                                                                                                               otb::ogr::Layer&       destination) const
{
  if (source != destination)
  {
    (*this)(source, destination); // if TransformedElementType == layer
    // m_TransformationFunctor(source, destination); // if TransformedElementType == layer
  }
  else
  {
    // m_TransformationFunctor(destination); // if TransformedElementType == layer
    (*this)(destination); // if TransformedElementType == layer
  }
}

/*===========================================================================*/
/*================[ TransformationFunctorDispatcher<layer> ]=================*/
/*===========================================================================*/

template <class TransformationFunctor, class FieldTransformationPolicy>
inline void otb::TransformationFunctorDispatcher<TransformationFunctor, otb::ogr::Layer, FieldTransformationPolicy>::operator()(otb::ogr::Layer const& in,
                                                                                                                                otb::ogr::Layer& out) const
{
  m_functor(in, out);
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline void otb::TransformationFunctorDispatcher<TransformationFunctor, otb::ogr::Layer, FieldTransformationPolicy>::operator()(otb::ogr::Layer& inout) const

{
  m_functor(inout);
}

/*===========================================================================*/
/*=============[ TransformationFunctorDispatcher<OGRGeometry> ]==============*/
/*===========================================================================*/
template <class TransformationFunctor, class FieldTransformationPolicy>
inline void otb::TransformationFunctorDispatcher<TransformationFunctor, OGRGeometry, FieldTransformationPolicy>::operator()(otb::ogr::Layer const& in,
                                                                                                                            otb::ogr::Layer&       out) const
{
  OGRFeatureDefn& defn = out.GetLayerDefn();
  for (ogr::Layer::const_iterator b = in.begin(), e = in.end(); b != e; ++b)
  {
    ogr::Feature const feat = *b;
    ogr::Feature       dest(defn);
    dest.SetGeometryDirectly(m_functor(feat.GetGeometry()));
    this->fieldsTransform(feat, dest);
    out.CreateFeature(dest);
  }
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline void otb::TransformationFunctorDispatcher<TransformationFunctor, OGRGeometry, FieldTransformationPolicy>::operator()(otb::ogr::Layer& inout) const
{
  //  OGRFeatureDefn & defn = inout.GetLayerDefn();
  // NB: We can't iterate with begin()/end() as SetFeature may invalidate the
  // iterators depending of the underlying drivers
  // => we use start_at(), i.e. SetNextByIndex()
  for (int i = 0, N = inout.GetFeatureCount(true); i != N; ++i)
  {
    ogr::Feature feat = *inout.start_at(i);
    this->fieldsTransform(feat);
    feat.SetGeometryDirectly(m_functor(feat.GetGeometry()));
    inout.SetFeature(feat);
  }
}

#endif

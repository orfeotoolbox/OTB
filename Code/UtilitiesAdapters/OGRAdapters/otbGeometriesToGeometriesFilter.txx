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

/*===========================================================================*/
/*==================[ DefaultGeometriesToGeometriesFilter ]==================*/
/*===========================================================================*/
template <class TransformationFunctor, class FieldTransformationPolicy>
inline
otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::DefaultGeometriesToGeometriesFilter()
{}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
/*virtual*/
otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::~DefaultGeometriesToGeometriesFilter()
{}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
/*virtual*/
void otb::DefaultGeometriesToGeometriesFilter<TransformationFunctor, FieldTransformationPolicy>::DoProcessLayer(
  otb::ogr::Layer const& source, otb::ogr::Layer & destination) const
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
inline
void otb::TransformationFunctorDispatcher<TransformationFunctor,otb::ogr::Layer,FieldTransformationPolicy>::operator()(
  otb::ogr::Layer const& in, otb::ogr::Layer & out) const
{
  m_functor(in, out);
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
void otb::TransformationFunctorDispatcher<TransformationFunctor,otb::ogr::Layer,FieldTransformationPolicy>::operator()(
  otb::ogr::Layer & inout) const

{
  m_functor(inout);
}

/*===========================================================================*/
/*=============[ TransformationFunctorDispatcher<OGRGeometry> ]==============*/
/*===========================================================================*/
template <class TransformationFunctor, class FieldTransformationPolicy>
inline
void otb::TransformationFunctorDispatcher<TransformationFunctor,OGRGeometry,FieldTransformationPolicy>::operator()(
  otb::ogr::Layer const& in, otb::ogr::Layer & out) const
{
  OGRFeatureDefn & defn = out.GetLayerDefn();
  for (ogr::Layer::const_iterator b = in.begin(), e = in.end(); b != e; ++b)
    {
    ogr::Feature const feat = *b;
    ogr::UniqueGeometryPtr g = m_functor(feat.GetGeometry());
    ogr::Feature dest(defn);
    dest.SetGeometryDirectly(otb::move(g));
    this->fieldsTransform(feat, dest);
    out.CreateFeature(dest);
    }
}

template <class TransformationFunctor, class FieldTransformationPolicy>
inline
void otb::TransformationFunctorDispatcher<TransformationFunctor,OGRGeometry,FieldTransformationPolicy>::operator()(
  otb::ogr::Layer & inout) const
{
//  OGRFeatureDefn & defn = inout.GetLayerDefn();
  // NB: We can't iterate with begin()/end() as SetFeature may invalidate the
  // iterators depending of the underlying drivers
  // => we use start_at(), i.e. SetNextByIndex()
  for (int i=0, N=inout.GetFeatureCount(true); i!=N; ++i)
    {
    ogr::Feature feat = *inout.start_at(i);
    this->fieldsTransform(feat);
    ogr::UniqueGeometryPtr g = m_functor(feat.GetGeometry());
    feat.SetGeometryDirectly(otb::move(g));
    inout.SetFeature(feat);
    }
}

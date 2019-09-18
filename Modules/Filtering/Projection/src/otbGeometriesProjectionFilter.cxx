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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbGeometriesProjectionFilter.h"
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include "ogr_srs_api.h"    // OCTDestroyCoordinateTransformation
#include "ogr_spatialref.h" // OGRCoordinateTransformation
#include "itkMacro.h"
#include "otbGeometriesSet.h"
#include "itkMetaDataObject.h"
#include "otbOGRGeometryWrapper.h"
#include "otbOGRGeometriesVisitor.h"


/*===========================================================================*/
/*======================[ Reprojection Transformation ]======================*/
/*===========================================================================*/

#if 0
void do_transform(OGRLinearRing & g) const
{
  assert(!"don't know how to reproject a ring yet");
}
#endif

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRPoint& g) const
{
  typedef InternalTransformType::InputPointType  InputPointType;
  typedef InternalTransformType::OutputPointType OutputPointType;
  // in our case:
  BOOST_MPL_ASSERT((boost::is_same<InputPointType, OutputPointType>));
  const double          rawPoint[] = {g.getX(), g.getY()};
  const InputPointType  inPoint(rawPoint);
  const OutputPointType outPoint = m_Transform->TransformPoint(inPoint);
  g.setX(outPoint[0]);
  g.setY(outPoint[1]);
}

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRLineString& g) const
{
  OGRPoint point;
  for (int i = 0, N = g.getNumPoints(); i != N; ++i)
  {
    g.getPoint(i, &point);
    do_transform(point); // no need to go through the redispatch cycle
    g.setPoint(i, &point);
  }
}

#if 0
void otb::internal::ReprojectTransformationFunctor::do_transform(OGRCurve & g) const
{
  assert(!"OGRCurve is just an interface, they can't be reprojected yet");
}
#endif

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRPolygon& g) const
{
  // Note: OGRPolygon interface states that rings obtained through
  // getInteriorRing() and getExteriorRing() shall not be modified.
  // In this context, it seems to make sense as it gives a direct access to the
  // ring (yes this is a de-encapsulation), and the polygon is a fresh one has
  // just been cloned in the () operator.
  // The fact that OGRPolygon::transform() directly works on each sub-ring seems
  // to confirm that hyppothesis.
  for (int i = 0, N = g.getNumInteriorRings(); i != N; ++i)
  {
    OGRLinearRing* ring = g.getInteriorRing(i);
    assert(ring && "Rings in polygons are not expected to be nil");
    if (ring)
    {
      do_transform(*ring); // no need to go through the redispatch cycle
      // how to commit the change ? Are ring modifications automatically committed ?
    }
  }
  OGRLinearRing* ring = g.getExteriorRing();
  assert(ring && "Rings in polygons are not expected to be nil");
  if (ring)
  {
    do_transform(*ring); // no need to go through the redispatch cycle
    // how to commit the change ? Are ring modifications automatically committed ?
  }
}

#if 0
void otb::internal::ReprojectTransformationFunctor::do_transform(OGRSurface & g) const
{
  assert(!"OGRSurface is just an interface, they can't be reprojected yet");
}

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRMultiLineString & g) const
{
}

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRMultiPoint & g) const
{
}

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRMultiPolygon & g) const
{
}
#endif

void otb::internal::ReprojectTransformationFunctor::do_transform(OGRGeometryCollection& col) const
{
  for (int i = 0, N = col.getNumGeometries(); i != N; ++i)
  {
    OGRGeometry* g = col.getGeometryRef(i);
    assert(g && "geometries to transform shall not be null");
    apply_inplace(g);
  }
}

otb::ogr::UniqueGeometryPtr otb::internal::ReprojectTransformationFunctor::operator()(OGRGeometry const* in) const
// otb::internal::ReprojectTransformationFunctor::apply(OGRGeometry const* in) const
{
  otb::ogr::UniqueGeometryPtr res = ogr::apply<otb::ogr::UniqueGeometryPtr>(in, ByCopy(*this));
  return otb::move(res);
}

void otb::internal::ReprojectTransformationFunctor::apply_inplace(OGRGeometry* inout) const
{
  if (inout)
    ogr::apply<void>(inout, InPlace(*this));
}

/*===========================================================================*/
/*======================[ GeometriesProjectionFilter ]=======================*/
/*===========================================================================*/

otb::GeometriesProjectionFilter::GeometriesProjectionFilter() : m_InputImageReference(*this), m_OutputImageReference(*this)
{
}

/*virtual*/
otb::GeometriesProjectionFilter::~GeometriesProjectionFilter()
{
}

/*virtual*/
void otb::GeometriesProjectionFilter::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  OutputGeometriesType::Pointer output = this->GetOutput();
  itk::MetaDataDictionary&      dict   = output->GetMetaDataDictionary();

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, m_OutputProjectionRef);
}

/*virtual*/
void otb::GeometriesProjectionFilter::DoFinalizeInitialization()
{
  m_Transform = InternalTransformType::New();

  // The InputProjectionRef can only be set once per layer, once the layer to
  // process is known
  // m_Transform->SetInputProjectionRef(m_InputProjectionRef);
  m_Transform->SetOutputProjectionRef(m_OutputProjectionRef);
  m_Transform->SetInputKeywordList(m_InputKeywordList);
  m_Transform->SetOutputKeywordList(m_OutputKeywordList);

  m_Transform->SetInputSpacing(m_InputImageReference.GetSpacing());
  m_Transform->SetInputOrigin(m_InputImageReference.GetOrigin());
  m_Transform->SetOutputSpacing(m_OutputImageReference.GetSpacing());
  m_Transform->SetOutputOrigin(m_OutputImageReference.GetOrigin());

  // As the InputProjectionRef can't be known yet, InstantiateTransform() will
  // be called from DoProcessLayer
  // m_Transform->InstantiateTransform();

  m_TransformationFunctor->SetOnePointTransformation(m_Transform);

  // retrieve the output projection ref
  // if it is not specified and end up being geographic,
  // only the m_Transform will know
  m_OutputProjectionRef = m_Transform->GetOutputProjectionRef();

  // InputGeometriesType::ConstPointer   input      = this->GetInput();
  OutputGeometriesType::Pointer output = this->GetOutput();
  output->SetImageReference(m_OutputImageReference);
}

/*virtual*/
OGRSpatialReference* otb::GeometriesProjectionFilter::DoDefineNewLayerSpatialReference(ogr::Layer const& itkNotUsed(source)) const
{
  if (!m_OutputProjectionRef.empty())
  {
    char const*          wkt_string = m_OutputProjectionRef.c_str();
    OGRSpatialReference* osr        = static_cast<OGRSpatialReference*>(OSRNewSpatialReference(wkt_string));
    return osr;
  }
  else
  {
    return nullptr;
  }
}

/*virtual*/
void otb::GeometriesProjectionFilter::DoProcessLayer(ogr::Layer const& source, ogr::Layer& destination) const
{
  // Finish the initialization phase as somethings depends on the current layer
  // to process.
  m_Transform->SetInputProjectionRef(source.GetProjectionRef());
  m_Transform->InstantiateTransform();

  if (source == destination)
  {
    itkExceptionMacro(<< "Geometries projection filter cannot work in-place as the resulting layers will have a new spatial reference."
                         " Please supply too different geometries sets to work on.");
  }

  m_TransformationFunctor(source, destination); // if TransformedElementType == layer
}

/*virtual*/
void otb::GeometriesProjectionFilter::DoDefineNewLayerFields(ogr::Layer const& source, ogr::Layer& dest) const

{
  m_TransformationFunctor.DefineFields(source, dest);
}

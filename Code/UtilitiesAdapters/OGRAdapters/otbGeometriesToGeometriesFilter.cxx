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
#include "otbGeometriesToGeometriesFilter.h"
#include <cassert>
#include "otbGeometriesSet.h"
#include "itkMacro.h"
#include "itkTimeProbe.h"
#include "otbMacro.h"

/*===========================================================================*/
/*============================[ ProcessVisitor ]=============================*/
/*===========================================================================*/
/**\ingroup GeometriesFilters
 * \class ProcessVisitor
 * Internal class used to dispatch filters on either \c ogr::DataSource, or \c
 * ogr::Layer.
 * As \c GeometriesSet is a variant that can contain either a \c ogr::DataSource
 * or a \c ogr::Layer, we need a way to dispath filtering operations on the
 * right type. This is done thanks to \c ProcessVisitor.
 * \since OTB v 3.14.0
 */
struct otb::internal::ProcessVisitor : boost::static_visitor<>
{
  ProcessVisitor(otb::GeometriesToGeometriesFilter const& filter)
    : m_filter(filter) {}
  /**
   * Processes layers (\em by-copy).
   * \param[in] source  source layer
   * \param[in,out] destination  destination layer.
   * \throw WhatEver the filter throws.
   *
   * This is the \em leaf operation that forwards the call to the polymorphic
   * \c GeometriesToGeometriesFilter::DoProcessLayer() function.
   */
  void operator()(otb::ogr::Layer const& source, otb::ogr::Layer & destination) const
    {
    m_filter.DoProcessLayer(source, destination);
    }

  /**
   * Processes data sources (\em by-copy).
   * \param[in] source  source data source
   * \param[in,out] destination  destination data source
   * \throw WhatEver the filter throws.
   * \throw itk::ExceptionObject if the construction of a new layer fails
   *
   * For each layer in the \c source, this operation:
   * - builds a new layer in the \c destination data source (according to the
   *   choices made in the actual specialization of \c
   *   GeometriesToGeometriesFilter regarding layer options, layer spatial
   *   reference, geometries type, ...);
   * - defines new fileds in the new layer;
   * - and finally processes the layer thanks to the polymorphic \c
   *   GeometriesToGeometriesFilter::DoProcessLayer() function.
   */
  void operator()(otb::ogr::DataSource::Pointer source, otb::ogr::DataSource::Pointer destination) const
    {
    assert(source && "can't filter a nil datasource");
    assert(destination && "can't filter to a nil datasource");
    for (otb::ogr::DataSource::const_iterator b = source->begin(), e = source->end()
; b != e
; ++b
    )
      {
      otb::ogr::Layer const& sourceLayer = *b;
      assert(sourceLayer && "unexpected nil source layer");
      otb::ogr::Layer destLayer = destination->CreateLayer(
        sourceLayer.GetName(),
        m_filter.DoDefineNewLayerSpatialReference(sourceLayer),
        m_filter.DoDefineNewLayerGeometryType(sourceLayer),
        m_filter.DoDefineNewLayerOptions(sourceLayer)
      );
      m_filter.DoDefineNewLayerFields(sourceLayer, destLayer);
      m_filter.DoProcessLayer(sourceLayer, destLayer);
      }
    }

  /**
   * Processes layers (\em in-place).
   * \param[in,out] inout  layer to modify
   * \throw WhatEver the filter throws.
   *
   * This is the \em leaf operation that forwards the call to the polymorphic
   * \c GeometriesToGeometriesFilter::DoProcessLayer() function.
   */
  void operator()(otb::ogr::Layer & inout) const
    {
    m_filter.DoProcessLayer(inout, inout);
    }

  /**
   * Processes data sources (\em in-place).
   * \param[in,out] inout  data source to modify
   * \throw WhatEver the filter throws.
   *
   * This operation finally processes each layer of the \c source thanks to the
   * polymorphic \c GeometriesToGeometriesFilter::DoProcessLayer() function.
   */
  void operator()(otb::ogr::DataSource::Pointer inout) const
    {
    assert(inout && "can't filter a nil datasource");
    for (otb::ogr::DataSource::iterator b = inout->begin(), e = inout->end(); b != e; ++b)
      {
      otb::ogr::Layer layer = *b;
      assert(layer && "unexpected nil source layer");
      m_filter.DoProcessLayer(layer, layer);
      }
    }

  /**
   * Fall-back visiting function for invalid mixed tranformations (layer ->
   * datasource).
   * The only transformations accepted are
   * - layer -> layer,
   * - and data source -> data source.
   */
  template <typename GT1, typename GT2> void operator()(GT1 const&, GT2 &) const
    {
    assert(!"You shall not mix DataSources and Layers in GeometriesToGeometriesFilter");
    itkGenericExceptionMacro(<<"You shall not mix DataSources and Layers in GeometriesToGeometriesFilter");
    }
private:
  otb::GeometriesToGeometriesFilter const& m_filter;
};

/*===========================================================================*/
/*=====================[ GeometriesToGeometriesFilter ]======================*/
/*===========================================================================*/

otb::GeometriesToGeometriesFilter::GeometriesToGeometriesFilter()
{
}

/*virtual*/
otb::GeometriesToGeometriesFilter::~GeometriesToGeometriesFilter()
{
}

/*virtual*/ void otb::GeometriesToGeometriesFilter::SetInput(
  const InputGeometriesType * input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(
    0,
    const_cast<InputGeometriesType *>(input));
}

const otb::GeometriesToGeometriesFilter::InputGeometriesType *
otb::GeometriesToGeometriesFilter::GetInput(void )
{
  return static_cast <InputGeometriesType*>(Superclass::GetInput(0));
}

/*virtual*/ void otb::GeometriesToGeometriesFilter::Process(
  InputGeometriesType const& source, OutputGeometriesType & destination)
{
  // si layer, appelle virt process layer
  // si DS, loop et appelle virt process layer
  source.apply(internal::ProcessVisitor(*this), destination);
}

/*virtual*/ void otb::GeometriesToGeometriesFilter::Process(
  OutputGeometriesType & inout)
{
  // si layer, appelle virt process layer
  // si DS, loop et appelle virt process layer
  inout.apply(internal::ProcessVisitor(*this));
}

/*virtual*/
void otb::GeometriesToGeometriesFilter::GenerateData(void )
{
  this->DoAllocateOutputs();
  this->DoFinalizeInitialisation();

  InputGeometriesType::ConstPointer  input  = this->GetInput();
  // assert(input && "Cann't filter to a nil geometries set");
  OutputGeometriesType::Pointer      output = this->GetOutput();
  assert(output && "Cann't filter a nil geometries set");

  // Start recursive processing
  itk::TimeProbe chrono;
  chrono.Start();
  if (input)
    {
    this->Process(*input, *output);
    }
  else
    {
    this->Process(*output);
    }

  chrono.Stop();
  otbMsgDevMacro(<< "GeometriesToGeometriesFilter: geometries processed in " << chrono.GetMeanTime() << " seconds.");
}

/*virtual*/
OGRSpatialReference* otb::GeometriesToGeometriesFilter::DoDefineNewLayerSpatialReference(
  ogr::Layer const& source) const
{
  return 0;
}

/*virtual*/
OGRwkbGeometryType otb::GeometriesToGeometriesFilter::DoDefineNewLayerGeometryType(
  ogr::Layer const& source) const
{
  return source.GetGeomType();
}

/*virtual*/
std::vector<std::string> otb::GeometriesToGeometriesFilter::DoDefineNewLayerOptions(
  ogr::Layer const& source) const
{
  return std::vector<std::string>();
}

/*===========================================================================*/
/*========================[ FieldCopyTransformation ]========================*/
/*===========================================================================*/
void otb::FieldCopyTransformation::DefineFields(
  otb::ogr::Layer const& source, otb::ogr::Layer & dest) const
{
  OGRFeatureDefn & inDefinition = source.GetLayerDefn();
  OGRFeatureDefn & outDefinition = dest.GetLayerDefn();
  for (int i=0,N=inDefinition.GetFieldCount(); i!=N; ++i)
    {
    dest.CreateField(*inDefinition.GetFieldDefn(i));
    // assume the definition is updated automatically
    m_SourceToDestFieldIndicesMap[i] = outDefinition.GetFieldCount()-1;
    }
}


void otb::FieldCopyTransformation::fieldsTransform(
  otb::ogr::Feature const& inFeature, otb::ogr::Feature & outFeature) const
{
  // default => copy all fields for copy transformation

  // The following can't be assumed because of Drivers like KML that always add
  // two fields: "Description" and "Name"
  //assert(inFeature.GetSize() == outFeature.GetSize());

  for (size_t i=0,N=inFeature.GetSize(); i!=N; ++i)
    {
    int const indexNewField = m_SourceToDestFieldIndicesMap[i];
    outFeature[indexNewField].Assign(inFeature[i]);
    }
}

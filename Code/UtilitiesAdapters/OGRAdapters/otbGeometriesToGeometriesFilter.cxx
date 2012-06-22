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
 * \since OTB v 3.14.0
 * \todo Group -> internal
 * \todo Namespace -> ogr::internal ?
 */
struct ProcessVisitor : boost::static_visitor<>
{
  ProcessVisitor(otb::GeometriesToGeometriesFilter const& filter)
    : m_filter(filter) {}
  void operator()(otb::ogr::Layer const& source, otb::ogr::Layer & destination) const
    {
    m_filter.DoProcessLayer(source, destination);
    }

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

  void operator()(otb::ogr::Layer & inout) const
    {
    m_filter.DoProcessLayer(inout, inout);
    }

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
  source.apply(ProcessVisitor(*this), destination);
}

/*virtual*/ void otb::GeometriesToGeometriesFilter::Process(
  OutputGeometriesType & inout)
{
  // si layer, appelle virt process layer
  // si DS, loop et appelle virt process layer
  inout.apply(ProcessVisitor(*this));
}

/*virtual*/
void otb::GeometriesToGeometriesFilter::GenerateOutputInformation(void )
{
  Superclass::GenerateOutputInformation();
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

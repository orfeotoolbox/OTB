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

// Software Guide : BeginLatex
//
// Starting with the version 3.14.0 of the OTB library, a wrapper around OGR API
// is provided. The purposes of the wrapper are:
// \begin{itemize}
// \item to permit OTB to handle very large vector data sets;
// \item and to offer a modern (in the
// \href{http://en.wikipedia.org/wiki/RAII}{RAII} sense) interface to handle
// vector data.
// \end{itemize}
//
// As OGR already provides a rich set of geometric related data, as well as the
// algorithms to manipulate and serialize them, we've decided to wrap it into a
// new \emph{exception-safe} interface.
//
// This example illustrates the use of OTB's OGR wrapper framework. This
// program takes a source of polygons (a shape file for instance) as input and
// produces a datasource of multi-polygons as output.
//
// We will start by including the header files for the OGR wrapper classes,
// plus other header files that are out of scope here.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbOGRDataSourceWrapper.h"
// Software Guide : EndCodeSnippet

#include <cstdio> // EXIT_*

// Software Guide : BeginLatex
//
// The following declarations will permit to merge the
// \subdoxygen{otb}{ogr}{Field}s from each \subdoxygen{otb}{ogr}{Feature} into
// list-fields. We'll get back to this point later.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "otbJoinContainer.h"

typedef std::vector<int>                                    IntList_t;
typedef std::vector<std::string>                            StringList_t;
typedef std::vector<double>                                 RealList_t;
// TODO: handle non recognized fields
typedef boost::variant<IntList_t, StringList_t, RealList_t> AnyListField_t;
typedef std::vector<AnyListField_t>                         AnyListFieldList_t;

AnyListFieldList_t prepareNewFields(
  OGRFeatureDefn /*const*/& defn, otb::ogr::Layer & destLayer);
void printField(
  otb::ogr::Field const& field, AnyListField_t const& newListField);
void assignField(
  otb::ogr::Field field, AnyListField_t const& newListFieldValue);
void  pushFieldsToFieldLists(
  otb::ogr::Feature const& inputFeature, AnyListFieldList_t & field);
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile outputFile" << std::endl;
    return EXIT_FAILURE;
    }

  try
    {
// Software Guide : BeginLatex
//
// We caw now instantiate first the input \subdoxygen{otb}{ogr}{DataSource}.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(
      argv[1], otb::ogr::DataSource::Modes::Read);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// And then, we can instantiate the output \subdoxygen{otb}{ogr}{DataSource} and
// its unique \subdoxygen{otb}{ogr}{Layer} multi-polygons.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    otb::ogr::DataSource::Pointer destination = otb::ogr::DataSource::New(
      argv[2], otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
    otb::ogr::Layer destLayer = destination->CreateLayer(
      argv[2], 0, wkbMultiPolygon);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The data obtained from the reader mimics the interface of
// \code{OGRDataSource}. To access the geometric objects stored, we need first
// to iterate on the \subdoxygen{otb}{ogr}{Layer}s from the
// \subdoxygen{otb}{ogr}{DataSource}, then on the \subdoxygen{otb}{ogr}{Feature}
// from each layer.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    // for (auto const& inputLayer : *source)
    for (otb::ogr::DataSource::const_iterator lb=source->begin(), le=source->end()
; lb != le
; ++lb)
      {
      otb::ogr::Layer const& inputLayer = *lb;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// In this example we will only read polygon objects from the input
// file before writing them to the output file. As all features from a
// layer share the same geometric type, we can filter on the layer
// geometric type.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      if (inputLayer.GetGeomType() != wkbPolygon)
        {
        std::cout << "Warning: Ignoring layer: ";
        inputLayer.PrintSelf(std::cout, 2);
        continue; // skip to next layer
        }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// In order to prepare the fields for the new layer, we first need to extract
// the fields definition from the input layer in order to deduce the new fields
// of the result layer.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      OGRFeatureDefn & sourceFeatureDefn = inputLayer.GetLayerDefn();
      AnyListFieldList_t fields = prepareNewFields(sourceFeatureDefn, destLayer);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The result layer will contain only one feature, per input layer, that stores
// a multi-polygon shape. All geometric shapes are plain \code{OGRGeometry}
// objects.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      OGRMultiPolygon destGeometry; // todo: use UniqueGeometryPtr
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The transformation algorithm is as simple as aggregating all the polygons
// from the features from the input layer into the destination multi-polygon
// geometric object.
//
// Note that \subdoxygen{otb}{ogr}{Feature}\code{::GetGeometry()} provides a
// direct access to a non-mutable \code{OGRGeometry} pointer and that
// \code{OGRGeometryCollection::addGeometry()} copies the received pointer. As
// a consequence, the following code is optimal regarding the geometric objects
// manipulated.
//
// This is also at this point that we fetch the field values from the input
// features to accumulate them into the \code{fields} list.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      // for (auto const& inputFeature : inputLayer)
      for (otb::ogr::Layer::const_iterator fb=inputLayer.begin(), fe=inputLayer.end()
; fb != fe
; ++fb)
        {
        otb::ogr::Feature const& inputFeature = *fb;
        destGeometry.addGeometry(inputFeature.GetGeometry());
        pushFieldsToFieldLists(inputFeature,fields);
        } // for each feature
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Then the new geometric object can be added to a new feature, that will be
// eventually added to the destination layer.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      otb::ogr::Feature newFeature(destLayer.GetLayerDefn());
      newFeature.SetGeometry(&destGeometry); // SetGeom -> copies
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set here the fields of the new feature with the ones accumulated over the
// features from the input layer.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      for (size_t i=0, N=sourceFeatureDefn.GetFieldCount(); i!=N; ++i)
        {
        printField(newFeature[i], fields[i]);
        assignField(newFeature[i], fields[i]);
        }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Finally we add (with \subdoxygen{otb}{ogr}{Layer}\code{::CreateFeature()}
// the new feature to the destination layer, and we can process the next layer
// from the input datasource.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      destLayer.CreateFeature(newFeature); // adds feature to the layer
      } // for each layer
// Software Guide : EndCodeSnippet

    return EXIT_SUCCESS;
    }
  catch (std::exception const& e)
    {
    std::cerr << e.what() << "\n";
    }
  return EXIT_FAILURE;
}

// Software Guide : BeginLatex
//
// In order to \emph{simplify} the manipulation of \subdoxygen{otb}{ogr}{Field}s
// and to avoid copy-paste for each possible case of field-type, this example
// relies on
// \href{http://www.boost.org/doc/libs/release/doc/html/variant.html}{boost.Variant}.
//
// As such, we have defined \code{AnyListField\_t} as a variant type on all
// possible types of field. Then, the manipulation of the variant field values is
// done through the templatized functions
// \subdoxygen{otb}{ogr}{Field}\code{::SetValue<>()} and
// \subdoxygen{otb}{ogr}{Field}\code{::GetValue<>()}, from the various
// variant-visitors.
//
// Before using the visitors, we need to operate a switch on the exact type of
// each field from the input layers. An empty field-values container is first
// added to the set of fields containers. Finally, the destination layer is
// completed with a new field of the right deduced type.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
AnyListFieldList_t prepareNewFields(
  OGRFeatureDefn /*const*/& defn, otb::ogr::Layer & destLayer)
{
  AnyListFieldList_t fields;
  for (int i=0, N=defn.GetFieldCount(); i!=N; ++i)
    {
    const char* name = defn.GetFieldDefn(i)->GetNameRef();
    OGRFieldType type  = static_cast<OGRFieldType>(-1);
    switch (defn.GetFieldDefn(i)->GetType())
      {
    case OFTInteger:
      fields.push_back (IntList_t());
      type = OFTIntegerList;
      break;
    case OFTString:
      fields.push_back (StringList_t());
      type = OFTStringList;
      break;
    case OFTReal:
      fields.push_back (RealList_t());
      type = OFTRealList;
      break;
    default:
      std::cerr << "Unsupported field type: " <<
        OGRFieldDefn::GetFieldTypeName(defn.GetFieldDefn(i)->GetType())
        << " for " << name << "\n";
      break;
      }
    OGRFieldDefn newFieldDefn(name, type); // name is duplicated here => no dangling pointer
    destLayer.CreateField(newFieldDefn, false);
    }
  return fields;
}
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The first visitor, \code{PushVisitor()}, takes the value from one field
// and pushes it into a container of list-variant. The type of the field to
// fetch is deduced from the type of the values stored in the container. It is
// called by \code{pushFieldsToFieldLists()}, that for each field of the input
// feature applies the visitor on the container.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
struct PushVisitor : boost::static_visitor<>
{
  PushVisitor(otb::ogr::Field const& f) : m_f(f) {}

  template <typename T> void operator()(T & container) const
    {
    typedef typename T::value_type value_type;
    value_type const value = m_f.GetValue<value_type>();
    container.push_back(value);
    }
private:
  otb::ogr::Field const& m_f;
};

void  pushFieldsToFieldLists(
  otb::ogr::Feature const& inputFeature, AnyListFieldList_t & fields)
{
  // For each field
  for (size_t i=0, N=inputFeature.GetSize(); i!=N; ++i)
    {
    otb::ogr::Field field = inputFeature[i];
    boost::apply_visitor(PushVisitor(field), fields[i]);
    }
}
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// A second simple visitor, \code{PrintVisitor}, is defined to trace the
// values of each field (which contains a list of typed data (integers, strings
// or reals).
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
struct PrintVisitor : boost::static_visitor<>
{
  template <typename T> void operator()(T const& container) const
    {
    otb::Join(std::cout, container, ", ");
    }
};

void printField(otb::ogr::Field const& field, AnyListField_t const& newListField)
{
  std::cout << field.GetName() << " -> ";
  boost::apply_visitor(PrintVisitor(), newListField);
}
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The third visitor, \code{SetFieldVisitor}, sets the field of the
// destination features, which have been accumulated in the list of typed
// values.
//
// Software Guide : BeginCodeSnippet
struct SetFieldVisitor : boost::static_visitor<>
{
  SetFieldVisitor(otb::ogr::Field f) : m_f(f) {}
  // operator() from visitors are expected to be const
  template <typename T> void operator()(T const& container) const
    {
    m_f.SetValue(container);
    }
private:
  otb::ogr::Field mutable m_f; // this is a proxy -> a reference in a sort
};

void assignField(otb::ogr::Field field, AnyListField_t const& newListFieldValue)
{
  boost::apply_visitor(SetFieldVisitor(field), newListFieldValue);
}
// Software Guide : EndCodeSnippet
// Software Guide : EndLatex
//
// Software Guide : BeginLatex
//
//  Note that this example does not handle the case when the input layers don't
//  share a same fields-definition.
//
// Software Guide : EndLatex

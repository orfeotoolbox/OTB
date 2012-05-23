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
// Starting from version 3.14.0 of the OTB library, a wrapper around OGR API is
//  provided. The purposes of the wrapper are:
//  \begin{itemize}
//  \item To permit OTB to handle very large vector data sets;
//  \item and to offer a modern (in the
//  \href{http://en.wikipedia.org/wiki/RAII}{RAII} sense) interface to handle
//  vector data.
//  \end{itemize}
//
//  As OGR already provides a rich set of geometric related data, and the
//  algorithms to manipulate and serialize them, we've decided to wrap it into a
//  new \em{exception-safe} interface.
//
//  This example illustrates the use of OTB's OGR wrapper framework. This
//  program takes a source of polygons (a shape file for instance) as input and
//  produces a source of multi-polygon as output.
//
//  We will start by including the header files for the OGR wrapper classes,
//  plus other header files that are out of scope here.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbOGRDataSourceWrapper.h"
// Software Guide : EndCodeSnippet

#include <cstdio> // EXIT_*

// Software Guide : BeginLatex
//  The following declarations will permit to merge the
//  \subdoxygen{otb}{ogr}{Field}s from each \subdoxygen{otb}{ogr}{Feature} into
//  list-fields. We'll get back to this point later.
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "otbJoinContainer.h"
typedef std::vector<int>                                          IntListType;
typedef std::vector<std::string>                                  StringListType;
typedef std::vector<double>                                       RealListType;
typedef boost::variant<IntListType, StringListType, RealListType> AnyListFieldType;
typedef std::vector<AnyListFieldType>                             AnyListFieldListType;

AnyListFieldListType prepareNewFields(OGRFeatureDefn /*const*/& defn, otb::ogr::Layer & dest_layer);
void printField(otb::ogr::Field const& field, AnyListFieldType const& newListField);
void assignField(otb::ogr::Field field, AnyListFieldType const& newListFieldValue);
void  pushFieldsToFieldLists(otb::ogr::Feature const& source_feature, AnyListFieldListType & field);
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
//  We caw now instantiate first the input data source.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(argv[1], otb::ogr::DataSource::Modes::read);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
//  And then, we can instantiate the output data source.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    otb::ogr::DataSource::Pointer destination = otb::ogr::DataSource::New(argv[2], otb::ogr::DataSource::Modes::write);
    otb::ogr::Layer dest_layer = destination->CreateLayer(argv[2], 0, wkbMultiPolygon);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
//  The data obtained from the reader mimics the interface of OGRDataSource. To
//  access the geometric objects stored we need first to iterate on the
//  \subdoxygen{otb}{ogr}{Layer}s from the \subdoxygen{otb}{ogr}{DataSource},
//  then on the \subdoxygen{otb}{ogr}{Feature} from each layer.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
    for (otb::ogr::DataSource::const_iterator lb = source->begin(), le = source->end(); lb != le; ++lb)
      {
      otb::ogr::Layer const& source_layer = *lb;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
//  In this example we will only read polygon objects from the input
//  file before writing them to the output file. As all features from a
//  layer share the same geometric type, we can filter on the layer
//  geometric type.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      if (source_layer.GetGeomType() != wkbPolygon)
        {
        std::cout << "Warning: Ignoring layer: ";
        source_layer.PrintSelf(std::cout, 2);
        continue; // skip to next layer
        }
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  In order to prepare the fields for the new layer, we first need to extract
//  the fields definition from the original layer in order to deduce the new
//  fields of the result layer.
//
//  Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      OGRFeatureDefn & sourceFeatureDefn = source_layer.GetLayerDefn();
      AnyListFieldListType fields = prepareNewFields(sourceFeatureDefn, dest_layer);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
//  The result layer will contain only one feature that stores a Multi-polygon
//  shape. All geometric shapes are plain \tt{OGRGeometry} objects.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      OGRMultiPolygon dest_geometry; // todo: ptr to move
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
//  The transformation algorithm is as simple as aggregating all the polygon
//  from the features from the original layer into the destination multi-polygon
//  geometric object.
//
//  Note: GetGeometry() provides a direct access to a non-mutable OGRGeometry
//  pointer. OGRMultixxx::addGeometry() copies the received pointer. As a
//  consequence, the following code is optimal regarding the geometric objects
//  manipulated.
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
      for (otb::ogr::Layer::const_iterator fb = source_layer.begin(), fe = source_layer.end(); fb != fe; ++fb)
        {
        otb::ogr::Feature const& source_feature = *fb;
        dest_geometry.addGeometry(source_feature.GetGeometry());
        pushFieldsToFieldLists(source_feature,fields);
        } // for each feature
// Software Guide : EndCodeSnippet
      otb::ogr::Feature newFeature(dest_layer.GetLayerDefn());
      newFeature.SetGeometry(&dest_geometry); // SetGeom -> copies
      for (size_t i=0, N=sourceFeatureDefn.GetFieldCount(); i!=N; ++i)
        {
        printField(newFeature[i], fields[i]);
        assignField(newFeature[i], fields[i]);
        }

      dest_layer.CreateFeature(newFeature); // add feature to the layer
      } // for each layer

    return EXIT_SUCCESS;
    }
  catch (std::exception const& e)
    {
    std::cerr << e.what() << "\n";
    }
  return EXIT_FAILURE;
}

struct push_visitor : boost::static_visitor<>
{
  push_visitor(otb::ogr::Field const& f) : m_f(f) {}

  template <typename T> void operator()(T & container) const
    {
    typedef typename T::value_type value_type;
    value_type const value = m_f.GetValue<value_type>();
    container.push_back(value);
    }
private:
  otb::ogr::Field const& m_f;
};

void  pushFieldsToFieldLists(otb::ogr::Feature const& source_feature, AnyListFieldListType & fields)
{
  // For each field
  for (size_t i=0, N=source_feature.GetSize(); i!=N; ++i)
    {
    otb::ogr::Field field = source_feature[i];
    boost::apply_visitor(push_visitor(field), fields[i]);
    }
}

AnyListFieldListType prepareNewFields(OGRFeatureDefn /*const*/& defn, otb::ogr::Layer & dest_layer)
{
  AnyListFieldListType fields;
  for (size_t i=0, N=defn.GetFieldCount(); i!=N; ++i)
    {
    const char* name = defn.GetFieldDefn(i)->GetNameRef();
    OGRFieldType type = OFTMaxType;
    switch (defn.GetFieldDefn(i)->GetType())
      {
    case OFTInteger:
      fields.push_back (IntListType());
      type = OFTIntegerList;
      break;
    case OFTString:
      fields.push_back (StringListType());
      type = OFTStringList;
      break;
    case OFTReal:
      fields.push_back (RealListType());
      type = OFTRealList;
      break;
    default:
      std::cerr << "Unsupported field type: " <<
        OGRFieldDefn::GetFieldTypeName(defn.GetFieldDefn(i)->GetType())
        << " for " << name << "\n";
      break;
      }
    OGRFieldDefn newFieldDefn(name, type); // name is duplicated here => no dangling pointer
    dest_layer.CreateField(newFieldDefn, false);
    }
  return fields;
}

struct print_visitor : boost::static_visitor<>
{
  template <typename T> void operator()(T const& container) const
    {
    otb::Join(std::cout, container, ", ");
    }
};

void printField(otb::ogr::Field const& field, AnyListFieldType const& newListField)
{
  std::cout << field.GetName() << " -> ";
  boost::apply_visitor(print_visitor(), newListField);
}

struct set_field_visitor : boost::static_visitor<>
{
  set_field_visitor(otb::ogr::Field f) : m_f(f) {}
  template <typename T> void operator()(T const& container) const
    {
    m_f.SetValue(container);
    }
private:
  otb::ogr::Field mutable m_f; // this is a proxy -> a reference in a sort
};

void assignField(otb::ogr::Field field, AnyListFieldType const& newListFieldValue)
{
  boost::apply_visitor(set_field_visitor(field), newListFieldValue);
}

// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the implementatio of the EntityMapper class and the
// CreateExpandedEntities function.

#include "kml/engine/entity_mapper.h"
#include "kml/dom/xsd.h"  // TODO: should be Xsd class be public?
#include "kml/engine/kml_uri.h"

using kmlbase::StringMap;
using kmldom::DataPtr;
using kmldom::ExtendedDataPtr;
using kmldom::FeaturePtr;
using kmldom::SchemaPtr;
using kmldom::SchemaDataPtr;
using kmldom::SimpleDataPtr;
using kmldom::SimpleFieldPtr;
using kmldom::Xsd;

namespace kmlengine {

static const char kDisplayNamePfx[] = "/displayName";

EntityMapper::EntityMapper(const KmlFilePtr& kml_file, StringMap* string_map)
    : kml_file_(kml_file), entity_map_(string_map) {}
EntityMapper::~EntityMapper() {}

void EntityMapper::GetEntityFields(const FeaturePtr& feature) {
  GatherObjectFields(feature);
  GatherFeatureFields(feature);
  GatherExtendedDataFields(feature);
}

// Private.
void EntityMapper::GatherObjectFields(const FeaturePtr& feature) {
  // Object id="...
  if (feature->has_id()) {
    (*entity_map_)["id"] = feature->get_id();  // KMLDOM has no attr types.
  }
  // Object targetId="...
  if (feature->has_targetid()) {
    (*entity_map_)["targetId"] = feature->get_targetid();
  }
}

// Private.
// TODO: OGC KML 2.2 does not single out specific elements. Any simple field of
// Feature, or any attribute, is an entity candidate. We could use a Serializer
// specialization like FieldMerger to handle this for us.
void EntityMapper::GatherFeatureFields(const FeaturePtr& feature) {
  // <name>...
  if (feature->has_name()) {
    (*entity_map_)[Xsd::GetSchema()->ElementName(kmldom::Type_name)] =
      feature->get_name();
  }
  // <address>...
  if (feature->has_address()) {
    (*entity_map_)[Xsd::GetSchema()->ElementName(kmldom::Type_address)] =
      feature->get_address();
  }
  // <Snippet>...
  if (feature->has_snippet()) {
    (*entity_map_)[Xsd::GetSchema()->ElementName(kmldom::Type_Snippet)] =
      feature->get_snippet()->get_text();
  }
  // <description>...
  if (feature->has_description()) {
    (*entity_map_)[Xsd::GetSchema()->ElementName(kmldom::Type_description)] =
      feature->get_description();
  }
}

// Private.
void EntityMapper::GatherExtendedDataFields(const FeaturePtr& feature) {
  if (feature->has_extendeddata()) {
    ExtendedDataPtr ed = kmldom::AsExtendedData(feature->get_extendeddata());
    for (size_t i = 0; i < ed->get_data_array_size(); ++i) {
      GatherDataFields(ed->get_data_array_at(i));
    }
    for (size_t i = 0; i < ed->get_schemadata_array_size(); ++i) {
      GatherSchemaDataFields(ed->get_schemadata_array_at(i));
    }
  }
}

// Private.
void EntityMapper::GatherDataFields(const DataPtr& data) {
  // If <ExtendedData><Data name="..."><value>...
  if (data->has_name()) {
    (*entity_map_)[data->get_name()] = data->get_value();
    // If <ExtendedData><Data name="..."><displayName>...
    if (data->has_displayname()) {
      (*entity_map_)[data->get_name() + kDisplayNamePfx] =
        data->get_displayname();
    }
  }
}

// Private.
void EntityMapper::GatherSchemaDataFields(const SchemaDataPtr& schemadata) {
  // We need to take the schemaUrl and find the <Schema> with that id.
  schemadata_prefix_ = "/";
  if (schemadata->has_schemaurl()) {
    // We need to get the name= attr of the <Schema> with the id= attr that
    // matches this schemaUrl.
    string schema_id;
    if (SplitUriFragment(schemadata->get_schemaurl(), &schema_id)) {
      SchemaPtr schema = kmldom::AsSchema(kml_file_->GetObjectById(schema_id));
      if (schema) {
        // Now walk all SimpleFields in schema building concatenations of
        // Schema_name/SimpleField_name/displayName (if we have displayName).
        for (size_t i = 0; i < schema->get_simplefield_array_size(); ++i) {
          GatherSimpleFieldFields(schema->get_simplefield_array_at(i),
                                  schema);
        }
        schemadata_prefix_ = schema->get_name() + schemadata_prefix_;
      }
    }
  }

  // Iterate over any SimpleData elements in SchemaData.
  for (size_t i = 0; i < schemadata->get_simpledata_array_size(); ++i) {
    GatherSimpleDataFields(schemadata->get_simpledata_array_at(i));
  }
}

// Private.
void EntityMapper::GatherSimpleFieldFields(const SimpleFieldPtr& simplefield,
                                           const SchemaPtr& schema) {
  if (simplefield->has_name() && simplefield->has_displayname()) {
    (*entity_map_)[schema->get_name() + "/" + simplefield->get_name() +
                  kDisplayNamePfx] =
      simplefield->get_displayname();
  }
}

// Private.
void EntityMapper::GatherSimpleDataFields(const SimpleDataPtr& simpledata) {
  if (simpledata->has_name()) {  // Required.
    (*entity_map_)[schemadata_prefix_ + simpledata->get_name()] =
      simpledata->get_text();
  }
}

string CreateExpandedEntities(const string & in,
                                   const StringMap& entity_map) {
  // TODO: $[geDirections].
  return kmlbase::CreateExpandedStrings(in, entity_map, "$[", "]");
}

}  // end namespace kmlengine

// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the implementation of the internal UpdateProcessor class.

// TODO: nothing here updates the id maps in KmlFile.  <Create> can add
// an Object with an id (such as a shared style) and <Delete> always deletes
// an Object with an id.  <Change> should probably be prevented from ever
// changing an id.

#include "kml/engine/update_processor.h"
#include "kml/base/string_util.h"
#include "kml/engine/clone.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/merge.h"
#include "kml/engine/update.h"

using kmlbase::StringMap;
using kmldom::AsChange;
using kmldom::AsContainer;
using kmldom::AsCreate;
using kmldom::AsDelete;
using kmldom::AsFeature;
using kmldom::AsKml;
using kmldom::ChangePtr;
using kmldom::ContainerPtr;
using kmldom::CreatePtr;
using kmldom::DeletePtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;
using kmldom::ObjectPtr;
using kmldom::UpdatePtr;
using kmldom::UpdateOperationPtr;

namespace kmlengine {

void UpdateProcessor::ProcessUpdate(const UpdatePtr& update) {
  size_t size = update->get_updateoperation_array_size();
  for (size_t i = 0; i < size; ++i) {
    const UpdateOperationPtr& op = update->get_updateoperation_array_at(i);
    if (ChangePtr change = AsChange(op)) {
      ProcessUpdateChange(change);
    } else if (CreatePtr create = AsCreate(op)) {
      ProcessUpdateCreate(create);
    } else if (DeletePtr deleet = AsDelete(op)) {
      ProcessUpdateDelete(deleet);
    }
  }
}

void UpdateProcessor::ProcessUpdateChange(const ChangePtr& change) {
  size_t size = change->get_object_array_size();
  for (size_t i = 0; i < size; ++i) {
    const ObjectPtr& source_object = change->get_object_array_at(i);
    string targetid;
    if (GetTargetId(source_object, &targetid)) {
      if (ObjectPtr target_object = kml_file_.GetObjectById(targetid)) {
        MergeElements(source_object, target_object);
        // It's easier to just clear the target's targetId= attribute than
        // to teach MergeElements() how to avoid copying targetId from
        // source to target.  This does imply that targetId is treated as
        // any other attribute and merged over on anything other than the
        // root Object.  Ideally the targetId would not be _within_ the
        // source Object at all, but such is the OGC KML 2.2 standard.
        target_object->clear_targetid();
      }
    }
  }
}

void UpdateProcessor::ProcessUpdateCreate(const CreatePtr& create) {
  size_t container_count = create->get_container_array_size();
  for (size_t i = 0; i < container_count; ++i) {
    const ContainerPtr& source_container = create->get_container_array_at(i);
    string targetid;
    if (GetTargetId(source_container, &targetid)) {
      if (ContainerPtr target_container =
          AsContainer(kml_file_.GetObjectById(targetid))) {
        CopyFeatures(source_container, target_container);
      }
    }
  }
}

void UpdateProcessor::ProcessUpdateDelete(const DeletePtr& deleet) {
  size_t feature_count = deleet->get_feature_array_size();
  for (size_t i = 0; i < feature_count; ++i) {
    const FeaturePtr& source_feature = deleet->get_feature_array_at(i);
    string targetid;
    if (GetTargetId(source_feature, &targetid)) {
      DeleteFeatureById(targetid);
    }
  }
}

FeaturePtr UpdateProcessor::DeleteFeatureById(const string& id) {
  if (FeaturePtr feature = AsFeature(kml_file_.GetObjectById(id))) {
    if (ContainerPtr container = AsContainer(feature->GetParent())) {
      return container->DeleteFeatureById(id);
    }
    if (KmlPtr kml = AsKml(feature->GetParent())) {
      kml->clear_feature();
      return feature;
    }
  }
  return NULL;
}

// This is a key reason for this class: to remap the targetId against
// the supplied id map (if one is supplied).
bool UpdateProcessor::GetTargetId(const kmldom::ObjectPtr& object,
                 string* targetid) const {
  if (!object->has_targetid()) {
    return false;
  }
  if (id_map_) {
    StringMap::const_iterator find = id_map_->find(object->get_targetid());
    if (find != id_map_->end()) {
      if (targetid) {
        *targetid = find->second;
      }
      return true;
    }
    return false;
  }
  if (targetid) {
    *targetid = object->get_targetid();
  }
  return true;
}

void CopyFeatures(const ContainerPtr& source_container,
                  ContainerPtr target_container) {
  size_t feature_count = source_container->get_feature_array_size();
  for (size_t j = 0; j < feature_count; ++j) {
    target_container->add_feature(
        AsFeature(Clone(source_container->get_feature_array_at(j))));
  }
}

}  // end namespace kmlengine

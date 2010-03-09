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

// TODO: nothing here updates the id maps in KmlFile.  <Create> can add
// an Object with an id (such as a shared style) and <Delete> always deletes
// an Object with an id.  <Change> should probably be prevented from ever
// changing an id.

#include "kml/engine/update.h"
#include "kml/base/string_util.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/update_processor.h"

using kmlbase::StringMap;
using kmldom::UpdatePtr;

namespace kmlengine {

void ProcessUpdate(const UpdatePtr& update, KmlFilePtr kml_file) {
  if (update && kml_file) {
    UpdateProcessor update_processor(*kml_file, NULL);
    update_processor.ProcessUpdate(update);
  }
}

void ProcessUpdateWithIdMap(const UpdatePtr& update, const StringMap* id_map,
                            KmlFilePtr kml_file) {
  if (update && kml_file) {  // UpdateProcessor handles NULL id_map.
    UpdateProcessor update_processor(*kml_file, id_map);
    update_processor.ProcessUpdate(update);
  }
}

}  // end namespace kmlengine

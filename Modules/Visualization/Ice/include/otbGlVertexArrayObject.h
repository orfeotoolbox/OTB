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

#ifndef otb_GlVertexArrayObject_h
#define otb_GlVertexArrayObject_h

#include "OTBIceExport.h"


#include "otbGlHandle.h"
#include "otbGlTypeTraits.h"


namespace otb { namespace gl {

/**
 * @class OpenGL vertex-arary object handle policy.
 */
struct OTBIce_EXPORT VertexArrayObjectPolicy
{
  static void Generate( Id_t & );
  static void Bind( Id_t );
  static void Release( Id_t & );
};


/** VertexArrayObject type definition. */
using VertexArrayObject = Handle< VertexArrayObjectPolicy >;


} // end namespace gl.

} // end namespace otb.


#endif // otb_GlVetexArrayObject_h

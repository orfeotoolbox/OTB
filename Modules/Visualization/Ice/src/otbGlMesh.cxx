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

#include "otbGlMesh.h"


namespace otb { namespace gl {

Mesh
MakeTexturedQuad( Id_t xy, Id_t uv )
{
  Mesh mesh(
    { // vertices (x, y)
      -1.0f, -1.0f,
      +1.0f, -1.0f,
      +1.0f, +1.0f,
      -1.0f, +1.0f,
      // texcoords (u, v)
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f },
    //
    // Components
    2,
    //
    // triangle indices
    { 0u, 1u, 2u,
      2u, 3u, 0u }
  );

  mesh.VertexAttribPointer( xy, 2, 0 );
  mesh.VertexAttribPointer( uv, 2, 8 );

  return std::move( mesh );
}


Mesh
MakeInterleavedTexturedQuad( Id_t xy, Id_t uv )
{
  Mesh mesh(
    //
    // vertices: (x, y, u, v )
    { -1.0f, -1.0f, 0.0f, 1.0f,
      +1.0f, -1.0f, 1.0f, 1.0f,
      +1.0f, +1.0f, 1.0f, 0.0f,
      -1.0f, +1.0f, 0.0f, 0.0f },
    //
    // Components
    4,
    //
    // triangle indices
    { 0u, 1u, 2u,
      2u, 3u, 0u }
  );

  mesh.VertexAttribPointer( xy, 2, 0 );
  mesh.VertexAttribPointer( uv, 2, 2 );

  return std::move( mesh );
}

Mesh
MakeQuad( Id_t xy)
{
  Mesh mesh(
    //
    // vertices: (x, y )
    { -1.0f, -1.0f,
      +1.0f, -1.0f,
      +1.0f, +1.0f,
      -1.0f, +1.0f},
    //
    // Components
    2,
    //
    // indices compatible with triangles and line loop modes
    { 0u, 1u, 2u,
      3u, 0u, 2u }
  );

  mesh.VertexAttribPointer( xy, 2, 0 );

  return std::move( mesh );
}

void
Mesh
::VertexAttribPointer( Id_t id,
		       std::size_t components,
		       std::ptrdiff_t first ) const
{
  assert( components );

  glVertexAttribPointer(
    id,
    components,
    m_VBO.GlType(),
    GL_FALSE,
    m_VBO.Stride( components ),
    reinterpret_cast< GLvoid const * >( first * m_VBO.Size() )
    );

  CheckError();

  glEnableVertexAttribArray( id );

  CheckError();
}


} // End namespace 'gl'.

} // End namespace 'otb'.

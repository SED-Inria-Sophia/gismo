/** @file MeshElement.hpp

    @brief Provides implementation of gsMeshElement static factory methods

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris
*/

#pragma once

#include <gismo/Core/Mesh/Vertex.h>
#include <gismo/Core/Mesh/Face.h>

namespace gismo
{

template<class T>
typename gsMeshElement<T>::gsVertexHandle
gsMeshElement<T>::makeVertex( scalar_t x, scalar_t y, scalar_t z)
{
    return new gsVertex<T>(x, y, z);
}

template<class T>
typename gsMeshElement<T>::gsVertexHandle
gsMeshElement<T>::makeVertex( gsVector<T> const & u )
{
    return new gsVertex<T>(u);
}

template<class T>
typename gsMeshElement<T>::gsFaceHandle
gsMeshElement<T>::makeFace( std::vector<gsVertexHandle> const & vert)
{
    return new gsFace<T>(vert);
}

template<class T>
typename gsMeshElement<T>::gsFaceHandle
gsMeshElement<T>::makeFace(gsVertexHandle v0, gsVertexHandle v1,
                           gsVertexHandle v2, gsVertexHandle v3)
{
    return new gsFace<T>(v0, v1, v2, v3);
}

template<class T>
typename gsMeshElement<T>::gsFaceHandle
gsMeshElement<T>::makeFace(gsVertexHandle v0, gsVertexHandle v1, gsVertexHandle v2)
{
    return new gsFace<T>(v0, v1, v2);
}

} // namespace gismo
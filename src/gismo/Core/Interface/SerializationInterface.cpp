/** @file SerializationInterface.cpp

    @brief Implementation of serialization interface.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/Core/Interface/SerializationInterface.h>

namespace gismo
{

// Global serialization interface instance
static std::shared_ptr<SerializationInterface> g_serializer;

std::shared_ptr<SerializationInterface> getDefaultSerializer()
{
    return g_serializer; // May be null if no serializer is registered
}

void setDefaultSerializer(std::shared_ptr<SerializationInterface> serializer)
{
    g_serializer = serializer;
}

} // namespace gismo
/** @file XmlFactoryRegistrar.h

    @brief Auto-registration of XmlIO with Core FeatureFactory.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#pragma once

#include <gismo/Core/Interface/FeatureFactory.h>

namespace gismo
{

/**
 * @brief Auto-registrar for XmlIO implementations
 *
 * This class automatically registers XmlIO implementations with the
 * Core FeatureFactory when the XmlIO module is linked. The registration
 * happens through static initialization.
 */
class GISMO_EXPORT XmlIOAutoRegistrar
{
public:
    /**
     * @brief Constructor performs auto-registration
     *
     * When an instance of this class is created (via static initialization),
     * it automatically registers the XmlIO implementations with Core.
     */
    XmlIOAutoRegistrar();

    /**
     * @brief Manual registration function
     *
     * This can be called explicitly if needed, though auto-registration
     * through static initialization is preferred.
     */
    static void registerXmlIOImplementations();

private:
    static bool s_registered;
};

} // namespace gismo
/** @file XmlFactoryRegistrar.cpp

    @brief Implementation of XmlIO auto-registration.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/XmlIO/XmlFactoryRegistrar.h>
#include <gismo/XmlIO/XmlSerializer.h>
#include <iostream>

namespace gismo
{

bool XmlIOAutoRegistrar::s_registered = false;

XmlIOAutoRegistrar::XmlIOAutoRegistrar()
{
    if (!s_registered) {
        registerXmlIOImplementations();
        s_registered = true;
    }
}

void XmlIOAutoRegistrar::registerXmlIOImplementations()
{
    auto& factory = FeatureFactory::getInstance();

    // Register file manager implementation
    factory.registerFileManagerCreator([]() {
        return std::make_shared<XmlFileManagerImpl>();
    });

    // Register XML serialization implementation
    factory.registerSerializationCreator("XML", []() {
        return std::make_shared<XmlSerializationImpl>();
    });

    #ifdef GISMO_DEBUG
    std::cout << "[XmlIO] Registered XML serialization and file management with Core" << std::endl;
    #endif
}

} // namespace gismo
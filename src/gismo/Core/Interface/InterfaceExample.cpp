/** @file InterfaceExample.cpp

    @brief Example usage of the new Core interface architecture.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): Architecture Refactoring Team
*/

#include <gismo/Core/Interface/FeatureFactory.h>
#include <gismo/Core/Interface/FileManagerInterface.h>
#include <gismo/Core/Interface/SerializationInterface.h>
#include <iostream>

namespace gismo
{

/**
 * @brief Example demonstrating the new interface architecture
 */
void demonstrateInterfaceArchitecture()
{
    std::cout << "=== GISMO Core Interface Architecture Demo ===" << std::endl;

    // 1. File Manager Interface Usage
    std::cout << "\n1. File Manager Interface:" << std::endl;
    auto fileManager = createFileManager();
    if (fileManager) {
        std::cout << "   Temp directory: " << fileManager->getTempPath() << std::endl;
        std::cout << "   File exists test: " << fileManager->fileExists("/nonexistent") << std::endl;
        std::cout << "   Base name of 'test.xml': " << fileManager->getBaseName("test.xml") << std::endl;
    }

    // 2. Serialization Interface Usage
    std::cout << "\n2. Serialization Interface:" << std::endl;
    auto xmlSerializer = createSerializer("XML");
    if (xmlSerializer) {
        std::cout << "   XML serialization available: " << xmlSerializer->getFormatName() << std::endl;
    } else {
        std::cout << "   XML serialization not available (gsIO not linked)" << std::endl;
    }

    // 3. Feature Factory Usage
    std::cout << "\n3. Feature Factory:" << std::endl;
    std::cout << "   XML available: " << isXmlSerializationAvailable() << std::endl;

    // 4. Conditional Compilation Demo
    std::cout << "\n4. Conditional Features:" << std::endl;
#ifdef GISMO_WITH_XML_SERIALIZATION
    std::cout << "   XML serialization support: ENABLED" << std::endl;
#else
    std::cout << "   XML serialization support: DISABLED" << std::endl;
#endif

#ifdef GISMO_WITH_ASSEMBLER_INTEGRATION
    std::cout << "   Assembler integration: ENABLED" << std::endl;
#else
    std::cout << "   Assembler integration: DISABLED" << std::endl;
#endif

    std::cout << "\n=== Demo Complete ===" << std::endl;
}

} // namespace gismo

// Example usage in a main function:
/*
int main() {
    gismo::demonstrateInterfaceArchitecture();
    return 0;
}
*/
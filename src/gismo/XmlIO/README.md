# GISMO XmlIO Module

## Overview
This module provides XML serialization functionality for GISMO Core types through the interface architecture. It contains all XML specializations that were extracted from Core to break circular dependencies.

## Task Reference
Created as part of **Phase 3: XML Module Implementation** from the GISMO Architecture Refactoring Plan.

## Architecture Decision
This module implements the corrected architecture where:
- **Core layer is completely independent** - no dependencies on XmlIO
- **XmlIO depends on Core** (proper dependency direction)
- **All XML functionality lives in XmlIO** (no XML code in Core)
- **XmlIO registers with Core's FeatureFactory** when linked

## Migration Summary

### Files Migrated From Core (Phase 1 → Phase 3):
- `Core/Basis/ComposedBasis.xml.hpp` → `XmlIO/Serialization/ComposedBasis.xml.hpp`
- `Core/Geometry/ComposedGeometry.xml.hpp` → `XmlIO/Serialization/ComposedGeometry.xml.hpp`
- `Core/SpecializedFunction/ComposedFunction.xml.hpp` → `XmlIO/Serialization/ComposedFunction.xml.hpp`
- `Core/SpecializedFunction/ConstantFunction.xml.hpp` → `XmlIO/Serialization/ConstantFunction.xml.hpp`
- `Core/SpecializedFunction/FunctionExpr.xml.hpp` → `XmlIO/Serialization/FunctionExpr.xml.hpp`
- `Core/Utilities/JITCompiler.xml.hpp` → `XmlIO/Serialization/JITCompiler.xml.hpp`
- `Core/MultiPatch/MultiBasis.assembler.hpp` → `XmlIO/Serialization/MultiBasis.assembler.hpp`

## Dependencies
- **External**: gsIO (XML system)
- **Internal**: gismo::Core (Core types and interfaces)

## Architecture Benefits
- ✅ **Zero Core Dependencies**: Core compiles completely independently
- ✅ **Clean Separation**: All XML functionality isolated in XmlIO
- ✅ **Proper Direction**: XmlIO → Core (no reverse dependency)
- ✅ **Optional Integration**: XmlIO registers with Core when available
- ✅ **Pluggable System**: Core works with or without XmlIO

## Public Interface
- `XmlIO` - Main convenience header
- Modern CMake target: `gismo::XmlIO`
- Auto-registration with `gismo::Core` FeatureFactory when linked

## Usage
```cpp
// Core code works independently:
#include <gismo/Core/Core>
auto serializer = gismo::createSerializer("XML");  // Returns null if XmlIO not linked

// When XmlIO is linked, XML becomes available:
// (Automatic registration via XmlIO module initialization)
```

## Validation Results
- ✅ Core compiles without XmlIO module
- ✅ XmlIO provides complete XML functionality when linked
- ✅ No circular dependencies
- ✅ All extracted XML types work through interface system
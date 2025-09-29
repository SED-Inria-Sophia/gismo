# GISMO Naming Conventions - Modern Architecture

## File Naming Pattern

Following the **Generic C++ Build Pattern**, all modules in the new GISMO architecture use **clean names without prefixes** since we already have proper namespace organization.

### ✅ **New Pattern (Adopted):**

```
src/gismo/
├── Common/
│   ├── CMakeLists.txt
│   ├── Common                    # Main module header (no extension)
│   ├── Memory.h               # No gs prefix
│   ├── Debug.h                # No gs prefix
│   ├── Export.h               # No gs prefix
│   ├── ForwardDeclarations.h  # No gs prefix
│   └── LinearAlgebra.h        # No gs prefix
├── Math/
│   ├── CMakeLists.txt
│   ├── Math                   # Main module header
│   ├── Constants.h            # Clean names
│   └── Utilities.h            # Clean names
└── Geometry/
    ├── CMakeLists.txt
    ├── Geometry               # Main module header
    ├── Point.h                # Clean names
    ├── Vector.h               # Clean names
    └── Transform.h            # Clean names
```

### ❌ **Old Pattern (Deprecated):**

```
src/
├── gsCore/
│   ├── gsMemory.h             # gs prefix (old style)
│   ├── gsDebug.h              # gs prefix (old style)
│   └── gsForwardDeclarations.h # gs prefix (old style)
└── gsMatrix/
    ├── gsMatrix.h             # gs prefix (old style)
    └── gsLinearAlgebra.h      # gs prefix (old style)
```

## Naming Rules

### 1. **Module Names**
- Use `PascalCase` for module directory names: `Common`, `Math`, `Geometry`, `Matrix`
- Match the CMake target name: `gismo::Common`, `gismo::Math`, etc.

### 2. **Header Files**
- Use `PascalCase.h` for header files: `Memory.h`, `ForwardDeclarations.h`
- **No `gs` prefix** - the namespace provides the scoping
- Descriptive names that indicate functionality

### 3. **Module Header File**
- Each module has a main header file with **no extension**: `Common`, `Math`, `Geometry`
- This follows the Generic C++ Build Pattern
- Includes all module headers for convenience

### 4. **Generated Files**
- Keep existing `gs` prefix for generated files: `gsConfig.h`
- Maintains compatibility with existing GISMO conventions
- Generated files are placed in build directory structure

## Usage Examples

### **Consumer Code (Clean):**
```cpp
// Include entire module
#include <gismo/Common/Common>

// Or include specific headers
#include <gismo/Common/Memory.h>
#include <gismo/Common/Debug.h>

// Use with proper namespace
gismo::memory::shared_ptr<int> ptr;
```

### **CMake Targets (Namespace):**
```cmake
# Link against clean module targets
target_link_libraries(MyApp
    PRIVATE
        gismo::Common
        gismo::Math
        gismo::Geometry
)
```

## Benefits of New Naming

1. **Follows Standard Practices** - Aligns with Generic C++ Build Pattern
2. **Cleaner Code** - No redundant prefixes when namespace exists
3. **Better IDE Support** - Clearer autocomplete and navigation
4. **Modern CMake** - Proper target-based dependency management
5. **Easier Maintenance** - Less typing, clearer intent

## Migration Strategy

### **For New Modules:**
- Use clean names from the start
- Follow the `Common` module as template
- No `gs` prefixes in new architecture

### **For Existing Modules:**
- Remove `gs` prefix when porting to new architecture
- Update all internal includes
- Maintain backward compatibility during transition

## Implementation Checklist

When creating a new module:

- [ ] Directory name in `PascalCase` (e.g., `Math`)
- [ ] Main header with no extension (e.g., `Math`)
- [ ] Individual headers in `PascalCase.h` (e.g., `Constants.h`)
- [ ] CMakeLists.txt with proper target (`gismo::Math`)
- [ ] Update includes to use clean names
- [ ] No `gs` prefix on any new files
- [ ] Proper FILE_SET HEADERS configuration

---

**Status**: ✅ Implemented in Common module
**Next**: Apply to Math, Geometry, and Matrix modules
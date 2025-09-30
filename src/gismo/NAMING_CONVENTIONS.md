# GISMO Naming Conventions - Modern Architecture

## File Naming Pattern

Following the **Generic C++ Build Pattern**, all modules in the new GISMO architecture use **clean names without prefixes** since we already have proper namespace organization.

### ✅ **New Pattern (Adopted):**

```
src/gismo/
├── Module1/
│   ├── CMakeLists.txt
│   ├── Component1.h      # No gs prefix
│   └── Component2.h      # No gs prefix
└── Module2/
    ├── Component3.h      # gs prefix (old style)
    └── Component4.h      # gs prefix (old style)
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
- Use `PascalCase` for module directory names: `Module1`, `Module2`
- Match the CMake target name: `gismo::Module1`, `gismo::Module2`, etc.

### 2. **Header Files**
- Use `PascalCase.h` for header files: `Component1.h`, `Component2.h`
- **No `gs` prefix** - the namespace provides the scoping
- Descriptive names that indicate functionality

### 3. **Module Header File**
- This follows the Generic C++ Build Pattern
- Includes all module headers for convenience

### 4. **Generated Files**
- Maintains compatibility with existing GISMO conventions
- Generated files are placed in build directory structure

## Usage Examples

### **Consumer Code (Clean):**
```cpp
// include specific headers
#include <gismo/Module1/Component1.h>
#include <gismo/Module2/Component3.h>

// Use with proper namespace
gismo::memory::shared_ptr<int> ptr;
```

### **CMake Targets (Namespace):**
```cmake
# Link against clean module targets
target_link_libraries(MyApp
    PRIVATE
        gismo::Module1
        gismo::Module2
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
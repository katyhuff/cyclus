# Fix macOS AppleClang preprocessing and modern C++ compatibility issues in Cyclus builds

## Summary

This PR fixes a set of build failures encountered on modern macOS systems using AppleClang and contemporary Python/CMake toolchains. The failures manifested during the `cycpp.py` preprocessing stage and later during compilation against `libxml++-2.6`.

The core issues were:

1. `cycpp.py` preprocessing failures on AppleClang/macOS caused by use of `cpp` and the combined `-xc++` argument.
2. Incorrect compiler detection in `UseCyclus.cmake` for AppleClang.
3. Fallback to the legacy `libxml++-2.6` package, which is incompatible with modern C++ standards because it relies on `std::auto_ptr`.

This PR addresses the preprocessing and compiler-selection issues directly and documents the dependency compatibility problem with `libxml++-2.6`.

---

## Symptoms

### Initial preprocessing failures

Builds failed during the `cycpp.py` preprocessing phase with repeated errors such as:

```text
cc: error: no such file or directory: 'c'
cc: error: no input files
```

followed later by downstream exceptions such as:

```text
KeyError: 'cyclus::NullInst'
```

The `KeyError` was not the root cause; it resulted from earlier preprocessing failures preventing superclass metadata from being accumulated correctly.

---

### Secondary failure: missing standard C++ headers

After correcting the preprocessing invocation, preprocessing still failed on macOS with errors such as:

```text
fatal error: 'string' file not found
```

This occurred because the build system was invoking the C preprocessor (`cpp`) rather than the configured C++ compiler frontend.

---

### Later compilation failure against libxml++-2.6

Once preprocessing was corrected, builds progressed further but failed compiling against `libxml++-2.6`:

```text
error: no template named 'auto_ptr' in namespace 'std'
```

This is due to `libxml++-2.6` relying on `std::auto_ptr`, which was removed in modern C++ standards.

---

## Root Cause Analysis

### 1. Incorrect preprocessing invocation in `cycpp.py`

`cycpp.py` used:

```python
cpp_args=('-xc++', '-pipe', '-E', '-DCYCPP')
```

On AppleClang/macOS, this combined argument was being interpreted incorrectly during subprocess invocation, ultimately producing malformed compiler invocations that treated `c` as a standalone input filename.

Using the portable split form:

```python
('-x', 'c++', ...)
```

avoids this issue and works correctly across AppleClang and other compilers.

---

### 2. Incorrect AppleClang handling in `UseCyclus.cmake`

The previous logic special-cased `"Clang"` but not `"AppleClang"`:

```cmake
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(SYS_CPP "clang++")
ELSE()
    SET(SYS_CPP "cpp")
ENDIF()
```

On macOS, `CMAKE_CXX_COMPILER_ID` is `"AppleClang"`, so the build incorrectly fell through to plain `cpp`.

This caused the preprocessing stage to use the system C preprocessor instead of the configured C++ compiler frontend, which in turn prevented standard C++ headers such as `<string>` from being found correctly.

The fix is to treat `"AppleClang"` the same way as `"Clang"` and use `${CMAKE_CXX_COMPILER}`.

---

### 3. Legacy `libxml++-2.6` incompatibility

The build system currently falls back to `libxml++-2.6` if `libxml++-4.0` is unavailable.

`libxml++-2.6` uses `std::auto_ptr`, which was removed from modern C++ standards. On systems compiling in C++17-or-newer modes, this dependency fails to compile.

This is not caused by Cyclus source code itself but by the legacy dependency version.

---

## Changes in This PR

### `cycpp.py`

Changed:

```python
cpp_args=('-xc++', '-pipe', '-E', '-DCYCPP')
```

to:

```python
cpp_args=('-x', 'c++', '-pipe', '-E', '-DCYCPP')
```

This uses the portable argument form expected by AppleClang and avoids malformed preprocessing invocations.

---

### `UseCyclus.cmake`

Updated compiler detection logic from:

```cmake
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(SYS_CPP "clang++")
```

to:

```cmake
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    SET(SYS_CPP "${CMAKE_CXX_COMPILER}")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(SYS_CPP "${CMAKE_CXX_COMPILER}")
```

This ensures that macOS builds use the configured C++ compiler frontend rather than plain `cpp`.

---

## Notes on `libxml++`

This PR does not directly replace `libxml++-2.6`, but the debugging process demonstrated that modern macOS builds should strongly prefer `libxml++-4.0` or newer.

If `libxml++-2.6` remains supported, builds may need to force older C++ standards (e.g. C++14). However, using newer `libxml++` releases is likely the better long-term path.

---

## Tested Environment

- macOS
- AppleClang 16
- CMake 3.30+
- Python 3.14
- MacPorts-installed dependencies
- Conda Python environment

---

## Result

After these preprocessing fixes:

- `cycpp.py` preprocessing succeeds correctly on macOS.
- Standard C++ headers resolve correctly.
- The build progresses normally until encountering the independent `libxml++-2.6` compatibility issue.

Using `libxml++-4.0` resolves the remaining compilation failure.

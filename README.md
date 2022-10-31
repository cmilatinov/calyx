# Calyx

```shell
# Install vcpkg
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.bat

# Install dependencies
vcpkg install "@.vcpkg" --triplet x64-windows

# Run cmake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg.cmake"
```
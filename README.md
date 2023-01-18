# Calyx

```shell
# Install vcpkg
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh

# Install clang llvm-libs uuid

# Install vcpkg dependencies
vcpkg install "@.vcpkg"

# Run cmake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg.cmake"
```
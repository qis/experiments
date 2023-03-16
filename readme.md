# Experiments
C++23 project template that uses [Conan][conan] center for dependencies.

## Tools
Install tools.

<details>
<summary>Linux</summary>

```sh
# CMake
sudo rm -rf /opt/cmake; sudo mkdir -p /opt/cmake
wget https://github.com/Kitware/CMake/releases/download/v3.25.3/cmake-3.25.3-linux-x86_64.tar.gz
sudo tar xf cmake-3.25.3-linux-x86_64.tar.gz -C /opt/cmake --strip-components=1

sudo tee /etc/profile.d/cmake.sh >/dev/null <<'EOF'
export PATH="/opt/cmake/bin:${PATH}"
EOF

sudo chmod 0755 /etc/profile.d/cmake.sh
. /etc/profile.d/cmake.sh

# LLVM
sudo rm -rf /opt/llvm; sudo mkdir -p /opt/llvm
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.6/clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz
sudo tar xf clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz -C /opt/llvm --strip-components=1

sudo tee /etc/profile.d/llvm.sh >/dev/null <<'EOF'
export PATH="/opt/llvm/bin:${PATH}"
EOF

sudo chmod 0755 /etc/profile.d/llvm.sh
. /etc/profile.d/llvm.sh

sudo tee /etc/ld.so.conf.d/llvm.conf >/dev/null <<'EOF'
/opt/llvm/lib/x86_64-unknown-linux-gnu
/opt/llvm/lib
EOF

sudo ldconfig

# Python
sudo apt install -y python3 python3-pip

# Conan
pip install "conan<2.0.0"
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
```

</details>

<details>
<summary>Windows</summary>

1. Download and install [Python][py] and [Visual Studio][vs] with C++ and CMake support.
3. Install [Conan][conan] in `x64 Native Tools Command Prompt for VS 2022`.

```cmd
pip install "conan<2.0.0"
conan profile new default --detect
```

</details>

## Workflow
Use `x64 Native Tools Command Prompt for VS 2022` on Windows.

1. Install dependencies on Windows.

```cmd
conan install third_party -if third_party/windows -pr third_party/windows.profile
```

2. Build dependencies on Linux.

```sh
conan install third_party -if third_party/linux -pr third_party/linux.profile
```

3. Configure project.

```sh
cmake --list-presets

# Windows
cmake --preset debug
cmake --preset release

# Windows & Linux
cmake --preset debug-clang
cmake --preset release-clang

# Windows
cmake --preset debug-clang-cl
cmake --preset release-clang-cl
```

4. Build project.

```sh
# Windows
cmake --build build/debug
cmake --build build/release

# Windows & Linux
cmake --build build/debug-clang
cmake --build build/release-clang

# Windows
cmake --build build/debug-clang-cl
cmake --build build/release-clang-cl
```

[py]: https://www.python.org/downloads/windows/
[vs]: https://visualstudio.microsoft.com/vs/
[cmake]: https://cmake.org/download/
[conan]: https://conan.io/center/
[llvm]: https://llvm.org/

<!--
# Find package on conan center.
conan search -r conancenter boost

# Show build options for package.
conan inspect -r conancenter boost/1.81.0@

# Set executable bit.
git update-index --chmod=+x res/build.sh

# Untrack local changes.
git update-index --no-skip-worktree res/report.md
git add res/report.md
git update-index --skip-worktree res/report.md
-->

# EngagementEstimator

## Requirements
- C++17
- CMake >= 3.14
- OpenCV
- Apache TVM
- Qt

## Building Apache TVM
Full documentation you can find [here](https://tvm.apache.org/docs/install/index.html).

Brief guide:
1. Get Apache TVM source code from GitHub:
   ```bash
   git clone https://github.com/apache/tvm.git; cd tvm
   ```
2. Get submodules:
   ```bash
   git submodule update --init --recursive
   ```
3. Create build directory:
   ```bash
   mkdir build && cd build
   ```
4. Configure build:
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Release -DUSE_LLVM=ON -DUSE_RANDOM=ON -DUSE_GRAPH_RUNTIME=ON -DUSE_SORT=ON  ..
   ```
5. Build `libtvm_runtime`:
   ```bash
   make -j4 tvm_runtime
   ```

## Building
0. Update and initialize submodules:
   ```
   $ git submodule update --init --recursive
   ```
1. Open project in QtCreator.
2. Specify `TVM_BUILD_DIR`. This variable should contain path to directory with `libtvm_runtime`. 
   Go to: `Projects` menu, in section `Build Environment` click on the `Add` button.
3. Click on the `Run` button and the build process will be started. After finishing, the application will run.

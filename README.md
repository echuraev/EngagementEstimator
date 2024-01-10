# EngagementEstimator

This repository contains materials for research related to facial expression
recognition and engagement prediction based on video. 

The research materials are located in the folder [research](research/). Also,
this repository contains desktop C++ application written in Qt.

## Research materials
In [User-independent engagement.ipynb](research/User-independant engagement.ipynb),
the results related to the research of engagement estimation are presented.

### Engagement dataset
To estimate the efficiency of the user-adapted approach, we collected a small
engagement dataset from online lectures. Examples of frames from this dataset
are presented below. The videos in the dataset include 5 persons and videos were
recorded in different orientations. In total 160 videos were recorded, 80 videos
per class (distracted and engaged), and 16 videos for each person. The length of
each video is 4--6 seconds.

![Examples of frames from our dataset](research/our_dataset.png)

This dataset can be downloaded from Google Drive: https://drive.google.com/drive/folders/1e5_H8FSYhpioTb_u_AgEaMKDiXhcqR5t?usp=sharing

## C++ application
### Requirements
- C++17
- CMake >= 3.14
- OpenCV
- Apache TVM
- Qt

### Building Apache TVM
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

### Building
0. Update and initialize submodules:
   ```
   $ git submodule update --init --recursive
   ```
1. Open project in QtCreator.
2. Specify `TVM_BUILD_DIR`. This variable should contain path to directory with `libtvm_runtime`. 
   Go to: `Projects` menu, in section `Build Environment` click on the `Add` button.
3. Click on the `Run` button and the build process will be started. After finishing, the application will run.

## Research papers

[Article](https://www.sciencedirect.com/science/article/pii/S2665963823000441) in Software Impacts:
```BibTex
@article{churaev2023standalone,
  title={A standalone software for real-time facial analysis in online conferences and e-lessons},
  author={Churaev, Egor and Savchenko, Andrey V},
  journal={Software Impacts},
  pages={100507},
  year={2023},
  publisher={Elsevier}
}
```

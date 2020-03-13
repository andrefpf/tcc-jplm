JPEG Pleno Model RC
===

[TOC]

# Part 4, Reference software

Provides reference implementations for the standardized technologies within the JPEG Pleno framework for purpose of reference for prospective implementers of the standard and compliance testing.

## Scope of the project

The aim is to be recognized by ISO/IEC as JPEG Pleno Reference Software (JPEG Pleno Part 4).

### JPEG Pleno
JPEG Pleno aims to provide a standard framework for representing new imaging modalities, such as texture-plus-depth, [light field](https://jpeg.org/jpegpleno/lightfield.html), [point cloud](https://jpeg.org/jpegpleno/pointcloud.html), and [holographic imaging](https://jpeg.org/jpegpleno/holography.html). Such imaging should be understood as light representations inspired by the plenoptic function, regardless of which model captured or created all or part of the content. [Read more](https://jpeg.org/jpegpleno/index.html)

## Licence

[![badge-license]][link-license]


## Current Status



## Known issues and limitations
Initially, only Lightfield images are supported in JPLM (JPEG Pleno Model).


## Prerequisites

#### Compiler
Tested with gcc 9
Must support CXX 17

#### Cmake
Minimum version: 3.10

#### latex (optional)
Latex is required for generating the documentation.

## Quick start 
After building this project, you can use the provided software to encode and decode plenoptic images. 

### Lightfield (Part 2)

#### Encoding
Example:
  ```bash
  ~/jplm/build/$ ../bin/TAppEncoder --cfg ../cfg/config.json --input ~/RAW/Greek/ --output ~/output.jpl
  ```  
#### Decoding
Example:
  ```bash
  ~/jplm/build/$ ../bin/TAppDecoder --input ~/output.jpl --output ~/decoded/greek/
  ```  


#### Encoding Lenslets 13x13 (with darker views)

In VM there are two different treatments for Lenslets 13x13. 
Firstly, the dataset contains more than 13x13 views, but instead of beginning in 000_000, the encoder begins in 001_001.
Secondly, there are four views that are much darker then the others.
To rename the views to start in 000_000 and to brighten the four darker ones, we created a tool called lenslet_13x13_shifter. 

Example:
  ```bash
  ORIGINALS_INPUT_PATH="/home/RAW/TAU/I01_Bikes/"
  ORIGINAL_TEMPORARY_PATH="/home/RAW/TAU/I01_Bikes_shifted/"
  JPLM_BINS="/home/jpeg-pleno/jplm/bin"

  ${JPLM_BINS}/lenslet_13x13_shifter ${ORIGINALS_INPUT_PATH} ${ORIGINAL_TEMPORARY_PATH}
  ${JPLM_BINS}/jpl-encoder-bin --part 2 --type 0 --input ${ORIGINAL_TEMPORARY_PATH} --output ./test_bikes.jpl --transform_size_maximum_inter_view_vertical 13 --transform_size_maximum_inter_view_horizontal 13 --transform_size_maximum_intra_view_vertical 31 --transform_size_maximum_intra_view_horizontal 31 --transform_size_minimum_inter_view_vertical 13 --transform_size_minimum_inter_view_horizontal 13 --transform_size_minimum_intra_view_vertical 4 --transform_size_minimum_intra_view_horizontal 4 --lambda 10000 -t 13 -s 13 -v 434 -u 626
  ```  

We still need to create a inverse lenslet_13x13_shifter to be executed after decoding. 
Yet, the PSNR between a original dark view and its decoded should be the same as the PSNR between a original brighten view and its decoded (also brighten).

## Architecture overview
TODO

## Build instructions

  ```bash
  ~$ cd jplm
  ~/jplm/$ mkdir build; cd build
  ~/jplm/build/$ cmake ..
  ~/jplm/build/$ make -j
  ```

## Library dependencies

The included lightfield visualization tool depends on the X11 libraries. 
If they are not present the compilation will fail. 
To install X11 libraries (must run as sudo):
  ```bash
  ~$ apt install libx11-*
  ```  

On the other hand, it is possible to skip x11 library install and to avoid failing compilation by disabing the LF visualization tool.
Fo that, one needs to call cmake as follows: 
  ```bash
  ~/jplm/build/$ cmake -DVISUALIZATION_TOOL=OFF ..
  ```  


## Testing instructions

  ```bash
  ~/jplm/build/$ ctest
  ```  
  
###  Other usefull commands
#### Colorfull output
```bash
  ~/jplm/build/$ export GTEST_COLOR=1
  ```

#### To show all tests
```bash
  ~/jplm/build/$ ctest --verbose
  ```
or the alternative on Windows Powershell
```powershell
Get-ChildItem "..\bin-debug\tests\" -Filter *_tests.exe | Foreach-Object { Start-Process -NoNewWindow -PassThru -Wait $_.Fullname -ArgumentList "../resources" }
```

#### To run again only failed tests

```bash
  ~/jplm/build/$ ctest --rerun-failed
  ```
or
```bash
  ~/jplm/build/$ ctest --verbose --rerun-failed
  ```

## Deployment instructions
TODO (check if applicable)
##  Contribution instructions
TODO
## Directory layout
For an illustration, see docs/diagrams/PacketDiagram.pdf
  - JPLM
    - cfg
    - doc
      - diagrams
    - bin
    - build
    - source
      - App
            - Common
            - Encoder
            - Decoder
       - Lib
           - Part1
               - Common
               - Encoder
               - Decoder
           - Part2
               - Common
               - Encoder
               - Decoder
           - PartX
               - Common
               - Encoder
               - Decoder



[badge-license]: https://img.shields.io/badge/license-BSD--3--Clause-blue.svg "BSD 3-clause license"
[link-license]: https://gitlab.com/smtlightfieldsteam/jplm/blob/master/LICENSE.md "BSD 3-clause license"
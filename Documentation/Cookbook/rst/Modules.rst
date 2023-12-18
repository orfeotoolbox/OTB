OTB Modules
===========

Since OTB 9.0, the toolbox is split in packages that can be installed independently. This table shows the list of modules :

+-----------------------+-----------------------+
| **Module Name**       | **Criticity**         |                                                           
+=======================+=======================+
| Core and Dependencies | **Mandatory**         |
+-----------------------+-----------------------+
| FeaturesExtraction    | Optional              |
+-----------------------+-----------------------+
| Learning              | Optional              |
+-----------------------+-----------------------+
| StereoProcessing      | Optional              |                           
+-----------------------+-----------------------+
| SAR                   | Optional              |
+-----------------------+-----------------------+
| Hyperspectral         | Optional              | 
+-----------------------+-----------------------+

Core and Dependencies
`````````````````````
These are the two only mandatory modules for OTB.

**OTB-Core** contains optical image preprocessing applications (Pansharpening, Calibration, Orthorectification, etc.), most useful image manipulation tools (ExtractROI, Superimpose, Mosaic, Concatenate, ..) and various applications to manipulate vector data.
**OTB-Dependencies** contains all third-parties libraries needed to execute Core / All applications

For the dependencies, there are two packages available depending on your needs :

- **OTB-All-Dependencies** this is the **package that you should install by default** : contains the dependencies for the core **and** optional modules.If you install the Core and decide to use an optional module afterwards, you just need to install this optional module over the current OTB install. 

- **OTB-Core-Dependencies** suits users who only need the core fonctionnalities of OTB.If you plan to install an optional module afterwards, you need to download and install the "All-Dependencies" package before installing the module.

The application list for each package is described in the ::doc::`Applications`` section

FeaturesExtraction
```````````````````
This optional module contains classical filtering applications, such as texture extraction, edge extraction, smoothing, morphological operations. This module relies on external libraries (MuParser / MuParserX) contained in OTB-All-Dependencies.
it can be installed along with the Core package with a simple tar extract command and directly available after sourcing the otbenv.profile

Learning
````````
This module contains Machine Learning applications based on classical supervised or unsupervised algorithms (SVM, Random Forest, K-Means, Multi-layer Perceptron Neural Network, etc.). Its applications handle the whole processing chain : sample selection, learning, prediction and finalization of a classification map. It also contains a regression framework.

StereoProcessing
````````````````
This module allows to register stereoscopic images and compute disparity in order to build a 3D model from a couple of satellite images in raw / sensor geometry.

SAR
```
This module contains SAR (Synthetic Aperture Radar) applications and covers all the scope of SAR imagery manipulation : SAR Calibration, Unspeckle with temporal filtering, Interferogram, etc.

Hyperspectral
`````````````
This module contains specific applications to handle hyperspectral images : spectral angle classification, end member number estimation or unmixing.
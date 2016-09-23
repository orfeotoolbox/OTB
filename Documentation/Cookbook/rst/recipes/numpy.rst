Numpy processing in OTB Applications
====================================

Input and output images to any OTB application in the form of numpy array is now possible in OTB python wrapping.
The python wrapping only exposes OTB ApplicationEngine module which allow to access existing C++ applications. 
Due to blissful nature of ApplicationEngine's loading mechanism no specific wrapping is required for each application. 

Numpy extension to Python wrapping allows data exchange to application as an array rather than a disk file. 
Ofcourse, it is possible to load an image from file and then convert to numpy array or just provide a file as earlier via
Application.SetParameterString(...). 

This bridge that completes numpy and OTB makes it easy to plug OTB into any image processing chain via python code that uses 
GIS/Image processing tools such as GDAL, GRASS GIS, OSSIM that can deal with numpy.


Below code reads an input image using python pillow (PIL) and convert it to numpy array. This numpy array is
used an input to the application set *SetImageFromNumpyArray(...)* method.
The application used in this example is `ExtractROI <../Applications/app_ExtractROI.html>`_. After extracting 
a small area the output image is taken as numpy array with *GetImageFromNumpyArray(...)* method

::

   import sys
   import os
   import numpy as np
   import otbApplication
   from PIL import Image as PILImage

   pilimage = PILImage.open('poupees.jpg')
   npimage = np.asarray(pilimage)
   inshow(pilimage)

   ExtractROI = otbApplication.Registry.CreateApplication('ExtractROI')
   ExtractROI.SetImageFromNumpyArray('in', npimage)
   ExtractROI.SetParameterInt('startx', 140)
   ExtractROI.SetParameterInt('starty', 120)
   ExtractROI.SetParameterInt('sizex', 150)
   ExtractROI.SetParameterInt('sizey', 150)
   ExtractROI.Execute()

   ExtractOutput = ExtractROI.GetImageAsNumpyArray('out')
   output_pil_image = PILImage.fromarray(np.uint8(ExtractOutput))
   imshow(output_pil_image)


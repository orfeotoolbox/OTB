Common Use Cases
================

You are now ready to use OTB. In this page we describe simple use cases that you may encounter in your OTB usage.

Use Command Line Interface
~~~~~~~~~~~~~~~~~~~~~~~~~~


Use OTB in QGIS
~~~~~~~~~~~~~~~



Use Python API
~~~~~~~~~~~~~~

This use case is based on a raw Pleides product that you want to orthorectify and extract an ROI from.
That product is typically composed of an Panchromatic image (high resolution black and white image) and an XS image (lower resolution with colors) requiering
a BundleToPerfectSensor step, in order to have only one colored image to orthorectify.

.. code-block:: bash

    #!/usr/bin/python
    import otbApplication as otb
    import argparse
    import sys
    import glob
    import os


    def main():
        """
        """
        parser = argparse.ArgumentParser()
        parser.add_argument("inp", help="Input image (Panchro)")
        parser.add_argument("inxs", help="Input image (Multispectral)")
        parser.add_argument("roi", help="Region of interest (georeferenced vector data - shapefile, sqlite, etc.")
        parser.add_argument("out", help="Pansharpened georeferenced image cropped on input ROI")
        parser.add_argument("-geoid", default="none", required=False, action="store", dest="geoid", help="Geoid file (EGM 96) for orthorectification")
        parser.add_argument("-dem", default="none", required=False, action="store", dest="dem", help="DEM directory for orthorectification")
        
        args = parser.parse_args()

        print(f"{args=}")
    

        appPXS = otb.Registry.CreateApplication("BundleToPerfectSensor")
        params_PXS = {"inp":args.inp, "inxs":args.inxs, "out":"in_mem_PXS.tif", "method":"bayes"}
        appPXS.SetParameters(params_PXS)
        # No need to run Execute() because we'll connect output to next application with ConnectImage method
        
        appOrtho = otb.Registry.CreateApplication("OrthoRectification")
        params_Ortho = {"io.out":"in_mem_Ortho.tif"} 
        if args.geoid != "none" and args.dem != "none":
            params_Ortho.update({"elev.geoid":args.geoid, "elev.dem":args.dem})
        # Use BundleToPerfectSensor output as OrthoRectification input
        appOrtho.ConnectImage("io.in", appPXS, "out")

        appROI = otb.Registry.CreateApplication("ExtractROI")
        params_ROI = {"out":args.out,"mode":"fit","mode.fit.vect":args.roi}
        appROI.SetParameters(params_ROI)
        # Plug the last step of our pipeline
        appROI.ConnectImage("in", appOrtho, "io.out")

        # Resolve the pipeline and run !!
        # Only pixels within the ROI will be pansharpened and orthorectified !!
        appROI.ExecuteAndWriteOutput()

    if __name__ == "__main__":
        main()

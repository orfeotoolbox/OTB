Visual product and contrast enhancement
========================================================
Sensor images have often a wide dynamic range. Whereas it is very helpful
to have high precision to do complex processing, it is pretty hard to display
high dynamic images, even on modern screen. The dynamic range for basic
screen is of 8 bits. Images can contain 12 or 16 bits (or even more!) of data.

|image5| |image6|

The contrast enhancement application is aiming at reducing this dynamic
by reorganizing it in a smarter way than just linear compression.
In a linear compression, compression changes the dynamic range (for instance
from 12 to 8 bits) but does not change the repartition of the pixel.

|image1| |image2|

Here the equalization of histogram is creating a look up table in order to
maximize the use of dynamic. The target histogram is a perfectly flat one.
The gain applied on each pixel comes from the computation of the transfer
function :math:`T` such that :

.. math:: \forall i \quad  \int_{min}^{i*T(i)}h_{istogram}(j)dj = 
          \int_{min}^{i}h_{target}(j)dj

where :math:`h_{target}` is the corresponding flat histogram and we have
the constraint that white and black are still white and black after
equalization : 

.. math:: T(min) = T(max) = 1

With this in mind you can then try it on your own images with the simple
following use :

:: 

    otbcli_ContrastEnhancement -in input_image.tif 
                               -out output_image.tif 
                               -spatial global

You can then compress your dynamic without loosing too much detail and
contrast.

Advanced parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
You can see that more parameters are available in the application. Let us see
what there are for.
First what you want to equalize. There are two modes :
* luminance : you provide the application with a 3 bands image and the
equalization will be done on a single band which will be a composition of
the original bands. The computed gain will then be applied on the different
bands. The classical use of this method is to conserve ratio between the
different color, conserve the hue.
* channel : you provide the application with a n bands image and each of
them are equalized independently.

The other option is the local equalization. You can choose a window size
that will be use to split the image in tiles and histograms will be
computed over those tiles. Gain will be interpolated between the adjacent
tiles in order to give a smooth result.

::

    otbcli_ContrastEnhancement -in input_image.tif
                               -out output_image.tif
                               -spatial.local.h 500
                               -spatial.local.w 500
                               -mode lum

The application also offers a way to limit contrast by adjusting original
histogram. The limitation factor represent the limit height that can have
any bucket of the histogram : we compute the height of the perfect flat
histogram and the maximal height is the limitation factor time this "flat
height".

|image4|

Finally you have the choice to ignore a particular value with the "nodata"
parameter, and also the choice to put manually your minimum and maximum value,
which can be a gain of time. Any value out of bound will be ignored.


.. |image1| image:: ../Art/contrast1.png
            :scale: 30%

.. |image2| image:: ../Art/contrast2.png
            :scale: 30%

.. |image4| image:: ../Art/contrast4.png

.. |image5| image:: ../Art/contrast_orig.jpg
            :scale: 30%
            :alt: original


.. |image6| image:: ../Art/contrast_result.jpg
            :scale: 30%
            :alt: result

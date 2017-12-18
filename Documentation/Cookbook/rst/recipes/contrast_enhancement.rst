Visual product and contrast enhancement
========================================================
Sensor images have often a wide dynamic range. Whereas it is very helpful
to have high precision to do complex processing, it is pretty hard to display
wide range of dynamic, even on modern screen. The dynamic range for basic
screen is of 8 bits. Data can contain 12 or 16 bits (or even more!).
The contrast enhancement application is aiming at reducing this dynamic
by compressing it in a smarter way than just linear compression.
In a linear compression, compression changes the dynamic range (for instance
from 12 to 8 bits) but does not change the repartition of the pixel.
//math here
Here the equalization of histogram is creating a look up table in order to
maximize the use of dynamics.
//math here
With this in mind you can then try it on your own images with the simple
following use :
//Syntax here

Advanced parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
You can see that more parameter are available in the application. Let us see
what there are for.
First what you want to equalize. There is two modes :
* luminance : you provide the application with a 3 bands image and the
equalization will be done on a single band which will be a composition of
the original bands. The computed gain will then be applied on the different
bands. The classical use of this method is to conserve ratio between the
different color, conserve the hue.
* channel : you provide the application with a n bands image and each of
them are equalized independently.
//syntax here?
The other main mode is the local equalization. You can choose a window
size that will be use to split the image in tiles and histograms will be
computed over those tiles. Gain will be interpolated between the adjacent
tiles in order to give a smooth result.

The application also offers a way to limit contrast by adjusting original
histogram. The limitation factor represent the limit height that can have
any bucket of the histogram : we compute the height of the perfect flat
histogram and the maximal height is the limitation factor time this "flat
height".
//image here

Finally you have the choice to ignore a particular value with the nodata
parameter, and also the choice to put manually your minimum and maximum value.
Any value out of bound will be ignored.


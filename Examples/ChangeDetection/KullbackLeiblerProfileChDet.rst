This example illustrates the class
:doxygen:`KullbackLeiblerProfileImageFilter` for detecting changes
between pairs of images, according to a range of window size.
This example is very similar, in its principle, to all of the change
detection examples, especially the distance between distributions one
which uses a fixed window size.

The main differences are:

* a set of window range instead of a fixed size of window
* an output of type :doxygen:`VectorImage`

Then, the program begins with the :doxygen:`VectorImage` and the
:doxygen:`KullbackLeiblerProfileImageFilter` header files in addition
to those already detailed in the :doxygen:`MeanRatioImageFilter` example.

.. figure:: /Output/KLProfileChDet.png
    :align: center

    Result of the Kullback-Leibler profile change detector, colored composition including the first, 12th and 24th channel of the generated output.

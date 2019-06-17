On one hand, satellite images are commonly coded on more than 8 bits to provide
the dynamic range required from shadows to clouds. On the other hand, image formats
in use for printing and display are usually limited to 8 bits. We need to convert the value
to enable a proper display. This is usually done using linear scaling. Of course, you have
to be aware that some information is lost in the process.

.. |image1| image:: /Output/QB_Toulouse_Ortho_PAN_casted.png

.. |image2| image:: /Output/QB_Toulouse_Ortho_PAN_rescaled.png

.. _Figure1:

+--------------------------+-------------------------+
|        |image1|          |         |image2|        |
+--------------------------+-------------------------+

    On the left, the image obtained by truncated pixel values at the dynamic acceptable for a png file (between 0 and 255). On the right, the same image with a proper rescaling.

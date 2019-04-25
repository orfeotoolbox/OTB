.. _FunctorFilter:

Using FunctorImageFilter
========================

In image processing and remote sensing, it is very common to write custom pixel-based or neighborhood-based operations between one or several co-registered images. Starting OTB 7.0, there is now a unique filter that will handle most cases:
- Any number of input images, being either Image, VectorImage or a mix of both,
- An Image or VectorImage output
- Operation based on pixels, neighborhoods or a mix of both,
- Functor classes, function, lambdas ...
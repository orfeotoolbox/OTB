.. _FunctorFilter:

Using FunctorImageFilter
========================

In image processing and remote sensing, it is very common to write custom pixel-based or neighborhood-based operations between one or several co-registered images. Starting OTB 7.0, there is now a unique filter ``otb::FunctorImageFilter`` that will handle most cases:
- Any number of input images, being either ``Image``, ``VectorImage`` or a mix of both,
- An ``Image`` or ``VectorImage`` output
- Operation based on pixels, neighborhoods or a mix of both,
- Functor classes, function, lambdas ...

With ``otb::FunctorImageFilter`` you only need to write the operation you want to perform, and the filter will take care of everything (including multi-threading and streaming).

This section will present how to use this filter and its various features.

Quickstart
----------

Defining the operation to perform
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The operation to perform can be defined as a free function:

.. code-block:: cpp

    double myFreeFunction(const double & int in) {...}
    
It can also be defined as a functor (i.e. a class defining operator ``()``:

.. code-block:: cpp

    class MyFunctor
    {
    public:
        double operator()(const double & int in) {...}
    };
    
It can also be defined as a lambda:

.. code-block:: cpp

    auto myLambda = [](const double & int in) -> double {...}
    
Building an instance of ``FunctorImageFilter``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Once the operation to perform has been implemented, it is very easy to get a fully functionning instance of ``FunctorImageFilter`` from it:

.. code-block:: cpp

    auto filterFromFreeFunction = NewFunctorFilter(myFreeFunction);
    auto filterFromFunctor      = NewFunctorFilter(MyFunctor);
    auto filterFromLambda       = NewFunctorFilter(myLambda);

And you can use it just like any other filter:

.. code-block:: cpp
    
    filterFromLambda->SetInput(upStreamFilter->GetOutput());
    downstreamFilter->SetInput(filterFromLambda->GetOutput());
    
You can also directly define instances of ``FunctorImageFilter`` with built-in math functions:

.. code-block:: cpp

    using CosType  = double(double);
    auto filterCos = NewFunctorFilter(static_cast<CosType*>(std::cos));

Note, the ``static_cast`` trick, which allows to disambguiate between different prototypes of the ``cos`` function.

Automatic types deduction
-------------------------
You probably notice that, contrary to other filters in ITK and OTB, there is no need to specify input and output image types. This is because ``FunctorImageFilter`` uses advanced C++ concepts to automatically derive the input and output image types from the free function, operator() or lambda, with the following rules.

Let ``R (T1 t1, T2 t2 ..., TN tn)`` be the signature of the free function, operator() or lambda. Note that the filter conversely supports passing by value ``TN tn`` or by const reference ``const TN & tn``.

First lets define basic types:

- Scalar type (double, float, unsigned int, short ...)
- ``std::complex<T>`` with T a scalar type
- ``itk::FixedArray<T,N>``, ``itk::RGBPixel<T>``, ``itk::RGBAPixel<T>`` with T a scalar type


Automatic input type deduction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
From the basic types, the following deduction rules apply:

- If ``TN`` is a basic type as defined above, the Nth input will be of type ``otb::Image<TN>``
- If ``TN`` is of type ``itk::VariableLengthVector<T>`` with T a basic type as defined above, the Nth input will be of type ``otb::VectorImage<T>``
- If ``TN`` is of type  ``const itk::ConstNeighborhoodIterator<otb::Image<T>> &`` with T a basic type as defined above, the Nth input will be of type ``otb::Image<TN>``
- If ``TN`` is of type  ``const itk::ConstNeighborhoodIterator<otb::VectorImage<T>> &`` with T a basic type as defined above, the Nth input will be of type ``otb::VectorImage<TN>``

Automatic output type deduction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Rules for output type deduction are simpler:
- If ``R`` is a basic type, output of the filter will be of type ``otb::Image<R>``
- If ``R`` is of type ``itk::VariableLengthVector<T>`` with T a basic type as defined above, the output of the filter will be of type ``otb::VectorImage<R>``

Alternative prototype for performance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Automatic type deduction will also work with the following signature:
``void (const R&, T1 t1, T2 t2 ..., TN tn)``

This will be more efficient when ``R`` is of type ``itk::VariableLengthVector<T>`` and should be prefered in this case.

Automatic type deduction examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Consider the following free function:

.. code-block:: cpp

    itk::VariableLenghtVector<double> myFreeFunction(unsigned char a, 
                                                     const std::complex<float> & b,
                                                     const itk::VariableLengthVector<short> &c,
                                                     const itk::ConstNeighborhoodIterator<otb::Image<double>> & d) {...}

When a ``FunctorImageFilter`` is built from this function, the following types will be deduced:

- First input (corresponding to a) will be of type ``otb::Image<unsigned char>``
- Second input (corresponding to b) will be of type ``otb::Image<std::complex<float>>``
- Third input (corresponding to c) will be of type ``otb::VectorImage<short>``
- Fourth input (corresponding to d) will be of type ``otb::Image<double>``
- Output type will be of type ``otb::VectorImage<double>``

This is strictly equivalent to:

.. code-block:: cpp

    void myFreeFunction(const itk::VariableLengthVector<double> & out ,
                        unsigned char a, 
                        const std::complex<float> & b,
                        const itk::VariableLengthVector<short> &c,
                        const itk::ConstNeighborhoodIterator<otb::Image<double>> & d) {...}

Since the output type is of type ``itk::VariableLengthVector<T>``, the latter should be preferred.







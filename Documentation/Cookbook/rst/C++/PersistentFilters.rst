Persistent filters
==================

Introduction
------------

As presented in chapter :ref:`StreamingAndThreading`, OTB has two main mechanisms
to handle large data: streaming allows to process image piece-wise, and
multi-threading allows to process concurrently several pieces of one streaming
block. Using these concepts, one can easily write pixel-wise or
neighborhood-based filters and insert them into a pipeline which will be
scalable with respect to the input image size.

Yet, sometimes we need to compute global features on the whole image.
One example is to determine image mean and variance of the input image
in order to produce a centered and reduced image. The operation of
centering and reducing each pixel is fully compliant with streaming and
threading, but one has to first estimate the mean and variance of the
image. This first step requires to walk the whole image once, and
traditional streaming and multi-threading based filter architecture is
of no help here.

This is because there is a fundamental difference between these two
operations: one supports streaming, and the other needs to perform
streaming. In fact we would like to stream the whole image piece by
piece through some filter that will collect and keep mean and variance
cumulants, and then synthetize theses cumulants to compute the final
mean and variance once the full image as been streamed. Each stream
would also benefit from parallel processing. This is exactly what
persistent filters are for.

Architecture
------------

There are two main objects in the persistent filters framework. The
first is the :doxygen:`PersistentImageFilter`, the second is the
:doxygen:`PersistentFilterStreamingDecorator`.

The persistent filter class
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :doxygen:`PersistentImageFilter` class is a regular
:doxygen-itk:`ImageToImageFilter`, with two additional pure virtual
methods: the ``Synthetize()`` and the ``Reset()`` methods.

Imagine that the ``GenerateData()`` or ``ThreadedGenerateData()``
progressively computes some global feature of the whole image, using
some member of the class to store intermediate results. The
``Synthetize()`` is an additional method which is designed to be called
one the whole image has been processed, in order to compute the final
results from the intermediate results. The ``Reset()`` method is
designed to allow the reset of the intermediate results members so as to
start a fresh processing.

Any sub-class of the :doxygen:`PersistentImageFilter` can be used as a
regular :doxygen-itk:`ImageToImageFilter` (provided that both
``Synthetize()`` and ``Reset()`` have been implemented, but the real
interest of these filters is to be used with the streaming decorator
class presented in the next section.

The streaming decorator class
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :doxygen:`PersistentFilterStreamingDecorator` is a class designed to
be templated with subclasses of the :doxygen:`PersistentImageFilter`. It
provides the mechanism to stream the whole image through the templated
filter, using a third class called
:doxygen:`StreamingImageVirtualWriter`. When the ``Update()`` method is
called on a :doxygen:`PersistentFilterStreamingDecorator`, a pipeline
plugging the templated subclass of the :doxygen:`PersistentImageFilter`
to an instance of :doxygen:`StreamingImageVirtualWriter` is created. The
latter is then updated, and acts like a regular
:doxygen:`ImageFileWriter` but it does not actually write anything to
the disk : streaming pieces are requested and immediately discarded. The
:doxygen:`PersistentFilterStreamingDecorator` also calls the ``Reset()``
method at the beginning and the ``Synthetize()`` method at the end of
the streaming process. Therefore, it packages the whole mechanism for
the use of a :doxygen:`PersistentImageFilter`:

#. Call the ``Reset()`` method on the filter so as to reset any
   temporary results members,

#. Stream the image piece-wise through the filter,

#. Call the ``Synthetize()`` method on the filter so as to compute the
   final results.

There are some methods that allows to tune the behavior of the
:doxygen:`StreamingImageVirtualWriter`, allowing to change the image
splitting methods (tiles or strips) or the size of the streams with
respect to some target available amount of memory. Please see the class
documentation for details. The instance of the
:doxygen:`StreamingImageVirtualWriter` can be retrieved from the
:doxygen:`PersistentFilterStreamingDecorator` through the
``GetStreamer()`` method.

Though the internal filter of the
:doxygen:`PersistentFilterStreamingDecorator` can be accessed through
the ``GetFilter()`` method, the class is often derived to package the
streaming-decorated filter and wrap the parameters setters and getters.

An end-to-end example
---------------------

This is an end-to-end example to compute the mean over a full image,
using a streaming and threading-enabled filter. Please note that only
specific details are explained here. For more general information on how
to write a filter, please refer to section :ref:`Filters`.

First step: writing a persistent filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first step is to write a persistent mean image filter. We need to
include the appropriate header :

.. code-block:: cpp

    #include "otbPersistentImageFilter.h"

Then, we declare the class prototype as follows:

.. code-block:: cpp

    template<class TInputImage>
    class ITK_EXPORT PersistentMeanImageFilter :
      public PersistentImageFilter<TInputImage, TInputImage>

Since the output image will only be used for streaming purpose, we do
not need to declare different input and output template types.

In the *private* section of the class, we will declare a member which
will be used to store temporary results, and a member which will be used
to store the final result.

.. code-block:: cpp

    private:
      // Temporary results container
      std::vector<PixelType> m_TemporarySums;

      // Final result member
      double m_Mean;

Next, we will write the ``Reset()`` method implementation in the
*protected* section of the class. Proper allocation of the temporary
results container with respect to the number of threads is handled here.

.. code-block:: cpp

    protected:
      virtual void Reset()
      {
        // Retrieve the number of threads
        unsigned int numberOfThreads = this->GetNumberOfThreads();

        // Reset the temporary results container
        m_TemporarySums = std::vector<PixelType>(numberOfThreads, itk::NumericTraits<PixelType>::Zero);

        // Reset the final result
        m_Mean = 0.;
      }

Now, we need to write the ``ThreadedGenerateData()`` methods (also in
the *protected* section), were temporary results will be computed for
each piece of stream.

.. code-block:: cpp

    virtual void ThreadedGenerateData(const RegionType&
                                      outputRegionForThread,
                                      itk::ThreadIdType threadId)
    {
      // Enable progress reporting
      itk::ProgressReporter(this,threadId,outputRegionForThread.GetNumberOfPixels());

      // Retrieve the input pointer
      InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());

      // Declare an iterator on the region
      itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr,
      outputRegionForThread);

      // Walk the region of the image with the iterator
      for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel())
      {
        // Retrieve pixel value
        const PixelType& value = it.Get();

        // Update temporary results for the current thread
        m_TemporarySums[threadId]+= value;
      }
    }

Last, we need to define the ``Synthetize()`` method (still in the
*protected* section), which will yield the final results:

.. code-block:: cpp

    virtual void Synthetize()
    {
      // For each thread
      for(unsigned int threadId = 0; threadId <this->GetNumberOfThreads();++threadId)
      {
        // Update final result
        m_Mean+=m_TemporarySums[threadId];
      }

      // Complete calculus by dividing by the total number of pixels
      unsigned int nbPixels = this->GetInput()->GetLargestPossibleRegion().GetNumberOfPixels();

      if(nbPixels != 0)
      {
        m_Mean /= nbPixels;
      }
    }

Second step: Decorating the filter and using it
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now, to use the filter, one only has to decorate it with the
:doxygen:`PersistentFilterStreamingDecorator`. First step is to include
the appropriate header:

.. code-block:: cpp

    #include "otbPersistentMeanImageFilter.h"
    #include "otbPersistentFilterStreamingDecorator.h"

Then, we decorate the filter with some typedefs:

.. code-block:: cpp

    typedef otb::PersistentMeanImageFilter<ImageType> PersitentMeanFilterType;
    typedef otb::PersistentFilterStreamingDecorator <PersitentMeanFilterType> StreamingMeanFilterType;

Now, the decorated filter can be used like any standard filter:

.. code-block:: cpp

    StreamingMeanFilterType::Pointer filter = StreamingMeanFilterType::New();

    filter->SetInput(reader->GetOutput());
    filter->Update();

Third step: one class to rule them all
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It is often convenient to avoid the few typedefs of the previous section
by deriving a new class from the decorated filter:

.. code-block:: cpp

    template<class TInputImage>
    class ITK_EXPORT StreamingMeanImageFilter :
      public PersistentFilterStreamingDecorator<PersistentImageFilter<TInputImage, TInputImage>>

This also allows to redefine setters and getters for parameters,
avoiding to call the ``GetFilter()`` method to set them.

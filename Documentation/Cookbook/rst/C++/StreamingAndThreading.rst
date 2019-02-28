.. _StreamingAndThreading:

Streaming and Threading
=======================

Streaming and threading are two
different things even if they are linked to a certain extent. In OTB:

-  Streaming describes the ability to combine the processing of several
   portion of a big image and to make the output identical as what you
   would have got if the whole image was processed at once. Streaming is
   compulsory when you’re processing gigabyte images.

-  Threading is the ability to process simultaneously different parts of
   the image. Threading will give you some benefits only if you have a
   fairly recent processor.

To sum up: streaming is good if you have big images, threading is good
if you have several processing units.

However, these two properties are not unrelated. Both rely on the filter
ability to process parts of the image and combine the result, that's what
the ``ThreadedGenerateData()`` method can do.

Streaming and threading in OTB
------------------------------

For OTB, streaming is pipeline related while threading is filter
related. If you build a pipeline where one filter is not streamable, the
whole pipeline is not streamable: at one point, you would hold the
entire image in memory. Whereas you will benefit from a threaded filter
even if the rest of the pipeline is made of non-threadable filters (the
processing time will be shorter for this particular filter).

Even if you use a non streamed writer, each filter which has a
``ThreadedGenerateData()`` will split the image into two and send each part
to one thread and you will notice two calls to the function.

If you have some particular requirement and want to use only one thread,
you can call the ``SetNumberOfThreads()`` method on each of your filter.

When you are writing your own filter, you have to follow some rules to
make your filter streamable and threadable. Some details are provided in
sections :ref:`StreamingLargeData` and :ref:`ThreadedFilterExecution`.

Division strategies
-------------------

The division of the image occurs generally at the writer level.
Different strategies are available and can be specified explicitly. In
OTB, these are referred as *splitter*. Several available splitters are:

-  :doxygen-itk:`ImageRegionSplitter`

-  :doxygen-itk:`ImageRegionMultidimensionalSplitter`

-  :doxygen:`ImageRegionNonUniformMultidimensionalSplitter`

You can add your own strategies based on these examples.

To change the splitting strategy of the writer, you can use the
following model:

.. code-block:: cpp

    typedef otb::ImageRegionNonUniformMultidimensionalSplitter<3> splitterType;
    splitterType::Pointer splitter=splitterType::New() ;
    writer->SetRegionSplitter(splitter);

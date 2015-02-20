/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbObjectListSource_h
#define __otbObjectListSource_h

#include "itkProcessObject.h"
#include "otbObjectList.h"

namespace otb
{

/** \class ObjectListSource
   * \brief Base class for all process objects that output ObjectList data.
  *
  *
   * ObjectListSource is the base class for all process objects that output ObjectList data.
   * Specifically, this class defines the GetOutput() method that returns a pointer to the
   * output ObjectList.
   *
   * Be aware that this class is templated over the list type, not the object type. It will
   * be typically something like otb::ObjectList<ObjectType>. This is to enable the use of
   * class derived from ObjectList or other implementations.
   *
   * \ingroup ObjectListFilter
 *
 * \ingroup OTBObjectList
 */
template <class TOutputList>
class ITK_EXPORT ObjectListSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ObjectListSource              Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ObjectListSource, ProcessObject);

  /** Some typedefs. */
  typedef TOutputList                   OutputListType;
  typedef typename TOutputList::Pointer OutputListPointer;

  typedef itk::DataObject::Pointer DataObjectPointer;

  /** Make a DataObject of the correct type to used as the specified
   * output.  Every ProcessObject subclass must be able to create a
   * DataObject that can be used as a specified output. This method
   * is automatically called when DataObject::DisconnectPipeline() is
   * called.  DataObject::DisconnectPipeline, disconnects a data object
   * from being an output of its current source.  When the data object
   * is disconnected, the ProcessObject needs to construct a replacement
   * output data object so that the ProcessObject is in a valid state.
   * So DataObject::DisconnectPipeline eventually calls
   * ProcessObject::MakeOutput. Note that MakeOutput always returns a
   * SmartPointer to a DataObject. If a subclass of ImageSource has
   * multiple outputs of different types, then that class must provide
   * an implementation of MakeOutput(). */
  virtual DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx);

  /** Graft the specified DataObject onto this ProcessObject's output.
  * This method grabs a handle to the specified DataObject's path
  * data to use as its output's own path data. It also copies the
  * region ivars (RequestedRegion, BufferedRegion,
  * LargestPossibleRegion) and meta-data (Spacing, Origin) from the
  * specified data object into this filter's output data object. Most
  * importantly, however, it leaves the Source ivar untouched so the
  * original pipeline routing is intact. This method is used when a
  * process object is implemented using a mini-pipeline which is
  * defined in its GenerateData() method.  The usage is:
  *
  * \code
  *    // setup the mini-pipeline to process the input to this filter
  *    firstFilterInMiniPipeline->SetInput( this->GetInput() );
  *
  *    // setup the mini-pipeline to calculate the correct regions
  *    // and write to the appropriate bulk data block
  *    lastFilterInMiniPipeline->GraftOutput( this->GetOutput() );
  *
  *    // execute the mini-pipeline
  *    lastFilterInMiniPipeline->Update();
  *
  *    // graft the mini-pipeline output back onto this filter's output.
  *    // this is needed to get the appropriate regions passed back.
  *    this->GraftOutput( lastFilterInMiniPipeline->GetOutput() );
  * \endcode
  *
  * For proper pipeline execution, a filter using a mini-pipeline
  * must implement the GeneratseInputRequestedRegion(),
  * GenerateOutputRequestedRegion(), GenerateOutputInformation() and
  * EnlargeOutputRequestedRegion() methods as necessary to reflect
  * how the mini-pipeline will execute (in other words, the outer
  * filter's pipeline mechanism must be consistent with what the
   * mini-pipeline will do). */
  void GraftOutput(itk::DataObject *graft);

  /** Graft the specified data object onto this ProcessObject's idx'th
  * output. This is the similar to GraftOutput method except is
  * allows you specify which output is affected. The specified index
  * must be a valid output number (less than
  * ProcessObject::GetNumberOfOutputs()). See the GraftOutput for
   * general usage information. */
  void GraftNthOutput(DataObjectPointerArraySizeType idx, itk::DataObject *graft);

  /** Get the output data of this process object.  The output of this
  * function is not valid until an appropriate Update() method has
  * been called, either explicitly or implicitly.  Both the filter
  * itself and the data object have Update() methods, and both
  * methods update the data.  Here are three ways to use
  * GetOutput() and make sure the data is valid.  In these
  * examples, \a image is a pointer to some Image object, and the
  * particular ProcessObjects involved are filters.  The same
  * examples apply to non-image (e.g. Mesh) data as well.
  *
  * \code
  *   anotherFilter->SetInput( someFilter->GetOutput() );
  *   anotherFilter->Update();
  * \endcode
  *
  * In this situation, \a someFilter and \a anotherFilter are said
  * to constitute a \b pipeline.
  *
  * \code
  *   image = someFilter->GetOutput();
  *   image->Update();
  * \endcode
  *
  * \code
  *   someFilter->Update();
  *   image = someFilter->GetOutput();
  * \endcode
  * (In the above example, the two lines of code can be in
  * either order.)
  *
  * Note that Update() is not called automatically except within a
  * pipeline as in the first example.  When \b streaming (using a
  * StreamingImageFilter) is activated, it may be more efficient to
  * use a pipeline than to call Update() once for each filter in
  * turn.
  *
  * For an image, the data generated is for the requested
  * Region, which can be set using ImageBase::SetRequestedRegion().
  * By default, the largest possible region is requested.
   */
  OutputListType * GetOutput(void);
  OutputListType * GetOutput(DataObjectPointerArraySizeType idx);

protected:
  /** Constructor */
  ObjectListSource();
  /** Destructor */
  virtual ~ObjectListSource() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Ensure that the output lists are cleared before processing */
  virtual void  AllocateOutputs();

  /** ObjectListSource can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void GenerateData(void);

private:
  ObjectListSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectListSource.txx"
#endif

#endif

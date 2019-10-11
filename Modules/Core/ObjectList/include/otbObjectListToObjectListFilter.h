/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbObjectListToObjectListFilter_h
#define otbObjectListToObjectListFilter_h

#include "otbObjectListSource.h"

namespace otb
{

/** \class ObjectListToObjectListFilter
   * \brief Base class for filters that take an ObjectList as input and produce an ObjectList as output.
   *
   * ObjectListToObjectListFilter is the base class for all process objects that output
   * ObjectList data and require ObjectList data as input. Specifically, this class
   * defines the SetInput() method for defining the input to a filter.
   *
   * Be aware that this class is templated over the list type, not the object type. It will
   * be typically something like otb::ObjectList<ObjectType>. This is to enable the use of
   * class derived from ObjectList or other implementations.
   *
   *
   * \ingroup ObjectListFilter
 *
 * \ingroup OTBObjectList
 */
template <class TInputList, class TOutputList>
class ITK_EXPORT ObjectListToObjectListFilter : public otb::ObjectListSource<TOutputList>
{
public:
  /** Standard class typedefs. */
  typedef ObjectListToObjectListFilter       Self;
  typedef otb::ObjectListSource<TOutputList> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ObjectListToObjectListFilter, ObjectListSource);

  /** Some typedefs. */
  typedef TInputList                          InputListType;
  typedef TOutputList                         OutputListType;
  typedef typename TInputList::ConstPointer   InputListPointer;
  typedef typename TOutputList::Pointer       OutputListPointer;
  typedef typename TInputList::ConstIterator  InputListIterator;
  typedef typename InputListType::ObjectType  InputObjectType;
  typedef typename OutputListType::ObjectType OutputObjectType;

  typedef itk::DataObject::Pointer DataObjectPointer;

  using Superclass::SetInput;
  virtual void SetInput(const InputListType* input);
  const InputListType* GetInput(void);

protected:
  /** Constructor */
  ObjectListToObjectListFilter();
  /** Destructor */
  ~ObjectListToObjectListFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData(void) override;

  /** Multi-threading implementation */

  typedef std::vector<OutputListPointer> OutputListForThreadType;

  virtual void BeforeThreadedGenerateData();

  virtual void AfterThreadedGenerateData()
  {
  }

  virtual int SplitRequestedRegion(itk::ThreadIdType threadId, int threadCount, unsigned int requestedElements, unsigned int& startIndex,
                                   unsigned int& stopIndex);

  /** startIndex and stopIndex represent the indices of the Objects
   * to examine in thread threadId */
  virtual void ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex, itk::ThreadIdType threadId);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
  static ITK_THREAD_RETURN_TYPE ThreaderCallback(void* arg);

  /** Internal structure used for passing image data into the threading library */
  struct ThreadStruct
  {
    Pointer Filter;
  };

  OutputListForThreadType m_ObjectListPerThread;

  /** End Multi-threading implementation */

private:
  ObjectListToObjectListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectListToObjectListFilter.hxx"
#endif

#endif

/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbUnaryFunctorObjectListBooleanFilter_h
#define otbUnaryFunctorObjectListBooleanFilter_h

#include "otbObjectListToObjectListFilter.h"

namespace otb
{

/** \class UnaryFunctorObjectListBooleanFilter
 * \brief Filter an ObjectList and keep elements according to the functor value
 *
 * Only elements of the list for which the functor evaluate as true are kept in the
 * output list. This is similar to the python filter() function.
 *
 *
   * \ingroup ObjectListFilter
 *
 * \ingroup OTBObjectList
 */
template <class TInputList, class TOutputList, class TFunction>
class ITK_EXPORT UnaryFunctorObjectListBooleanFilter : public otb::ObjectListToObjectListFilter<TInputList, TOutputList>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorObjectListBooleanFilter Self;
  typedef otb::ObjectListToObjectListFilter<TInputList, TOutputList> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorObjectListBooleanFilter, ObjectListToObjectListFilter);

  /** Some typedefs. */
  typedef TFunction                           FunctorType;
  typedef TInputList                          InputListType;
  typedef TOutputList                         OutputListType;
  typedef typename TInputList::ConstPointer   InputListPointer;
  typedef typename TOutputList::Pointer       OutputListPointer;
  typedef typename TInputList::ConstIterator  InputListIterator;
  typedef typename TOutputList::ConstIterator OutputListIterator;

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    if (m_Functor != functor)
    {
      m_Functor = functor;
      this->Modified();
    }
  }

protected:
  UnaryFunctorObjectListBooleanFilter();
  ~UnaryFunctorObjectListBooleanFilter() override
  {
  }

  /** Multi-threading implementation */

  void AfterThreadedGenerateData() override;

  /** startIndex and stopIndex represent the indices of the Objects to
  examine in thread threadId */
  void ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex, itk::ThreadIdType threadId) override;

  /** Internal structure used for passing image data into the threading library */
  struct ThreadStruct
  {
    Pointer Filter;
  };

  /** End Multi-threading implementation */

private:
  UnaryFunctorObjectListBooleanFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  FunctorType m_Functor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryFunctorObjectListBooleanFilter.hxx"
#endif

#endif

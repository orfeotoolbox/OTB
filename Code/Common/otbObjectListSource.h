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
   *
   * \ingroup ObjectListFilter
 */
  template <class TOutputList >
      class ITK_EXPORT ObjectListSource : public itk::ProcessObject 
      {
        public:
          /** Standard class typedefs. */
          typedef ObjectListSource  Self;
          typedef itk::ProcessObject  Superclass;
          typedef itk::SmartPointer<Self>   Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;

          /** Method for creation through the object factory. */
          itkNewMacro(Self);
  
          /** Run-time type information (and related methods). */
          itkTypeMacro(ObjectListSource, ProcessObject);

          /** Some typedefs. */
          typedef TOutputList OutputListType;
          typedef typename TOutputList::Pointer OutputListPointer;
  
          typedef itk::DataObject::Pointer DataObjectPointer;

          virtual DataObjectPointer MakeOutput(unsigned int idx);
          void GraftOutput(itk::DataObject *graft);
          void GraftNthOutput(unsigned int idx, itk::DataObject *graft);
          OutputListType * GetOutput(void);

  
        protected:
          ObjectListSource();
          virtual ~ObjectListSource() {};


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
          void GenerateData(void);

        private:
          ObjectListSource(const Self&); //purposely not implemented
          void operator=(const Self&); //purposely not implemented

      };

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbObjectListSource.txx"
#endif

#endif

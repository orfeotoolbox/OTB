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

#ifndef __otbVectorDataSource_h
#define __otbVectorDataSource_h


#include "itkProcessObject.h"
#include "itkMacro.h"

namespace otb
{
/** \class VectorDataSource
 * \brief Filter hierarchy for generating VectorData
 *
 *
 * \ingroup IO
 *
 */

template <class TOutputVectorData>
class ITK_EXPORT VectorDataSource : public itk::ProcessObject
{
public:

  /** Standard class typedefs. */
  typedef VectorDataSource              Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataSource, itk::ProcessObject);

  typedef TOutputVectorData                   OutputVectorDataType;
  typedef typename TOutputVectorData::Pointer OutputVectorDataPointer;

  /** Graft the specified DataObject onto this ProcessObject's output.
   * This method grabs a handle to the specified DataObject's bulk
   * data to used as its output's own bulk data.
   * Most importantly, however, it leaves the Source ivar untouched so
   * the original pipeline routing is intact. This method is used when
   * a process object is implemented using a mini-pipeline which is
   * defined in its GenerateData() method.
   *  */
  virtual void GraftOutput(itk::DataObject *output);

  /** Graft the specified data object onto this ProcessObject's idx'th
   * output. This is similar to the GraftOutput method except it
   * allows you to specify which output is affected. The specified index
   * must be a valid output number (less than
   * ProcessObject::GetNumberOfOutputs()). See the GraftOutput for
   * general usage information. */
  virtual void GraftNthOutput(unsigned int idx, itk::DataObject *output);

  /** Overriding GetOutput() method */
  virtual OutputVectorDataType* GetOutput(void);
  virtual OutputVectorDataType* GetOutput(unsigned int idx);

protected:
  VectorDataSource();
  virtual ~VectorDataSource();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Ensure that the output vector data are cleared before processing */
  virtual void  AllocateOutputs();

private:
  VectorDataSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataSource.txx"
#endif

#endif // __otbVectorDataSource_h

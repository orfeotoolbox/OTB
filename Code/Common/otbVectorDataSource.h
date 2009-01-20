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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkProcessObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

namespace otb
{
/** \class VectorDataSource
 * \brief Famille de filtres ayant un object de type shapeFile en sortie
 *
 *
 * \ingroup IO
 *
 */

template <class TOutputVectorData>
class ITK_EXPORT VectorDataSource : public itk::ProcessObject
{
public :

  /** Standard class typedefs. */
  typedef VectorDataSource                      Self;
  typedef itk::ProcessObject                    Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataSource, itk::ProcessObject);

  typedef TOutputVectorData     OutputVectorDataType;
  typedef typename TOutputVectorData::Pointer OutputVectorDataPointer;

  /** Overriding GetOutput() method */
  virtual OutputVectorDataType* GetOutput(void);
  virtual OutputVectorDataType* GetOutput(unsigned int idx);

protected:
  VectorDataSource();
  ~VectorDataSource();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Ensure that the output vector data are cleared before processing */
  virtual void  AllocateOutputs();

private:
  VectorDataSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataSource.txx"
#endif

#endif // __otbVectorDataSource_h



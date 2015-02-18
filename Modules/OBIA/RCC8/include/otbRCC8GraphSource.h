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
#ifndef __otbRCC8GraphSource_h
#define __otbRCC8GraphSource_h

#include "itkProcessObject.h"

namespace otb
{
/** \class RCC8GraphSource
 *  \brief Base class for all the filters producing an otbRCC8Graph
 * \ingroup DataSources
 *
 * \ingroup OTBRCC8
 */
template <class TOutputGraph>
class ITK_EXPORT RCC8GraphSource
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef RCC8GraphSource               Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(RCC8GraphSource, itk::ProcessObject);
  /** Data object pointer type */
  typedef itk::DataObject::Pointer DataObjectPointer;
  /** Template parameter typedef*/
  typedef TOutputGraph                      OutputGraphType;
  typedef typename OutputGraphType::Pointer OutputGraphPointerType;
  /** Overiding of the GetOutput() method */
  virtual OutputGraphType * GetOutput(void);

protected:
  /** Constructor */
  RCC8GraphSource();
  /** Destructor */
  virtual ~RCC8GraphSource() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RCC8GraphSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8GraphSource.txx"
#endif
#endif

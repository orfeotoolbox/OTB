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

#ifndef __otbGISTableSource_h
#define __otbGISTableSource_h


#include "itkProcessObject.h"
#include "itkMacro.h"

namespace otb
{
/** \class GISTableSource
 * \brief Filter hierarchy for generating GISTables
 *
 *
 * \ingroup IO
 *
 */

template <class TOutputGISTable>
class ITK_EXPORT GISTableSource : public itk::ProcessObject
{
public:

  /** Standard class typedefs. */
  typedef GISTableSource                Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GISTableSource, itk::ProcessObject);

  typedef TOutputGISTable                   OutputGISTableType;
  typedef typename TOutputGISTable::Pointer OutputGISTablePointer;

  /** Overriding GetOutput() method */
  virtual OutputGISTableType* GetOutput(void);
  virtual OutputGISTableType* GetOutput(unsigned int idx);

protected:
  GISTableSource();
  virtual ~GISTableSource();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Ensure that the output vector data are cleared before processing */
  virtual void  AllocateOutputs();

private:
  GISTableSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTableSource.txx"
#endif

#endif // __otbGISTableSource_h

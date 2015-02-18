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

#ifndef __otbLabelMapSource_h
#define __otbLabelMapSource_h


#include "itkProcessObject.h"
#include "itkMacro.h"

namespace otb
{
/** \class LabelMapSource
 * \brief Base class for filter that output LabelMap
 *
 * \ingroup IO
 *
 */

template <class TOutputLabelMap>
class ITK_EXPORT LabelMapSource : public itk::ProcessObject
{
public:

  /** Standard class typedefs. */
  typedef LabelMapSource                Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapSource, itk::ProcessObject);

  typedef TOutputLabelMap                   OutputLabelMapType;
  typedef typename TOutputLabelMap::Pointer OutputLabelMapPointer;

  /** Overriding GetOutput() method */
  virtual OutputLabelMapType* GetOutput(void);
  virtual OutputLabelMapType* GetOutput(unsigned int idx);

protected:
  LabelMapSource();
  ~LabelMapSource();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Ensure that the output vector data are cleared before processing */
  virtual void  AllocateOutputs();

private:
  LabelMapSource(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapSource.txx"
#endif

#endif // __otbLabelMapSource_h

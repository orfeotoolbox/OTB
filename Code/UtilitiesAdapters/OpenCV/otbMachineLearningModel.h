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
#ifndef __otbMachineLearningModel_h
#define __otbMachineLearningModel_h

#include "itkLightObject.h"

namespace otb
{
template <class TInputValue, class TOutputValue>
class ITK_EXPORT MachineLearningModel 
  : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModel                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(MachineLearningModel, itk::DataObject);

protected:
  /** Constructor */
  MachineLearningModel();

  /** Destructor */
  virtual ~MachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMachineLearningModel.txx"
#endif

#endif

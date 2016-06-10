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

#ifndef __otbDEMConvertAdapter_h
#define __otbDEMConvertAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

class DEMConvertAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef DEMConvertAdapter             Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EllipsoidAdapter, itk::Object);

  int Convert(std::string tempFilename, std::string output);

protected:
  DEMConvertAdapter();
  ~DEMConvertAdapter() ITK_OVERRIDE;

private:
  DEMConvertAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // namespace otb

#endif

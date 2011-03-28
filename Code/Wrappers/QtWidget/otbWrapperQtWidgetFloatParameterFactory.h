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
#ifndef __otbWrapperQtWidgetFloatParameterFactory_h
#define __otbWrapperQtWidgetFloatParameterFactory_h

#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterFactory.h"

namespace otb
{
namespace Wrapper
{

class QtWidgetFloatParameterFactory
: public QtWidgetParameterFactory
{
public:
  /** Standard class typedef */
  typedef QtWidgetFloatParameterFactory Self;
  typedef QtWidgetParameterFactory          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);
  
  /** RTTI support */
  itkTypeMacro(QtWidgetFloatParameterFactory,otb::Wrapper::QtWidgetParameterFactory);

  /** Create the appropriate ImageIO depending on the particulars of the file. */
  static QWidget * CreateQtWidget(Parameter * param);

protected:
  QtWidgetFloatParameterFactory();
  ~QtWidgetFloatParameterFactory();

private:
  QtWidgetFloatParameterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}
}

#endif

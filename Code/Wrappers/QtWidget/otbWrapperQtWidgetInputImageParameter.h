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
#ifndef __otbWrapperQtWidgetInputImageParameter_h
#define __otbWrapperQtWidgetInputImageParameter_h

#include <QtGui>
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetInputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputImageParameter(InputImageParameter*, QtWidgetModel*);
  virtual ~QtWidgetInputImageParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

protected:
  void CreateWidget();

private:
  QtWidgetInputImageParameter(const QtWidgetInputImageParameter&); //purposely not implemented
  void operator=(const QtWidgetInputImageParameter&); //purposely not implemented

  InputImageParameter::Pointer m_InputImageParam;
};


}
}

#endif

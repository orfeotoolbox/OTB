/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdWrapperQtWidgetParameterFactory.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbConfigure.h"

#include "otbWrapperParameter.h"
#include "otbWrapperQtWidgetModel.h"

#include "otbWrapperQtWidgetParameterBase.h"
#include "otbWrapperQtWidgetEmptyParameter.h"
#include "otbWrapperQtWidgetIntParameter.h"
#include "otbWrapperQtWidgetFloatParameter.h"
#include "otbWrapperQtWidgetStringParameter.h"
#include "otbWrapperQtWidgetStringListParameter.h"
#include "otbWrapperQtWidgetListViewParameter.h"
#include "otbWrapperQtWidgetComplexInputImageParameter.h"
#include "otbWrapperQtWidgetComplexOutputImageParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputFilenameListParameter.h"
#include "otbWrapperQtWidgetOutputFilenameParameter.h"
#include "otbWrapperQtWidgetDirectoryParameter.h"
#include "otbWrapperQtWidgetInputVectorDataListParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetRAMParameter.h"
#if OTB_VERSION_CHECK( 3, 18, 2 )
#  include "otbWrapperQtWidgetOutputProcessXMLParameter.h"
#  include "otbWrapperQtWidgetInputProcessXMLParameter.h"
#endif

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdWrapperQtWidgetChoiceParameter.h"
#include "mvdWrapperQtWidgetInputImageParameter.h"
#include "mvdWrapperQtWidgetParameterGroup.h"
#include "mvdWrapperQtWidgetInputImageListParameter.h"

namespace mvd
{
namespace Wrapper
{

/*
  TRANSLATOR mvd::ApplicationLauncher

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
template <class TParameterType, class TQtWidget>
class QtWidgetParameterGenericFactory
{
public:

  static bool CanCreate( otb::Wrapper::Parameter* param )
  {
    return dynamic_cast<TParameterType *>(param) != 0;
  }

  static
  otb::Wrapper::QtWidgetParameterBase*
  Create( otb::Wrapper::Parameter* param, 
	  otb::Wrapper::QtWidgetModel* model )
  {
    otb::Wrapper::QtWidgetParameterBase* widget = 0;
    TParameterType* specificParam = dynamic_cast<TParameterType *>(param);

    if (specificParam)
      {
      widget = new TQtWidget(specificParam, model);
      }

    return widget;
  }
};

/*******************************************************************************/
QtWidgetParameterFactory::QtWidgetParameterFactory()
{
}

/*******************************************************************************/
QtWidgetParameterFactory::~QtWidgetParameterFactory()
{
}

/*******************************************************************************/
otb::Wrapper::QtWidgetParameterBase*
QtWidgetParameterFactory::CreateQtWidget( otb::Wrapper::Parameter* param, 
                                          otb::Wrapper::QtWidgetModel* model )
{
  otb::Wrapper::QtWidgetParameterBase* widget = 0;

#define CREATEWIDGET( ParameterType, WidgetType ) \
  else if ( mvd::Wrapper::QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::CanCreate(param) ) \
    { \
    widget = mvd::Wrapper::QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::Create(param, model); \
    }

  if (0) {}
  CREATEWIDGET(otb::Wrapper::EmptyParameter,          
               otb::Wrapper::QtWidgetEmptyParameter)
  CREATEWIDGET(otb::Wrapper::IntParameter,            
               otb::Wrapper::QtWidgetIntParameter)
  CREATEWIDGET(otb::Wrapper::FloatParameter,          
               otb::Wrapper::QtWidgetFloatParameter)
  CREATEWIDGET(otb::Wrapper::InputFilenameParameter,       
               otb::Wrapper::QtWidgetInputFilenameParameter)
  CREATEWIDGET(otb::Wrapper::InputFilenameListParameter,       
               otb::Wrapper::QtWidgetInputFilenameListParameter)
  CREATEWIDGET(otb::Wrapper::OutputFilenameParameter,       
               otb::Wrapper::QtWidgetOutputFilenameParameter)
  CREATEWIDGET(otb::Wrapper::DirectoryParameter,      
               otb::Wrapper::QtWidgetDirectoryParameter)
  CREATEWIDGET(otb::Wrapper::StringParameter,         
               otb::Wrapper::QtWidgetStringParameter)
  CREATEWIDGET(otb::Wrapper::StringListParameter,     
               otb::Wrapper::QtWidgetStringListParameter)
  CREATEWIDGET(otb::Wrapper::ChoiceParameter,         
               mvd::Wrapper::QtWidgetChoiceParameter)
  CREATEWIDGET(otb::Wrapper::ListViewParameter,       
               otb::Wrapper::QtWidgetListViewParameter)
  CREATEWIDGET(otb::Wrapper::InputImageParameter,     
               mvd::Wrapper::QtWidgetInputImageParameter)
  CREATEWIDGET(otb::Wrapper::ComplexInputImageParameter,  
               otb::Wrapper::QtWidgetComplexInputImageParameter)
  CREATEWIDGET(otb::Wrapper::ComplexOutputImageParameter,     
               otb::Wrapper::QtWidgetComplexOutputImageParameter)
  CREATEWIDGET(otb::Wrapper::InputImageListParameter, 
               mvd::Wrapper::QtWidgetInputImageListParameter)
  CREATEWIDGET(otb::Wrapper::InputVectorDataListParameter, 
               otb::Wrapper::QtWidgetInputVectorDataListParameter)
  CREATEWIDGET(otb::Wrapper::InputVectorDataParameter, 
               otb::Wrapper::QtWidgetInputVectorDataParameter)
  CREATEWIDGET(otb::Wrapper::OutputImageParameter,    
               otb::Wrapper::QtWidgetOutputImageParameter)
  CREATEWIDGET(otb::Wrapper::OutputVectorDataParameter, 
               otb::Wrapper::QtWidgetOutputVectorDataParameter)
  CREATEWIDGET(otb::Wrapper::EmptyParameter,          
               otb::Wrapper::QtWidgetEmptyParameter)
  CREATEWIDGET(otb::Wrapper::ParameterGroup,          
               mvd::Wrapper::QtWidgetParameterGroup)
  CREATEWIDGET(otb::Wrapper::RAMParameter,            
               otb::Wrapper::QtWidgetRAMParameter)
#if OTB_VERSION_CHECK( 3, 18, 2 )
  CREATEWIDGET(otb::Wrapper::OutputProcessXMLParameter,        
               otb::Wrapper::QtWidgetOutputProcessXMLParameter)
  CREATEWIDGET(otb::Wrapper::InputProcessXMLParameter,
               otb::Wrapper::QtWidgetInputProcessXMLParameter)
#endif

#undef CREATEWIDGET

  if (widget)
    {
    widget->CreateWidget();
    widget->UpdateGUI();
    }

  return widget;
}


}
}

/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperQtWidgetStringListParameter.h"


#include "otbWrapperStringListParameter.h"


namespace otb
{


namespace Wrapper
{


/*****************************************************************************/
QtWidgetStringListParameter
::QtWidgetStringListParameter( StringListParameter * param,
			       QtWidgetModel * m , QWidget * parent) :
  QtWidgetParameterList( param, m , parent)
{
}


/*****************************************************************************/
QtWidgetStringListParameter
::~QtWidgetStringListParameter()
{
}

#if 0

void
QtWidgetStringListParameter::UpdateStringList()
{
  // save value
  std::vector<std::string> updatedList;

  for(unsigned int j=0; j<m_LineEditList.size(); j++ )
    {
    updatedList.push_back(m_LineEditList[j]->ToStdString());
    }
  m_StringListParam->SetValue(updatedList);

  // notify model text changed
  emit Change();

  QString key( m_StringListParam->GetKey() );
  emit ParameterChanged(key);
}


void QtWidgetStringListParameter::SetString(const QString& value)
{
  m_StringListParam->AddString(value.toLatin1().constData());
  m_StringListParam->SetUserValue(true);
  QString key( m_StringListParam->GetKey() );
  emit ParameterChanged(key);
}


void
QtWidgetStringListParameter::AddString()
{
  m_StringLayout = new QVBoxLayout();
  m_StringLayout->setSpacing(0);

  for(unsigned int i=0; i<m_LineEditList.size(); i++ )
    {
    m_StringLayout->addWidget( m_LineEditList[i] );
    }

  QtStringSelectionWidget * stringInput = new  QtStringSelectionWidget();
  stringInput->setFixedHeight( 30 );
  m_StringLayout->addWidget( stringInput );
  m_LineEditList.push_back(stringInput);
  m_StringListParam->AddNullElement();
  connect( stringInput,
           SIGNAL(InternalQLineEditEditionFinished()),
           this,
           SLOT(UpdateStringList()));

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
}


void
QtWidgetStringListParameter::SuppressString()
{
  m_StringLayout = new QVBoxLayout();
  m_StringLayout->setSpacing(0);
  std::vector<QtStringSelectionWidget *> tmpList;
  for(unsigned int i=0; i<m_LineEditList.size(); i++ )
    {
    if( !m_LineEditList[i]->IsChecked() )
      {
      m_StringLayout->addWidget( m_LineEditList[i] );
      tmpList.push_back(m_LineEditList[i]);
      }
    }

  m_LineEditList =  tmpList;

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
}

#endif

}

}

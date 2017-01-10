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
#include "otbWrapperQtWidgetStringListParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetStringListParameter::QtWidgetStringListParameter(StringListParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(param, m),
  m_StringListParam(param)
{
 connect( this,
          SIGNAL(Change()),
          GetModel(),
          SLOT(NotifyUpdate()) );
}

QtWidgetStringListParameter::~QtWidgetStringListParameter()
{
}

void QtWidgetStringListParameter::DoUpdateGUI()
{
  if(!m_StringListParam)
    return;

  std::vector<std::string> strList = m_StringListParam->GetValue();
  for( unsigned int i = m_LineEditList.size(); i < strList.size(); i++ )
    {
      this->AddString();
    }
  int i = 0;
  std::vector<std::string>::iterator it;
  for (it = strList.begin(); it != strList.end(); ++it)
    {
      m_LineEditList[i++]->SetText(QString( (*it).c_str() ));
    }
}

void QtWidgetStringListParameter::DoCreateWidget()
{
  m_LineEditList.clear();
  const unsigned int sp(2);
  const unsigned int buttonSize(30);

  // Global layout
  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->setSpacing(sp);
  hLayout->setContentsMargins(sp, sp, sp, sp);

  if( m_StringListParam->GetRole() != Role_Output )
    {
    // Button layout
    QVBoxLayout * buttonLayout = new QVBoxLayout;
    buttonLayout->setSpacing(sp);
    buttonLayout->setContentsMargins(sp, sp, sp, sp);

    QHBoxLayout * addSupLayout = new QHBoxLayout;
    addSupLayout->setSpacing(sp);
    addSupLayout->setContentsMargins(sp, sp, sp, sp);

    QHBoxLayout * upDownLayout = new QHBoxLayout;
    upDownLayout->setSpacing(sp);
    upDownLayout->setContentsMargins(sp, sp, sp, sp);

    // Add file button
    QPushButton * addButton = new QPushButton;
    addButton->setText("+");
    addButton->setFixedWidth(buttonSize);
    addButton->setToolTip("Add a string selector...");
    connect( addButton, SIGNAL(clicked()), this, SLOT(AddString()) );
    addSupLayout->addWidget(addButton);

    // Suppress file button
    QPushButton * supButton = new QPushButton;
    supButton->setText("-");
    supButton->setFixedWidth(buttonSize);
    supButton->setToolTip("Suppress the selected string...");
    connect( supButton, SIGNAL(clicked()), this, SLOT(SuppressString()) );
    addSupLayout->addWidget(supButton);
    buttonLayout->addLayout(addSupLayout);

    hLayout->addLayout(buttonLayout);
    }

  QVBoxLayout * fileLayout = new QVBoxLayout();
  fileLayout->setSpacing(0);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(fileLayout);
  QScrollArea * s = new QScrollArea();
  s->setWidget(mainGroup);
  s->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  s->setWidgetResizable(true);

  hLayout->addWidget(s);


  this->setLayout(hLayout);

  m_HLayout = hLayout;
  m_Scroll = s;

}

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
}


void QtWidgetStringListParameter::SetString(const QString& value)
{
  m_StringListParam->AddString(value.toAscii().constData());
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

}
}

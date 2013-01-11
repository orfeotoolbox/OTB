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
 connect( this, SIGNAL(Change()), GetModel(), SLOT(NotifyUpdate()) );
}

QtWidgetStringListParameter::~QtWidgetStringListParameter()
{
}

void QtWidgetStringListParameter::DoUpdateGUI()
{
  m_LineEditList.clear();

  m_StringLayout = new QVBoxLayout();

  for(unsigned int i=0; i<m_StringListParam->GetValue().size(); i++)
    {
    QtStringSelectionWidget * stringSelection = new QtStringSelectionWidget();
    stringSelection->setFixedHeight( 30 );
    QString val(m_StringListParam->GetNthElement(i).c_str());
    stringSelection->GetInput()->setText( m_StringListParam->GetNthElement(i).c_str() ); //val );
    m_StringLayout->addWidget( stringSelection );
    m_LineEditList.push_back(stringSelection);

    connect( stringSelection->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateStringList()) );
    connect( stringSelection->GetInput(), SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );
    }

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
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

    // Supress file button
    QPushButton * supButton = new QPushButton;
    supButton->setText("-");
    supButton->setFixedWidth(buttonSize);
    supButton->setToolTip("Supress the selected string...");
    connect( supButton, SIGNAL(clicked()), this, SLOT(SupressString()) );
    addSupLayout->addWidget(supButton);
    buttonLayout->addLayout(addSupLayout);

    // Up file edit
    QPushButton * upButton = new QPushButton;
    upButton->setText("Up");
    upButton->setFixedWidth(buttonSize);
    upButton->setToolTip("Up the selected string in the list...");
    connect( upButton, SIGNAL(clicked()), this, SLOT(UpString()) );
    upDownLayout->addWidget(upButton);

    // Down file edit
    QPushButton * downButton = new QPushButton;
    downButton->setText("Down");
    downButton->setFixedWidth(buttonSize);
    downButton->setToolTip("Down the selected string in the list...");
    connect( downButton, SIGNAL(clicked()), this, SLOT(DownString()) );
    upDownLayout->addWidget(downButton);
    buttonLayout->addLayout(upDownLayout);

    // Erase file edit
    QPushButton * eraseButton = new QPushButton;
    eraseButton->setText("Erase");
    eraseButton->setFixedWidth(2*(buttonSize+sp));
    eraseButton->setToolTip("Erase the selected string of the list...");
    connect( eraseButton, SIGNAL(clicked()), this, SLOT(EraseString()) );
    buttonLayout->addWidget(eraseButton);

    hLayout->addLayout(buttonLayout);
    }

  QVBoxLayout * fileLayout = new QVBoxLayout();
  fileLayout->setSpacing(0);

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(fileLayout);
  QScrollArea * scroll = new QScrollArea();
  scroll->setWidget(mainGroup);
  scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scroll->setWidgetResizable(true);

  hLayout->addWidget(scroll);


  this->setLayout(hLayout);

  //m_StringLayout = fileLayout;
  m_HLayout = hLayout;
  m_Scroll = scroll;

}

void
QtWidgetStringListParameter::UpdateStringList()
{
  // save value
  for(unsigned int j=0; j<m_StringListParam->GetValue().size(); j++ )
    {
      m_StringListParam->SetNthElement(j, m_LineEditList[j]->GetStringName());
    }
  emit Change();
}


void
QtWidgetStringListParameter::UpString()
{
 if(m_LineEditList.size() < 2 )
    return;

  m_StringLayout = new QVBoxLayout();
  m_StringLayout->setSpacing(2);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;
  
  // Init map
  for(unsigned int i=0; i<m_LineEditList.size(); i++ )
    {
    idMap[i] = i;
    }
  
  // If the first item is checked, uncheck it...
  // It won't be moved
  if( m_LineEditList[0]->IsChecked() )
    {
    m_LineEditList[0]->SetChecked(false);
    }

 
  // If other item are checked, up the index
  // Starts at 1 because the first item mustn't move
  for(unsigned int i=1; i<m_LineEditList.size(); i++ )
    {
    if( m_LineEditList[i]->IsChecked() )
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i-1;
      idMap[idMap[i-1]] = tmp;
      }
    }

  this->UpdateStringList( idMap );
 
  this->RecreateStringList();
}

void
QtWidgetStringListParameter::DownString()
{
  if(m_LineEditList.size() < 2 )
    return;

  m_StringLayout = new QVBoxLayout();
  m_StringLayout->setSpacing(0);

  // Map link between old and new index in the list
  std::map<unsigned int, unsigned int> idMap;
  
  // Init map
  for(unsigned int i=0; i<m_LineEditList.size(); i++ )
    {
    idMap[i] = i;
    }
  
  // If the last item is checked, uncheck it...
  // It won't be moved
  if( m_LineEditList[m_LineEditList.size()-1]->IsChecked() )
    {
    m_LineEditList[m_LineEditList.size()-1]->SetChecked(false);
    }

 
  // If other item are checked, up the index
  // Stops at size-1 because the last item mustn't move
  for(int i=m_LineEditList.size()-2; i>=0; i-- )
    {
    if( m_LineEditList[i]->IsChecked() )
      {
      unsigned int tmp = idMap[i];
      idMap[i] = i+1;
       idMap[idMap[i+1]] = tmp;
      }
    }
    
  this->UpdateStringList( idMap );

  this->RecreateStringList();
}


void
QtWidgetStringListParameter::UpdateStringList( std::map<unsigned int, unsigned int> idMap )
{
  std::vector<QtStringSelectionWidget *> tmpList;
  // Keys become values and inverse
  std::map<unsigned int, unsigned int> idMapBis;
  for(unsigned int i=0; i<idMap.size(); i++ )
    {
    idMapBis[ idMap[i] ] = i;
    }
  
  // Create the new item list
  for(unsigned int i=0; i<m_LineEditList.size(); i++ )
    {
    m_StringLayout->addWidget( m_LineEditList[ idMapBis[i] ] );
    tmpList.push_back(m_LineEditList[ idMapBis[i] ]);
    }
  
  
  m_LineEditList =  tmpList;
  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);
  
  this->update();

    // notify of value change
  QString key( m_StringListParam->GetKey() );
  emit ParameterChanged(key);
}


void QtWidgetStringListParameter::SetString(const QString& value)
{
  m_StringListParam->AddString(value.toStdString());
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
  connect( stringInput->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateStringList()));
  connect( stringInput->GetInput(), SIGNAL(textChanged(const QString&)), GetModel(), SLOT(NotifyUpdate()) );

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);

 
  this->update();
}


void
QtWidgetStringListParameter::SupressString()
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
  this->RecreateStringList();
}


void
QtWidgetStringListParameter::EraseString()
{
  m_LineEditList.clear();

  m_StringLayout = new QVBoxLayout();

  QtStringSelectionWidget * stringSelection = new QtStringSelectionWidget();
  stringSelection->setFixedHeight( 30 );
  m_StringLayout->addWidget( stringSelection );
  m_LineEditList.push_back(stringSelection);
  m_StringListParam->AddNullElement();
  connect( stringSelection->GetInput(), SIGNAL(textChanged(const QString&)), this, SLOT(UpdateStringList()) );

  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_StringLayout);
  m_Scroll->setWidget(mainGroup);

  this->update();
  this->RecreateStringList();
}


void QtWidgetStringListParameter::RecreateStringList()
{
  // save value
  m_StringListParam->ClearValue();

  if( m_LineEditList.size() != 0)
    {
    for(unsigned int j=0; j<m_LineEditList.size(); j++ )
      {
      m_StringListParam->AddString(m_LineEditList[j]->GetStringName());
      }

    emit Change();
    // notify of value change
    QString key( m_StringListParam->GetKey() );
    emit ParameterChanged(key);
    }
}


}
}

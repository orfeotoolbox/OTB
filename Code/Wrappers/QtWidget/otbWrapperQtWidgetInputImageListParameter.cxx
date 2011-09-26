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
#include "otbWrapperQtWidgetInputImageListParameter.h"
#include "otbQtFileSelectionWidget.h"

namespace otb
{
namespace Wrapper
{

QtWidgetInputImageListParameter::QtWidgetInputImageListParameter(InputImageListParameter* param, QtWidgetModel* m)
: QtWidgetParameterBase(m),
  m_InputImageListParam(param)/*,
  m_FileSelectionList()*/
{
}

QtWidgetInputImageListParameter::~QtWidgetInputImageListParameter()
{
}

void QtWidgetInputImageListParameter::DoUpdateGUI()
{

}

void QtWidgetInputImageListParameter::DoCreateWidget()
{
  //m_FileSelectionList.clear();
  const unsigned int sp = 2;
  // Global layout
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(sp);
  m_HLayout->setContentsMargins(sp, sp, sp, sp);

  // Button layout
  m_ButtonLayout = new QVBoxLayout;
  m_ButtonLayout->setSpacing(sp);
  m_ButtonLayout->setContentsMargins(sp, sp, sp, sp);

  m_AddSupLayout = new QHBoxLayout;
  m_AddSupLayout->setSpacing(sp);
  m_AddSupLayout->setContentsMargins(sp, sp, sp, sp);

  m_UpDownLayout = new QHBoxLayout;
  m_UpDownLayout->setSpacing(sp);
  m_UpDownLayout->setContentsMargins(sp, sp, sp, sp);

  // Add file button
  m_AddButton = new QPushButton;
  m_AddButton->setText("+");
  m_AddButton->setToolTip("Add a file selector...");
  m_AddButton->setMaximumWidth(m_AddButton->width());
  connect( m_AddButton, SIGNAL(clicked()), this, SLOT(AddFile()) );
  m_AddSupLayout->addWidget(m_AddButton);

  // Supress file button
  m_SupButton = new QPushButton;
  m_SupButton->setText("-");
  m_SupButton->setToolTip("Supress the selected file...");
  m_SupButton->setMaximumWidth(m_SupButton->width());
  connect( m_SupButton, SIGNAL(clicked()), this, SLOT(SupressFile()) );
  m_AddSupLayout->addWidget(m_SupButton);
  m_ButtonLayout->addLayout(m_AddSupLayout);

  // Up file edit
  m_UpButton = new QPushButton;
  m_UpButton->setText("Up");
  m_UpButton->setToolTip("Up the selected file in the list...");
  m_UpButton->setMaximumWidth(m_UpButton->width());
  connect( m_UpButton, SIGNAL(clicked()), this, SLOT(UpFile()) );
  m_UpDownLayout->addWidget(m_UpButton);

  // Down file edit
  m_DownButton = new QPushButton;
  m_DownButton->setText("Down");
  m_DownButton->setToolTip("Down the selected file in the list...");
  m_DownButton->setMaximumWidth(m_DownButton->width());
  connect( m_DownButton, SIGNAL(clicked()), this, SLOT(DownFile()) );
  m_UpDownLayout->addWidget(m_DownButton);
  m_ButtonLayout->addLayout(m_UpDownLayout);

  // Erase file edit
  m_EraseButton = new QPushButton;
  m_EraseButton->setText("Erase");
  m_EraseButton->setToolTip("Erase the selected file of the list...");
  m_EraseButton->setMaximumWidth(m_EraseButton->width());
  connect( m_EraseButton, SIGNAL(clicked()), this, SLOT(EraseFile()) );
  m_ButtonLayout->addWidget(m_EraseButton);

  m_FileLayout = new QVBoxLayout();
/*
  QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection );
QtFileSelectionWidget * fileSelection1 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection1 );
QtFileSelectionWidget * fileSelection2 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection2 );
QtFileSelectionWidget * fileSelection3 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection3 );
QtFileSelectionWidget * fileSelection4 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection4 );
QtFileSelectionWidget * fileSelection5 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection5 );
QtFileSelectionWidget * fileSelection6 = new QtFileSelectionWidget();
  m_FileLayout->addWidget( fileSelection6 );
*/
 QLineEdit * fileSelection = new QLineEdit();
  m_FileLayout->addWidget( fileSelection );
  QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll = new QScrollArea();
  //m_Scroll->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);
  m_Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);
  m_Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);

  QVBoxLayout  *scrollLayout = new QVBoxLayout();
  scrollLayout->addWidget(m_Scroll);


  //m_HLayout->addWidget(m_Scroll);
  m_HLayout->addLayout(scrollLayout);
  m_HLayout->addLayout(m_ButtonLayout);

  m_HLayout->addStretch();
  
  this->setLayout(m_HLayout);
}

void QtWidgetInputImageListParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    //this->SetFileName(fileDialog.selectedFiles().at(0));
    //m_Input->setText(fileDialog.selectedFiles().at(0));
    }
}


void
QtWidgetInputImageListParameter::UpFile()
{
  std::cout<<"upupup"<<std::endl;
}
void
QtWidgetInputImageListParameter::DownFile()
{
 std::cout<<"downdowndown"<<std::endl;
}
void
QtWidgetInputImageListParameter::AddFile()
{
  //QtFileSelectionWidget * fileSelection = new
  //QtFileSelectionWidget();
  QLineEdit * fileSelection = new QLineEdit();
  //std::cout<< fileSelection->height()<<std::endl;
  //fileSelection->setMinimumHeight( 20 );//fileSelection->height() / 2 );
  //std::cout<<fileSelection->minimumHeight()<<std::endl;
  
  m_FileLayout->addWidget( fileSelection );
  m_FileLayout->update();
  m_Scroll->update();
  m_HLayout->update();
  this->update();
}

void
QtWidgetInputImageListParameter::SupressFile()
{
 std::cout<<"supsupsup"<<std::endl;
}
void
QtWidgetInputImageListParameter::EraseFile()
{
  //m_HLayout->removeItem( m_FileLayout );
  m_HLayout->removeWidget( m_Scroll );
  m_HLayout->update();

  m_FileLayout = new QVBoxLayout();
  //QtFileSelectionWidget * fileSelection = new QtFileSelectionWidget();
  QLineEdit * fileSelection = new QLineEdit();
  m_FileLayout->addWidget( fileSelection );

 QGroupBox *mainGroup = new QGroupBox();
  mainGroup->setLayout(m_FileLayout);
  m_Scroll = new QScrollArea();
  //m_Scroll->setLayout(m_FileLayout);
  m_Scroll->setWidget(mainGroup);
  m_Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn);
  m_Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//ScrollBarAlwaysOn)
/*
  m_Scroll = new QScrollArea();
    m_Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//AsNeeded);
  m_Scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  m_Scroll->setLayout(m_FileLayout);
*/

  m_HLayout->insertWidget(0, m_Scroll);
  m_HLayout->update();
  //m_HLayout->takeAt(2) = m_Scroll;
 std::cout<<"cleancleanclean"<<std::endl;
}
/*
void QtWidgetInputImageListParameter::SetFileName(const QString& value)
{
  // save value
  m_InputImageParam->SetFromFileName(value.toStdString());

  // notify of value change
  QString key( QString::fromStdString(m_InputImageParam->GetKey()) );
  emit ParameterChanged(key);
}
*/


}
}

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
#ifndef __otbQtLogOutput_h
#define __otbQtLogOutput_h

#include <QtGui>
#include "itkStdStreamLogOutput.h"
//#include "itkObjectFactory.h"
//#include "itkProcessObject.h"

namespace otb
{

/** \class QtLogOutput
 *  \brief Class that get log output and launch an event when received.
 *
 */

class ITK_ABI_EXPORT QtLogOutput : public QObject, public itk::LogOutput
{
Q_OBJECT
public:
  typedef QtLogOutput                    Self;
  typedef itk::LogOutput                 Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkTypeMacro(QtLogOutput, itk::LogOutput);

  itkNewMacro(QtLogOutput);

  /** flush a buffer */
  virtual void Flush();

  /** Write to multiple outputs */
  virtual void Write(double timestamp);

  /** Write to a buffer */
  virtual void Write(std::string const &content);

  /** Write to a buffer */
  virtual void Write(std::string const &content, double timestamp);

signals:
  void NewContentLog(QString);

protected:
  /** Constructor */
  QtLogOutput();

  /** Destructor */
  virtual ~QtLogOutput();

  void PrintSelf(std::ostream &os, itk::Indent indent) const;
};

}

#endif

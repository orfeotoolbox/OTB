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
#ifndef otb_GlActor_h
#define otb_GlActor_h

#include "itkObject.h"
#include "otbViewSettings.h"

namespace otb
{

class GlActor 
  : public itk::Object
{
public:
  typedef GlActor                                          Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  itkSetObjectMacro(Settings,ViewSettings);
  itkGetObjectMacro(Settings,ViewSettings);
  itkGetConstObjectMacro(Settings,ViewSettings);
  
  itkSetMacro(Visible,bool);
  itkGetMacro(Visible,bool);
  itkBooleanMacro(Visible);

  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  // Retrieve the full extent of the actor
  virtual void GetExtent(double & ulx, double & uly, double & lrx, double & lry) const = 0;

  // Update internal actor state with respect to ViewSettings
  virtual void ProcessViewSettings() = 0;

  // Heavy load/unload operations of data
  virtual void UpdateData() = 0;

  // Gl rendering of current state
  virtual void Render() = 0;

protected:
  GlActor();

  virtual ~GlActor();

private:
  // prevent implementation
  GlActor(const Self&);
  void operator=(const Self&);

  ViewSettings::Pointer m_Settings;

  // Visibility flag
  bool m_Visible;

  // Name field
  std::string m_Name;

}; // End class GlActor

} // End namespace otb

#endif

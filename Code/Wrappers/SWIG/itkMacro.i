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

%define itkSetStringMacro(name)
  void Set##name (const char* _arg);
%enddef

%define itkGetStringMacro(name)
  const char* Get##name () const;
%enddef

%define itkSetMacro(name, type)
  void Set##name (type _arg);
%enddef

%define itkGetMacro(name, type)
  type Get##name ();
%enddef

%define itkBooleanMacro(name)
  void name##On ();
  void name##Off ();
%enddef

%define itkSetEnumMacro(name, type)
  void Set##name (const type _arg);
%enddef

%define itkGetEnumMacro(name, type)
  type Get##name () const;
%enddef


%define itkNewMacro(name)
  static name##_Pointer New(void);
%enddef


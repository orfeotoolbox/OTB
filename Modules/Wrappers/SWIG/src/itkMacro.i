/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


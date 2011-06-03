
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

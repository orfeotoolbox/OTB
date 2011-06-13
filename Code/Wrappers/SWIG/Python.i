#if SWIGPYTHON


%typemap(out)  unsigned char &, const unsigned char &, signed char &, const signed char &, unsigned short &, const unsigned short &, signed short &, const signed short &, unsigned int &, const unsigned int &, signed int &, const signed int &, signed long &, const signed long &, unsigned long &, const unsigned long &
  {$result = PyInt_FromLong( *$1 );}

%typemap(out) float &, const float &, double &, const double &
  {$result = PyFloat_FromDouble( *$1 );}

// transform std::list< std::string > to Python list of Python string
%typemap(out) std::list< std::string >
  {
  $result = PyList_New( $1.size() );
  Py_ssize_t i = 0;
  for (std::list< std::string >::const_iterator it = $1.begin();
       it != $1.end();
       ++it, ++i )
    {
    PyList_SetItem( $result, i, PyString_FromString( it->c_str() ) );
    }
  }
  
%extend itkLightObject {
      %pythoncode %{  
        def __str__( self ):
          return self.ToString()
      %}
}

#endif

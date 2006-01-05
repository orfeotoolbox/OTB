
#ifndef __otbExceptionObject_h
#define __otbExceptionObject_h

#include "itkExceptionObject.h"

namespace otb
{

/** \class ExceptionObject
 * \brief Classe standard de traitement des exceptions de l'OTB.
 *
 */
class ExceptionObject : public itk::ExceptionObject
{
public:
  /** Default constructor.  Needed to ensure the exception object can be
   * copied. */
  ExceptionObject() : itk::ExceptionObject() {}

  /** Constructor. Needed to ensure the exception object can be copied. */
  ExceptionObject(const char *file="Inconnu", unsigned int lineNumber=0,
                  const char *desc="Neant", const char *loc="Inconnue" ) :
		  itk::ExceptionObject(file, lineNumber, desc, loc) {}

  /** Constructor. Needed to ensure the exception object can be copied. */
  ExceptionObject(const std::string& file, unsigned int lineNumber,
                  const std::string& desc="Neant",
                  const std::string& loc="Inconnue") :
		  itk::ExceptionObject(file, lineNumber, desc, loc) {}  
  /** Copy Constructor. */
  ExceptionObject( const ExceptionObject &orig ): itk::ExceptionObject(orig) {}
   
  /** Virtual destructor needed for subclasses. Has to have empty throw(). */
  virtual ~ExceptionObject() throw() {}

  virtual void Print(std::ostream& os) const;
  
  virtual const char* GetNameOfClass() const
    { return "ExceptionObject"; }
};

} // end namespace otb

#endif

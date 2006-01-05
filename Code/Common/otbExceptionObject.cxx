#include "otbExceptionObject.h"
#include "itkIndent.h"
#include <iostream>

namespace otb
{

void
ExceptionObject
::Print(std::ostream& os) const
{
  itk::Indent indent;

  // Print header
  os << std::endl;
  os << indent << "otb::" << this->GetNameOfClass() << " (" << this << ")\n";

  // Print self
  indent.GetNextIndent();
  std::string location = this->GetLocation();
  if (! location.empty()) 
    {
    os << indent << "Localisation : \"" << this->GetLocation() << "\" " << std::endl;
    }
  std::string file = this->GetFile();
  if (! file.empty()) 
    {
    os << indent << "Fichier      : " << this->GetFile() << std::endl;
    os << indent << "Ligne        : " << this->GetLine() << std::endl;
    }
  std::string description = this->GetDescription();
  if (! description.empty()) 
    {
    os << indent << "Description  : " << this->GetDescription() << std::endl;  
    }

  // Print trailer
  os << indent << std::endl;
}  

} // end namespace otb

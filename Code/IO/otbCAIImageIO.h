/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Classe IO image pour le format CAI
  $Id$

=========================================================================*/
#ifndef __otbCAIImageIO_h
#define __otbCAIImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <string>
#include <fstream>
#include "itkImageIOBase.h"
#include <stdio.h>

namespace otb
{

/** \class CAIImageIO
 *
 * \brief ImageIO object for reading and writing CAI images
 *
 * The streaming read/write are implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT CAIImageIO : public itk::ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef CAIImageIO            Self;
  typedef itk::ImageIOBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CAIImageIO, Superclass);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) ;

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead(){  return true; };

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();
  
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite() { return true; };

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();
  
  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);


  CAIImageIO();
  ~CAIImageIO();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
private:
  CAIImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Methode permettant de recuperer le format et le nom de l'image a partir du
  nom d'un fichier image. */
  bool GetInfosCAI( const char * filename, std::string & CaiFileName, std::string & CaiFormat  );
  /** Determine si l'image est au format SPOT */
  bool DetermineFormatSpot (char typspot, const char * str_repertoire ,
				char * str_debut );

  /** Pointer sur la structure CAI */
  char *   m_ptrCai;
  /** Nombre d'octets par pixel */
  int           m_NbOctetPixel;
};

} // end namespace otb

#endif // __otbCAIImageIO_h

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
#ifndef __otbMSTARImageIO_h
#define __otbMSTARImageIO_h

#include "otbImageIOBase.h"

namespace otb
{

/** \class MSTARImageIO
 *
 * \brief ImageIO object for reading and writing MSTAR images
 *
 * The streaming read/write are implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT MSTARImageIO : public otb::ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef MSTARImageIO            Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MSTARImageIO, Superclass);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead()
  {
    return true;
  }

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite()
  {
    return true;
  }

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);

  MSTARImageIO();
  virtual ~MSTARImageIO();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MSTARImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Methode permettant de recuperer le format et le nom de l'image a partir du
  nom d'un fichier image. */
  float          byteswap_SR_IR(unsigned char *pointer);
  unsigned short byteswap_SUS_IUS(unsigned char *pointer);
  int            CheckByteOrder(void);

  FILE *MSTARfp;        /* Input FILE ptr to MSTAR image file     */
  FILE *RAWfp;          /* Output FILE ptr to MSTAR RAW data file */
  FILE *HDRfp;          /* Output FILE ptr to MSTAR header file   */

  int i, j, rv, n, numrows, numcols, numgot;

  const char *MSTARname;      /* Input MSTAR filename           */
  char        RAWname[80];   /* Output MSTAR RAW filename      */
  char        HDRname[80];   /* Phoenix header filename buffer */

  int  outOpt;               /* ALL data, or MAG ONLY...    */
  int  phlen, nhlen, mstartype;
  long magloc, bytesPerImage, nchunks, totchunks;

  char *tptr;           /* Temp buffer ptr */
  char *phdr;           /* Ptr to buffer to hold Phoenix header */
  char  tbuff[1024];

  unsigned short *FSCENEdata; /* Ptr to Fullscene data buffer */
  float *         CHIPdata;   /* Ptr to CHIp data buffer      */

  /* Byte Order Variables */
  int            byteorder;
  unsigned char  bigfloatbuf[4];   /* BigEndian float buffer... */
  float          littlefloatval;   /* LittleEndian float value  */
  unsigned char  bigushortbuf[2];  /* BigEndian ushort buffer...*/
  unsigned short littleushortval;  /* LittleEndian ushort value.*/

  int m_BytePerPixel;
};

} // end namespace otb

#endif // __otbMSTARImageIO_h

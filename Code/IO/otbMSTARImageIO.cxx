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
#include "otbMSTARImageIO.h"

//For MSTAR
#define MAIN

/* Define MSTAR image type */
#define CHIP_IMAGE   0
#define FSCENE_IMAGE 1

#define ALL_DATA   0
#define MAG_DATA   1

#define SUCCESS    0
#define FAILURE   -1

#define LSB_FIRST    0             /* Implies little-endian CPU... */
#define MSB_FIRST    1             /* Implies big-endian CPU...    */

#include <iostream>
#include <list>
#include <string>
#include <cstring>

#include "otbMacro.h"

namespace otb
{

/** Constructor */
MSTARImageIO::MSTARImageIO()
{
  m_ByteOrder = BigEndian;
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;

  // Set default origin to [0.5 , 0.5] 
  // (consistency between ImageIO, see Mantis #942)
  m_Origin[0] = 0.5;
  m_Origin[1] = 0.5;

  m_BytePerPixel = 0;

  MSTARfp = NULL;        /* Input FILE ptr to MSTAR image file     */
  HDRfp = NULL;          /* Output FILE ptr to MSTAR header file   */

  MSTARname = NULL;      /* Input MSTAR filename           */

  tptr = NULL;  /* Temp buffer ptr */
  phdr = NULL;  /* Ptr to buffer to hold Phoenix header */

  FSCENEdata = NULL; /* Ptr to Fullscene data buffer */
  CHIPdata = NULL;   /* Ptr to CHIp data buffer      */

}

/** Destructor */
MSTARImageIO::~MSTARImageIO()
{
  if (phdr != NULL)
    {
    free(phdr);
    phdr = NULL;
    }
}

bool MSTARImageIO::CanReadFile(const char* filename)
{
  // First check the filename extension
  std::string fname = filename;
  if (fname == "")
    {
    itkDebugMacro(<< "Fichier image non specifie.");
    }

  MSTARname = filename;

  MSTARfp = fopen(MSTARname, "rb");
  if (MSTARfp == NULL)
    {
    otbMsgDevMacro(<< "\nError: Unable to open [" << MSTARname << "] for reading!\n");
    return false;
    }

  /****************************************************
      * Read first 512 bytes to figure out some header   *
      * parameters....                                   *
   ****************************************************/

  int returnVal = fread(tbuff, sizeof(char), 1024, MSTARfp);
  if (returnVal != 1024)
    {
    otbMsgDevMacro(<< "\nError: Unable in reading [" << MSTARname << "] header... Only read ["<< returnVal <<" of 1024 bytes !\n");
    fclose(MSTARfp);
    return false;
    }
  rewind(MSTARfp);

  /* Extract Phoenix Summary header length */
  tptr = (char *) strstr(tbuff, "PhoenixHeaderLength= ");
  if (tptr == (char *) NULL)
    {
//    fprintf(stderr,"Can not determine Phoenix header length!\n");
    fclose(MSTARfp);
    return false;
    }
  else
    {
    sscanf((tptr + 20), "%d", &phlen);
    }

  /* Check for and extract native header length */
  tptr = (char *) strstr(tbuff, "native_header_length= ");
  if (tptr == (char *) NULL)
    {
//    fprintf(stderr,"Can not determine native header length!\n");
    fclose(MSTARfp);
    return false;
    }
  else
    {
    sscanf((tptr + 21), "%d", &nhlen);
    }

  /* Extract MSTAR column width */
  tptr = (char *) strstr(tbuff, "NumberOfColumns= ");
  if (tptr == (char *) NULL)
    {
    otbMsgDevMacro(<< "Error: Can not determine MSTAR image width");
    fclose(MSTARfp);
    return false;
    }
  else
    {
    sscanf((tptr + 16), "%d", &numcols);
    //NbColonnes = static_cast<int>(numcols); // Unused
    }

  /* Extract MSTAR row height */
  tptr = (char *) strstr(tbuff, "NumberOfRows= ");
  if (tptr == (char *) NULL)
    {
    otbMsgDevMacro(<< "Error: Can not determine MSTAR image height!");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 13), "%d", &numrows);
    //NbLignes = static_cast<int>(numrows); // Unused
    }

  /* Set MSTAR image type */
  if (nhlen == 0)
    { /* Implies FLOAT MSTAR chip image */
    mstartype = CHIP_IMAGE;
    }
  else
    {
    mstartype = FSCENE_IMAGE; /* UnShort Fullscene */
    }

  /*******************************************************
      * Allocate memory to header buffer, read Phoenix hdr, *
      * and write out to output file...                     *
   *******************************************************/

  // /* Allocate memory to Phoenix header buffer */
// phdr = (char *) malloc(phlen+1);
// if (phdr == (char *) NULL)
//    {
//     fprintf(stderr,
//               "Error: unable to allocate Phoenix header memory!\n");
//     fclose(MSTARfp);
//     return 0;
//    }

  // /* Read Phoenix header into buffer */
// n = fread(phdr, sizeof(char), phlen, MSTARfp);
// if (n != phlen)
//    {
//     fprintf(stderr,
//             "Error: in reading Phoenix header..only read [%d of %d] bytes\n",
//               n, phlen);
//     free(phdr);
//     fclose(MSTARfp);
//     return 0;
//    }

  // /* Write Phoenix header to output header file */

// free(phdr);

  fclose(MSTARfp);
  otbMsgDevMacro(<< "Can read MSTAR");
  return bool(true);
}

bool MSTARImageIO::CanWriteFile(const char * filename)
{
  bool        formatFound = false;
  std::string fname = filename;

  if (fname == "")
    {
    itkDebugMacro(<< "Fichier image non specifie.");
    }

  if (formatFound == false)
    {
    std::string identificationErreur;
    identificationErreur = "Format MSTAR non detecte dans l'extension du fichier.";
    itkDebugMacro(<< identificationErreur.c_str());
    }

  return formatFound;

}

void MSTARImageIO::Read(void* buffer)
{

  MSTARfp = fopen(MSTARname, "rb");
  if (MSTARfp == NULL)
    {
    itkExceptionMacro(<< "Error: Unable to open file for reading!\n\n " << m_FileName.c_str() << ").");
    }

  /****************************************************
      * Read first 512 bytes to figure out some header   *
      * parameters....                                   *
   ****************************************************/

  int returnVal = fread(tbuff, sizeof(char), 1024, MSTARfp);
  if (returnVal != 1024)
    {
    fclose(MSTARfp);
    itkExceptionMacro(<< "\nError: Unable in reading [" << MSTARname << "] header... Only read ["<< returnVal <<" of 1024 bytes !\n");
    }
  rewind(MSTARfp);

  /* Extract Phoenix Summary header length */
  tptr = (char *) strstr(tbuff, "PhoenixHeaderLength= ");
  if (tptr == (char *) NULL)
    {
    fclose(MSTARfp);
    itkExceptionMacro(<< "Error: Can not determine Phoenix header length!");
    }
  else
    {
    sscanf((tptr + 20), "%d", &phlen);
    }

  /* Check for and extract native header length */
  tptr = (char *) strstr(tbuff, "native_header_length= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Can not determine native header length!\n");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 21), "%d", &nhlen);
    }

  /* Extract MSTAR column width */
  tptr = (char *) strstr(tbuff, "NumberOfColumns= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Error: Can not determine MSTAR image width");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 16), "%d", &numcols);
    m_Dimensions[0] = static_cast<int>(numcols);
    }

  /* Extract MSTAR row height */
  tptr = (char *) strstr(tbuff, "NumberOfRows= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Error: Can not determine MSTAR image height!");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 13), "%d", &numrows);
    m_Dimensions[1] = static_cast<int>(numrows);
    }

  /* Set MSTAR image type */
  if (nhlen == 0)
    { /* Implies FLOAT MSTAR chip image */
    mstartype = CHIP_IMAGE;
    otbMsgDevMacro(<< " Chip type ");
    }
  else
    {
    mstartype = FSCENE_IMAGE; /* UnShort Fullscene */
    otbMsgDevMacro(<< " Scene type ");
    }

  unsigned long step = this->GetNumberOfComponents() * (unsigned long) (m_BytePerPixel);
  float *       p = static_cast<float *>(buffer);

  int lNbLines   = this->GetIORegion().GetSize()[1];
  int lNbColumns = this->GetIORegion().GetSize()[0];

  unsigned long lNbPixels = (unsigned long) (lNbColumns * lNbLines);
  unsigned long lTailleBuffer = (unsigned long) (m_BytePerPixel) * lNbPixels;

  unsigned char* value = new unsigned char[lTailleBuffer];

  otbMsgDevMacro(
    << "MSTARImageIO::Read() IORegion Start[" << this->GetIORegion().GetIndex()[0] << "," <<
    this->GetIORegion().GetIndex()[1] << "] Size [" << this->GetIORegion().GetSize()[0] << "," <<
    this->GetIORegion().GetSize()[1] << "] on Image size [" << m_Dimensions[0] << "," << m_Dimensions[1] << "]");

  /******************************************************
      * Set up location to point to MSTAR magnitude data.. *
   ******************************************************/
  switch (mstartype)
    {
    case CHIP_IMAGE:
      magloc  = phlen;
      fseek(MSTARfp, magloc, 0);
      nchunks = numrows * numcols;
      break;
    case FSCENE_IMAGE:
      magloc  = phlen + nhlen; /* nhlen = 512 */
      fseek(MSTARfp, magloc, 0);
      nchunks = numrows * numcols;
      break;
    }

  /******************************************************
      * Check byte-order, swap bytes if necessary...       *
      * Allocate memory, read data,  & convert to 8-bit    *
      * based on 'mstartype'                               *
   ******************************************************/

  /* Check byteorder */
  byteorder = (int) CheckByteOrder();
  switch (byteorder)
    {
    case LSB_FIRST:
      printf("==> Little-Endian CPU detected: Will byteswap before scaling data!\n");
      break;
    case MSB_FIRST:
      printf("==> Big-Endian CPU detected: No byteswap needed!\n");
      break;
    }

  /******************************************************
      * Allocate memory, read data,  & write out based on  *
      * type of MSTAR image...and which data to write out  *
      *                                                    *
      * NOTE: For Chip data, I allocate all of the memory  *
      *       needed (magnitude+phase), read and then write*
      *       all of it out...                             *
      *                                                    *
      *       For fullscene data, because of the size of   *
      *       memory needed, I allocate only enough to     *
      *       grab the magnitude or the phase.  I then     *
      *       process first the magnitude and then the     *
      *       phase using the same buffer pointer....      *
      *                                                    *
      *       The code will read & write out ONLY the MAG  *
      *       image data if so specified by the user...    *
   ******************************************************/

  /* Open output file for writing... */
  switch (mstartype)
    {
    case CHIP_IMAGE:
      otbMsgDevMacro(<< " Chip and all ");
      totchunks = nchunks * 2;
      bytesPerImage = totchunks * sizeof(float);
      CHIPdata = (float *) malloc(bytesPerImage);

      if (CHIPdata == (float *) NULL)
        {
        fclose(MSTARfp);
        itkExceptionMacro("Error: Unable to malloc CHIP memory!\n");

        }

      switch (byteorder)
        {
        case LSB_FIRST: /* Little-endian..do byteswap */

          otbMsgDevMacro(<< "Performing auto-byteswap...\n");
          int returnVal;
          for (i = 0; i < totchunks; ++i)
            {
            returnVal = fread(bigfloatbuf, sizeof(char), 4, MSTARfp);
            if (!returnVal) itkExceptionMacro("Read failure");
            littlefloatval = byteswap_SR_IR(bigfloatbuf);
            CHIPdata[i] = littlefloatval;
            }
          break;

        case MSB_FIRST: /* Big-endian..no swap */

          numgot = fread(CHIPdata, sizeof(float), totchunks, MSTARfp);
          break;
        }

      /* Writes ALL data or MAG only data based on totchunks */
      //n = fwrite(CHIPdata, sizeof(float), totchunks, RAWfp);

      // Recopie dans le buffer

      for (int ci = 0; ci < nchunks; ++ci)
        {
        p[ci * 2] = CHIPdata[ci]; //magnitude
        p[ci * 2 + 1] = CHIPdata[nchunks + ci]; //phase
        }

      /* Cleanup: Close file..free memory */
      free(CHIPdata);
      break; /* End of CHIP_IMAGE case */

    case FSCENE_IMAGE:
      bytesPerImage = nchunks * sizeof(short);
      FSCENEdata = (unsigned short *) malloc(bytesPerImage);
      if (FSCENEdata == (unsigned short *) NULL)
        {
        fclose(MSTARfp);
        itkExceptionMacro(<< "Error: Unable to malloc fullscene memory!\n");
        }

      otbMsgDevMacro(<< "Reading MSTAR fullscene magnitude data from [" << MSTARname << "].");

      switch (byteorder)
        {
        case LSB_FIRST: /* Little-endian..do byteswap */
          otbMsgDevMacro(<< "Performing auto-byteswap...");
          int returnVal;
          for (i = 0; i < nchunks; ++i)
            {
            returnVal = fread(bigushortbuf, sizeof(char), 2, MSTARfp);
            if (!returnVal) itkExceptionMacro("Read failure");
            littleushortval = byteswap_SUS_IUS(bigushortbuf);
            FSCENEdata[i] = littleushortval;
            }
          break;

        case MSB_FIRST: /* Big-endian..no swap */
          numgot = fread(FSCENEdata, sizeof(short), nchunks, MSTARfp);
          break;
        }

      otbMsgDevMacro(<< "Writing MSTAR fullscene magnitude data to [" << RAWname << "].");
      //n = fwrite(FSCENEdata, sizeof(short), nchunks, RAWfp);
      for (int nbComponents = 0; nbComponents < 1; ++nbComponents)
        {
        // Recopie dans le buffer

        unsigned long cpt(0);
        cpt = (unsigned long) (nbComponents) * (unsigned long) (m_BytePerPixel);
        for (unsigned long i = 0; i < lTailleBuffer; i = i + m_BytePerPixel)
          {
          memcpy((void*) (&(p[cpt])), (const void*) (&(FSCENEdata[i])), (size_t) (m_BytePerPixel));
          cpt += step;
          }

        }

      if (n != nchunks)
        {
        fclose(MSTARfp);
        itkExceptionMacro(<< "Error: in writing MSTAR Fullscene data!");
        }

      otbMsgDevMacro(<< "Reading MSTAR fullscene phase data from [" << MSTARname << "].");

      switch (byteorder)
        {
        case LSB_FIRST: /* Little-endian..do byteswap */
          otbMsgDevMacro(<< "Performing auto-byteswap...");
          int returnVal;
          for (i = 0; i < nchunks; ++i)
            {
            returnVal = fread(bigushortbuf, sizeof(char), 2, MSTARfp);
            if (!returnVal) itkExceptionMacro("Read failure");
            littleushortval = byteswap_SUS_IUS(bigushortbuf);
            FSCENEdata[i] = littleushortval;
            }
          break;

        case MSB_FIRST: /* Big-endian..no swap */
          numgot = fread(FSCENEdata, sizeof(short), nchunks, MSTARfp);
          break;
        }

      otbMsgDevMacro(<< "Writing MSTAR fullscene phase data to [" << RAWname << "].");
//     n = fwrite(FSCENEdata, sizeof(short), nchunks, RAWfp);
      for (unsigned int nbComponents = 1; nbComponents < this->GetNumberOfComponents(); ++nbComponents)
        {
        // Recopie dans le buffer

        unsigned long cpt(0);
        cpt = (unsigned long) (nbComponents) * (unsigned long) (m_BytePerPixel);
        for (unsigned long i = 0; i < lTailleBuffer; i = i + m_BytePerPixel)
          {
          memcpy((void*) (&(p[cpt])), (const void*) (&(CHIPdata[i])), (size_t) (m_BytePerPixel));
          cpt += step;
          }

        }

      if (n != nchunks)
        {
        fclose(MSTARfp);
        itkExceptionMacro(<< "Error: in writing MSTAR Fullscene Phase data!");
        }

      /* Cleanup: free memory */
      free(FSCENEdata);

      break; /* End of FSCENE_IMAGE case */

    } /* End of 'mstartype' switch */

  /* Cleanup: close files */
  fclose(MSTARfp);

  delete[] value;
}

void MSTARImageIO::ReadImageInformation()
{
  int NbLignes;                /* Nombre de lignes de l'image */
  int NbColonnes;              /* Nombre de colonnes de l'image */
  int BytePerPixel = sizeof(float);            /* Nombre octets/pixel l'image */

  MSTARname = m_FileName.c_str();
  MSTARfp = fopen(MSTARname, "rb");
  if (MSTARfp == NULL)
    {
    itkExceptionMacro(<< "Error: Unable to open file for reading!\n\n " << m_FileName.c_str());
    }

  /****************************************************
      * Read first 512 bytes to figure out some header   *
      * parameters....                                   *
   ****************************************************/

  int returnVal = fread(tbuff, sizeof(char), 1024, MSTARfp);
  if (returnVal != 1024)
    {
    fclose(MSTARfp);
    itkExceptionMacro(<< "\nError: Unable in reading [" << MSTARname << "] header... Only read ["<< returnVal <<" of 1024 bytes !\n");
    }
  rewind(MSTARfp);

  /* Extract Phoenix Summary header length */
  tptr = (char *) strstr(tbuff, "PhoenixHeaderLength= ");
  if (tptr == (char *) NULL)
    {
    fclose(MSTARfp);
    itkExceptionMacro(<< "Error: Can not determine Phoenix header length!");
    }
  else
    {
    sscanf((tptr + 20), "%d", &phlen);
    }

  /* Check for and extract native header length */
  tptr = (char *) strstr(tbuff, "native_header_length= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Can not determine native header length!\n");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 21), "%d", &nhlen);
    }

  /* Extract MSTAR column width */
  tptr = (char *) strstr(tbuff, "NumberOfColumns= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Error: Can not determine MSTAR image width");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 16), "%d", &numcols);
    NbColonnes = static_cast<int>(numcols);
    }

  /* Extract MSTAR row height */
  tptr = (char *) strstr(tbuff, "NumberOfRows= ");
  if (tptr == (char *) NULL)
    {
    itkExceptionMacro(<< "Error: Can not determine MSTAR image height!");
    fclose(MSTARfp);
    }
  else
    {
    sscanf((tptr + 13), "%d", &numrows);
    NbLignes = static_cast<int>(numrows);
    }

  /* Set MSTAR image type */
  if (nhlen == 0)
    { /* Implies FLOAT MSTAR chip image */
    mstartype = CHIP_IMAGE;
    }
  else
    {
    mstartype = FSCENE_IMAGE; /* UnShort Fullscene */
    }

  otbMsgDebugMacro(<< "Driver: MSTAR - ");

  this->SetNumberOfComponents(2);
  this->SetNumberOfDimensions(2);
  m_Dimensions[0] = NbColonnes;
  m_Dimensions[1] = NbLignes;
  m_BytePerPixel = BytePerPixel;
  otbMsgDebugMacro(<< "Image size cree : " << m_Dimensions[0] << "," << m_Dimensions[1]);

  m_PixelType = VECTOR;

  /* Cleanup: close files */
  fclose(MSTARfp);

  this->SetComponentType(FLOAT);

}

/** The write function is not implemented */
void MSTARImageIO::WriteImageInformation(void)
{
  itkExceptionMacro(<< "Error: The MSTARImageIO::WriteImageInformation is not implemented!");
}

/** The write function is not implemented */
void MSTARImageIO::Write(const void* /* *buffer*/)
{
  itkExceptionMacro(<< "Error: The MSTARImageIO::Write is not implemented!");
}

/** Print Self Method */
void MSTARImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

float MSTARImageIO::byteswap_SR_IR(unsigned char *pointer)
{
  float *       temp;
  unsigned char iarray[4], *charptr;

  iarray[0] = *(pointer + 3);
  iarray[1] = *(pointer + 2);
  iarray[2] = *(pointer + 1);
  iarray[3] = *(pointer);
  charptr = iarray;
  temp    = (float *) charptr;
  return *(temp);
}

unsigned short MSTARImageIO::byteswap_SUS_IUS(unsigned char *pointer)
{
  unsigned short *temp;
  unsigned char   iarray[2], *charptr;

  iarray[0] = *(pointer + 1);
  iarray[1] = *(pointer);
  charptr = iarray;
  temp    = (unsigned short *) charptr;
  return *(temp);
}

int MSTARImageIO::CheckByteOrder(void)
{
  short w = 0x0001;
  char *b = (char *) &w;

  return (b[0] ? LSB_FIRST : MSB_FIRST);
}

} // end namespace otb

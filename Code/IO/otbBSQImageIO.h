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
#ifndef __otbBSQImageIO_h
#define __otbBSQImageIO_h

#include "itkImageIOBase.h"
#include "cai_image.h"

namespace otb
{

/** \class BSQImageIO
 *
 * \brief ImageIO object for reading and writing BSQ format images
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT BSQImageIO : public itk::ImageIOBase
{
public:

  typedef unsigned char InputPixelType;
  
  /** Standard class typedefs. */
  typedef BSQImageIO            Self;
  typedef itk::ImageIOBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSQImageIO, itk::ImageIOBase);


  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);
  
  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead(){  return true; };

  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();
 
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);
    
  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite() { return true; };

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);
  
protected:
  /** Construtor.*/
  BSQImageIO();
  /** Destructor.*/
  ~BSQImageIO();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Dimension along Ox of the image*/
  int m_width;
  /** Dimension along Oy of the image*/
  int m_height;
  /** Number of bands of the image*/
  int m_NbBands;
  /** Write all information on the image*/
  void InternalWriteImageInformation();

  
private:
  BSQImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Nombre d'octets par pixel */
  int     m_NbOctetPixel;
  bool    m_FlagWriteImageInformation;
  CAI_IMAGE *m_image;
    
  CAI_OK_KO cai_dest_ima_bsq(char *repertoire,char *nom_ima);
  CAI_IMAGE *cai_ouvre_lecture_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
  CAI_IMAGE *cai_ouvre_modifie_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig);
  CAI_OK_KO cai_ferme_bsq(CAI_IMAGE *image1);
  CAI_IMAGE *cai_ouvre_creation_bsq(char *repert,char *nom_image,
					  int  *nb_can, int  *nboct_pix,
					  int  *nb_col, int  *nb_lig,
					  char* label);
  CAI_OK_KO cai_ecriture_canal_bsq(CAI_IMAGE *image1, int *canal, 
         	 int *premiere_ligne, int *nombre_lignes_ecrire,
		 int *nombre_colonnes_ecrire,unsigned char *data_image);
  CAI_OK_KO cai_lecture_canal_bsq(CAI_IMAGE *image1, int *canal,
         	 int *premiere_ligne, int *premiere_colonne,
		 int *nombre_lignes_lire, int *nombre_colonnes_lire,
		 int *pas_lig, int *pas_col,unsigned char *data);
  CAI_OK_KO cai_modif_nbbitspix_bsq(CAI_IMAGE *image1,
					    int  NbBitsPix);
		 
  void  cai_inverser_tableau(short *BufL,short *BufE,int Nb);
  void  cai_inverser_tableau(unsigned char *BufL,unsigned char *BufE,int Nb)
      {
      cai_inverser_tableau( (short *)BufL, (short *)BufE, Nb);
      }
  char* cai_test_pFpf();
  int   cai_istyp_connu(char *type_code);
  char* cai_En_majuscule( char *mot );

};

} // end namespace otb

#endif // __otbLUMImageIO_h

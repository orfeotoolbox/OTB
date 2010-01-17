#ifndef system_HEADER
#define system_HEADER

#ifdef __cplusplus
extern "C" {
#endif

/* SYSTEM.H */

int getfilename( char *filename );

int get_input_string( char *prompt, char *string );

int verify( char **text,
	    int  n );

char *pickfile( char *path,
		char attrib,
		char *title );

void displayinfo( char *text[],
		  int  nlines );

char *currentdir( void );

void showdir( void );

void listdirs( void );

void changedir( void );

void createdir( void );

void deletedir( void );

void viewfile( char *filespec, int height );

void listfiles( char *filespec );

void deletefile( void );

void movefile( void );

void filecopy( char *from, char *to );

void copyfile( void );

void diskspace( void );

void changedrive( void );

void showdrive( void );

#ifdef __cplusplus
}
#endif

   
#endif

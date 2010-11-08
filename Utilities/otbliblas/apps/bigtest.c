#include <liblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dumperror (const char * appmsg)
{
   printf("\n%s.\n\tMessage:  %s\n\tMethod: %s",appmsg, LASError_GetLastErrorMsg(), LASError_GetLastErrorMethod());
}
int main()
{

    LASHeaderH header = NULL;
    LASWriterH writer = NULL;
    LASReaderH reader = NULL;
    LASPointH pt = NULL;
    LASError err;
    // Limitation about seeking past 4GB output size.  At 20 bytes / record, we
    // can successfully write 204 million records, but not 205.
    const unsigned long nMillionPoints = 205;
    const unsigned long NPOINTS = 1024*1024*nMillionPoints ;
    const char * OutputName = "Issue147.las";
    unsigned long i = 0;
    unsigned long npoints = 0;

    // Write a LAS file and after the points are in, update the header.
    header = LASHeader_Create();
    writer = LASWriter_Create(OutputName, header, LAS_MODE_WRITE);
    
    for (i = 0; i < NPOINTS; i++)
    {
      if (i % 1000 == 0)
         printf("\b\b\b\b\b\b\b%6.2f%%", ((double)i)/NPOINTS * 100.0);
      
      pt = LASPoint_Create();
      err = LASPoint_SetX(pt, 0);
      if (err) printf ("For point %lu, failed to set point value X\n", i);
      err = LASPoint_SetY(pt, 0);
      if (err) printf ("For point %lu, failed to set point value Y\n", i);
      err = LASPoint_SetZ(pt, 0);
      if (err) printf ("For point %lu, failed to set point value Z\n", i);
      err = LASWriter_WritePoint(writer, pt);  
      if (err) printf ("For point %lu, failed to WritePoint\n", i);
      LASPoint_Destroy(pt);
    }
   err = LASHeader_SetPointRecordsCount(header, NPOINTS);
   if (err) dumperror ("Failed to LASHeader_SetPointRecordsCount\n");
   err = LASWriter_WriteHeader(writer, header);
   if (err) dumperror ("Failed to LASWriter_WriteHeader");
   LASWriter_Destroy(writer);
   LASHeader_Destroy(header);
   
   // Read the file we just wrote and check the header data.
    reader = LASReader_Create(OutputName);
    header = LASReader_GetHeader(reader);
    npoints = LASHeader_GetPointRecordsCount(header);
    printf ("\n\nWrote %lu, Read %lu (testing %lu Million (1024 x 1024) Points)\n", NPOINTS, npoints, nMillionPoints);
}

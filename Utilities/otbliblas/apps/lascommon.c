#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <liblas.h>




static const char * LASPointClassification [] = {
  "Created, never classified",
  "Unclassified",
  "Ground",
  "Low Vegetation",
  "Medium Vegetation",
  "High Vegetation",
  "Building",
  "Low Point (noise)",
  "Model Key-point (mass point)",
  "Water",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Overlap Points",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition",
  "Reserved for ASPRS Definition"
};


LASPointSummary* SummarizePoints(LASReaderH reader) {
    
    LASPointSummary* summary;
    LASPointH p;
    uint8_t cls = 0;
    int i = 0;

    summary = (LASPointSummary*) malloc(sizeof(LASPointSummary));

    summary->number_of_point_records = 0;
    for (i=0; i<8;i++)
        summary->number_of_points_by_return[i] = 0;
    for (i=0; i<8;i++)
        summary->number_of_returns_of_given_pulse[i] = 0;
    for (i=0; i<32;i++)
        summary->classification[i] = 0;
    summary->classification_synthetic = 0;
    summary->classification_keypoint = 0;
    summary->classification_withheld = 0;
    



    p  = LASReader_GetNextPoint(reader);
    
    if (!p) {
        if (LASError_GetLastErrorNum()) 
            LASError_Print("Not able to fetch a point.  LASReaderH is invalid");
        else
            LASError_Print("File does not contain any points to read.");
        exit(1);
    }

    summary->pmin = LASPoint_Copy(p);
    summary->pmax = LASPoint_Copy(p);

    /* If we got this far we have one point */
    i = 1;
    while (p)
    {
    
        summary->x = LASPoint_GetX(p);
        LASPoint_SetX(summary->pmin, MIN(summary->x, LASPoint_GetX(summary->pmin)));
        LASPoint_SetX(summary->pmax, MAX(summary->x, LASPoint_GetX(summary->pmax)));

        summary->y = LASPoint_GetY(p);
        LASPoint_SetY(summary->pmin, MIN(summary->y, LASPoint_GetY(summary->pmin)));
        LASPoint_SetY(summary->pmax, MAX(summary->y, LASPoint_GetY(summary->pmax)));

        summary->z = LASPoint_GetZ(p);
        LASPoint_SetZ(summary->pmin, MIN(summary->z, LASPoint_GetZ(summary->pmin)));
        LASPoint_SetZ(summary->pmax, MAX(summary->z, LASPoint_GetZ(summary->pmax)));

        summary->intensity = LASPoint_GetIntensity(p);
        LASPoint_SetIntensity(summary->pmin, MIN(summary->intensity, LASPoint_GetIntensity(summary->pmin)));
        LASPoint_SetIntensity(summary->pmax, MAX(summary->intensity, LASPoint_GetIntensity(summary->pmax)));

        summary->t = LASPoint_GetTime(p);
        LASPoint_SetTime(summary->pmin, MIN(summary->t, LASPoint_GetTime(summary->pmin)));
        LASPoint_SetTime(summary->pmax, MAX(summary->t, LASPoint_GetTime(summary->pmax)));

        summary->retnum = LASPoint_GetReturnNumber(p);
        LASPoint_SetReturnNumber(summary->pmin, MIN(summary->retnum, LASPoint_GetReturnNumber(summary->pmin)));
        LASPoint_SetReturnNumber(summary->pmax, MAX(summary->retnum, LASPoint_GetReturnNumber(summary->pmax)));                    

        summary->numret = LASPoint_GetNumberOfReturns(p);
        LASPoint_SetNumberOfReturns(summary->pmin, MIN(summary->numret, LASPoint_GetNumberOfReturns(summary->pmin)));
        LASPoint_SetNumberOfReturns(summary->pmax, MAX(summary->numret, LASPoint_GetNumberOfReturns(summary->pmax)));  
        
        summary->scandir = LASPoint_GetScanDirection(p);
        LASPoint_SetScanDirection(summary->pmin, MIN(summary->scandir, LASPoint_GetScanDirection(summary->pmin)));
        LASPoint_SetScanDirection(summary->pmax, MAX(summary->scandir, LASPoint_GetScanDirection(summary->pmax)));  
        
        summary->fedge = LASPoint_GetFlightLineEdge(p);
        LASPoint_SetFlightLineEdge(summary->pmin, MIN(summary->fedge, LASPoint_GetFlightLineEdge(summary->pmin)));
        LASPoint_SetFlightLineEdge(summary->pmax, MAX(summary->fedge, LASPoint_GetFlightLineEdge(summary->pmax)));  

        summary->scan_angle = LASPoint_GetScanAngleRank(p);
        LASPoint_SetScanAngleRank(summary->pmin, MIN(summary->scan_angle, LASPoint_GetScanAngleRank(summary->pmin)));
        LASPoint_SetScanAngleRank(summary->pmax, MAX(summary->scan_angle, LASPoint_GetScanAngleRank(summary->pmax)));  

        summary->user_data = LASPoint_GetUserData(p);
        LASPoint_SetUserData(summary->pmin, MIN(summary->user_data, LASPoint_GetUserData(summary->pmin)));
        LASPoint_SetUserData(summary->pmax, MAX(summary->user_data, LASPoint_GetUserData(summary->pmax)));  

        summary->number_of_point_records = i;

        if (LASPoint_GetReturnNumber(p))
            summary->number_of_points_by_return[LASPoint_GetReturnNumber(p)-1]++;
        else
            summary->number_of_points_by_return[LASPoint_GetReturnNumber(p)]++;        

        summary->number_of_returns_of_given_pulse[LASPoint_GetNumberOfReturns(p)]++;
        
        cls = LASPoint_GetClassification(p);
        LASPoint_SetClassification(summary->pmin, MIN(cls, LASPoint_GetClassification(summary->pmin)));
        LASPoint_SetClassification(summary->pmax, MAX(cls, LASPoint_GetClassification(summary->pmax)));  

        summary->classification[(cls & 31)]++;            
        if (cls & 32) summary->classification_synthetic++;          
        if (cls & 64) summary->classification_keypoint++; 
        if (cls & 128) summary->classification_withheld++;
          
          /*
          // if (lasreader->point.point_source_ID < point_min.point_source_ID) point_min.point_source_ID = lasreader->point.point_source_ID;
          // else if (lasreader->point.point_source_ID > point_max.point_source_ID) point_max.point_source_ID = lasreader->point.point_source_ID;

          */
        p  = LASReader_GetNextPoint(reader);

        if (LASError_GetLastErrorNum()) {
            LASError_Print("Not able to fetch point");
            exit(1);
        }
        
        i++;
    }
    
    return summary; 
}

void print_point(FILE *file, LASPointH point) {

    fprintf(file, "---------------------------------------------------------\n");
    
    fprintf(file, "  X: \t\t%.6f\n", 
                  LASPoint_GetX(point)
                  );
    fprintf(file, "  Y: \t\t%.6f\n", 
                  LASPoint_GetY(point)
                  );
    fprintf(file, "  Z: \t\t%.6f\n", 
                  LASPoint_GetZ(point)
                  );
    fprintf(file, "  Time:\t\t\t%.6f\n", 
                  LASPoint_GetTime(point)
                  );
    fprintf(file, "  Return Number:\t%d\n", 
                  LASPoint_GetReturnNumber(point)
                  );
    fprintf(file, "  Return Count:\t\t%d\n", 
                  LASPoint_GetNumberOfReturns(point)
                  );
    fprintf(file, "  Flightline Edge:\t%d\n", 
                  LASPoint_GetFlightLineEdge(point)
                  );
    fprintf(file, "  Intensity:\t\t%d\n",
                  LASPoint_GetIntensity(point)
                  );
    fprintf(file, "  Scan Direction Flag:\t%d\n",
                  LASPoint_GetScanDirection(point)
                  );
    fprintf(file, "  Scan Angle Rank:\t%d\n",
                  LASPoint_GetScanAngleRank(point)
                  );
                    
    fprintf(file, "  Classification:\t%d\n",
                  LASPoint_GetClassification(point)
                  );

}
void print_point_summary(FILE *file, LASPointSummary* summary, LASHeaderH header) {

    long rgpsum = 0;
    long pbretsum = 0;
    
    int i = 0;

    if (!summary) {LASError_Print("Point Summary does not exist!"); exit(1);}

    fprintf(file, "\n---------------------------------------------------------\n");
    fprintf(file, "  Point Inspection Summary\n");
    fprintf(file, "---------------------------------------------------------\n");
    
    fprintf(file, "  Header Point Count: %d\n",
                  LASHeader_GetPointRecordsCount(header));
                    
    fprintf(file, "  Actual Point Count: %d\n", 
                  summary->number_of_point_records);

    fprintf(file, "\n  Minimum and Maximum Attributes (min,max)\n");
    fprintf(file, "---------------------------------------------------------\n");
    
    fprintf(file, "  Min X,Y,Z: \t\t%.6f,%.6f,%.6f\n", 
                  LASPoint_GetX(summary->pmin),
                  LASPoint_GetY(summary->pmin),
                  LASPoint_GetZ(summary->pmin)
                    );
    fprintf(file, "  Max X,Y,Z: \t\t%.6f,%.6f,%.6f\n", 
                  LASPoint_GetX(summary->pmax),
                  LASPoint_GetY(summary->pmax),
                  LASPoint_GetZ(summary->pmax)
                  );
    fprintf(file, "  Bounding Box:\t\t%.2f,%.2f,%.2f,%.2f\n",
                  LASPoint_GetX(summary->pmin),
                  LASPoint_GetY(summary->pmin),
                  LASPoint_GetX(summary->pmax),
                  LASPoint_GetY(summary->pmax)
                  );
    fprintf(file, "  Time:\t\t\t%.6f,%.6f\n", 
                  LASPoint_GetTime(summary->pmin),
                  LASPoint_GetTime(summary->pmax)
                  );
    fprintf(file, "  Return Number:\t%d,%d\n", 
                  LASPoint_GetReturnNumber(summary->pmin),
                  LASPoint_GetReturnNumber(summary->pmax)
                  );
    fprintf(file, "  Return Count:\t\t%d,%d\n", 
                  LASPoint_GetNumberOfReturns(summary->pmin),
                  LASPoint_GetNumberOfReturns(summary->pmax)
                  );
    fprintf(file, "  Flightline Edge:\t%d,%d\n", 
                  LASPoint_GetFlightLineEdge(summary->pmin),
                  LASPoint_GetFlightLineEdge(summary->pmax)
                  );
    fprintf(file, "  Intensity:\t\t%d,%d\n",
                  LASPoint_GetIntensity(summary->pmin),
                  LASPoint_GetIntensity(summary->pmax)
                  );
    fprintf(file, "  Scan Direction Flag:\t%d,%d\n",
                  LASPoint_GetScanDirection(summary->pmin),
                  LASPoint_GetScanDirection(summary->pmax)
                  );
    fprintf(file, "  Scan Angle Rank:\t%d,%d\n",
                  LASPoint_GetScanAngleRank(summary->pmin),
                  LASPoint_GetScanAngleRank(summary->pmax)
                  );
    fprintf(file, "  Classification:\t%d,%d\n",
                  LASPoint_GetClassification(summary->pmin),
                  LASPoint_GetClassification(summary->pmax)
                  );

    fprintf(file, "\n  Number of Points by Return\n");
    fprintf(file, "---------------------------------------------------------\n");

    for (i = 0; i < 5; i++) {
        pbretsum = pbretsum + summary->number_of_points_by_return[i];
        fprintf(file, "\t(%d) %d", i,summary->number_of_points_by_return[i]);
    }
    fprintf(file, "\n Total Points: %ld\n", pbretsum); 
                    
    fprintf(file, "\n  Number of Returns by Pulse\n");
    fprintf(file, "---------------------------------------------------------\n");
    
    for (i = 1; i < 8; i++) {
        rgpsum = rgpsum + summary->number_of_returns_of_given_pulse[i];
        fprintf(file, "\t(%d) %d", i,summary->number_of_returns_of_given_pulse[i]);
    }
    fprintf(file, "\n Total Pulses: %ld\n", rgpsum); 


    for (i = 0; i < 5; i++) {
        if (LASHeader_GetPointRecordsByReturnCount(header, i) != summary->number_of_points_by_return[i]) 
        {
            fprintf(file, " \n Actual number of points by return \n is different from header (actual, header):\n"); 
            for (i = 0; i < 5; i++) {
                fprintf(file, "\t(%d,%d)", 
                        summary->number_of_points_by_return[i],
                        LASHeader_GetPointRecordsByReturnCount(header, i)
                        );
            } 
            fprintf(file, "\n");
        }
    }

    fprintf(file, "\n  Point Classifications\n");
    fprintf(file, "---------------------------------------------------------\n");

    for (i = 0; i < 32; i++) {
        if (summary->classification[i]) {
            fprintf(file, "\t%8d %s (%d)\n", 
                          summary->classification[i], 
                          LASPointClassification[i], 
                          i);
        }
    }

    if (summary->classification_synthetic || summary->classification_keypoint ||  summary->classification_withheld) {
        fprintf(file, "\n  Point Classification Histogram\n");
        fprintf(file, "---------------------------------------------------------\n");

        if (summary->classification_synthetic) fprintf(file, " +-> flagged as synthetic: %d\n", summary->classification_synthetic);
        if (summary->classification_keypoint) fprintf(file,  " +-> flagged as keypoints: %d\n", summary->classification_keypoint);
        if (summary->classification_withheld) fprintf(file,  " +-> flagged as withheld:  %d\n", summary->classification_withheld);
    }
}



void print_header(FILE *file, LASHeaderH header, const char* file_name, int bSkipVLR) {

    char *pszSignature = NULL;
    char *pszProjectId = NULL;
    char *pszSystemId = NULL;
    char *pszSoftwareId = NULL;
    char *pszProj4 = NULL;
    
    char *pszVLRUser = NULL;
    char *pszVLRDescription = NULL;
    uint16_t nVLRLength = 0;
    uint16_t nVLRRecordId = 0;
    
    LASVLRH pVLR = NULL;

    uint32_t nVLR = 0;
    int i = 0;
    
    pszSignature = LASHeader_GetFileSignature(header);
    pszProjectId = LASHeader_GetProjectId(header);
    pszSystemId = LASHeader_GetSystemId(header);
    pszSoftwareId = LASHeader_GetSoftwareId(header);
    pszProj4 = LASHeader_GetProj4(header);
    
    nVLR = LASHeader_GetRecordsCount(header);
    
    fprintf(file, "\n---------------------------------------------------------\n");
    fprintf(file, "  Header Summary\n");
    fprintf(file, "---------------------------------------------------------\n");


    fprintf(file, "  File Name: %s\n", file_name);
    
    if (strcmp(pszSignature,"LASF") !=0) {
        LASError_Print("File signature is not 'LASF'... aborting");
        exit(1);
    }
    fprintf(file, "  Version:                    %d.%d\n", 
                    LASHeader_GetVersionMajor(header), 
                    LASHeader_GetVersionMinor(header));

    fprintf(file, "  Source ID:                  %d\n", 
                    LASHeader_GetFileSourceId(header) ) ;

    fprintf(file, "  Reserved:                   %d\n", 
                    LASHeader_GetReserved(header) );

    fprintf(file, "  Project ID/GUID:           '%s'\n", 
                    pszProjectId);

    fprintf(file, "  System Identifier:         '%s'\n", 
                    pszSystemId);

    fprintf(file, "  Generating Software:       '%s'\n", 
                    pszSoftwareId);

    fprintf(file, "  File Creation Day/Year:    %d/%d\n", 
                    LASHeader_GetCreationDOY(header), 
                    LASHeader_GetCreationYear(header));

    fprintf(file, "  Header Size                %d\n", 
                    LASHeader_GetHeaderSize(header));

    fprintf(file, "  Offset to Point Data       %d\n", 
                    LASHeader_GetDataOffset(header));

    fprintf(file, "  Number Var. Length Records %d\n", 
                    LASHeader_GetRecordsCount(header));

    fprintf(file, "  Point Data Format          %d\n", 
                    LASHeader_GetDataFormatId(header));

    fprintf(file, "  Point Data Record Length   %d\n", 
                    LASHeader_GetDataRecordLength(header));

    fprintf(file, "  Number of Point Records    %d\n", 
                    LASHeader_GetPointRecordsCount(header));

    fprintf(file, "  Number of Points by Return %d %d %d %d %d\n", 
                    LASHeader_GetPointRecordsByReturnCount(header, 0), 
                    LASHeader_GetPointRecordsByReturnCount(header, 1), 
                    LASHeader_GetPointRecordsByReturnCount(header, 2), 
                    LASHeader_GetPointRecordsByReturnCount(header, 3), 
                    LASHeader_GetPointRecordsByReturnCount(header, 4));

    fprintf(file, "  Scale Factor X Y Z         %.6g %.6g %.6g\n", 
                    LASHeader_GetScaleX(header), 
                    LASHeader_GetScaleY(header),
                    LASHeader_GetScaleZ(header));

    fprintf(file, "  Offset X Y Z               %.6f %.6f %.6f\n", 
                    LASHeader_GetOffsetX(header), 
                    LASHeader_GetOffsetY(header), 
                    LASHeader_GetOffsetZ(header));

    fprintf(file, "  Min X Y Z                  %.6f %.6f %.6f\n",
                    LASHeader_GetMinX(header), 
                    LASHeader_GetMinY(header), 
                    LASHeader_GetMinZ(header));

    fprintf(file, "  Max X Y Z                  %.6f %.6f %.6f\n", 
                    LASHeader_GetMaxX(header), 
                    LASHeader_GetMaxY(header), 
                    LASHeader_GetMaxZ(header));
    
    fprintf(file, " Spatial Reference           %s\n",
                    pszProj4);

    if (nVLR && !bSkipVLR) {
        
    fprintf(file, "\n---------------------------------------------------------\n");
    fprintf(file, "  VLR Summary\n");
    fprintf(file, "---------------------------------------------------------\n");

        for (i = 0; i < (int)nVLR; i++) {
            pVLR = LASHeader_GetVLR(header, i);

            if (LASError_GetLastErrorNum()) {
                LASError_Print("Unable to fetch VLR");
                exit(1);
            }
            
            pszVLRUser = LASVLR_GetUserId(pVLR);
            pszVLRDescription = LASVLR_GetDescription(pVLR);
            nVLRLength = LASVLR_GetRecordLength(pVLR);
            nVLRRecordId = LASVLR_GetRecordId(pVLR);
            

            fprintf(file, "   User: '%s' - Description: '%s'\n", pszVLRUser, pszVLRDescription);
            fprintf(file, "   ID: %d Length: %d\n\n", nVLRRecordId, nVLRLength);
            
            LASVLR_Destroy(pVLR);
            pVLR = NULL;
            
            free(pszVLRUser);
            free(pszVLRDescription);
        }
        
    }
    free(pszSignature);
    free(pszProjectId);
    free(pszSystemId);
    free(pszSoftwareId);
    free(pszProj4);
}

void repair_header(FILE *file, LASHeaderH header, LASPointSummary* summary) {

    int repair_bounding_box = FALSE;
    int update_return_counts = FALSE;    
    int err = 0;
    int i = 0;
 
    if (! header) {
        LASError_Print("Inputted header to repair_header was NULL ");
        exit(1);
    } 

    if (! summary) {
        LASError_Print("Inputted summary to repair_header was NULL ");
        exit(1);

    } 


    if (! repair_bounding_box) {
        if ( LASHeader_GetMinX(header) != LASPoint_GetX(summary->pmin) )
            repair_bounding_box = TRUE;
        if ( LASHeader_GetMinY(header) != LASPoint_GetY(summary->pmin) )
            repair_bounding_box = TRUE;
        if ( LASHeader_GetMinZ(header) != LASPoint_GetZ(summary->pmin) )
            repair_bounding_box = TRUE;

        if ( LASHeader_GetMaxX(header) != LASPoint_GetX(summary->pmax) )
            repair_bounding_box = TRUE;
        if ( LASHeader_GetMaxY(header) != LASPoint_GetY(summary->pmax) )
            repair_bounding_box = TRUE;
        if ( LASHeader_GetMaxZ(header) != LASPoint_GetZ(summary->pmax) )
            repair_bounding_box = TRUE;
    }
    
    if (repair_bounding_box) {
        fprintf(file, "  Reparing Bounding Box...\n");
        err = LASHeader_SetMin( header, 
                                LASPoint_GetX(summary->pmin), 
                                LASPoint_GetY(summary->pmin), 
                                LASPoint_GetZ(summary->pmin)
                              );
        if (err) {
            LASError_Print("Could not set minimum for header ");
            exit(1);
        }
        err = LASHeader_SetMax( header, 
                                LASPoint_GetX(summary->pmax), 
                                LASPoint_GetY(summary->pmax), 
                                LASPoint_GetZ(summary->pmax)
                              );
        if (err) {
            LASError_Print("Could not set minimum for header ");
            exit(1);
        }

    }


    for (i = 0; i < 5; i++) {

        if (LASHeader_GetPointRecordsByReturnCount(header, i) != 
            summary->number_of_points_by_return[i]) 
        {
            update_return_counts = TRUE;
            break;
        }
    }
    
    if (update_return_counts) {
        fprintf(file, "  Reparing Point Count by Return...\n");
        for (i = 0; i < 5; i++) {
            LASHeader_SetPointRecordsByReturnCount( header,  
                                                    i, 
                                                    summary->number_of_points_by_return[i]);
        }                
    }   
}


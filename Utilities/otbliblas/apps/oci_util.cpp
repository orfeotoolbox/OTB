#include "oci_util.hpp"



std::string ReadSQLData(std::string filename)
{
    std::istream* infile = OpenInput(filename.c_str(), true);
    std::ifstream::pos_type size;
    char* data;
    if (infile->good()){
        size = infile->tellg();
        data = new char [size];
        infile->seekg (0, std::ios::beg);
        infile->read (data, size);
        // infile->close();

        std::string output = std::string(data, (std::size_t) size);
        delete[] data;
        delete infile;
        return output;
    } 
    else 
    {   
        delete infile;
        return std::string("");
    }
}

bool EnableTracing(OWConnection* connection)
{
    std::ostringstream oss;
// http://www.oracle-base.com/articles/10g/SQLTrace10046TrcsessAndTkprof10g.php
    oss << "ALTER SESSION SET EVENTS '10046 trace name context forever, level 12'";

    OWStatement* statement = 0;
    
    statement = connection->CreateStatement(oss.str().c_str());
    
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        delete statement;
        std::ostringstream oss;
        oss << "Failed to Enable tracing "  << std::endl << e.what() << std::endl;
        throw std::runtime_error(oss.str());
    }    
    
    return true;
}

bool IsGeographic(OWConnection* connection, long srid) 
{

    std::ostringstream oss;
    char* kind = (char* ) malloc (OWNAME * sizeof(char));
    oss << "SELECT COORD_REF_SYS_KIND from MDSYS.SDO_COORD_REF_SYSTEM WHERE SRID = :1";
    
    OWStatement* statement = 0;

    statement = connection->CreateStatement(oss.str().c_str());
    long* p_srid = (long*) malloc( 1 * sizeof(long));
    p_srid[0] = srid;
    
    statement->Bind(p_srid);
    statement->Define(kind);    
    
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        delete statement;
        std::ostringstream oss;
        oss << "Failed to fetch geographicness of srid " << srid << std::endl << e.what() << std::endl;
        throw std::runtime_error(oss.str());
    }  
    
    if (compare_no_case(kind, "GEOGRAPHIC2D",12) == 0) {
        delete statement;
        free(kind);
        free(p_srid);
        return true;
    }
    if (compare_no_case(kind, "GEOGRAPHIC3D",12) == 0) {
        delete statement;
        free(kind);
        free(p_srid);
        return true;
    }

    free(kind);
    free(p_srid);

    return false;
}

OWStatement* RunSQL(OWConnection* connection, std::ostringstream& command) 
{
    OWStatement* statement = 0;
    statement = connection->CreateStatement(command.str().c_str());
    
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        delete statement;
        std::ostringstream oss;
        oss << "Failed to run SQL:" << command.str() << std::endl << e.what() << std::endl;
        throw std::runtime_error(oss.str());
    }      
    return statement;    
}

bool Cleanup(OWConnection* connection, std::string tableName)
{
    std::ostringstream oss;
    OWStatement* statement = 0;
    
    oss << "DELETE FROM " << tableName;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement;
    oss.str("");

    oss << "DROP TABLE " << tableName;
    std::cout << oss.str() << std::endl;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; 
    oss.str("");       

    oss << "DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME='"<< tableName << "'";
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; 
    oss.str("");

    connection->Commit();
    
    return true;    
}

bool CreateBlockTable(OWConnection* connection, std::string tableName)
{
    std::ostringstream oss;
    OWStatement* statement = 0;
    
    oss << "CREATE TABLE " << tableName << " AS SELECT * FROM MDSYS.SDO_PC_BLK_TABLE";

    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else return false;
    oss.str("");

    connection->Commit();
    
    return true;

}


bool BlockTableExists(OWConnection* connection, std::string tableName)
{
    std::ostringstream oss;

    char szTable[OWNAME]= "";
    oss << "select table_name from user_tables where table_name like upper('%%"<< tableName <<"%%') ";

    OWStatement* statement = 0;
    
    statement = connection->CreateStatement(oss.str().c_str());
    statement->Define(szTable);
    
    try {
        statement->Execute();
    } catch (std::runtime_error const& e) {
        delete statement;
        std::ostringstream oss;
        oss << "Failed select if block table "<< tableName << " exists.  Do you have rights to select?"  
            << std::endl << e.what() << std::endl;
        throw std::runtime_error(oss.str());
    }  
    
    return true;
        
}

bool CreateBlockIndex(  OWConnection* connection, 
                        std::string tableName, 
                        long srid, 
                        bool bUse3d)
{
    boost::ignore_unused_variable_warning(srid);    
    std::ostringstream oss;
    OWStatement* statement = 0;
    
    oss << "CREATE INDEX "<< tableName <<"_cloud_idx on "<<tableName<<"(blk_extent) INDEXTYPE IS MDSYS.SPATIAL_INDEX";
    
    if (bUse3d) {
        oss <<" PARAMETERS('sdo_indx_dims=3')" ;
    }
    
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else return false;
    oss.str("");

    oss << "CREATE INDEX "<< tableName <<"_objectid_idx on "<<tableName<<"(OBJ_ID,BLK_ID) COMPRESS 2" ;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else return false;
    oss.str("");    
    return true;
        
}


std::string to_upper(const std::string& input)
{
    std::string inp = std::string(input);
    std::string output = std::string(input);
    
    std::transform(inp.begin(), inp.end(), output.begin(), static_cast < int(*)(int) > (toupper));
    
    return output;
}

bool WipeBlockTable(   OWConnection* connection, 
                    std::string tableName, 
                    std::string cloudTableName, 
                    std::string cloudColumnName)
{
    std::ostringstream oss;
    OWStatement* statement = 0;

    oss << "DELETE from " <<cloudTableName;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else 
    {
        // if we failed, try dropping the index
        std::cout << "Dropping index ..." << std::endl;
        oss.str("");  
        oss << "DROP INDEX "<<tableName<<"_cloud_idx" ;
        statement = RunSQL(connection, oss);
        if (statement != 0) delete statement; else return false;
        oss.str("");
        
        // redelete from the table
        oss << "DELETE from " <<cloudTableName;
        statement = RunSQL(connection, oss);
        if (statement != 0) delete statement; else return false;
        oss.str("");
    }
    oss.str("");
    
    std::string cloudColumnName_u = to_upper(cloudColumnName);
    std::string cloudTableName_u = to_upper(cloudTableName);


oss << "declare\n"
"begin \n"
"  mdsys.sdo_pc_pkg.drop_dependencies('"<<cloudTableName_u<<"', '"<<cloudColumnName_u<<"');"
"end;";
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; 
    oss.str("");
    
    oss << "DROP TABLE "<< tableName ;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; 
    oss.str("");

    // Oracle upper cases the table name when inserting it in the 
    // USER_SDO_GEOM_METADATA.  We'll use std::transform to do it. 
    // See http://forums.devx.com/showthread.php?t=83058 for the 
    // technique
    // string table(tableName);
    std::string table = to_upper(tableName);
    oss << "DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME='"<<table<<"'" ;
    statement = RunSQL(connection, oss);
    if (statement != 0) delete statement; else return false;
    oss.str("");   

    connection->Commit();
   
    return true;

}

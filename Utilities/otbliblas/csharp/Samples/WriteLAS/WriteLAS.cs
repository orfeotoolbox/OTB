using System;
using System.Collections.Generic;
using System.Text;
using LibLAS;
using System.IO;


namespace WriteLAS
{
    public class WriteLAS
    {
        static void Main(string[] args)
        {
            try
            {

                string filename = @".\test.las";
                LASHeader hdr = new LASHeader();
                hdr.VersionMajor = 1;
                hdr.VersionMinor = 1;
                hdr.DataFormatId = (byte)LASHeader.PointFormat.ePointFormat1;
                hdr.PointRecordsCount = 1000; // should be corrected automatically by writer
                LASWriter laswriter = new LASWriter(filename, hdr, LASReadWriteMode.LASModeWrite);
                LASPoint p=new LASPoint();
                p.X = 10;
                p.Y = 20;
                p.Z = 30;
                laswriter.WritePoint(p);
                //File.Delete(filename);

            }
            catch (LASException e)
            {
                Console.WriteLine("\nLASException! Msg: {0}", e.Message);
            }
            catch (SystemException e)
            {
                Console.WriteLine("\nException! Msg: {0}", e.Message);
            }
            catch
            {
                Console.WriteLine("Unknown exception caught");
            }

            Console.WriteLine("End of file");
            Console.Read();
        }

    }
}

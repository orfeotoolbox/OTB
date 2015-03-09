/*
 * Example on the use of the Rescale with outxml 
 */
 
import java.lang.Float;
import org.otb.application.*;

class RescaleOutXMLTest {

  public static void main( String argv[] ) {

    Application app = Registry.CreateApplication("Rescale");
    app.SetParameterString("in",     argv[0], true);
    app.SetParameterString("out",    argv[1], true);
    app.SetParameterFloat ("outmin", Float.valueOf(argv[2]).floatValue(), true);
    app.SetParameterFloat ("outmax", Float.valueOf(argv[3]).floatValue(), true);
    app.SetParameterString("outxml", argv[4]);
    int result = app.ExecuteAndWriteOutput();

    System.exit( result );
  }

}

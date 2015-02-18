/*
 * Example on the use of the Rescale with inxml
 */

import java.lang.Float;
import org.otb.application.*;

class RescaleInXMLTest {

  public static void main( String argv[] ) {

    Application app = Registry.CreateApplication("Rescale");

    app.SetParameterString("inxml", argv[0]);
    app.UpdateParameters();

    app.SetParameterString("in",     argv[1]);
    app.SetParameterString("out",    argv[2]);

    int result = app.ExecuteAndWriteOutput();

    System.exit( result );
  }

}

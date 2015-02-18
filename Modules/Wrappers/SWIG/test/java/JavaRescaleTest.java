/*
 * Example on the use of the Rescale 
 */
 
import java.lang.Float;
import org.otb.application.*;

class RescaleTest {

  public static void main( String argv[] ) {

    String[] appAvailable = Registry.GetAvailableApplications();
    System.out.println( "Available applications :" );
    
    for (int i = 0; i < appAvailable.length; i++)
    {
      System.out.println( "  " + appAvailable[i] );
    }
    System.out.println();
    
    System.out.println( "Creating application " + "Rescale");
    Application app = Registry.CreateApplication("Rescale");
    System.out.println( "Created application Rescale");
    System.out.println( "Rescale doc : " + app.GetDescription() );
    
    String[] paramlist = app.GetParametersKeys();
    System.out.println( );
    System.out.println( "Rescale parameters : " );
    for (int i = 0; i < paramlist.length; i++)
    {
      System.out.println( "  " + paramlist[i] );
    }
    System.out.println();
    
    app.SetParameterString("in",     argv[0]);
    app.SetParameterString("out",    argv[1]);
    app.SetParameterFloat ("outmin", Float.valueOf(argv[2]).floatValue() );
    app.SetParameterFloat ("outmax", Float.valueOf(argv[3]).floatValue() );

    int result = app.ExecuteAndWriteOutput();
    
    System.exit( result );
  }

}

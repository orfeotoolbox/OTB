/*
 * Example on the use of the Rescale 
 */
 
import org.otb.application.*;


class RescaleTest {

  public static void main( String argv[] ) {

    String[] appAvailable = Registry.GetAvailableApplications();
    System.out.println( "Available applications :" );
    
    for (int i = 0; i < appAvailable.length; i++)
    {
      System.out.println( appAvailable[i] );
    }

    System.out.println( Registry.CreateApplication("Rescale").GetDescription() );
    System.out.println( "Creating application " + "Rescale");
    Application app = Registry.CreateApplication("Rescale");
    System.out.println( "Created application Rescale");
    System.out.println( "Rescale doc : " + app.GetDescription() );
    
  }

}

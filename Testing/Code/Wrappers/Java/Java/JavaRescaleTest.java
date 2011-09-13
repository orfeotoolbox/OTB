/*
 * Example on the use of the Rescale 
 */
 
import org.otb.application.*;


class RescaleTest {

  public static void main( String argv[] ) {

    vectorstring appAvailable = Registry.GetAvailableApplications();
    System.out.println( "Available applications :" );
    
    for (int i = 0; i < appAvailable.size(); i++)
    {
      System.out.println( appAvailable.get(i) );
    }

    System.out.println( "Creating application " + "Rescale");
    Application_Pointer app = Registry.CreateApplication("Rescale");
    
    System.out.println( Registry.CreateApplication("Rescale").GetDescription() );
    
  }

}
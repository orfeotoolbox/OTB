/*
 * Example on the use of the Smoothing 
 */
 
import org.otb.application.*;


class SmoothingTest {

  public static void main( String argv[] ) {

    vectorstring appAvailable = Registry.GetAvailableApplications();
    System.out.println( "Available applications :" );
    
    for (int i = 0; i < appAvailable.size(); i++)
    {
      System.out.println( appAvailable.get(i) );
    }

    System.out.println( "Creating application " + "Smoothing");
    Application_Pointer app = Registry.CreateApplication("Smoothing");
    
    System.out.println( Registry.CreateApplication("Smoothing").GetDescription() );
    
  }

}
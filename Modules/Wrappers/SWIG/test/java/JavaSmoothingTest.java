/*
 * Example on the use of the Smoothing 
 */
 
import org.otb.application.*;


class SmoothingTest {

  public static void main( String argv[] ) {

    String[] appAvailable = Registry.GetAvailableApplications();
    System.out.println( "Available applications :" );
    
    for (int i = 0; i < appAvailable.length; i++)
    {
      System.out.println( appAvailable[i] );
    }

    System.out.println( "Creating application " + "Smoothing");
    Application app = Registry.CreateApplication("Smoothing");
    
    System.out.println( Registry.CreateApplication("Smoothing").GetDescription() );
    
  }

}

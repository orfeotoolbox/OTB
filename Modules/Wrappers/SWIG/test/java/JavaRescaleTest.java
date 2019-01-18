/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

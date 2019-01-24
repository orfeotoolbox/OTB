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

/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

// This piece of javascript is used to display the versions widget at the lower
// left corner of the html CookBook for OTB

window.addEventListener('load', function() {
    var request = new XMLHttpRequest();

    request.onreadystatechange = function () {
        if (request.readyState === XMLHttpRequest.DONE) {
            if (request.status == 200) {

                // Display the html div received with the AJAX request
                document.body.insertAdjacentHTML('beforeend', request.responseText);

                // Element showing version number in the header (made by sphinx)
                var e = document.querySelector(".version");

                // Element showing version number in the bottom left (the one we want)
                var y = document.querySelector("#current-version-text");

                if (e && y) {
                    // Swap them
                    y.innerHTML = "v: " + e.innerHTML;
                    e.remove();
                }

            } else {
                console.log("Request for versions.html failed with HTTP ", request.status);
            }
        }
    };

    var versions_div_url = "_static/html/versions.html";

    // If we are hosted on OTB website, get the latest version of the widget from
    // develop so that past releases can show links to future ones
    if (window.location.hostname == "www.orfeo-toolbox.org") {
        versions_div_url = "https://www.orfeo-toolbox.org/CookBook-develop/_static/html/versions.html";
    }

    request.open('GET', versions_div_url);
    request.send();
});

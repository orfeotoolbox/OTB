
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

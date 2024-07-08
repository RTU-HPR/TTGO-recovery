#ifndef _WEBPAGE_H
#define _WEBPAGE_H
#include <Arduino.h>
const PROGMEM char* webPage = 
"<!DOCTYPE html>\n"
"<html>\n"
"  <head>\n"
"    <title>TTGO Recovery</title>\n"
"    <style>\n"
"      *,\n"
"      *:before,\n"
"      *:after {\n"
"        box-sizing: border-box;\n"
"      }\n"
"\n"
"      body {\n"
"        font-family: Arial, sans-serif;\n"
"        margin: 0;\n"
"        padding: 0;\n"
"        display: flex;\n"
"        justify-content: center;\n"
"        align-items: center;\n"
"        height: 100vh;\n"
"        background-color: #333;\n"
"        color: #fff;\n"
"      }\n"
"\n"
"      #settings {\n"
"        background-color: #444;\n"
"        padding: 30px;\n"
"        border-radius: 8px;\n"
"        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);\n"
"        display: flex;\n"
"        flex-direction: column;\n"
"        align-items: center;\n"
"      }\n"
"\n"
"      label {\n"
"        font-weight: bold;\n"
"        font-size: 1.2em;\n"
"      }\n"
"\n"
"      input,\n"
"      select {\n"
"        width: 100%;\n"
"        padding: 15px;\n"
"        margin: 10px 0 20px;\n"
"        border-radius: 4px;\n"
"        border: 1px solid #ccc;\n"
"        font-size: 1.1em;\n"
"        color: #fff;\n"
"        background-color: #555;\n"
"      }\n"
"\n"
"      button {\n"
"        background-color: #4caf50;\n"
"        color: white;\n"
"        padding: 15px;\n"
"        margin: 10px 0;\n"
"        border: none;\n"
"        cursor: pointer;\n"
"        font-size: 1.1em;\n"
"        border-radius: 4px;\n"
"      }\n"
"\n"
"      button:hover {\n"
"        background-color: #45a049;\n"
"      }\n"
"\n"
"      @media (max-width: 600px) {\n"
"        #settings {\n"
"          width: 100%;\n"
"          font-size: 2em;\n"
"        }\n"
"      }\n"
"    </style>\n"
"    <script>\n"
"      function createOptions(id, options) {\n"
"        var select = document.getElementById(id);\n"
"        for (var i = 0; i < options.length; i++) {\n"
"          var option = document.createElement(\"option\");\n"
"          option.value = options[i];\n"
"          option.text = options[i];\n"
"          select.appendChild(option);\n"
"        }\n"
"      }\n"
"\n"
"      function createBWOptions(id, options) {\n"
"        var select = document.getElementById(id);\n"
"        for (var i = 0; i < options.length; i++) {\n"
"          var option = document.createElement(\"option\");\n"
"          option.value = options[i];\n"
"          option.text = options[i];\n"
"          select.appendChild(option);\n"
"        }\n"
"      }\n"
"\n"
"      function sendPostRequest(url, value) {\n"
"        // Use the fetch function to send a POST request\n"
"        fetch(url, {\n"
"          method: \"POST\",\n"
"          headers: {\n"
"            \"Content-Type\": \"text/htnml charset=UTF-8\",\n"
"          },\n"
"          body: value,\n"
"        });\n"
"      }\n"
"      window.onload = function () {\n"
"        var spreadingFactors = [6, 7, 8, 9, 10, 11, 12]; // Replace with your actual spreading factors\n"
"        var bandwidth = [\n"
"          \"10\",\n"
"          \"15\",\n"
"          \"20\",\n"
"          \"31\",\n"
"          \"41\",\n"
"          \"62\",\n"
"          \"125\",\n"
"          \"250\",\n"
"          \"500\",\n"
"        ]; // Replace with your actual bandwidths\n"
"        var displayToRealBW = {\n"
"          \"10\": \"10.4\",\n"
"          \"15\": \"15.6\",\n"
"          \"20\": \"20.8\",\n"
"          \"31\": \"31.25\",\n"
"          \"41\": \"41.7\",\n"
"          \"62\": \"62.5\",\n"
"          \"125\": \"125.0\",\n"
"          \"250\": \"250.0\",\n"
"          \"500\": \"500.0\",\n"
"        }; // Replace with your actual bandwidths\n"
"\n"
"        var realToDisplayBW = {\n"
"          \"10.40\": \"10\",\n"
"          \"15.60\": \"15\",\n"
"          \"20.80\": \"20\",\n"
"          \"31.25\": \"31\",\n"
"          \"41.70\": \"41\",\n"
"          \"62.50\": \"62\",\n"
"          \"125.00\": \"125\",\n"
"          \"250.00\": \"250\",\n"
"          \"500.00\": \"500\"\n"
"        };\n"
"        var codingRates = [\"4/5\", \"4/6\", \"4/7\", \"4/8\"]; // Replace with your actual coding rates\n"
"\n"
"        createOptions(\"spreadingFactor\", spreadingFactors);\n"
"        createOptions(\"bandwidth\", bandwidth);\n"
"        createOptions(\"codingRate\", codingRates);\n"
"\n"
"        var button = document.getElementById(\"submit-btn\");\n"
"\n"
"        // Fetch the initial values\n"
"        Promise.all([\n"
"          fetch(\"/frequency\")\n"
"            .then((response) => response.text())\n"
"            .then(\n"
"              (value) => (document.getElementById(\"frequency\").value = value)\n"
"            ),\n"
"          fetch(\"/spreadingFactor\")\n"
"            .then((response) => response.text())\n"
"            .then(\n"
"              (value) =>\n"
"                (document.getElementById(\"spreadingFactor\").value = value)\n"
"            ),\n"
"          fetch(\"/bandwidth\")\n"
"            .then((response) => response.text())\n"
"            .then(\n"
"              (value) => (document.getElementById(\"bandwidth\").value = realToDisplayBW[value])\n"
"            ),\n"
"          fetch(\"/codingRate\")\n"
"            .then((response) => response.text())\n"
"            .then(\n"
"              (value) => (document.getElementById(\"codingRate\").value = value)\n"
"            ),\n"
"        ]).catch((error) => console.error(\"Error:\", error));\n"
"\n"
"        // Add an event listener for the click event\n"
"        button.addEventListener(\"click\", function (event) {\n"
"          // Prevent the button from submitting a form\n"
"          event.preventDefault();\n"
"          // Send a POST request for each input field\n"
"          Promise.all([\n"
"            sendPostRequest(\n"
"              \"/frequency\",\n"
"              document.getElementById(\"frequency\").value\n"
"            ),\n"
"            sendPostRequest(\n"
"              \"/spreadingFactor\",\n"
"              document.getElementById(\"spreadingFactor\").value\n"
"            ),\n"
"            sendPostRequest(\n"
"              \"/bandwidth\",\n"
"              displayToRealBW[document.getElementById(\"bandwidth\").value]\n"
"            ),\n"
"            sendPostRequest(\n"
"              \"/codingRate\",\n"
"              document.getElementById(\"codingRate\").value.slice(-1)\n"
"            ), // Extract the last digit\n"
"          ]).then(function () {\n"
"            // All fetch requests have completed\n"
"            // You can now do something else\n"
"          });\n"
"        });\n"
"      };\n"
"    </script>\n"
"  </head>\n"
"\n"
"  <body>\n"
"    <div id=\"settings\">\n"
"      <h1>RTU HPR Recovery</h1>\n"
"      <!-- Add this line to insert a title -->\n"
"      <label for=\"frequency\">Frequency:</label><br />\n"
"      <input\n"
"        type=\"number\"\n"
"        id=\"frequency\"\n"
"        name=\"frequency\"\n"
"        step=\"0.001\"\n"
"        value=\"433.000\"\n"
"      /><br />\n"
"      <label for=\"spreadingFactor\">Spreading Factor:</label><br />\n"
"      <select id=\"spreadingFactor\" name=\"spreadingFactor\"></select\n"
"      ><br />\n"
"      <label for=\"bandwidth\">Bandwidth:</label><br />\n"
"      <select id=\"bandwidth\" name=\"bandwidth\"></select\n"
"      ><br />\n"
"      <label for=\"codingRate\">Coding Rate:</label><br />\n"
"      <select id=\"codingRate\" name=\"codingRate\"></select\n"
"      ><br />\n"
"      <button id=\"submit-btn\">Apply Settings</button>\n"
"    </div>\n"
"  </body>\n"
"</html>\n";
#endif // _WEBPAGE_H

#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>

const char* ssid = "ahmed";
const char* password = "12345678";

WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to Wi-Fi...");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Mount SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("SPIFFS mounted successfully");

  // Serve background image
  server.serveStatic("/bg.gif", SPIFFS, "/bg.gif");

  // Main HTML page
  server.on("/", HTTP_GET, []() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Car Remote Control</title>
      <style>
        body {
          background-image: url("/bg.gif");
          display: flex;
          justify-content: center;
          align-items: center;
          height: 100vh;
          margin: 0;
          font-family: Arial, sans-serif;
          background-position: center;
          background-repeat: no-repeat;
          background-size: cover;
        }

        .remote {
          background-color: rgba(0, 0, 0, 0.6);
          padding: 20px;
          border-radius: 20px;
          box-shadow: 0 0 20px rgba(0,0,0,0.7);
          text-align: center;
        }

        .grid {
          display: grid;
          grid-template-columns: repeat(3, 1fr);
          gap: 10px;
          margin: 20px;
        }

        .button {
          background: linear-gradient(135deg, #00c6ff, #0072ff);
          color: white;
          border: none;
          padding: 15px 20px;
          border-radius: 15px;
          font-size: 20px;
          font-weight: bold;
          box-shadow: 0 4px 6px rgba(0,0,0,0.3);
          cursor: pointer;
          transition: transform 0.2s ease, box-shadow 0.2s ease;
        }

        .button:hover {
          transform: scale(1.05);
          box-shadow: 0 6px 12px rgba(0,0,0,0.4);
        }

        .stop-btn {
          grid-column: 2 / 3;
          background: #ff1900;
        }

        .lights {
          margin-top: 20px;
          display: flex;
          justify-content: space-around;
        }

        .moo {
          margin-top: 50px;
          display: flex;
          justify-content: space-around;
        }

        .light-btn {
          padding: 10px 15px;
          background: #2ecc71;
        }

        .light-btn.off {
          background: red;
        }

        .btn-size {
          font-size: 30px;
        }

        h1 {
          color: white;
        }
      </style>
    </head>
    <body>
      <div class="remote">
        <h1>Car Remote Control</h1>

        <div class="grid">
          <button class="button btn-size" onclick="fetch('/backwardright')">↖</button>
          <button class="button" onclick="fetch('/forward')">▲</button>
          <button class="button btn-size" onclick="fetch('/forwardleft')">↗</button>
          
          <button class="button" onclick="fetch('/left')">◀</button>
          <button class="button stop-btn" onclick="fetch('/stop')">⏹</button>
          <button class="button" onclick="fetch('/right')">▶</button>
          
          <button class="button btn-size" onclick="fetch('/forwardright')">↙</button>
          <button class="button" onclick="fetch('/backward')">▼</button>
          <button class="button btn-size" onclick="fetch('/backwardleft')">↘</button>
        </div>

        <div class="moo">
          <button class="button" onclick="fetch('/backlighton')">Back Lights ON</button>
          <button class="button" onclick="fetch('/frontlighton')">Front Lights ON</button>
        </div>

        <div class="lights">
          <button class="button light-btn off" onclick="fetch('/frontlightoff')">Front Lights OFF</button>
          <button class="button light-btn off" onclick="fetch('/backlightoff')">Back Lights OFF</button>
        </div>
      </div>
    </body>
    </html>
    )rawliteral";
    server.send(200, "text/html", html);
  });

  // Control routes
  server.on("/forward", HTTP_GET, []() { sendCommand("F"); });
  server.on("/backward", HTTP_GET, []() { sendCommand("B"); });
  server.on("/left", HTTP_GET, []() { sendCommand("L"); });
  server.on("/right", HTTP_GET, []() { sendCommand("R"); });
  server.on("/stop", HTTP_GET, []() { sendCommand("S"); });

  server.on("/forwardleft", HTTP_GET, []() { sendCommand("FL"); });
  server.on("/forwardright", HTTP_GET, []() { sendCommand("FR"); });
  server.on("/backwardleft", HTTP_GET, []() { sendCommand("BL"); });
  server.on("/backwardright", HTTP_GET, []() { sendCommand("BR"); });

  server.on("/frontlighton", HTTP_GET, []() { sendCommand("FO"); });
  server.on("/frontlightoff", HTTP_GET, []() { sendCommand("FF"); });
  server.on("/backlighton", HTTP_GET, []() { sendCommand("BO"); });
  server.on("/backlightoff", HTTP_GET, []() { sendCommand("BF"); });

  server.begin();
  Serial.println("HTTP server started");
}

void sendCommand(String command) {
  Serial.println(command);
  server.send(200, "text/plain", "Command: " + command + " sent");
}

void loop() {
  server.handleClient();
}

#include <WiFiNINA.h>
#include <Stepper.h>

// Setup für Motor 1 (an D8, D9, D10, D11)
const int stepsPerRevolution = 2048; // Schritte pro Umdrehung für 28BYJ-48
Stepper motor1(stepsPerRevolution, 8, 10, 9, 11); // Motor 1 Pins

// Setup für Motor 2 (an D3, D4, D5, D6)
Stepper motor2(stepsPerRevolution, 3, 5, 4, 6); // Motor 2 Pins

// WiFi-Verbindung
char ssid[] = "insert_wlan_name";       // WLAN-Name
char pass[] = "insert_wlan_password";   // WLAN-Passwort
WiFiServer server(80);          // HTTP-Server

void setup() {
  Serial.begin(9600);

  // WLAN-Verbindung
  if (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Verbindung zum WLAN fehlgeschlagen.");
    while (true);
  }
  Serial.println("Verbunden mit dem WLAN!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // Motor-Geschwindigkeit festlegen
  motor1.setSpeed(15);
  motor2.setSpeed(15);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n' && request.indexOf("GET") != -1) break;
      }
    }

    // HTML-Seite senden
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>Zwei Schrittmotoren Steuerung</title>");
    client.println("<style>");
    client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; margin: 0; padding: 20px; }");
    client.println(".container { max-width: 500px; margin: auto; padding: 20px; background: #fff; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }");
    client.println("input[type=number] { width: 100px; padding: 5px; margin: 10px; border-radius: 5px; border: 1px solid #ccc; }");
    client.println("select { width: 150px; padding: 5px; margin: 10px; border-radius: 5px; border: 1px solid #ccc; }");
    client.println("button { background-color: #28a745; color: white; border: none; padding: 10px 20px; border-radius: 5px; cursor: pointer; }");
    client.println("button:hover { background-color: #218838; }");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<div class='container'>");

    // Motor 1 Steuerung
    client.println("<h1>Motor 1 Steuerung</h1>");
    client.println("<form action='/' method='GET'>");
    client.println("<label>Schritte:</label>");
    client.println("<input type='number' name='steps1' value='0' min='1'>");
    client.println("<br>");
    client.println("<label>Richtung:</label>");
    client.println("<select name='direction1'>");
    client.println("<option value='forward'>Vorwärts</option>");
    client.println("<option value='backward'>Rückwärts</option>");
    client.println("</select>");
    client.println("<br><br>");
    client.println("<button type='submit'>Motor 1 bewegen</button>");
    client.println("</form>");

    // Motor 2 Steuerung
    client.println("<h1>Motor 2 Steuerung</h1>");
    client.println("<form action='/' method='GET'>");
    client.println("<label>Schritte:</label>");
    client.println("<input type='number' name='steps2' value='0' min='1'>");
    client.println("<br>");
    client.println("<label>Richtung:</label>");
    client.println("<select name='direction2'>");
    client.println("<option value='forward'>Vorwärts</option>");
    client.println("<option value='backward'>Rückwärts</option>");
    client.println("</select>");
    client.println("<br><br>");
    client.println("<button type='submit'>Motor 2 bewegen</button>");
    client.println("</form>");

    client.println("</div>");
    client.println("</body>");
    client.println("</html>");

    // Steuerung Motor 1
    if (request.indexOf("GET /?steps1=") != -1) {
      int stepsIndex1 = request.indexOf("steps1=") + 7;
      int ampIndex1 = request.indexOf("&", stepsIndex1);
      String stepsValue1 = request.substring(stepsIndex1, ampIndex1);

      int directionIndex1 = request.indexOf("direction1=") + 11;
      String directionValue1 = request.substring(directionIndex1);

      int steps1 = stepsValue1.toInt();
      if (steps1 > 0) {
        if (directionValue1.startsWith("forward")) {
          Serial.print("Motor 1 vorwärts um ");
          Serial.print(steps1);
          Serial.println(" Schritte.");
          motor1.step(steps1);
        } else if (directionValue1.startsWith("backward")) {
          Serial.print("Motor 1 rückwärts um ");
          Serial.print(steps1);
          Serial.println(" Schritte.");
          motor1.step(-steps1);
        }
      }
    }

    // Steuerung Motor 2
    if (request.indexOf("GET /?steps2=") != -1) {
      int stepsIndex2 = request.indexOf("steps2=") + 7;
      int ampIndex2 = request.indexOf("&", stepsIndex2);
      String stepsValue2 = request.substring(stepsIndex2, ampIndex2);

      int directionIndex2 = request.indexOf("direction2=") + 11;
      String directionValue2 = request.substring(directionIndex2);

      int steps2 = stepsValue2.toInt();
      if (steps2 > 0) {
        if (directionValue2.startsWith("forward")) {
          Serial.print("Motor 2 vorwärts um ");
          Serial.print(steps2);
          Serial.println(" Schritte.");
          motor2.step(steps2);
        } else if (directionValue2.startsWith("backward")) {
          Serial.print("Motor 2 rückwärts um ");
          Serial.print(steps2);
          Serial.println(" Schritte.");
          motor2.step(-steps2);
        }
      }
    }

    delay(1);
    client.stop();
  }
}

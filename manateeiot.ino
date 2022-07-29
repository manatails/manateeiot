#include <ESP8266WiFi.h>
#include <Wire.h>

const char* ssid = "ManaNET";
const char* password = "manatee12345";
WiFiServer server(80);

char printBuffer[32];

int test_value = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Init");

  sprintf(printBuffer, "Connecting to %s", ssid);
  printBoth(printBuffer);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  printBoth("");
  printBoth("Wi-Fi connected.");

  // Start the HTTP server
  server.begin();
  printBoth("HTTP Server started.");
 
  // Print the IP address
  IPAddress ip = WiFi.localIP();
  sprintf(printBuffer, "IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  printBoth(printBuffer);
  
  delay(1000);
  
  printBoth("Init complete");
}

void printBoth(char *message) {
  Serial.println(message);
  //displayPrint(message);
}

void displayPrint(char *message) {
  //Use this if you have a display for your project
  
  //display.clearDisplay();
  //display.setTextColor(WHITE);
  //display.setTextSize(2);
  //display.setCursor(0,0);
  //display.println(message);
  //display.display();
}

void loop() {

  //add your code here

  
  // --- HTTP SERVER ---
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }

  client.setTimeout(1000);

  IPAddress cip = client.remoteIP();
  sprintf(printBuffer, "HTTP: Connection from %d.%d.%d.%d", cip[0], cip[1], cip[2], cip[3]);
  printBoth(printBuffer);
  // Wait until the client sends some data
  while(!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  if (request.indexOf("GET / HTTP") != -1)  
  {
    Http200(client);
    MainPage(client);
  }
  else if (request.indexOf("GET /TOGGLE HTTP") != -1)  
  {
    printBoth("COMMAND: TOGGLE");
    test_value = !test_value;

    Http303(client);
  }
  else if (request.indexOf("GET /ON HTTP") != -1)  
  {
    printBoth("COMMAND: ON");
    test_value = 1;

    Http303(client);
  }
  else if (request.indexOf("GET /OFF HTTP") != -1)  
  {
    printBoth("COMMAND: OFF");
    test_value = 0;

    Http303(client);
  }
  else if (request.indexOf("GET") != -1)  
  {
    Http404(client);
    NotFoundPage(client);
  }
  else 
  {
    Http405(client);
    BadRequestPage(client);
  }
 
  Serial.println("HTTP: Connection closed");
  Serial.println("");
}

void Http200(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Server: ManateeIOT");
  
  client.println(""); // newline is necessary
}

void Http303(WiFiClient client)
{
  client.println("HTTP/1.1 303 See Other");
  client.println("Location: /");
  client.println("Server: ManateeIOT");
  
  client.println(""); // newline is necessary
}

void Http400(WiFiClient client)
{
  client.println("HTTP/1.1 400 Bad Request");
  client.println("Content-Type: text/html");
  client.println("Server: ManateeIOT");
  
  client.println(""); // newline is necessary
}

void Http404(WiFiClient client)
{
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-Type: text/html");
  client.println("Server: ManateeIOT");
  
  client.println(""); // newline is necessary
}

void Http405(WiFiClient client)
{
  client.println("HTTP/1.1 405 Method Not Allowed");
  client.println("Content-Type: text/html");
  client.println("Allow: GET");
  client.println("Server: ManateeIOT");
  
  client.println(""); // newline is necessary
}

void MainPage(WiFiClient client)
{
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><title>ManateeIOT</title></head>");
  client.println("<body>");
  client.println("<h1>ManateeIOT</h1>");
  client.print("test_value is now: ");
  client.print(test_value);
  client.println("<br><br>");
  client.println("<a href=\"/TOGGLE\">TOGGLE</a><br>");
  client.println("<a href=\"/ON\">ON</a><br>");
  client.println("<a href=\"/OFF\">OFF</a><br>");
  client.println("</body>");
  client.println("</html>");
}

void NotFoundPage(WiFiClient client)
{
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><title>ManateeIOT</title></head>");
  client.println("<body>");
  client.println("Not Found");
  client.println("</body>");
  client.println("</html>");
}

void BadRequestPage(WiFiClient client)
{
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><title>ManateeIOT</title></head>");
  client.println("<body>");
  client.println("Bad Request");
  client.println("</body>");
  client.println("</html>");
}

#include <FastLED.h>
#include <WiFi.h>

// Network Credentials
const char* ssid     = "username";
const char* password = "password";

WiFiServer server(80); // Port

// Client variables 
char linebuf[80];
int charcount=0;

// LED Light Strip Config
#define LED_PIN     12   // which pin your pixels are connected to

bool ledon = false;

#define ARDUINO_RUNNING_CORE 1


void setup() 
{
  delay( 2000 ); // power-up safety delay

  pinMode (LED_PIN, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while(!Serial) 
  {
  }

  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while(WiFi.status() != WL_CONNECTED) 
  {
    // Connect to WPA/WPA2 network.
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  server.begin();

  xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}


// Web Server
void loop() 
{  
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        //Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
 
        if (c == '\n' && currentLineIsBlank) 
        {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h1>Backlit Greys</h1>");
          client.println("<p>Lights <a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') 
        {
          // you're starting a new line
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /on") > 0)
          { 
            Serial.println("LED ON");
            ledon = true;
          }
          else if (strstr(linebuf,"GET /off") > 0)
          {
            Serial.println("LED OFF");
            ledon = false;
          }
 
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } 
        else if (c != '\r') 
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    
    // give the web browser time to receive the data
    delay(1);
 
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


// Task executed by Web Server
void loop1(void *pvParameters) 
{
  while(1)
  {
   if(ledon == true)
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
    }
     delay(300); 
  }
  
}




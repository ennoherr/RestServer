#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 20);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 2, 1);
IPAddress dns(8, 8, 8, 8);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
int milli;
float a0 = 0.0;
float a1 = 0.0;
float a2 = 0.0;
float a3 = 0.0;
float a4 = 0.0;
float a5 = 0.0;

// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void)
{
	// Start Serial
	Serial.begin(9600);

	// Init variables and expose them to REST API
	//milli = millis();
	//analog0 = analogRead(0);
	//rest.variable("milli", &milli);
	rest.variable("a0", &a0);
	rest.variable("a1", &a1);
	rest.variable("a2", &a2);
	rest.variable("a3", &a3);
	rest.variable("a4", &a4);
	rest.variable("a5", &a5);

	// Function to be exposed
	//rest.function("led", ledControl);

	// Give name & ID to the device (ID should be 6 characters long)
	rest.set_id("008");
	rest.set_name("dapper_drake");

	// Start the Ethernet connection and the server
	//if (Ethernet.begin(mac) == 0) {
	//  Serial.println("Failed to configure Ethernet using DHCP");
	// no point in carrying on, so do nothing forevermore:
	// try to congifure using IP address instead of DHCP:
	Ethernet.begin(mac, ip);
	//}
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());

	// Start watchdog
	wdt_enable(WDTO_4S);
}

void loop() {

	//milli = millis();
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	a0 = analogRead(0) * (5.0 / 1023.0);
	a1 = analogRead(1) * (5.0 / 1023.0);
	a2 = analogRead(2) * (5.0 / 1023.0);
	a3 = analogRead(3) * (5.0 / 1023.0);
	a4 = analogRead(4) * (5.0 / 1023.0);
	a5 = analogRead(5) * (5.0 / 1023.0);

	// listen for incoming clients
	EthernetClient client = server.available();

	if (client && client.available()) {
		rest.handle(client);
		delay(1);
		client.stop();
	}

	wdt_reset();
}

// Custom function accessible by the API
int ledControl(String command) {

	// Get state from command
	int state = command.toInt();

	digitalWrite(6, state);
	return 1;

}

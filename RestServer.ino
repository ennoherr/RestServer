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

// Variables
bool isDhcp = false;
float a0 = 0.0;
float a1 = 0.0;
float a2 = 0.0;
float a3 = 0.0;
float a4 = 0.0;
float a5 = 0.0;

// Declare functions
int ledControl(String command);

void setup(void)
{
	Serial.begin(9600);
	while (!Serial) {
		;
	}

	// Init variables and expose them to REST API
	rest.variable("a0", &a0);
	rest.variable("a1", &a1);
	rest.variable("a2", &a2);
	rest.variable("a3", &a3);
	rest.variable("a4", &a4);
	rest.variable("a5", &a5);

	// Function to be exposed
	rest.function("led", ledControl);

	// Give name & ID to the device (ID should be 6 characters long)
	rest.set_id(String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX));
	rest.set_name("arduino1");

	// Start the Ethernet connection and the server
	if (isDhcp) {
		if (Ethernet.begin(mac) == 0) {
			Serial.println("Failed to configure Ethernet using DHCP");
			while (true) {
				;
			}
		}
	}
	else {
		Ethernet.begin(mac, ip, dns, gateway, subnet);
	}

	server.begin();
	Serial.println("---------");
	Serial.print("IP: ");
	Serial.println(Ethernet.localIP());
	Serial.print("Subnet: ");
	Serial.println(Ethernet.subnetMask());
	Serial.print("Gateway: ");
	Serial.println(Ethernet.gatewayIP());
	Serial.print("DNS: ");
	Serial.println(Ethernet.dnsServerIP());
	Serial.println("---------");
	Serial.println("Server started...");

	// Start watchdog
	wdt_enable(WDTO_4S);
}

void loop() {

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
	return state;
}

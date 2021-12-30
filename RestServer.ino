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
//bool isDebug = false;

bool isDhcp = true;
//float a0 = 0.0;
//float a1 = 0.0;
//float a2 = 0.0;
//float a3 = 0.0;
//float a4 = 0.0;
//float a5 = 0.0;

//bool isLight = false;


// Declare functions
int ledControl(String command);
int ledOn(int pin);
int ledOff(int pin);
int ledOn(String pin);
int ledOff(String pin);

/*
void Debug(char* debug, bool newline = false) {
	if (isDebug) {
		if (newline) Serial.println(debug);
		else Serial.print(debug);
	}
}

void Debug(int debug, bool newline = false) {
	char *temp = new char[32];
	itoa(debug, temp, 10);
	Debug(temp, newline);
	delete[] temp;
}
*/

void setup(void)
{
	Serial.begin(9600);
	while (!Serial) {
		;
	}

	analogReference(INTERNAL);

	// Init variables and expose them to REST API
	//rest.variable("a0", &a0);
	//rest.variable("a1", &a1);
	//rest.variable("a2", &a2);
	//rest.variable("a3", &a3);
	//rest.variable("a4", &a4);
	//rest.variable("a5", &a5);

	// Function to be exposed
	rest.function("led", ledControl);
	rest.function("on", ledOn);
	rest.function("off", ledOff);

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

	//if (millis() % 1000 == 0) {
		//arefData(0);
		//bool isLight = (getAnalogRaw(0) > 500) ? false : true;

		//if (isLight) {
		//	ledOn(6);
		//} else {
		//	ledOff(6);
		//}
		//arefData(1);
		//arefData(3);
		//getTemp();
	//}

	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	//a0 = analogRead(0) * (5.0 / 1023.0);
	//a1 = analogRead(1) * (5.0 / 1023.0);
	//a2 = analogRead(2) * (5.0 / 1023.0);
	//a3 = analogRead(3) * (5.0 / 1023.0);
	//a4 = analogRead(4) * (5.0 / 1023.0);
	//a5 = analogRead(5) * (5.0 / 1023.0);

	// listen for incoming clients
	EthernetClient client = server.available();
	if (client && client.available()) {
		rest.handle(client);
		//ledOn(4);
		delay(1);
		client.stop();
		//ledOff(4);
	}

	wdt_reset();
}

// Custom function accessible by the API
int ledControl(String command) {
	int pin = 6;
	// Get state from command
	int state = command.toInt();
	Serial.print("ledControl - command = ");
	Serial.println(state);
	pinMode(pin, OUTPUT);
	digitalWrite(pin, state);
	return state;
}

int ledOn(String pin) {
	return ledOn(pin.toInt());
}

int ledOn(int pin) {
	Serial.print("ledOn - pin = ");
	Serial.println(pin);

	pinMode(pin, OUTPUT);
	digitalWrite(pin, 1);
	return 1;
}

int ledOff(String pin) {
	return ledOff(pin.toInt());
}

int ledOff(int pin) {
	Serial.print("ledOff - pin = ");
	Serial.println(pin, true);

	pinMode(pin, OUTPUT);
	digitalWrite(pin, 0);
	return 0;
}

float getTemp() {

	int pin = 0;
	float R1 = 10000;
	float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

	int Vo = analogRead(pin);
	float R2 = R1 * (1023.0 / (float)Vo - 1.0);
	float logR2 = log(R2);
	float T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
	float Tc = T - 273.15;
	float Tf = (Tc * 9.0) / 5.0 + 32.0;

	Serial.print("Temperature: ");
	Serial.print(Tf);
	Serial.print(" F; ");
	Serial.print(Tc);
	Serial.println(" C");

	return Tc;
}

int getAnalogRaw(int pin) {
	int a = analogRead(pin);

	Serial.print("Pin A");
	Serial.print(pin);
	Serial.print(" = ");
	Serial.println(a);

	return a;
}

void arefData(int pin) {
	int analogamount = analogRead(pin);
	float percentage = (analogamount / 1024.00) * 100;
	float voltage = analogamount * 3.222; // in millivolts

	Serial.print("% of AREF: ");
	Serial.print(percentage, 2);
	Serial.print(" ");
	Serial.print("A");
	Serial.print(pin);
	Serial.print(" (mV) : ");
	Serial.println(voltage, 2);
}
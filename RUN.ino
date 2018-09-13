#include <Ethernet.h>

IPAddress ip(10, 8, 4, 100);
IPAddress server(10, 8, 8, 10);

unsigned int localPort = 11213;
EthernetClient client;
int buttonOnPin = 2;
int buttonOffPin = 3;

int LEDOnPin = 5;
int LEDOffPin = 6;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
byte dns1[] = {
  10, 0, 0, 10
};

byte gw[] = {
  10, 8, 0, 1
};

byte sn[] = {
  255, 255, 0, 0
};

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,


int prevButtonOnState = LOW;
int prevButtonOffState = LOW;

long blockTimeBegin = 0;
int blockTime = 4000;

void setup() {
  Ethernet.begin(mac, ip, dns1, gw, sn);
  Serial.begin(9600);
  delay(1000);
  pinMode(13, OUTPUT);

  pinMode(buttonOnPin, INPUT_PULLUP);
  pinMode(buttonOffPin, INPUT_PULLUP);
  pinMode(LEDOnPin, OUTPUT);
  pinMode(LEDOffPin, OUTPUT);

  prevButtonOnState  = digitalRead(buttonOnPin);
  prevButtonOffState = digitalRead(buttonOffPin);
  digitalWrite(LEDOnPin, LOW);
  digitalWrite(LEDOffPin, LOW);
}


void loop() {
  long curTime = millis();
  if ((curTime -  blockTimeBegin ) < blockTime) {
    digitalWrite(LEDOnPin, HIGH);
    digitalWrite(LEDOffPin, HIGH);
    digitalWrite(13,LOW);
    //Serial.println("block");
    return;
  }
  else {
    digitalWrite(LEDOnPin, LOW);
    digitalWrite(LEDOffPin, LOW);
    digitalWrite(13,HIGH);
    //Serial.println("unblock");
    }

  int buttonOnState  = digitalRead(buttonOnPin);
  int buttonOffState = digitalRead(buttonOffPin);

  Serial.print(buttonOnState);
  Serial.print(" ");
  Serial.println(buttonOffState);


  if (buttonOnState == LOW && prevButtonOnState == HIGH) {
    blockTimeBegin = curTime;
    sendEvent("on");
    Serial.println("on");
    prevButtonOffState = buttonOffState;
    prevButtonOnState = buttonOnState;
    return;
  }

if (buttonOffState == LOW && prevButtonOffState == HIGH) {
    blockTimeBegin = curTime;
    sendEvent("off");
    Serial.println("off");
 }

  prevButtonOffState = buttonOffState;
  prevButtonOnState = buttonOnState;
}


void sendEvent(const char* str) {
  int res = client.connect(server, localPort);
  if (res) {
    Serial.println(1);
    client.print("starter:");
    client.println(str);
    client.stop();
  } else {
    Serial.println("connection failed");
    Serial.println(res);
  }

}


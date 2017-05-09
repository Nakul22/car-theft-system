#include <SoftwareSerial.h>
SoftwareSerial SIM900(11, 12);

int read_count = 0;
int i = 0, j = 0;
int ReceiveCheck = 0, NoCheck = 0, SensorCheck = 0, SmsCheck = 1;
int SensorPin = 8;
int Status = 0;

char data_temp, RFID_data[12];
char card_code[12] = {'1', '6', '0', '0', '6', '5', 'D', 'F', '6', '4', 'C', '8'};

void setup()
{
  SIM900.begin(19200);
  delay(100);
  Serial.begin(9600);
  pinMode(SensorPin, INPUT);
  pinMode(13, OUTPUT);
  pinMode(23,OUTPUT);
  digitalWrite(23,LOW);
}

void loop()
{
  RecieveData();
  CheckData();
  RunStatus();
}

void RecieveData()
{
  if (Serial.available() > 0)
  {
    RFID_data[read_count] = Serial.read();
    read_count++;
    ReceiveCheck = 1;
  }
}

void CheckData()
{
  if (read_count == 12)
  {
    read_count = 0;
    Serial.println();
    for (i = 0; i < 12; i++)
    {
      Serial.print(RFID_data[i]);
      if (RFID_data[i] != card_code[i])
      {
        NoCheck = 0;
      }
      else
      {
        NoCheck = 1;
      }
    }
    if (Status == 1)
    {
      Status = 0;
      digitalWrite(13, LOW);
      delay(1);
    }
    else if (Status == 0)
    {
      Status = 1;
      SmsCheck=0;
      digitalWrite(13, HIGH);
      delay(1);
    }
  }
}

void CheckSensor()
{
  if (digitalRead(SensorPin) == HIGH)
  {
    SensorCheck = 1;
    
  }
  else if (digitalRead(SensorPin) == LOW)
  {
    SensorCheck = 0;
  }
}

void RunStatus()
{
  if (Status == 0)
  {
    delay(1);
  }
  else if (Status == 1)
  {
    CheckSensor();
    if (SensorCheck == 1)
    {
      SendSms();
      delay(1);
    }
    else if (SensorCheck == 0)
    {
      delay(1);
    }
  }
}

void SendSms()
{
  if (SmsCheck == 0)
  {
       SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
       delay(100);
       SIM900.println("AT + CMGS = \"+919873717459\"");                                     // recipient's mobile number, in international format
       delay(100);
       SIM900.println("Car Theft Attempt Detected.160065DF64C8");        // message to send
       delay(100);
       SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
       delay(100);      ..
       SIM900.println();
       delay(5000);
       Serial.println("k");
    SmsCheck = 1;
  }
}

#include "FPS_GT511C3.h" //Get library from https://github.com/sparkfun/Fingerprint_Scanner-TTL 
#include "SoftwareSerial.h" //Software serial library

FPS_GT511C3 fps(4, 5); //FPS connected to D4 and D5

const int buttonPin = 2;
int led = 13;
int buttonState = 0;

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Current time,User ID");
  delay(100);
  fps.Open();         //send serial command to initialize fps 
  fps.SetLED(true);   //turn on LED so fps can see fingerprint
}

void loop()
{ 
  if (digitalRead(buttonPin)==HIGH)//If button pressed 
  {
    Enroll(); //Enroll a fingerprint 
  } 
  else {
    digitalWrite(led, LOW);
      if (fps.IsPressFinger())
      {
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
//          Serial.print("Welcome:"); 
          if (id==200) {
//            Serial.println("Unknown  "); //If not recognised 
          }
          Serial.print("DATA,TIME,");
          Serial.println(int(id));
          delay(1000);
      } 
	  else {
        delay(100);
//      Serial.println("Press Finger to Scan : "); //Display hi when ready to scan 
//      Serial.print("Scan...");
      }
  }
}

void Enroll()
{
  // Enroll test
  digitalWrite(led, HIGH);
  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

//  Serial.print("Press finger to Enroll #");
  Serial.println(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    Serial.println("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    Serial.println("Press same finger again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      Serial.println("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      Serial.println("Press same finger yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        Serial.println("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          Serial.println("Enrolling Successful");
        }
        else
        {
          Serial.print("Enrolling Failed with error code:");
          Serial.println(iret);
        }
      }
      else Serial.println("Failed to capture third time");
    }
    else Serial.println("Failed to capture second time");
  }
  else Serial.println("Failed to capture first time");
}

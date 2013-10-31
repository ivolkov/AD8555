int outpin = 12;
int FSG = 0;
int SSG = 0;
int OFS = 0;
int mode = 0;

//#define DBG

void setup() {
  Serial.begin(9600);
  pinMode(outpin, OUTPUT);
  digitalWrite(outpin, LOW);
}

void loop() {
  Serial.println("\n");
  
  Serial.print("Enter Second Stage Gain Code: ");
  if (!getSSG())
    return;
  Serial.println(SSG);  
  
  Serial.print("Enter First Stage Gain Code: ");
  if (!getFSG())
    return;
  Serial.println(FSG);
    
  Serial.print("Enter Offset Code: ");
  if (!getOFS())
    return;
  Serial.println(OFS);
  
  Serial.print("Enter mode: 0 - simulation, 1 - programming: ");
  if (!getMode())
    return;
  Serial.println(mode);
  
  if (mode == 0) {
    simulate();
    return;
  } else if (mode == 1) {
    if (!getConfirmation())
      return;
    program();
  }
}

bool getFSG()
{
  while (!Serial.available());
  
  FSG = Serial.parseInt();
  bool res = ((FSG >= 0) && (FSG <= 127));
  if (!res)
    Serial.println("Incorrect number. Valid range: 0..127");
    
  return res;
}

bool getSSG()
{
  while (!Serial.available());
  
  SSG = Serial.parseInt();
  bool res = ((SSG >= 0) && (SSG <= 7));
  if (!res)
    Serial.println("Incorrect number. Valid range: 0..7");
    
  return res;
}

bool getOFS()
{
  while (!Serial.available());
  
  OFS = Serial.parseInt();
  bool res = ((OFS >= 0) && (OFS <= 255));
  if (!res)
    Serial.println("Incorrect number. Valid range: 0..255");
    
  return res;
}

bool getMode()
{
  while (!Serial.available());
  
  mode = Serial.parseInt();
  bool res = ((mode == 0) || (mode == 1));
  if (!res)
    Serial.println("Incorrect number. Valid range: 0..1");
    
  return res;
}

bool getConfirmation()
{
  char c;
  bool res = false;
  
  Serial.println("Proceed? [Y/N]");
  while (!Serial.available());
  while (Serial.available()) {
    c = Serial.read();
    if ((c == 'y') || (c == 'Y'))
      res = true;
  }
  return res;
}

void simulate()
{
  sendField0();      // start packet
  sendField1(0b01);  //  simulate
  sendField2(0b00);  //  second stage gain
  sendField3();      // dummy
  sendField4(SSG);   // value 
  sendField5();      // stop packet
  
  sendField0();      // start packet
  sendField1(0b01);  //  simulate
  sendField2(0b01);  //  first stage gain
  sendField3();      // dummy
  sendField4(FSG);   // value
  sendField5();      // stop packet
  
  sendField0();      // start packet
  sendField1(0b01);  //  simulate
  sendField2(0b10);  //  output offset
  sendField3();      // dummy
  sendField4(OFS);   // value
  sendField5();      // stop packet
  
  Serial.println("Done!");
}

void program()
{
  unsigned char par;
  
  #ifdef DBG
  Serial.println("SSG");
  #endif
  
  for (int i = 0 ; i < 8 ; i++)
    if (((SSG >> i) & 1) == 1) {
      par = (1 << i);
      blowSSGFuse(par);
    }

  #ifdef DBG
  Serial.println("FSG");
  #endif
  for (int i = 0 ; i < 8 ; i++)
    if (((FSG >> i) & 1) == 1) {
      par = (1 << i);
      blowFSGFuse(par);
    }
    
  #ifdef DBG
  Serial.println("OFS");
  #endif
  for (int i = 0 ; i < 8 ; i++)
    if (((OFS >> i) &1) == 1) {
      par = (1 << i);
      blowOFSFuse(par);
    }
    
  #ifdef DBG
  Serial.println("MFS");
  #endif
  blowMSF();

  Serial.println("Done!");
}

void blowSSGFuse(unsigned char Value)
{
  sendField0();      // start packet
  sendField1(0b10);  // program
  sendField2(0b00);  // second stage gain
  sendField3();      // dummy
  sendField4(Value); // value
  sendField5();      // stop packet
  delay(1);
}

void blowFSGFuse(unsigned char Value)
{
  sendField0();      // start packet
  sendField1(0b10);  // program
  sendField2(0b01);  // second stage gain
  sendField3();      // dummy
  sendField4(Value); // value
  sendField5();      // stop packet
  delay(1);
}

void blowOFSFuse(unsigned char Value)
{
  sendField0();      // start packet
  sendField1(0b10);  // program
  sendField2(0b10);  // second stage gain
  sendField3();      // dummy
  sendField4(Value); // value
  sendField5();      // stop packet
  delay(1);
}

void blowMSF()
{
  sendField0();            // start packet
  sendField1(0b10);        // program
  sendField2(11);          // other functions
  sendField3();            // dummy
  sendField4(0b00000001);  // master fuse
  sendField5();            // stop packet
  delay(1);
}

void sendField0()
{
  sendBit(1);
  sendBit(0);
  sendBit(0);
  sendBit(0);
  
  sendBit(0);
  sendBit(0);
  sendBit(0);
  sendBit(0);
  
  sendBit(0);
  sendBit(0);
  sendBit(0);
  sendBit(1);
  
  #ifdef DBG
  Serial.print(' ');
  #endif
}

void sendField1(unsigned char value)
{
  sendBit((value >> 1) & 1);
  sendBit(value & 1);

  #ifdef DBG
  Serial.print(' ');
  #endif
}

void sendField2(unsigned char value)
{
  sendBit((value >> 1) & 1);  
  sendBit(value & 1);
  
  #ifdef DBG
  Serial.print(' ');
  #endif
}

void sendField3()
{
  sendBit(1);
  sendBit(0);
  
  #ifdef DBG
  Serial.print(' ');
  #endif  
}

void sendField4(unsigned char value)
{  
  sendBit((value >> 7) & 1);
  sendBit((value >> 6) & 1);
  sendBit((value >> 5) & 1);
  sendBit((value >> 4) & 1);
  sendBit((value >> 3) & 1);
  sendBit((value >> 2) & 1);
  sendBit((value >> 1) & 1);
  sendBit(value & 1);
  
  #ifdef DBG
  Serial.print(' ');
  #endif  
}

void sendField5()
{
  sendBit(0);
  sendBit(1);
  sendBit(1);
  sendBit(1);
  
  sendBit(1);
  sendBit(1);
  sendBit(1);
  sendBit(1);
  
  sendBit(1);
  sendBit(1);
  sendBit(1);
  sendBit(0);
  
  #ifdef DBG
  Serial.println();
  #endif  
}  
  
void sendBit(bool value)
{
  if (value)
    send1();
  else
    send0();
}

void send0()
{
  digitalWrite(outpin, HIGH);
  digitalWrite(outpin, LOW);
  delay(0);
  
  #ifdef DBG
  Serial.print('0');
  #endif
}

void send1()
{
  digitalWrite(outpin, HIGH);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  delay(0);
  digitalWrite(outpin, LOW);
  delay(0);

  #ifdef DBG
  Serial.print('1');
  #endif
}


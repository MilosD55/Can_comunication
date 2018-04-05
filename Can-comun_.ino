#include <mcp_can.h>
#include <SPI.h>
#include <string.h>


#define CAN_INT 2

MCP_CAN CAN0(10);

char msg[8];
char tmp[64];
char can_msg_tmp[16];
int i=0;
int j = 0;
int br=0;
int duzina;
char niz[8];
long unsigned int rxId;
unsigned char len = 0;
byte tempbyte = 0;
int inPin = 5;
int val=0;
unsigned char incByte;

char inData[80];
char flightdata[80];
char value,msg_length;
byte checksum = 0;
byte bytesread = 0;
char pniz[8];


void setup() {

  Serial.begin(115200);
  while(CAN0.begin(MCP_STDEXT, CAN_500KBPS, MCP_16MHZ) != CAN_OK){
    Serial.println("Error Initializing MCP2515");
  }
  pinMode(inPin,INPUT);

  CAN0.setMode(MCP_NORMAL);

     
}

void loop() {

  while(!Serial.available());
        
    delay(100);
   
    duzina=Serial.available();

    Serial.readBytes(tmp,duzina);

    Serial.println(tmp);

    for(i = 0; i < duzina; i++) {

       if(tmp[i] == 'p') {
        
        br++;
        
       } else {

        can_msg_tmp[j] = tmp[i];
        Serial.print("Karakter je: ");
        Serial.println(can_msg_tmp[j]);
        Serial.println(j);
        
       j++;
       }
      
    }

    
    Serial.print("Duzina poruke je: ");
    Serial.println(br);
   
    msg_length = ParseMessage(tmp, msg);
    
   
     CAN0.sendMsgBuf(0xFF,0,3,msg);
  
      Serial.println("message sent");

    }  


 int makeOutputValue(int br1, int br2) {
     int partOne = convertToHexValue(br1);
     int partTwo = convertToHexValue(br2);

  return partOne * 16 + partTwo;
}

int convertToHexValue(int br) {

  if(br / 10 == 3) {
    return br % 10;
  } else if(br / 10 == 4) {
    return (br % 10) + 9;
  }

}

int returnHex(char chr) {
  int chr_value = (int)chr;

  if(chr_value > 47 && chr_value < 58) return chr_value - 18;
  if(chr_value > 64 && chr_value < 71) return chr_value - 24;
  if(chr_value > 96 && chr_value < 103) return chr_value - 56;

}

int ParseMessage(char *string, int *msg) {
  int i;
  int j = 0;

  for(i = 0; i < strlen(string); i+=3) {
    msg[j] = makeOutputValue(returnHex(string[i]), returnHex(string[i+1]));
    j++;
  }

  return j;

}

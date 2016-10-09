#include <DSPI.h>
#include "mcp_can.h"

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_1000KBPS))              // init can bus : baudrate = 1000k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");

    /*
     * set mask, set both the mask to 0x3ff
     */
//    CAN.init_Mask(0, 0, 0x3ff);                         // there are 2 mask in mcp2515, you need to set both of them
//    CAN.init_Mask(1, 0, 0x3ff);
//
//
//    /*
//     * set filters
//     */
//    CAN.init_Filt(0, 0, 0x3ee);                          // there are 6 filters in mcp2515
//    CAN.init_Filt(1, 0, 0x3ee);                          // there are 6 filters in mcp2515
//
//    CAN.init_Filt(2, 0, 0x3ee);                          // there are 6 filters in mcp2515
//    CAN.init_Filt(3, 0, 0x3ee);                          // there are 6 filters in mcp2515
//    CAN.init_Filt(4, 0, 0x3ee);                          // there are 6 filters in mcp2515
//    CAN.init_Filt(5, 0, 0x3ee);                          // there are 6 filters in mcp2515


    attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
}

void MCP2515_ISR()
{
    flagRecv = 1;
}

void loop()
{
    if(flagRecv) 
    {                                   // check if get data

        flagRecv = 0;                   // clear flag

        // iterate over all pending messages
        // If either the bus is saturated or the MCU is busy,
        // both RX buffers may be in use and reading a single
        // message does not clear the IRQ conditon.
        while (CAN_MSGAVAIL == CAN.checkReceive()) 
        {
            // read data,  len: data length, buf: data buf
            CAN.readMsgBuf(&len, buf);


            Serial.print(String(CAN.getCanId(), HEX));
            Serial.print(":\t");
            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(String(buf[i], HEX));
                Serial.print("\t");
            }
            Serial.println();
        }
    }
    
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

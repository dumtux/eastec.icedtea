#include <Wire.h>
#include <string.h>

#include "font.h"
#include "images.h"

#define OLED_Write_Address 0x3C

#define ROT_D 2
#define ROT_A 4
#define ROT_B 6
#define ROT_C 5

#define ENC_PIN_A 14
#define ENC_PIN_B 15

#define BOOST  8
#define NFAULT 11
#define DRV_1  12
#define DRV_2  13

#define STEP 1


bool enc_a_raised = false;
bool enc_b_raised = false;

int value = 0;

void rotary_init() {
    pinMode(DRV_1, OUTPUT);
    pinMode(DRV_2, OUTPUT);
    pinMode(BOOST, OUTPUT);

    delay(20);

    digitalWrite(DRV_1, LOW);
    digitalWrite(DRV_2, LOW);
    digitalWrite(BOOST, HIGH);

    delay(20);

    pinMode(ROT_C, OUTPUT);
    digitalWrite(ROT_C, LOW);
    pinMode(ROT_A, INPUT_PULLUP);
    pinMode(ROT_B, INPUT_PULLUP);
    pinMode(ROT_D, INPUT_PULLUP);
    attachInterrupt(ROT_A, ISR_encode_a, CHANGE);
    attachInterrupt(ROT_D, ISR_encode_d, FALLING);
}

void ISR_encode_a(){
    if (digitalRead(ROT_A) == HIGH && digitalRead(ROT_B) == HIGH) {
        decrease_value();
    } else if (digitalRead(ROT_A) == HIGH && digitalRead(ROT_B) == LOW) {
        increase_value();
    } else if (digitalRead(ROT_A) == LOW && digitalRead(ROT_B) == HIGH) {
        increase_value();
    } else if (digitalRead(ROT_A) == LOW && digitalRead(ROT_B) == LOW) {
        decrease_value();
    }
}

void ISR_encode_d(){
    showValue(999);
}

void increase_value() {
    if (value < 256) {
        value += STEP;
    }
   analogWrite(DRV_2, value);
//    showValue(value);
}

void decrease_value() {
    if (value > 0) {
        value -= STEP;
    }
   analogWrite(DRV_2, value);
//    showValue(value);
}

void showValue(int v) {
    char myArray[20]="";
    int z = v;
    for (int i = 2; i>=0; i--) {
        myArray[i] = (z % 10) + 0x30;
        z = z / 10;
    }
    OLED_setXY(0x20, 0x7F, 0x02, 0x01);
    OLED_Data(myArray, 4, 0);
    OLED_setXY(0x20, 0x7F, 0x03, 0x01);
    OLED_Data(myArray, 4, 1);
    OLED_setXY(0x20, 0x7F, 0x04, 0x01);
    OLED_Data(myArray, 4, 2);
    OLED_setXY(0x20, 0x7F, 0x05, 0x01);
    OLED_Data(myArray, 4, 3);
}

void OLED_Data(char *DATA, int m, int n) /* Function for sending data to OLED */
{
    int len = strlen(DATA);
    for (int g=0; g<len; g++)
    {
        for (int index=F32_DLEN/m*n; index<F32_DLEN/m*(n+1); index++)
        {
            Wire.beginTransmission(OLED_Write_Address); /* Begin transmission to slave device */
        /* Queue data to be transmitted */
            Wire.write(0x40); /* For Data Transmission, C = 0 and D/C = 1 */
            Wire.write(F32[DATA[g] - 0x20][index]);
            Wire.endTransmission(); /* Transmit the queued bytes and end transmission to slave device */
        }
    }
}

void OLED_Command(char DATA) /* Function for sending command to OLED */
{
    Wire.beginTransmission(OLED_Write_Address); /* Begin transmission to slave device */
    /* Queue data to be transmitted */
    Wire.write(0x00); /* For Data Transmission, C = 0 and D/C = 0 */
    Wire.write(DATA);
    Wire.endTransmission(); /* Transmit the queued bytes and end transmission to slave device */
}

void OLED_clear(void) /* Function for clearing OLED */
{
    OLED_setXY(0x00, 0x7F, 0x00, 0x07); /* Column Start Address 0, Column End Address 127, Page Start Address 0, Page End Address 7    */
    for (int k=0; k<=1023; k++)
    {
        Wire.beginTransmission(OLED_Write_Address); /* Begin transmission to slave device */
    /* Queue data to be transmitted */
        Wire.write(0x40); /* For Data Transmission, C = 0 and D/C = 1 */
        Wire.write(0x00);
        Wire.endTransmission(); /* Transmit the queued bytes and end transmission to slave device */
    }
}

void OLED_setXY(char col_start, char col_end, char page_start, char page_end) /* Function for setting cursor for writing data */
{
    Wire.beginTransmission(OLED_Write_Address); /* Begin transmission to slave device */
    /* Queue data to be transmitted */
    Wire.write(0x00); /* For Data Transmission, C = 0 and D/C = 0 */
    Wire.write(0x21); /* Set Column Start and End Address */
    Wire.write(col_start); /* Column Start Address col_start */
    Wire.write(col_end); /* Column End Address col_end */
    Wire.write(0x22); /* Set Page Start and End Address */
    Wire.write(page_start); /* Page Start Address page_start */
    Wire.write(page_end); /* Page End Address page_end */
    Wire.endTransmission(); /* Transmit the queued bytes and end transmission to slave device */
}

void OLED_init(void) /* Function for initializing OLED */
{
    OLED_Command(0xAE); /* Entire Display OFF */
    OLED_Command(0xD5); /* Set Display Clock Divide Ratio and Oscillator Frequency */
    OLED_Command(0x80); /* Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended */
    OLED_Command(0xA8); /* Set Multiplex Ratio */
    OLED_Command(0x3F); /* 64 COM lines */
    OLED_Command(0xD3); /* Set display offset */
    OLED_Command(0x00); /* 0 offset */
    OLED_Command(0x40); /* Set first line as the start line of the display */
    OLED_Command(0x8D); /* Charge pump */
    OLED_Command(0x14); /* Enable charge dump during display on */
    OLED_Command(0x20); /* Set memory addressing mode */
    OLED_Command(0x00); /* Horizontal addressing mode */
    OLED_Command(0xA1); /* Set segment remap with column address 127 mapped to segment 0 */
    OLED_Command(0xC8); /* Set com output scan direction, scan from com63 to com 0 */
    OLED_Command(0xDA); /* Set com pins hardware configuration */
    OLED_Command(0x12); /* Alternative com pin configuration, disable com left/right remap */
    OLED_Command(0x81); /* Set contrast control */
    OLED_Command(0x80); /* Set Contrast to 128 */
    OLED_Command(0xD9); /* Set pre-charge period */
    OLED_Command(0xF1); /* Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK */
    OLED_Command(0xDB); /* Set Vcomh deselect level */
    OLED_Command(0x20); /* Vcomh deselect level ~ 0.77 Vcc */
    OLED_Command(0xA4); /* Entire display ON, resume to RAM content display */
    OLED_Command(0xA6); /* Set Display in Normal Mode, 1 = ON, 0 = OFF */
    OLED_Command(0x2E); /* Deactivate scroll */
    OLED_Command(0xAF); /* Display on in normal mode */
}

void setup() {
    Wire.begin(); /* Initiate wire library and join I2C bus as a master */
    OLED_init(); /* Initialize OLED */

    delay(100);
    OLED_clear(); /* Clear OLED */
    delay(100);

    rotary_init();
}

void loop() {
  showValue(value);
  delay(1000);
}

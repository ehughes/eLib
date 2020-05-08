#include "MasterConfiguration.h"
#include "Board.h"
#include "stdint.h"
#include "System_I2C.h"
#include "LP5018.h"


#define LP5018_REG__DEVICE_CONFIG0         0x00 
#define LP5018_REG__DEVICE_CONFIG1         0x01
#define LP5018_REG__LED_CONFIG0            0x02
#define LP5018_REG__BANK_BRIGHTNESS        0x03
#define LP5018_REG__BANK_A_COLOR           0x04
#define LP5018_REG__BANK_B_COLOR           0x05
#define LP5018_REG__BANK_C_COLOR           0x06
#define LP5018_REG__LED0_BRIGHTNESS        0x07
#define LP5018_REG__LED1_BRIGHTNESS        0x08
#define LP5018_REG__LED2_BRIGHTNESS        0x09
#define LP5018_REG__LED3_BRIGHTNESS        0x0A
#define LP5018_REG__LED4_BRIGHTNESS        0x0B
#define LP5018_REG__LED5_BRIGHTNESS        0x0C
#define LP5018_REG__LED6_BRIGHTNESS        0x0D
#define LP5018_REG__LED7_BRIGHTNESS        0x0E
#define LP5018_REG__OUT0_COLOR             0x0F
#define LP5018_REG__OUT1_COLOR             0x10
#define LP5018_REG__OUT2_COLOR             0x11
#define LP5018_REG__OUT3_COLOR             0x12
#define LP5018_REG__OUT4_COLOR             0x13    
#define LP5018_REG__OUT5_COLOR             0x14
#define LP5018_REG__OUT6_COLOR             0x15
#define LP5018_REG__OUT7_COLOR             0x16
#define LP5018_REG__OUT8_COLOR             0x17
#define LP5018_REG__OUT9_COLOR             0x18
#define LP5018_REG__OUT10_COLOR            0x19
#define LP5018_REG__OUT11_COLOR            0x1A
#define LP5018_REG__OUT12_COLOR            0x1B
#define LP5018_REG__OUT13_COLOR            0x1C
#define LP5018_REG__OUT14_COLOR            0x1D
#define LP5018_REG__OUT15_COLOR            0x1E
#define LP5018_REG__OUT16_COLOR            0x1F
#define LP5018_REG__OUT17_COLOR            0x20
#define LP5018_REG__OUT18_COLOR            0x21
#define LP5018_REG__OUT19_COLOR            0x22
#define LP5018_REG__OUT20_COLOR            0x23
#define LP5018_REG__OUT21_COLOR            0x24
#define LP5018_REG__OUT22_COLOR            0x25
#define LP5018_REG__OUT23_COLOR            0x26
#define LP5018_REG__RESET                  0x27



#define LP5018_REG__DEVICE_CONFIG0__BIT_CHIP_EN     (1<<6)

#define LP5018_REG__DEVICE_CONFIG1__LED0_BANK_EN    (1<<0)
#define LP5018_REG__DEVICE_CONFIG1__LED1_BANK_EN    (1<<1)
#define LP5018_REG__DEVICE_CONFIG1__LED2_BANK_EN    (1<<2)
#define LP5018_REG__DEVICE_CONFIG1__LED3_BANK_EN    (1<<3)
#define LP5018_REG__DEVICE_CONFIG1__LED4_BANK_EN    (1<<4)
#define LP5018_REG__DEVICE_CONFIG1__LED5_BANK_EN    (1<<5)



void LP5018_Init()
{
    LP5018_EN_HIGH;

    System__Delay_mS(10);  //Need min 500uS.   10mS is our smallest threshold
    System_I2C_WriteRegister_8Bit(LP5018_I2C_ADDRESS,
                                  LP5018_REG__DEVICE_CONFIG0,
                                  LP5018_REG__DEVICE_CONFIG0__BIT_CHIP_EN);

}

void LP5018_SetLED_Brightness(uint8_t LED_Number, uint8_t Brightness)
{
    if(LED_Number < NUM_LP5018_LEDS)
    {
    
        System_I2C_WriteRegister_8Bit(LP5018_I2C_ADDRESS,
                                  LP5018_REG__LED0_BRIGHTNESS + LED_Number,
                                  Brightness);
    }
}


void LP5018_SetLED_Color(uint8_t LED_Number, LP5018_LED *LED)
{
    if(LED_Number < NUM_LP5018_LEDS)
    {
            uint8_t Buffer[4];
 
            Buffer[0] = (LED_Number*3)+LP5018_REG__OUT0_COLOR;
            Buffer[1] =LED->Green;
            Buffer[2] = LED->Red;
            
            Buffer[3] =LED->Blue;

            System_I2C_WriteRaw(LP5018_I2C_ADDRESS,Buffer,sizeof(Buffer));
    }
}


void LP5018_SetLED(uint8_t LED_Number, LP5018_LED *LED)
{

    LP5018_SetLED_Brightness(LED_Number,LED->Brightness);
    LP5018_SetLED_Color( LED_Number, LED);

}


const uint8_t GlowTable[256] = {
255     ,
255     ,
255     ,
255     ,
254     ,
254     ,
254     ,
253     ,
253     ,
252     ,
251     ,
250     ,
250     ,
249     ,
248     ,
246     ,
245     ,
244     ,
243     ,
241     ,
240     ,
238     ,
237     ,
235     ,
234     ,
232     ,
230     ,
228     ,
226     ,
224     ,
222     ,
220     ,
218     ,
215     ,
213     ,
211     ,
208     ,
206     ,
203     ,
201     ,
198     ,
196     ,
193     ,
190     ,
188     ,
185     ,
182     ,
179     ,
176     ,
173     ,
170     ,
167     ,
165     ,
162     ,
158     ,
155     ,
152     ,
149     ,
146     ,
143     ,
140     ,
137     ,
134     ,
131     ,
128     ,
124     ,
121     ,
118     ,
115     ,
112     ,
109     ,
106     ,
103     ,
100     ,
97      ,
93      ,
90      ,
88      ,
85      ,
82      ,
79      ,
76      ,
73      ,
70      ,
67      ,
65      ,
62      ,
59      ,
57      ,
54      ,
52      ,
49      ,
47      ,
44      ,
42      ,
40      ,
37      ,
35      ,
33      ,
31      ,
29      ,
27      ,
25      ,
23      ,
21      ,
20      ,
18      ,
17      ,
15      ,
14      ,
12      ,
11      ,
10      ,
9       ,
7       ,
6       ,
5       ,
5       ,
4       ,
3       ,
2       ,
2       ,
1       ,
1       ,
1       ,
0       ,
0       ,
0       ,
0       ,
0       ,
0       ,
0       ,
1       ,
1       ,
1       ,
2       ,
2       ,
3       ,
4       ,
5       ,
5       ,
6       ,
7       ,
9       ,
10      ,
11      ,
12      ,
14      ,
15      ,
17      ,
18      ,
20      ,
21      ,
23      ,
25      ,
27      ,
29      ,
31      ,
33      ,
35      ,
37      ,
40      ,
42      ,
44      ,
47      ,
49      ,
52      ,
54      ,
57      ,
59      ,
62      ,
65      ,
67      ,
70      ,
73      ,
76      ,
79      ,
82      ,
85      ,
88      ,
90      ,
93      ,
97      ,
100     ,
103     ,
106     ,
109     ,
112     ,
115     ,
118     ,
121     ,
124     ,
128     ,
131     ,
134     ,
137     ,
140     ,
143     ,
146     ,
149     ,
152     ,
155     ,
158     ,
162     ,
165     ,
167     ,
170     ,
173     ,
176     ,
179     ,
182     ,
185     ,
188     ,
190     ,
193     ,
196     ,
198     ,
201     ,
203     ,
206     ,
208     ,
211     ,
213     ,
215     ,
218     ,
220     ,
222     ,
224     ,
226     ,
228     ,
230     ,
232     ,
234     ,
235     ,
237     ,
238     ,
240     ,
241     ,
243     ,
244     ,
245     ,
246     ,
248     ,
249     ,
250     ,
250     ,
251     ,
252     ,
253     ,
253     ,
254     ,
254     ,
254     ,
255     ,
255     ,
};
/*
 * IO_EXP_Config.h
 *
 *  Created on: Mar 23, 2019
 *      Author: Eli Hughes
 */

#ifndef _EXP_CONF
#define _EXP_CONF

#define EXPANDER_CS_CHANNEL_MISC   0x00
#define EXPANDER_CS_CHANNEL_FIRING 0x01



extern uint8_t Expander_Misc__A_Shadow;
extern uint8_t Expander_Misc__B_Shadow;

extern uint8_t Expander_Misc__A_Dirty;
extern uint8_t Expander_Misc__B_Dirty;

#define FM_BUZZ_PIN           (1<<0)
#define FM_DUMP_PIN           (1<<1)
#define FM_TEST_PIN           (1<<2)
#define FM_FP_ENABLE_PIN      (1<<3)
#define FM_FIRENOW_PIN        (1<<4)
#define FM_CUE_FIRE_GND_PIN   (1<<5)

#define ACT_LED_C_PIN			(1<<0)
#define IN_RAND_IND_PIN         (1<<1)
#define CHARGE_IND_PIN          (1<<2)
#define LED_ON_WIRELESS_PIN     (1<<3)
#define FM_RADIO_PRESENT_PIN    (1<<4)
#define FM_RADIO_DATASEL_PIN    (1<<7)


#define EXPANDER_MISC_A_MARK_DIRTY Expander_Misc__A_Dirty = 1;
#define EXPANDER_MISC_B_MARK_DIRTY Expander_Misc__B_Dirty = 1;

#define EXPANDER_MISC_A_MARK_CLEAN Expander_Misc__A_Dirty = 0;
#define EXPANDER_MISC_B_MARK_CLEAN Expander_Misc__B_Dirty = 0;


#define ACT_LED_ON	    Expander_Misc__B_Shadow&=~ACT_LED_C_PIN;EXPANDER_MISC_B_MARK_DIRTY;
#define ACT_LED_OFF	    Expander_Misc__B_Shadow|=ACT_LED_C_PIN;EXPANDER_MISC_B_MARK_DIRTY;
#define ACT_LED_TOGGLE	Expander_Misc__B_Shadow^=ACT_LED_C_PIN;EXPANDER_MISC_B_MARK_DIRTY;


#define EXPANDER_MISC_B_FLUSH   if(Expander_Misc__B_Dirty){MCP23S17_WriteRegister_8Bit(EXPANDER_CS_CHANNEL_MISC,0,MCP23S17_BANK0_GPIOB,Expander_Misc__B_Shadow);EXPANDER_MISC_B_MARK_CLEAN;}
#define EXPANDER_MISC_A_FLUSH   if(Expander_Misc__A_Dirty){MCP23S17_WriteRegister_8Bit(EXPANDER_CS_CHANNEL_MISC,0,MCP23S17_BANK0_GPIOA,Expander_Misc__A_Shadow);EXPANDER_MISC_A_MARK_CLEAN;}

#endif

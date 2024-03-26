#pragma once

#include "mbed.h"

#define T 20
#define DBG 0
#define ARR_LEN(x) sizeof(x)/sizeof(*x)

bool leds_toggle = false;
bool leds_on     = true;
bool leds_off    = false;


class PWMDigOut {
private:
	Ticker 		m_ticker;
    DigitalOut 	m_led;
    uint8_t 	m_jas;
    uint32_t 	m_T;
    uint32_t 	m_ticks;

    void pwm_control() {
    	if(this->m_ticks < this->m_T && !leds_toggle && leds_on && !leds_off) {
    		this->m_led = 1;
    	} else {
    		this->m_led = 0;
    	}
    #if DBG
        printf("ticks: %d\n", this->m_ticks);
    #endif
    	this->m_ticks++;
    	if(this->m_ticks >= T) {
    		m_ticks = 0;
    	}
    }

public:
    PWMDigOut(PinName t_led_pin)
    	: m_led(t_led_pin)
    	, m_jas(0)
    	, m_T(0)
    	, m_ticks(0)
    {
        this->m_ticker.attach(callback(this, &PWMDigOut::pwm_control), 1ms);
    }

    void nastav_jas_proc(uint8_t t_jas) {
        this->m_jas = t_jas;
        this->m_T = float(T) * float((float)t_jas/100.0f);
    }

    uint8_t get_bright() const { return m_jas; }

};

#define T_btn 5000   // cooldown in ms?

class BTN_NOT {
private:
	Ticker 		m_ticker;
	DigitalIn 	m_btn;
	bool* 		m_state;
	uint32_t 	m_T;
	uint32_t 	m_ticks;
	bool 		m_pressed;

public:
	BTN_NOT(PinName t_pin, bool* t_state)
		: m_btn(t_pin)
		, m_state(t_state)
		, m_T(100)
		, m_ticks(0)
		, m_pressed(false)
	{
		this->m_ticker.attach(callback(this, &BTN_NOT::btn_control), 1ms);
	}

	void btn_control() {
		if(!m_btn && !m_pressed) {
			m_pressed = true;
			*m_state = !(*m_state);
		}

		if(this->m_pressed) this->m_ticks++;
    	if(this->m_ticks >= T) {
    		this->m_ticks = 0;
    		this->m_pressed = false;
    	}
	}
};

class BTN_SET {
private:
	Ticker 		m_ticker;
	DigitalIn 	m_btn;
	bool* 		m_true_state;
	bool* 		m_false_state;
	uint32_t 	m_T;
	uint32_t 	m_ticks;
	bool 		m_pressed;

public:
	BTN_SET(PinName t_pin, bool* t_true_state, bool* t_false_state)
		: m_btn(t_pin)
		, m_true_state(t_true_state)
		, m_false_state(t_false_state)
		, m_T(100)
		, m_ticks(0)
		, m_pressed(false)
	{
		this->m_ticker.attach(callback(this, &BTN_SET::btn_control), 1ms);
	}

	void btn_control() {
		if(!m_btn && !m_pressed) {
			m_pressed = true;
			*m_true_state = true;
			*m_false_state = false;
		}

		if(this->m_pressed) this->m_ticks++;
    	if(this->m_ticks >= T) {
    		this->m_ticks = 0;
    		this->m_pressed = false;
    	}
	}
};




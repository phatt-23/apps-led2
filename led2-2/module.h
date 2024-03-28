#pragma once

#include "mbed.h"

#define T 20
#define DBG 0
#define ARR_LEN(x) sizeof(x)/sizeof(*x)

bool leds_toggle = false;
bool leds_on     = true;
bool leds_off    = false;

class PWMDigOutRGB {
	Ticker 		m_ticker;
    DigitalOut 	m_r, m_g, m_b;
    uint8_t 	m_val1, m_val2, m_val3;
    uint32_t 	m_T1, m_T2, m_T3;
    uint32_t 	m_ticks1, m_ticks2, m_ticks3;

    void pwm_control() {
    	m_r = (m_ticks1 < this->m_T1) ? 1 : 0;
    	m_g = (m_ticks2 < this->m_T2) ? 1 : 0;
    	m_b = (m_ticks3 < this->m_T3) ? 1 : 0;
    #if DBG
        printf("ticks: %d\n", this->m_ticks);
    #endif

        m_ticks1++;
        m_ticks2++;
        m_ticks3++;
        if(m_ticks1 >= T) m_ticks1 = 0;
        if(m_ticks2 >= T) m_ticks2 = 0;
        if(m_ticks3 >= T) m_ticks3 = 0;
    }

public:
    PWMDigOutRGB(PinName t_pin1, PinName t_pin2, PinName t_pin3)
    	: m_r(t_pin1), m_g(t_pin2), m_b(t_pin3)
    	, m_val1(0), m_val2(0), m_val3(0)
    	, m_T1(0), m_T2(0), m_T3(0)
    	, m_ticks1(0), m_ticks2(0), m_ticks3(0)
    {
        m_ticker.attach(callback(this, &PWMDigOutRGB::pwm_control), 1ms);
    }

    void set_color(uint8_t t_r, uint8_t t_g, uint8_t t_b) {

    	m_val1 = t_r;
    	m_val2 = t_g;
    	m_val3 = t_b;

    	m_T1 = float(T) * float(float(m_val1)/255.0f);
		m_T2 = float(T) * float(float(m_val2)/255.0f);
		m_T3 = float(T) * float(float(m_val3)/255.0f);

    }

    uint8_t get_r() const { return m_val1; }
    uint8_t get_g() const { return m_val2; }
    uint8_t get_b() const { return m_val3; }

};

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

#define T_btn 100000   // cooldown in ms?

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
		static bool l_change = false;


		if(!m_btn && !m_pressed && !m_ticks) {
			//*m_state = !(*m_state);
			m_pressed = true;
			*m_state = true;
		} else {
			*m_state = false;
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

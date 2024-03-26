#include "mbed.h"
#include "module.h"
#include <cstdint>

PWMDigOut g_led[] = {
	{ PTC0 },
	{ PTC1 },
	{ PTC2 },
	{ PTC3 },
	{ PTC4 },
	{ PTC5 },
	{ PTC7 },
	{ PTC8 },
};

const uint8_t g_led_count = ARR_LEN(g_led);

#if 0
BTN_NOT g_toggle_btn  { PTC9,  &leds_toggle        };	// leds_toggle controls LEDS globally
BTN_XOR g_leds_on_btn { PTC10, &leds_on, &leds_off };	// sets the LEDS ON only if leds_toggle is true
BTN_XOR g_leds_off_btn{ PTC11, &leds_off, &leds_on };	// set the LEDS off
#endif

#if 0

bool btn1 = false,
	 btn2 = false;

BTN_NOT btns[] = {
		{PTC9, &btn1},
		{PTC10, &btn2},
};

const uint8_t g_jasy[] = {0,0,0,20, 40,60,80,100};
const size_t  g_jasy_len = ARR_LEN(g_jasy);

#define DBG 0

void main_fn_snake() {
    static uint32_t l_tiks       = 0;
    static uint32_t l_period     = 300;
	static bool l_right_left     = 0;
           // 0 = from left, 1 = from right
    static int      l_head_at    = 0;       // snake head at led[]

    if(btn1) {
    	l_right_left = 1;
    }

    if(btn2) {
    	l_right_left = 0;
    }

    if(!l_tiks) {
        if(!l_right_left) {
            // move from left to right

            for(int i = 0; i < g_jasy_len; ++i) {
            	if(DBG)printf("%d ", (l_head_at + i) % g_led_count);
            	g_led[(l_head_at + i) % g_led_count].nastav_jas_proc(g_jasy[i]);
            }
            if(DBG)printf("\n");
            l_head_at++;
            if(l_head_at > g_led_count - 1)
            	l_head_at = 0;

        } else {
            // move from right to left

            for(int i = 0; i < g_jasy_len; ++i) {
                int tmp_index = l_head_at - i;
                if(tmp_index < 0) {
                    tmp_index += g_led_count;
                }
                g_led[tmp_index].nastav_jas_proc(g_jasy[i]);
            }
            l_head_at--;
            if(l_head_at < 0) {
                l_head_at += g_led_count;
            }
        }
    }

    l_tiks++;
    if(l_tiks >= l_period) {
        l_tiks = 0;
        if(DBG)printf("TICK ");
    }
}
#endif

#if 0

bool btn_state[] = { 0, 0, 0, 0 };

BTN_NOT btn[] = {
    { PTC9, &btn_state[0] },
    { PTC10, &btn_state[1] },
    { PTC11, &btn_state[2] },
    { PTC12, &btn_state[3] },
};

#define DBG 1

void main_fn_selector() {
    static uint32_t l_tiks        = 0;
    static uint32_t l_period      = 50;
    static bool     l_right_left  = 0;       // 0 = from left, 1 = from right
    static int      l_head_at     = 0;       // snake head at led[]
    static int      l_current_led = 0;

    // printf("\e[1;1H\e[2J");
    if(DBG)printf("current_led_index: %d\n", l_current_led);

    if(btn_state[0]) {
        if(l_current_led > 0)
            l_current_led--;
        if(DBG)printf("Button 0 pressed!\n");
    }

    if(btn_state[1]) {
        if(l_current_led < g_led_count - 1)
            l_current_led++;
        if(DBG)printf("Button 1 pressed!\n");
    }

    if(btn_state[2]) {
        uint8_t tmp_bright = g_led[l_current_led].get_bright();
        if(tmp_bright >= 10) {
            tmp_bright -= 10;
        }
        g_led[l_current_led].nastav_jas_proc(tmp_bright);
        if(DBG)printf("Button 2 pressed!\n");
    }

    if(btn_state[3]) {
        uint8_t tmp_bright = g_led[l_current_led].get_bright();
        if(tmp_bright <= 90) {
            tmp_bright += 10;
        }
        g_led[l_current_led].nastav_jas_proc(tmp_bright);
        if(DBG)printf("Button 3 pressed!\n");
    }


    l_tiks++;
    if(l_tiks <= l_period) {
        l_tiks = 0;
    }
}
#endif

PWMDigOutRGB g_rgb[] = {
	{ PTB2,  PTB3,  PTB9 },
	{ PTB10, PTB11, PTB18 },
	{ PTB19,  PTB20, PTB23 },
};

#if 1

#define DBG 1

void main_fn_rgb() {


}


#endif

int main() {
	printf("Hello\n");
/*	for(size_t i = 0; i < ARR_LEN(g_led); ++i) {
		g_led[i].nastav_jas_proc(i * 5);
	}*/
	/*
	for(size_t i = 0; i < ARR_LEN(g_rgb); ++i) {
		g_rgb[i].set_color(20 * i, 20 * i, 20 * i + 30);
	}
	*/
	g_rgb[0].set_color(255,0,0);
	g_rgb[1].set_color(0,255,0);
	g_rgb[2].set_color(0,0,255);

	Ticker tiker;
	// tiker.attach(callback(&main_fn_selector), 1ms);
	// tiker.attach(callback(&main_fn_snake), 1ms);

    while(1) __WFI();
}

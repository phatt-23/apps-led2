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

BTN_NOT g_toggle_btn  { PTC9,  &leds_toggle        };	// leds_toggle controls LEDS globally
BTN_XOR g_leds_on_btn { PTC10, &leds_on, &leds_off };	// sets the LEDS ON only if leds_toggle is true
BTN_XOR g_leds_off_btn{ PTC11, &leds_off, &leds_on };	// set the LEDS off

const uint8_t g_jasy[] = {40, 35, 30, 25, 20, 15, 10, 0, 0};
const size_t  g_jasy_len = ARR_LEN(g_jasy);

void main_fn() {
    static uint32_t l_tiks       = 0;
    static uint32_t l_period     = 300;
    static bool     l_right_left = 0;       // 0 = from left, 1 = from right
    static int      l_head_at    = 0;       // snake head at led[]
    
    if(!l_tiks) {
        if(!l_right_left) {
            // move from left to right
            printf("Moving from LEFT to RIGHT\n");
            for(int i = l_head_at; i < g_jasy_len; ++i) {
                g_led[(i + l_head_at) % g_led_count].nastav_jas_proc(g_jasy[i]);
            }
            l_head_at++;
            l_head_at %= g_led_count;

        } else {
            // move from right to left
            printf("Moving from RIGHT to LEFT\n");
            for(int i = l_head_at; i < g_jasy_len; ++i) {
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
    if(l_tiks <= l_period) {
        l_tiks = 0;
    }
}

#if 1

bool btn_state[] = { 0, 0, 0, 0 };

BTN_NOT btn[] = {
    { PTC0, &btn_state[0] },
    { PTC1, &btn_state[1] },
    { PTC2, &btn_state[2] },
    { PTC4, &btn_state[3] },
};

void main_fn_selector() {
    static uint32_t l_tiks        = 0;
    static uint32_t l_period      = 300;
    static bool     l_right_left  = 0;       // 0 = from left, 1 = from right
    static int      l_head_at     = 0;       // snake head at led[]
    static int      l_current_led = 0;

    if(btn_state[0]) {
        if(l_current_led > 0)
            l_current_led--;
    }
    
    if(btn_state[1]) {
        if(l_current_led < g_led_count)
            l_current_led++;
    }

    if(btn_state[2]) {
        uint8_t tmp_bright = g_led[l_current_led].get_bright();
        if(tmp_bright >= 10) {
            tmp_bright -= 10;
        }
        g_led[l_current_led].nastav_jas_proc(tmp_bright);
    }

    if(btn_state[3]) {
        uint8_t tmp_bright = g_led[l_current_led].get_bright();
        if(tmp_bright <= 90) {
            tmp_bright += 10;
        }
        g_led[l_current_led].nastav_jas_proc(tmp_bright);
    }


    l_tiks++;
    if(l_tiks <= l_period) {
        l_tiks = 0;
    }
}
#endif

int main() {
	printf("Hello\n");
	for(size_t i = 0; i < ARR_LEN(g_led); ++i) {
		g_led[i].nastav_jas_proc(i * 5);
	}

    while(1) __WFI();
}

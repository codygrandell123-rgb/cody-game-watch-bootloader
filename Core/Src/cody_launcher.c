#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "bootloader.h"
#include "gw_buttons.h"
#include "gw_gui.h"
#include "gw_lcd.h"

#define MENU_COUNT 4

static const char *menu_items[MENU_COUNT] = {
    "Original Games",
    "Custom ROMs",
    "Cheats",
    "System Info",
};

static void launcher_screen_init(void)
{
    lcd_backlight_off();
    lcd_deinit(&hspi2);

    for (int i = 0; i < 4; i++) {
        wdog_refresh();
        HAL_Delay(50);
    }

    lcd_init(&hspi2, &hltdc);

    for (int i = 0; i < 4; i++) {
        wdog_refresh();
        HAL_Delay(50);
    }

    gw_gui_fill(GUI_BACKGROUND_COLOR);
    lcd_backlight_set(180);
}

static void draw_menu(uint8_t selected)
{
    gw_gui_fill(GUI_BACKGROUND_COLOR);

    gw_gui_draw_text(72, 28, "CODY GAME & WATCH", GUI_WHITE);
    gw_gui_draw_text(96, 42, "BANK 2 LAUNCHER", GUI_GREEN);

    for (uint8_t i = 0; i < MENU_COUNT; i++) {
        const int y = 82 + (i * 24);

        if (i == selected) {
            gw_gui_draw_text(54, y, ">", GUI_GREEN);
            gw_gui_draw_text(72, y, menu_items[i], GUI_GREEN);
        } else {
            gw_gui_draw_text(72, y, menu_items[i], GUI_WHITE);
        }
    }

    gw_gui_draw_text(54, 205, "D-PAD: MOVE   A: SELECT", GUI_WHITE);
}

static void wait_for_back(void)
{
    uint32_t previous = buttons_get();

    while (1) {
        const uint32_t now = buttons_get();
        const uint32_t pressed = now & ~previous;

        if (pressed & (B_B | B_PAUSE)) {
            return;
        }

        if (pressed & B_POWER) {
            GW_EnterDeepSleep();
        }

        previous = now;
        wdog_refresh();
        HAL_Delay(20);
    }
}

static void show_placeholder(uint8_t selected)
{
    gw_gui_fill(GUI_BACKGROUND_COLOR);
    gw_gui_draw_text(72, 36, menu_items[selected], GUI_GREEN);

    switch (selected) {
        case 0:
            gw_gui_draw_text(36, 84, "Original game loader", GUI_WHITE);
            gw_gui_draw_text(36, 98, "will be added next.", GUI_WHITE);
            break;
        case 1:
            gw_gui_draw_text(36, 84, "Custom ROM browser", GUI_WHITE);
            gw_gui_draw_text(36, 98, "will be added next.", GUI_WHITE);
            break;
        case 2:
            gw_gui_draw_text(36, 84, "Cheat manager", GUI_WHITE);
            gw_gui_draw_text(36, 98, "will be added next.", GUI_WHITE);
            break;
        case 3:
            gw_gui_draw_text(36, 84, "Cody Launcher v0.1", GUI_WHITE);
            gw_gui_draw_text(36, 98, "Target: internal Bank 2", GUI_WHITE);
            gw_gui_draw_text(36, 112, "Display: 320x240", GUI_WHITE);
            break;
        default:
            break;
    }

    gw_gui_draw_text(36, 190, "B / PAUSE: BACK", GUI_WHITE);
    wait_for_back();
}

void bootloader_main(void)
{
    uint8_t selected = 0;
    uint32_t previous;

    launcher_screen_init();
    draw_menu(selected);

    /* Ignore buttons that were already held while the device booted. */
    previous = buttons_get();

    while (1) {
        const uint32_t now = buttons_get();
        const uint32_t pressed = now & ~previous;

        if (pressed & B_POWER) {
            GW_EnterDeepSleep();
        }

        if (pressed & B_Up) {
            selected = (selected == 0) ? (MENU_COUNT - 1) : (selected - 1);
            draw_menu(selected);
        } else if (pressed & B_Down) {
            selected = (selected + 1) % MENU_COUNT;
            draw_menu(selected);
        } else if (pressed & B_A) {
            show_placeholder(selected);
            draw_menu(selected);
        }

        previous = now;
        wdog_refresh();
        HAL_Delay(20);
    }
}

#ifndef __SDL_H__
#define __SDL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_sdl_conf.h"
#include <lvgl.h>
#include SDL_INCLUDE_PATH

/**
 * @brief SDL to be used as display, mouse and mouse wheel drivers.
 */
void sdl_init(void);

/**
 * @brief a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdl_display_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

#if SDL_DUAL_DISPLAY
/**
 * @brief Flush a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdl_display_flush2(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
#endif

/**
 * @brief the current position and state of the mouse
 * @param indev_drv pointer to the related input device driver
 * @param data store the mouse data here
 */
void sdl_mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * @brief encoder (i.e. mouse wheel) ticks difference and pressed state
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 */
void sdl_mousewheel_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * @brief input from the keyboard.
 * @param indev_drv pointer to the related input device driver
 * @param data store the red data here
 */
void sdl_keyboard_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* __SDL_H__ */

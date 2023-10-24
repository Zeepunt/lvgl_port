/*
 * @Author       : Zeepunt
 * @Date         : 2023-07-08
 * @LastEditTime : 2023-10-21
 *  
 * Gitee : https://gitee.com/zeepunt
 * Github: https://github.com/zeepunt
 *  
 * Copyright (c) 2023 by Zeepunt, All Rights Reserved. 
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <lvgl_port/sdl/sdl.h>
#include SDL_INCLUDE_PATH

#ifndef KEYBOARD_BUFFER_SIZE
#define KEYBOARD_BUFFER_SIZE SDL_TEXTINPUTEVENT_TEXT_SIZE
#endif

#ifndef SDL_WINDOW_TITLE
#define SDL_WINDOW_TITLE "TFT Simulator"
#endif

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    volatile bool sdl_refr_qry;
#if SDL_DOUBLE_BUFFERED
    uint32_t *tft_fb_act;
#else
    uint32_t *tft_fb;
#endif
} monitor_t;

static monitor_t s_monitor;

#if SDL_DUAL_DISPLAY
static monitor_t s_monitor2;
#endif

static void monitor_sdl_clean_up(void);
static void window_update(monitor_t *m);

static volatile bool s_sdl_inited = false;
static volatile bool s_sdl_quit_qry = false;

static bool s_left_button_down = false;
static int16_t s_last_x = 0;
static int16_t s_last_y = 0;

static int16_t s_wheel_diff = 0;
static lv_indev_state_t s_wheel_state = LV_INDEV_STATE_RELEASED;

static char s_buf[KEYBOARD_BUFFER_SIZE];

static mouse_handler(SDL_Event *event)
{
    switch (event->type) {
    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_LEFT)
            s_left_button_down = false;
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT) {
            s_left_button_down = true;
            s_last_x = event->motion.x / SDL_ZOOM;
            s_last_y = event->motion.y / SDL_ZOOM;
        }
        break;

    case SDL_MOUSEMOTION:
        s_last_x = event->motion.x / SDL_ZOOM;
        s_last_y = event->motion.y / SDL_ZOOM;
        break;

    case SDL_FINGERUP:
        s_left_button_down = false;
        s_last_x = SDL_HOR_RES * event->tfinger.x / SDL_ZOOM;
        s_last_y = SDL_VER_RES * event->tfinger.y / SDL_ZOOM;
        break;

    case SDL_FINGERDOWN:
        s_left_button_down = true;
        s_last_x = SDL_HOR_RES * event->tfinger.x / SDL_ZOOM;
        s_last_y = SDL_VER_RES * event->tfinger.y / SDL_ZOOM;
        break;

    case SDL_FINGERMOTION:
        s_last_x = SDL_HOR_RES * event->tfinger.x / SDL_ZOOM;
        s_last_y = SDL_VER_RES * event->tfinger.y / SDL_ZOOM;
        break;
    }
}

/**
 * @brief is called periodically from the SDL thread to check mouse wheel state
 * @param event describes the event
 */
static mousewheel_handler(SDL_Event *event)
{
    switch (event->type) {
    case SDL_MOUSEWHEEL:
        /**
         * Scroll down (y = -1) means positive encoder turn,
         * so invert it
         */
#ifdef __EMSCRIPTEN__
        /* Escripten scales it wrong */
        if (event->wheel.y < 0)
            s_wheel_diff++;
        if (event->wheel.y > 0)
            s_wheel_diff--;
#else
        s_wheel_diff = -event->wheel.y;
#endif
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            s_wheel_state = LV_INDEV_STATE_PRESSED;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            s_wheel_state = LV_INDEV_STATE_RELEASED;
        }
        break;

    default:
        break;
    }
}

/**
 * @brief a SDL key code to it's LV_KEY_* counterpart or return '\0' if it's not a control character.
 * @param sdl_key the key code
 * @return LV_KEY_* control character or '\0'
 */
uint32_t keycode_to_ctrl_key(SDL_Keycode sdl_key)
{
    /* Remap some key to LV_KEY_... to manage groups */
    SDL_Keymod mode = SDL_GetModState();

    switch (sdl_key) {
    case SDLK_RIGHT:
    case SDLK_KP_PLUS:
        return LV_KEY_RIGHT;

    case SDLK_LEFT:
    case SDLK_KP_MINUS:
        return LV_KEY_LEFT;

    case SDLK_UP:
        return LV_KEY_UP;

    case SDLK_DOWN:
        return LV_KEY_DOWN;

    case SDLK_ESCAPE:
        return LV_KEY_ESC;

    case SDLK_BACKSPACE:
        return LV_KEY_BACKSPACE;

    case SDLK_DELETE:
        return LV_KEY_DEL;

    case SDLK_KP_ENTER:
    case '\r':
        return LV_KEY_ENTER;

    case SDLK_TAB:
        return (mode & KMOD_SHIFT) ? LV_KEY_PREV : LV_KEY_NEXT;

    case SDLK_PAGEDOWN:
        return LV_KEY_NEXT;

    case SDLK_PAGEUP:
        return LV_KEY_PREV;

    default:
        return '\0';
    }
}

/**
 * @brief periodically from the SDL thread, store text input or control characters in the buffer.
 * @param event describes the event
 */
static void keyboard_handler(SDL_Event *event)
{
    /* We only care about SDL_KEYDOWN and SDL_TEXTINPUT events */
    switch (event->type) {
    case SDL_KEYDOWN: {
        /*Button press*/
        const uint32_t ctrl_key = keycode_to_ctrl_key(event->key.keysym.sym);
        if (ctrl_key == '\0')
            return;
        const size_t len = strlen(s_buf);
        if (len < KEYBOARD_BUFFER_SIZE - 1) {
            s_buf[len] = ctrl_key;
            s_buf[len + 1] = '\0';
        }
        break;
    }

    case SDL_TEXTINPUT: {
        /* Text input */
        const size_t len = strlen(s_buf) + strlen(event->text.text);
        if (len < KEYBOARD_BUFFER_SIZE - 1)
            strcat(s_buf, event->text.text);

        break;
    }

    default:
        break;
    }
}

/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */
static void sdl_event_handler(lv_timer_t *t)
{
    (void)t;

    /* Refresh handling */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        mouse_handler(&event);
        mousewheel_handler(&event);
        keyboard_handler(&event);

        if ((&event)->type == SDL_WINDOWEVENT) {
            switch ((&event)->window.event) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
            case SDL_WINDOWEVENT_EXPOSED:
                window_update(&s_monitor);
#if SDL_DUAL_DISPLAY
                window_update(&s_monitor2);
#endif
                break;
            default:
                break;
            }
        }
    }

    /* Run until quit event not arrives */
    if (s_sdl_quit_qry) {
        monitor_sdl_clean_up();
        exit(0);
    }
}

/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */
static void monitor_sdl_refr(lv_timer_t *t)
{
    (void)t;

    /* Refresh handling */
    if (s_monitor.sdl_refr_qry != false) {
        s_monitor.sdl_refr_qry = false;
        window_update(&s_monitor);
    }

#if SDL_DUAL_DISPLAY
    if (s_monitor2.sdl_refr_qry != false) {
        s_monitor2.sdl_refr_qry = false;
        window_update(&s_monitor2);
    }
#endif
}

static void monitor_sdl_clean_up(void)
{
    SDL_DestroyTexture(s_monitor.texture);
    SDL_DestroyRenderer(s_monitor.renderer);
    SDL_DestroyWindow(s_monitor.window);

#if SDL_DUAL_DISPLAY
    SDL_DestroyTexture(s_monitor2.texture);
    SDL_DestroyRenderer(s_monitor2.renderer);
    SDL_DestroyWindow(s_monitor2.window);

#endif

    SDL_Quit();
}

static void window_create(monitor_t *m)
{
    int flag = 0;

    m->window = SDL_CreateWindow(SDL_WINDOW_TITLE,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_HOR_RES * SDL_ZOOM,
                                 SDL_VER_RES * SDL_ZOOM,
                                 flag); /* last param. SDL_WINDOW_BORDERLESS to hide borders */

    m->renderer = SDL_CreateRenderer(m->window,
                                     -1,
                                     SDL_RENDERER_SOFTWARE);

    m->texture = SDL_CreateTexture(m->renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STATIC,
                                   SDL_HOR_RES,
                                   SDL_VER_RES);

    SDL_SetTextureBlendMode(m->texture, SDL_BLENDMODE_BLEND);

    /* Initialize the frame buffer to gray (77 is an empirical value) */
#if SDL_DOUBLE_BUFFERED
    SDL_UpdateTexture(m->texture,
                      NULL,
                      m->tft_fb_act,
                      SDL_HOR_RES * sizeof(uint32_t));
#else
    m->tft_fb = (uint32_t *)malloc(sizeof(uint32_t) * SDL_HOR_RES * SDL_VER_RES);
    memset(m->tft_fb, 0x44, SDL_HOR_RES * SDL_VER_RES * sizeof(uint32_t));
#endif

    m->sdl_refr_qry = true;
}

static void window_update(monitor_t *m)
{
#if SDL_DOUBLE_BUFFERED == 0
    SDL_UpdateTexture(m->texture,
                      NULL,
                      m->tft_fb,
                      SDL_HOR_RES * sizeof(uint32_t));
#else
    if (m->tft_fb_act == NULL)
        return;
    SDL_UpdateTexture(m->texture,
                      NULL,
                      m->tft_fb_act,
                      SDL_HOR_RES * sizeof(uint32_t));
#endif

    SDL_RenderClear(m->renderer);
    lv_disp_t *d = _lv_refr_get_disp_refreshing();
    if (d->driver->screen_transp) {
        SDL_SetRenderDrawColor(m->renderer,
                               0xff,
                               0,
                               0,
                               0xff);
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = SDL_HOR_RES;
        r.h = SDL_VER_RES;
        SDL_RenderDrawRect(m->renderer, &r);
    }

    /* Update the renderer with the texture containing the rendered image */
    SDL_RenderCopy(m->renderer,
                   m->texture,
                   NULL,
                   NULL);
    SDL_RenderPresent(m->renderer);
}

static int quit_filter(void *userdata, SDL_Event *event)
{
    (void)userdata;

    if (event->type == SDL_QUIT) {
        s_sdl_quit_qry = true;
    }

    return 1;
}

void sdl_init(void)
{
    /* Initialize the SDL */
    SDL_Init(SDL_INIT_VIDEO);

    SDL_SetEventFilter(quit_filter, NULL);

    window_create(&s_monitor);
#if SDL_DUAL_DISPLAY
    window_create(&s_monitor2);
    int x;
    int y;
    SDL_GetWindowPosition(s_monitor2.window, &x, &y);
    SDL_SetWindowPosition(s_monitor2.window, x + (SDL_HOR_RES * SDL_ZOOM) / 2 + 10, y);
    SDL_SetWindowPosition(s_monitor2.window, x - (SDL_HOR_RES * SDL_ZOOM) / 2 - 10, y);
#endif

    SDL_StartTextInput();

    lv_timer_create(sdl_event_handler, 10, NULL);
}

void sdl_display_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    const lv_coord_t hres = (disp_drv->physical_hor_res == -1) ? disp_drv->hor_res : disp_drv->physical_hor_res;
    const lv_coord_t vres = (disp_drv->physical_ver_res == -1) ? disp_drv->ver_res : disp_drv->physical_ver_res;

    /* Return if the area is out the screen */
    if ((area->x2 < 0)
        || (area->y2 < 0)
        || (area->x1 > hres - 1)
        || (area->y1 > vres - 1)) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if SDL_DOUBLE_BUFFERED
    s_monitor.tft_fb_act = (uint32_t *)color_p;
#else
    int32_t y;
#if (LV_COLOR_DEPTH != 24) && (LV_COLOR_DEPTH != 32) /* 32 is valid but support 24 for backward compatibility too */
    int32_t x;
    for (y = area->y1; (y <= area->y2) && (y < vres); y++) {
        for (x = area->x1; x <= area->x2; x++) {
            s_monitor.tft_fb[y * hres + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
#else
    uint32_t w = lv_area_get_width(area);
    for (y = area->y1; (y <= area->y2) && (y < vres); y++) {
        memcpy(&s_monitor.tft_fb[y * hres + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif
#endif /* SDL_DOUBLE_BUFFERED */

    s_monitor.sdl_refr_qry = true;

    /**
     * TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.
     */
    if (lv_disp_flush_is_last(disp_drv)) {
        monitor_sdl_refr(NULL);
    }

    /* IMPORTANT! It must be called to tell the system the flush is ready */
    lv_disp_flush_ready(disp_drv);
}

#if SDL_DUAL_DISPLAY
/**
 * @brief Flush a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdl_display_flush2(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    const lv_coord_t hres = (disp_drv->physical_hor_res == -1) ? disp_drv->hor_res : disp_drv->physical_hor_res;
    const lv_coord_t vres = (disp_drv->physical_ver_res == -1) ? disp_drv->ver_res : disp_drv->physical_ver_res;

    /* Return if the area is out the screen */
    if ((area->x2 < 0)
        || (area->y2 < 0)
        || (area->x1 > hres - 1)
        || (area->y1 > vres - 1)) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if SDL_DOUBLE_BUFFERED
    s_monitor2.tft_fb_act = (uint32_t *)color_p;

    s_monitor2.sdl_refr_qry = true;

    /* IMPORTANT! It must be called to tell the system the flush is ready */
    lv_disp_flush_ready(disp_drv);
#else

    int32_t y;
#if (LV_COLOR_DEPTH != 24) && (LV_COLOR_DEPTH != 32) /* 32 is valid but support 24 for backward compatibility too */
    int32_t x;
    for (y = area->y1; (y <= area->y2) && (y < vres); y++) {
        for (x = area->x1; x <= area->x2; x++) {
            s_monitor2.tft_fb[y * hres + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
#else
    uint32_t w = lv_area_get_width(area);
    for (y = area->y1; (y <= area->y2) && (y < vres); y++) {
        memcpy(&s_monitor2.tft_fb[y * hres + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif

    s_monitor2.sdl_refr_qry = true;

    /**
     * TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.
     */
    if (lv_disp_flush_is_last(disp_drv)) {
        monitor_sdl_refr(NULL);
    }

    /* IMPORTANT! It must be called to tell the system the flush is ready */
    lv_disp_flush_ready(disp_drv);
#endif
}
#endif

void sdl_mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv;

    /* Store the collected data */
    data->point.x = s_last_x;
    data->point.y = s_last_y;
    data->state = s_left_button_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

/**
 * @brief encoder (i.e. mouse wheel) ticks difference and pressed state
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 */
void sdl_mousewheel_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv;

    data->state = s_wheel_state;
    data->enc_diff = s_wheel_diff;
    s_wheel_diff = 0;
}

/**
 * @brief input from the keyboard.
 * @param indev_drv pointer to the related input device driver
 * @param data store the red data here
 */
void sdl_keyboard_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    (void)indev_drv;

    static bool dummy_read = false;
    const size_t len = strlen(s_buf);

    if (dummy_read) {
        /* Send a release manually */
        dummy_read = false;
        data->state = LV_INDEV_STATE_RELEASED;
        data->continue_reading = len > 0;
    } else if (len > 0) {
        /* Send the pressed character */
        dummy_read = true;
        data->state = LV_INDEV_STATE_PRESSED;
        data->key = s_buf[0];
        memmove(s_buf, s_buf + 1, len);
        data->continue_reading = true;
    }
}

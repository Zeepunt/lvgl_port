#ifndef __LV_PORT_H__
#define __LV_PORT_H__

#ifdef TARGET_DISPLAY_SDL
#include "sdl/sdl.h"
#endif

void lv_port_disp_init(void);

#endif /* __LV_PORT_H__ */
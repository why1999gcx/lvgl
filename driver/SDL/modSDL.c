#include "../include/common.h"
#include "SDL_monitor.h"
#include "SDL_mouse.h"

//////////////////////////////////////////////////////////////////////////////

STATIC mp_obj_t mp_lv_task_handler(mp_obj_t arg)
{  
    lv_task_handler();
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mp_lv_task_handler_obj, mp_lv_task_handler);

STATIC int tick_thread(void * data)
{
    (void)data;

    while(1) {
        SDL_Delay(1);   /*Sleep for 1 millisecond*/
        lv_tick_inc(1); /*Tell LittelvGL that 1 milliseconds were elapsed*/
        mp_sched_schedule((mp_obj_t)&mp_lv_task_handler_obj, mp_const_none);
    }

    return 0;
}

STATIC mp_obj_t mp_init_SDL()
{
    monitor_init();
    mouse_init();
    SDL_CreateThread(tick_thread, "tick", NULL);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mp_init_SDL_obj, mp_init_SDL);

DEFINE_PTR_OBJ(monitor_flush);
DEFINE_PTR_OBJ(monitor_fill);
DEFINE_PTR_OBJ(monitor_map);
DEFINE_PTR_OBJ(mouse_read);

STATIC const mp_rom_map_elem_t SDL_globals_table[] = {
        { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_SDL) },
        { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&mp_init_SDL_obj) },
        { MP_ROM_QSTR(MP_QSTR_monitor_flush), MP_ROM_PTR(&PTR_OBJ(monitor_flush))},
        { MP_ROM_QSTR(MP_QSTR_monitor_fill), MP_ROM_PTR(&PTR_OBJ(monitor_fill))},
        { MP_ROM_QSTR(MP_QSTR_monitor_map), MP_ROM_PTR(&PTR_OBJ(monitor_map))},
        { MP_ROM_QSTR(MP_QSTR_mouse_read), MP_ROM_PTR(&PTR_OBJ(mouse_read))},
};
         

STATIC MP_DEFINE_CONST_DICT (
    mp_module_SDL_globals,
    SDL_globals_table
);

const mp_obj_module_t mp_module_SDL = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_SDL_globals
};


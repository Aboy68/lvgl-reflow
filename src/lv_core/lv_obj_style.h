/**
 * @file lv_obj_style.h
 *
 */

#ifndef LV_OBJ_STYLE_H
#define LV_OB_STYLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#define LV_OBJ_STYLE_CACHE_STATE_INVALID    LV_STATE_ANY

/**********************
 *      TYPEDEFS
 **********************/
/* Can't include lv_obj.h because it includes this header file */
struct _lv_obj_t;

typedef enum {
    _LV_STYLE_STATE_CMP_SAME,           /*The style properties in the 2 states are identical*/
    _LV_STYLE_STATE_CMP_DIFF_REDRAW,    /*The differences can be shown with a simple redraw*/
    _LV_STYLE_STATE_CMP_DIFF_DRAW_PAD,  /*The differences can be shown with a simple redraw*/
    _LV_STYLE_STATE_CMP_DIFF_LAYOUT,    /*The differences can be shown with a simple redraw*/
} _lv_style_state_cmp_t;

typedef struct {
    lv_style_t * style;
    uint16_t state;
    uint8_t part;
    uint8_t is_local :1;
    uint8_t is_trans :1;
}lv_obj_style_t;

typedef struct {
    lv_obj_style_t * styles;
    uint32_t skip_trans :1;
    uint32_t style_cnt  :5;
    uint32_t cache_state :10;

    uint32_t cache_opa_set :1;
    uint32_t cache_radius_zero:1;
    uint32_t cache_pad_zero:1;
    uint32_t cache_transform_zero:1;
    uint32_t cache_blend_mode_zero:1;
    uint32_t cache_filter_zero:1;
    uint32_t cache_clip_corner_enable:1;

    uint32_t cache_bg_opa_cover:1;

    uint32_t cache_border_width_zero:1;
    uint32_t cache_border_post_enable:1;

    uint32_t cache_text_any_set:1;

    uint32_t cache_img_opa_cover:1;
    uint32_t cache_outline_width_zero:1;
    uint32_t cache_shadow_width_zero:1;
    uint32_t cache_content_text_zero:1;
    uint32_t cache_bg_img_src_zero:1;
}lv_obj_style_list_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the object related style manager module.
 * Called by LVGL in `lv_init()`
 */
void _lv_obj_style_init(void);

/**
 * Add a style to an object.
 * @param obj       pointer to an object
 * @param part      a part of the object to which the style should be added E.g. `LV_PART_MAIN` or `LV_PART_KNOB`
 * @param state     a state or combination of states to which the style should be assigned
 * @param style     pointer to a style to add
 * @example         lv_obj_add_style_no_refresh(slider, LV_PART_KNOB, LV_STATE_PRESSED, &style1);
 */
void lv_obj_add_style(struct _lv_obj_t * obj, uint32_t part, uint32_t state, lv_style_t * style);

/**
 * Add a style to an object.
 * @param obj       pointer to an object
 * @param part      a part of the object from which the style should be removed E.g. `LV_PART_MAIN` or `LV_PART_KNOB`
 * @param state     a state or combination of states from which the style should be removed
 * @param style     pointer to a style to remove
 * @example lv_obj_remove_style(obj, LV_PART_ANY, LV_STATE_ANY, &style); //Remove a specific style
 * @example lv_obj_remove_style(obj, LV_PART_MAIN, LV_STATE_ANY, &style); //Remove all styles from the main part
 * @example lv_obj_remove_style(obj, LV_PART_ANY, LV_STATE_ANY, NULL); //Remove all styles
 */
void lv_obj_remove_style(struct _lv_obj_t * objj, uint32_t part, uint32_t state, lv_style_t * style);

/**
 * Notify all object if a style is modified
 * @param style     pointer to a style. Only the objects with this style will be notified
 *                  (NULL to notify all objects)
 */
void lv_obj_report_style_change(lv_style_t * style);

/**
 * Notify an object and its children about its style is modified.
 * @param obj       pointer to an object
 * @param part      the part whose style was changed. E.g. `LV_PART_ANY`, `LV_PART_MAIN`
 * @param prop      `LV_STYLE_PROP_ALL` or an `LV_STYLE_...` property.
 *                  It is used to optimize what needs to be refreshed.
 */
void lv_obj_refresh_style(struct _lv_obj_t * obj, lv_part_t part, lv_style_prop_t prop);

/**
 * Enable or disable automatic style refreshing when a new style is added/removed to/from an object
 * or any other style change happens.
 * @param en        true: enable refreshing; false: disable refreshing
 */
void lv_obj_enable_style_refresh(bool en);

/**
 * Get the value of a style property. The current state of the object will be considered.
 * Inherited properties will be inherited.
 * If a property is not set a default value will be returned.
 * @param obj       pointer to an object
 * @param part      a part from which the property should be get
 * @param prop      the property to get
 * @return          the value of the property.
 *                  Should be read from the correct field of the `lv_style_value_t` according to the type of the property.
 */
lv_style_value_t lv_obj_get_style_prop(const struct _lv_obj_t * obj, uint8_t part, lv_style_prop_t prop);

/**
 * Set local style property on an object's part and state.
 * @param obj       pointer to an object
 * @param part      a part to which the property should be added
 * @param state     a state to which the property should be added
 * @param prop      the property
 * @param value     value of the property. The correct element should be set according to the type of the property
 */
void lv_obj_set_local_style_prop(struct _lv_obj_t * obj, uint32_t part, uint32_t state, lv_style_prop_t prop, lv_style_value_t value);

/**
 * Remove a local style property from a part of an object with a given state.
 * @param obj       pointer to an object
 * @param part      the part of the object which style property should be removed.
 * @param state     the state from which the property should be removed.
 * @param prop      a style property to remove.
 * @return true     the property was found and removed; false: the property was not found
 */
bool lv_obj_remove_local_style_prop(struct _lv_obj_t * obj, uint32_t part, uint32_t state, lv_style_prop_t prop);

/**
 * Used internally to create a style tarnsition
 * @param obj
 * @param prop
 * @param part
 * @param prev_state
 * @param new_state
 * @param time
 * @param delay
 * @param path
 */
void _lv_obj_style_create_transition(struct _lv_obj_t * obj, lv_style_prop_t prop, uint8_t part, lv_state_t prev_state,
                                       lv_state_t new_state, uint32_t time, uint32_t delay, const lv_anim_path_t * path);

/**
 * Used internally to compare the appearance of an object in 2 states
 * @param obj
 * @param state1
 * @param state2
 * @return
 */
_lv_style_state_cmp_t _lv_obj_style_state_compare(struct _lv_obj_t * obj, lv_state_t state1, lv_state_t state2);

/**
 * Fade in an an object and all its children.
 * @param obj       the object to fade in
 * @param time      time of fade
 * @param delay     delay to start the animation
 */
void lv_obj_fade_in(struct _lv_obj_t * obj, uint32_t time, uint32_t delay);

/**
 * Fade out an an object and all its children.
 * @param obj       the object to fade out
 * @param time      time of fade
 * @param delay     delay to start the animation
 */
void lv_obj_fade_out(struct _lv_obj_t * obj, uint32_t time, uint32_t delay);

#include "lv_obj_style_gen.h"

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TEMPL_H*/
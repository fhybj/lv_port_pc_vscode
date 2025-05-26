// coffee_machine_ctrl/ui/theme/app_theme.c

#include "app_theme.h"

// 定义主颜色和副颜色
#define COFFEE_MACHINE_PRIMARY_COLOR    lv_color_hex(0x001824) // 主颜色：深色背景
#define COFFEE_MACHINE_SECONDARY_COLOR  lv_color_hex(0x16DFC7) // 副颜色：青绿色

// 定义全局的独立样式实例
lv_style_t style_default_font;
lv_style_t style_status_bar_bg; // 状态栏背景样式
static lv_theme_t coffee_theme;

lv_font_t *lv_default_small_font;
lv_font_t *lv_default_normal_font;
lv_font_t *lv_default_large_font;

// ---------------------------------------------------------------------------------
// 主题应用回调函数
// 当一个 LVGL 对象被创建时，LVGL 会调用当前主题的 apply_cb 来为其应用样式
static void coffee_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj) {

    // 设置所有对象的默认背景色和字体
    // LV_OBJ_CLASS 是所有 LVGL 对象的基类
    if (lv_obj_check_type(obj, &lv_obj_class)) {
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_PRIMARY_COLOR, LV_PART_MAIN); // LV_PART_MAIN 是默认部分
        // lv_style_set_text_font(obj, lv_default_normal_font);               // 默认字体
        lv_obj_set_style_text_color(obj, lv_color_hex(0xDBDBDD), LV_PART_MAIN);   // 默认字体颜色
    }

    // 按钮 (lv_btn)
    if (lv_obj_check_type(obj, &lv_btn_class)) {
        // 默认状态
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x58585A), LV_STATE_DEFAULT); // 按钮默认背景色
        lv_obj_set_style_bg_opa(obj, LV_OPA_20, LV_STATE_DEFAULT);               // 20% 透明度
        lv_obj_set_style_text_color(obj, lv_color_hex(0xDBDBDD), LV_STATE_DEFAULT); // 字体颜色
        lv_obj_set_style_radius(obj, 8, LV_STATE_DEFAULT);                       // 圆角

        // 按下状态
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_STATE_PRESSED); // 按下颜色
        lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_PRESSED);                     // 100% 透明度
        lv_obj_set_style_text_color(obj, lv_color_white(), LV_STATE_PRESSED);           // 字体颜色

        // 禁用状态
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x2C3439), LV_STATE_DISABLED); // 禁用颜色
        lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_DISABLED);              // 100% 透明度
        lv_obj_set_style_text_color(obj, lv_color_hex(0x8C8C8E), LV_STATE_DISABLED); // 字体颜色
    }

    // 标签 (lv_label)
    // 标签的默认字体和颜色已经在 LV_OBJ_CLASS 中设置，如果需要特定标签，可以在这里覆盖
    // 例如，如果某个标签是标题，可以这样设置：
    // if (lv_obj_get_user_data(obj) == SOME_TITLE_ID) {
    //    lv_obj_set_style_text_font(obj, &lv_font_oppo_sans_28, LV_PART_MAIN);
    // }

    // 文本输入框 (lv_textarea)
    if (lv_obj_check_type(obj, &lv_textarea_class)) {
        lv_obj_set_style_bg_color(obj, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_text_color(obj, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_border_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_PART_MAIN);
        lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN);
        lv_obj_set_style_radius(obj, 5, LV_PART_MAIN);
        lv_obj_set_style_pad_all(obj, 5, LV_PART_MAIN);
    }
    
    // 进度条 (lv_bar)
    if (lv_obj_check_type(obj, &lv_bar_class)) {
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x2C3439), LV_PART_MAIN); // 背景轨道色
        lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_MAIN);
        lv_obj_set_style_height(obj, 10, LV_PART_MAIN);

        // 指示器样式，使用 LV_PART_INDICATOR
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_PART_INDICATOR);
        lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);
    }

    // 下拉框 (lv_dropdown)
    if (lv_obj_check_type(obj, &lv_dropdown_class)) {
        lv_obj_set_style_bg_color(obj, lv_color_hex(0x2C3439), LV_PART_MAIN);
        lv_obj_set_style_text_color(obj, lv_color_hex(0xDBDBDD), LV_PART_MAIN);
        lv_obj_set_style_border_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_PART_MAIN);
        lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN);
        lv_obj_set_style_radius(obj, 5, LV_PART_MAIN);
        lv_obj_set_style_pad_all(obj, 8, LV_PART_MAIN);
    }

    // Tabview 按钮 (lv_tabview_get_tab_btns(obj)) - 在 LVGL v8 中，Tabview 的按钮实际上是内部管理的，
    // 其样式通常通过 LV_PART_ITEMS 和 LV_PART_MAIN 设置在 Tabview 对象本身。
    // 如果需要更精细控制，可能需要在创建 Tabview 后获取其内部按钮容器再设置。
    if (lv_obj_check_type(obj, &lv_tabview_class)) {
        // Tab 按钮的默认样式 (未选中) - 应用在 LV_PART_ITEMS
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_PRIMARY_COLOR, LV_PART_ITEMS);
        lv_obj_set_style_text_color(obj, lv_color_hex(0x8C8C8E), LV_PART_ITEMS); // 未选中字体颜色
        lv_obj_set_style_pad_hor(obj, 15, LV_PART_ITEMS);
        lv_obj_set_style_pad_ver(obj, 8, LV_PART_ITEMS);

        // Tab 按钮的选中状态 - 应用在 LV_PART_ITEMS + LV_STATE_CHECKED
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_STATE_CHECKED | LV_PART_ITEMS);
        lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_STATE_CHECKED | LV_PART_ITEMS);
        lv_obj_set_style_text_color(obj, lv_color_white(), LV_STATE_CHECKED | LV_PART_ITEMS); // 选中字体颜色
        
        // Tabview 指示器 (底部线条)
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_SECONDARY_COLOR, LV_PART_INDICATOR);
        lv_obj_set_style_height(obj, 3, LV_PART_INDICATOR);
    }

    // 菜单 (lv_menu)
    // LVGL v8 的 lv_menu 样式定制比较特殊。
    // 其主内容和侧边栏的背景和文本样式通常通过 LV_PART_MAIN 或在创建时手动设置。
    // 在 apply_cb 中直接修改 lv_menu_get_sidebar 这种方式不适用。
    // 建议在创建 lv_menu 时，手动设置其内部组件的样式，或者为其设置 user_data
    // 并在 apply_cb 中根据 user_data 来识别并设置样式。
    if (lv_obj_check_type(obj, &lv_menu_class)) {
        // 例如，设置菜单的默认背景和文本颜色
        lv_obj_set_style_bg_color(obj, COFFEE_MACHINE_PRIMARY_COLOR, LV_PART_MAIN);
        lv_obj_set_style_text_color(obj, lv_color_hex(0xDBDBDD), LV_PART_MAIN);
        // 对于菜单内部的标题、项，可能需要更具体的处理，例如通过获取子对象并设置样式
        // 这通常不在主题的 apply_cb 中直接完成，因为 apply_cb 针对单个对象，
        // 而 lv_menu 的子对象可能不是直接可访问的通用类型。
    }

    // 其他需要独立样式的情况，例如状态栏的背景色，
    // 由于 apply_cb 无法直接识别哪个是状态栏对象，我们需要在创建时显式添加。
    // lv_obj_add_style(g_status_bar_obj, &style_status_bar_bg, LV_PART_MAIN);
}
// ---------------------------------------------------------------------------------

// 全局样式初始化 (用于无法通过 apply_cb 统一设置的特殊情况，例如状态栏背景)
static void init_custom_styles(void) {
    lv_style_init(&style_status_bar_bg);
    lv_style_set_bg_color(&style_status_bar_bg, COFFEE_MACHINE_PRIMARY_COLOR); // 状态栏背景色与主颜色一致
    lv_style_set_bg_opa(&style_status_bar_bg, LV_OPA_0);
    lv_style_set_pad_all(&style_status_bar_bg, 5);

    lv_style_init(&style_default_font);
    lv_style_set_text_font(&style_default_font, lv_default_normal_font);
}

void app_theme_init(void) {
    

    lv_disp_t * disp = lv_disp_get_default();
    if (!disp) {
        LV_LOG_ERROR("No default display found for theme initialization!");
        return;
    }

    lv_default_small_font = lv_tiny_ttf_create_file("A:/Users/binake/Workspace/lvgl/lv_port_pc_vscode/coffee/ui/assets/fonts/OPPO Sans 4.0.ttf", 14);
    lv_default_normal_font = lv_tiny_ttf_create_file("A:/Users/binake/Workspace/lvgl/lv_port_pc_vscode/coffee/ui/assets/fonts/OPPO Sans 4.0.ttf", 20);
    lv_default_large_font = lv_tiny_ttf_create_file("A:/Users/binake/Workspace/lvgl/lv_port_pc_vscode/coffee/ui/assets/fonts/OPPO Sans 4.0.ttf", 30);
    
    // 创建一个基于默认主题的新主题实例
    // LVGL v8.4 的 lv_theme_create_from_default_values 函数：
    // lv_theme_create_from_default_values(disp, color_primary, color_secondary, is_dark, font)
    lv_theme_t * th_act = lv_disp_get_theme(disp);
    coffee_theme = *th_act;

        // 设置主题的应用回调
         lv_theme_set_parent(&coffee_theme, th_act);
        lv_theme_set_apply_cb(&coffee_theme, coffee_theme_apply_cb);
        // 将我们定制的主题设置为显示器的当前主题
        lv_disp_set_theme(disp, &coffee_theme);
        LV_LOG_INFO("Coffee theme initialized successfully.");

        // 首先初始化所有独立的样式
    init_custom_styles();
}
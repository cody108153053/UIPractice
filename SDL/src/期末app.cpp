
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

#include "outside.h"

#include "app.h"

std::string const App::tiles_[] = {
    "barn",
    "barn_back",
    "booth",
    "dirt",
    "fence_h_16",
    "fence_h_24",
    "fence_v_16",
    "fence_v_24",
    "flower",
    "fountain",
    "grass",
    "stump",
    "timber",
    "tree",
    "water",
};

// 函數 (方法) 的實作 (implementations)

/**
 *  App constructor.  Initialize the first SDL window and its
 *  renderer.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
App::App(const std::string& name):
    renderer_{ nullptr }, surface_{ nullptr}, window_{ nullptr },
    outside_{ nullptr },
    end_{ false }, invoked_{ name } {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // initialize SDL
        SDL_LogCritical(                // Video subsystem.
            SDL_LOG_CATEGORY_VIDEO, "Error: %s\n", SDL_GetError()
        );
    }

    if (TTF_Init() == -1) { // initialize SDL_ttf extension.
        SDL_LogCritical(
            SDL_LOG_CATEGORY_VIDEO, "Error: %s\n", SDL_GetError()
        );
    }

    window_ = SDL_CreateWindow( // create the main SDL window.
        name.c_str(), 100, 100, 800, 640, SDL_WINDOW_SHOWN
    );

    if (!window_) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, "Error: %s\n", SDL_GetError()
        );
    }

    renderer_ = SDL_CreateRenderer(window_, -1, 0); // create the renderer.

    if (!renderer_) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, "Error: %s\n", SDL_GetError()
        );
    }

    outside_ = new Outside(renderer_);
} // App::App()

/**
 *  App destructor.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
App::~App() {
    delete outside_;

    SDL_DestroyWindow(window_);

    TTF_Quit();

    SDL_Quit();
} // App::~App()

/**
 *  Register a Event Listener。
 *
 *  @param const std::string&: The reference of Event name.
 *  @param const Listener *: Pointer to the Event Listner.
 *  @return none.
 *  @since  0.1.0
 **/
void App::add_listener(const std::string& event, Listener *listener) {
    listener_[event] = listener;
} // App::add_listener

/**
 *  Invoke registered event handling function for passed_in event.
 *
 *  @param std::shared_ptr<Event>: Pointer to the Event object.
 *  @return int: 0: successuf.
 *  @since  0.1.0
 **/
int App::on(std::shared_ptr<Event> e) {
    return std::invoke(table_[e->event], this, e);
} // App::on()

/**
 *  Starts the game.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::start() {
    loop_();
} // App::start()

/**
 *  Print the App's version info.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::version() {
    std::cout
        << ENV_SYMBOL(PROJECT_NAME) << " "
        << ENV_SYMBOL(VERSION_MAJOR) << "."
        << ENV_SYMBOL(VERSION_MINOR) << "."
        << ENV_SYMBOL(VERSION_PATCH) << "b"
        << ENV_SYMBOL(BUILD_NUMBER) << ". "
        << "bulit at "
        << ENV_SYMBOL(BUILD_TIME) << ", "
        << ENV_SYMBOL(BUILD_DATE)
        << std::endl;
} // App::version()

/**
 *  Event processing loop.  Pop out Event from event queue,
 *  passing it to corresponding listeners, loop.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::dispatch_() {
    while (!queue_.empty()) {
        std::shared_ptr<Event> e = queue_.front();
        queue_.pop();

        listener_[e->event]->on(e);
    }
} // App::dispatch_()

/**
 *  Get user input from keyboard/mouse and put into event queue for
 *  later processing.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::get_input_() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {   // 取得使用者輸入
        switch (e.type) {
            case SDL_MOUSEBUTTONUP:
                get_mouse_(e);

                break;

            case SDL_WINDOWEVENT:
                if (e.window.event != SDL_WINDOWEVENT_CLOSE) {
                    break;
                }

            case SDL_QUIT:
                end_ = true;

                break;

            default:
                break;
        }
    }
} // App::get_input_()

/**
 *  Get mouse event, traslate into MouseEvent object, and put into
 *  event queue for later processing.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::get_mouse_(SDL_Event& e) {
    switch (e.button.button) {
        case SDL_BUTTON_LEFT:
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "Mouse Left Button clicked!\n"
            );

            queue_.push(
                std::make_shared<Event>(
                    "left_click", e.motion.x, e.motion.y, 0
                )
            );

            break;

        case SDL_BUTTON_RIGHT:
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "Mouse Right Button clicked!\n"
            );

            queue_.push(
                std::make_shared<Event>(
                    "right_click", e.motion.x, e.motion.y, 0
                )
            );

            break;

        default:
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION, "Mouse Button clicked!\n"
            );

            break;
    }
} // App::get_mouse_()

void App::layout_land_(int width, int height) {
    for (int x = 0; x < width * 32; x += 32) {
        for (int y = 0; y < height * 32;  y += 32) {
            SDL_Rect dst = { x, y, 0, 0 };

            outside_->tile("dirt", renderer_, &dst);
        }
    }
} // layout_land_()

/**
 *  Put objects on scene map.
 *  @return none.
 *  @since  0.1.0
 */
void App::layout_objects_(int off_x, int off_y, int (&objects)[12][12]) {
    for (int x = 0; x < 12; x ++) {
        for (int y = 0; y < 12;  y ++) {
            int tile = objects[y][x];

            if (tile >= 0) {
                SDL_Rect dst = { x * 32 + off_x, y * 32 + off_y, 0, 0 };

                outside_->tile(App::tiles_[tile], renderer_, &dst);
            }
        }
    }
} // layout_objects()

/**
 *  Use the whole spritesheet as background.
 *  @return none.
 *  @since  0.1.0
 */
//左下
void App::layout_map_() {
    int forest_ld[12][12] = {
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      {  6, 11, -1, -1, -1,  8, -1, 13, -1, -1, -1, -1, },
      {  6, 11, 11, -1, -1, -1, 13,  13, 13, -1, -1,  8, },
      {  6, -1, -1, -1, -1, 13, 13, 13, 13, 13, -1, -1, },
      {  6, -1,  0, -1, -1, -1, 13, 13, 13,  8, -1, -1, },
      {  6, -1, -1, -1, -1, -1, 13, 13, 13,  8, -1, -1, },
      {  6, -1, -1, -1, -1, -1, 13, 13, 13, -1, -1, -1, },
      {  6, -1, -1, -1, -1, 13, 13, 13, 13, 13, -1, -1, },
      {  6, -1,  8, -1,  8, 13, 13, 13, 13, 13, -1,  8, },
      {  1, -1, -1, -1,  8, -1, -1, -1, -1, -1, -1,  8, },
      { -1, -1, -1,  4,  4,  4,  4,  4,  4,  4,  4,  4, },
    };
    //左上
    int forest_lu[12][12] = {
      { -1, -1, -1, -1, -1, -1, -1, -1, -1 - 1, -1, -1, },
      {  6, 13, 13, 11, -1,  8, -1, -1,  2, -1, -1, -1, },
      {  6,  8, -1, 11,  8,  8, -1, -1, -1, -1, -1, -1, },
      {  6,  2, -1, -1, -1, 11, 11, -1, -1, -1, -1, -1, },
      {  6, -1, -1, -1, 11, 14, 14, 11, -1, -1, -1, -1, },
      {  6, -1, -1, 11, 14, 14, 14, 14, 11, -1, -1, -1, },
      {  6, -1, 11, 14, 14, 14, 14, 14, 14, 11, -1, -1, },
      {  6, 11, 14, 14, 14, 14, 14, 14, 14, 14, 11, -1, },
      {  6, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, -1, },
      { -1, -1, -1, 10, -1, 10, -1, 10, -1, 10, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
    };
    //右下
    int forest_rd[12][12] = {
      { -1, -1, -1, -1, -1,  8,  8,  8, -1, -1, -1, -1, },
      { -1, -1, -1, -1, 13, 13, 11, 13, 11, 11, -1, -1, },
      { -1, 13, -1, -1, -1, -1, 11, 13, -1, -1, -1, -1, },
      { -1, 13, 13, -1, -1, 13, 13, 13, -1, -1,  0, -1, },
      { -1, -1, -1, 13, 13, 13, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, 13, 13, 11, -1, -1, 13, -1, -1, -1, },
      { -1, -1, 13, 13,  8, 13, 13, -1, -1, -1, -1, -1, },
      { -1, 13, 13, -1, -1, -1, -1, 14, 14, 14, 14, 14, },
      { -1, -1, -1, -1, -1, -1, -1, 14, 14, 14, 14, 14, },
      { -1, -1, -1, -1,  0, -1, -1, 14, 14, 14, 14, 14, },
      { -1, -1, -1, -1, -1, -1, -1, 14, 14, 14, 14, 14, },
      { -1, -1, -1, -1, -1, -1, -1, 14, 14, 14, 14, 14, },
    };
    //右上
    int objects[12][12] = {
      {  8, 13, 13, -1,  8, -1,  8,  8, -1, 11, -1, -1, },
      { -1, 13, 13,  8,  8, -1, -1, -1, -1, 11, 11, -1, },
      { -1, -1, 13, -1, -1, -1,  0, -1, -1, -1, 11, -1, },
      { -1, -1, -1, 13, -1, -1, -1, -1, -1,  8,  8,  8, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  8, },
      { -1, 12, -1, 12, -1,  8,  8, -1, -1, -1, -1,  8, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  8, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
    };

    layout_land_(25, 20);
    layout_objects_(0, 0, forest_lu);
    layout_objects_(0, 8 * 32, forest_ld);
    layout_objects_(13 * 32, 8 * 32, forest_rd);
    layout_objects_(13 * 32, 2 * 32, objects);
} // App::layout_map_()

/**
 *  The main-loop of the game.  Game over when the loop ends.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::loop_() {
    while (!end_) {  // 程式主迴圈 (main-loop)
        SDL_RenderClear(renderer_);     // 清除畫面

        layout_map_();                  // 繪製地圖

        SDL_RenderPresent(renderer_);   // 呈現畫面

        get_input_();   // 取得使用者輸入；轉換成 Event 物件
                        // 放進 Event queue (佇列)

        dispatch_();    // 處理 Event queue 裡的事件
    } // od
} // App::loop_()

// app.cpp

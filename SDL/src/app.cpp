
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "SDL.h"

#include "app.h"

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
    page_x_{ 0 }, page_x_max_{ 0 },
    page_y_{ 0 }, page_y_max_{ 0 },
    renderer_{ nullptr }, surface_{ nullptr}, window_{ nullptr },
    sprite_world_{ nullptr },
    end_{ false }, invoked_{ name } {
    int width;
    int height;

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

    sprite_world_ = new SpriteSheet(
        renderer_, "../resources/png/overworld.png"
    );

    sprite_world_->dimension(width, height);

    page_x_max_ = (width + (64 * 6) -1) / (64 * 6);
    page_y_max_ = (height + (64 * 4) - 1) / (64 * 4);

    text_ = new Text("../resources/font/jason_writing_1.ttf");

    table_["left_click"] = &App::on_left_click_;
    table_["right_click"] = &App::on_right_click_;
} // App::App()

/**
 *  App destructor.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
App::~App() {
    delete sprite_world_;
    delete text_;

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
 *  Render the 'Hello'... messages to the screen.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 **/
void App::hello() {
    Text text_ = Text("../resources/font/jason_writing_1.ttf");
    SDL_Texture *hello = text_.get_text(renderer_, "哭巄摳巄幾匹馬?");

    SDL_Rect rect;
    rect.x = (20 - 10) / 2;
    rect.y = (40 - 20) / 3;
    rect.w = 80;
    rect.h = 160;

    SDL_Rect rect2;
    rect.x = (500 - 400) / 2;
    rect.y = (750- 85) / 3;
    rect.w = 777;
    rect.h = 666;

    SDL_RenderCopy(renderer_, hello, &rect2, &rect);
} // App::hello()

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

/**
 *  Use the whole spritesheet as background.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 */
void App::layout_background_() {
    sprite_world_->tile(renderer_, nullptr, nullptr);
} // App::layout_background_()

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

        layout_background_();           // 繪製背景
        hello();        // 印出訊息
//        tile_page_();

        SDL_RenderPresent(renderer_);   // 呈現畫面

        get_input_();   // 取得使用者輸入；轉換成 Event 物件
                        // 放進 Event queue (佇列)

        dispatch_();    // 處理 Event queue 裡的事件
    } // od
} // App::loop_()

/**
 *  Flip pages horizotally. The sprite_sheet might not fit in one
 *  screen. Mouse's `left-click` is used to flip pages horizontally
 *  while `right-click` flip vertically.
 *
 *  @param std::shared_ptr<Event>: pointer to the 'left-click' event
 *                                 object.
 *  @return int: o: success.
 *  @since  0.1.0
 **/
int App::on_left_click_(std::shared_ptr<Event> e) {
    page_x_ = (page_x_ + 1) % page_x_max_;

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "App::on_click called: %d:%d",
        page_x_, page_x_max_
    );

    return 0;
} // App::on_left_click_()

/**
 *  Flip pages vertically. The sprite_sheet might not fit in one
 *  screen.  Mouse's `right-click` is used to flip pages vertically
 *  while `left-click` flip horizontally.
 *
 *  @param std::shared_ptr<Event>: pointer to the 'right-click' event
 *                                 object.
 *  @return int: o: success.
 *  @since  0.1.0
 **/
int App::on_right_click_(std::shared_ptr<Event> e) {
    page_y_ = (page_y_ + 1) % page_y_max_;

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION, "App::on_click called: %d:%d",
        page_y_, page_y_max_
    );

    return 0;
} // App::on_right_click_()

/**
 *  Dump sprites (tiles) of the spritesheet to screen in
 *  6 (columns) x 4 (rows) manner.
 *
 *  @param none.
 *  @return none.
 *  @since  0.1.0
 */
void App::tile_page_() {
    for (int i = 0; i < 6; i ++) {
        int col = page_x_ * 6 + i;

        for (int j = 0; j < 4; j ++) {
            int row = page_y_ * 4 + j;

            tile_quad_(col, row);
        }
    }
} // App::tile_page_();

/**
 *  Output a 64x64 sprite (tile) at spritesheet's `col` x `row` to
 *  the screen.
 *
 *  @param int col the column in spritesheet
 *  @param int row the row in spritesheet
 *  @return none.
 *  @since  0.1.0
 */
void App::tile_quad_(int col, int row) {
    std::stringstream ss;

    SDL_Rect rect;
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture *xy_hint = nullptr;

    std::string index;

    ss << std::setfill('0') << std::setw(2) << col
       << "x"
       << std::setfill('0') << std::setw(2) << row;
    ss >> index;

    xy_hint = text_->get_text(renderer_, index);
    SDL_QueryTexture(xy_hint, nullptr, nullptr, &rect.w, &rect.h);

    rect.x = (col - page_x_ * 6) * 128 + (128 - rect.w) / 2 + 16;
    rect.y = (row - page_y_ * 4) * (rect.h + 64 + 16) + 16;

    SDL_RenderCopy(renderer_, xy_hint, nullptr, &rect);

    src.x = (col * 4) * 16;
    src.y = (row * 4) * 16;
    src.w = 64;
    src.h = 64;

    dst.x = (col - page_x_ * 6) * 128 + 64 / 2 + 16;
    dst.y = rect.y + rect.h + 8;
    dst.w = 64;
    dst.h = 64;

    sprite_world_->tile(renderer_, &src, &dst);
} // App::tile_quad_()

// app.cpp


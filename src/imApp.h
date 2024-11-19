// imApp, a simple wrapper around sokol_app, sokol_gfx, sokol_imgui, and sokol_log
// to make it easier to create simple applications with sokol libraries.
//
// Author: Ben McAvoy
//
// Example usage:
// ```cpp
// #include "imApp.h"
// #include <imgui.h>
//
// sapp_desc sokol_main(int argc, char *argv[]) {
//   imApp::SetDrawCallback([]() {
//     ImGui::Begin("Hello, world!");
//     ImGui::Text("This is some useful text.");
//     ImGui::End();
//   });
//
// 	imApp::SetTitle("imApp example");
//
//   return imApp::GetDesc();
// }
// ```

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_imgui.h"
#include "sokol_log.h"

#include <functional>

// The main namespace for the imApp library
namespace imApp {

// Sokol state
static struct {
  sg_pass_action pass_action;
} state;

//== Callbacks ==//
static inline std::function<void(void)> initCallback;
static inline std::function<void(const sapp_event *)> eventCallback;
static inline std::function<void(void)> drawCallback;
static inline std::function<void(void)> cleanupCallback;

// Forward declarations
static void init();
static void frame();
static void cleanup();
static void event(const sapp_event *ev);

// The main app descriptor, which will be used to initialize the app
static sapp_desc app_desc = {
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .event_cb = event,
    .width = 800,
    .height = 600,
    .window_title = "imApp",
    .icon = {.sokol_default = true},
    .logger = {.func = slog_func},
};

// Called when the app is initialized
static void init(void) {
  if (initCallback) {
    initCallback();
  }

  sg_setup(sg_desc{
      .logger = {.func = slog_func},
      .environment = sglue_environment(),
  });
  simgui_setup(simgui_desc_t{0});

  // initial clear color
  state.pass_action =
      (sg_pass_action){.colors = {{.load_action = SG_LOADACTION_CLEAR,
                                   .clear_value = {0.0f, 0.5f, 1.0f, 1.0}}}};
}

// Called every frame
static void frame(void) {
  simgui_new_frame(simgui_frame_desc_t{
      .width = sapp_width(),
      .height = sapp_height(),
      .delta_time = sapp_frame_duration(),
      .dpi_scale = sapp_dpi_scale(),
  });

  /*=== UI CODE STARTS HERE ===*/
  if (imApp::drawCallback)
    drawCallback();
  /*=== UI CODE ENDS HERE ===*/

  sg_begin_pass(
      sg_pass{.action = state.pass_action, .swapchain = sglue_swapchain()});
  simgui_render();
  sg_end_pass();
  sg_commit();
}

// Called when the app is cleaning up
static void cleanup(void) {
  if (cleanupCallback) {
    cleanupCallback();
  }

  simgui_shutdown();
  sg_shutdown();
}

// Used whenever sokol needs to handle an event
static void event(const sapp_event *ev) {
  if (eventCallback) {
    eventCallback(ev);
  }

  simgui_handle_event(ev);
}

// Set the drawing callback which will be called every frame
static void SetDrawCallback(std::function<void(void)> callback) {
  drawCallback = callback;
}

// Set the event callback which will be called whenever an event is received
static void SetEventCallback(std::function<void(const sapp_event *)> callback) {
  eventCallback = callback;
}

// Set the cleanup callback which will be called when the app is cleaning up
static void SetCleanupCallback(std::function<void(void)> callback) {
  cleanupCallback = callback;
}

// Set the init callback which will be called when the app is initialized
static void SetInitCallback(std::function<void(void)> callback) {
  initCallback = callback;
}

// Set the app title
static void SetTitle(const char *title) { app_desc.window_title = title; }

// Set the app width
static void SetWidth(int width) { app_desc.width = width; }

// Set the app height
static void SetHeight(int height) { app_desc.height = height; }

// Get the app descriptor to be used in the entry point
static sapp_desc GetDesc() { return app_desc; }
} // namespace imApp

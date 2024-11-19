#include "imApp.h"
#include <imgui.h>

sapp_desc sokol_main(int argc, char *argv[]) {
  imApp::SetDrawCallback([]() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::End();
  });

	imApp::SetTitle("imApp example");

  return imApp::GetDesc();
}

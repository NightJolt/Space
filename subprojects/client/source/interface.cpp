#include "interface.h"

namespace {
    bool brush_window_enabled = false;
    std::array<float, 3> selected_color = { 1.f, 1.f, 1.f };
    void show_brush_window(space::state_t& state) {
        selected_color = state.tool.color.to_float();

        ImGui::Begin("Brush", &brush_window_enabled);

            if (ImGui::ColorPicker3("color picker", (float*)&selected_color)) {
                state.tool.color = fun::rgb_t::from_float(selected_color[0], selected_color[1], selected_color[2]);
            }

        ImGui::End();
    }

    bool tools_window_enabled = false;
    void show_tools_window(space::state_t& state) {
        ImGui::Begin("Tools", &tools_window_enabled);
        
            if (ImGui::Button("B")) {
                state.tool.mode = space::tool_mode_t::brush;
            }

            if (ImGui::Button("E")) {
                state.tool.mode = space::tool_mode_t::eraser;
            }

            if (ImGui::Button("I")) {
                state.tool.mode = space::tool_mode_t::eyedropper;
            }

            if (ImGui::Button("G")) {
                state.tool.mode = space::tool_mode_t::bucket;
            }
            
            ImGui::SameLine();

        ImGui::End();
    }

    void draw_nav_bar(space::state_t& state) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("World")) {
                // if (!state.client.check_connection() && ImGui::MenuItem("Connect")) {
                //     connect_to_server_window_enabled = true;
                // }
                
                // if (state.client.check_connection() && ImGui::MenuItem("Disconnect")) {
                //     state.client.disconnect();

                //     state.canvas.clear();
                // }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Brush", nullptr, brush_window_enabled)) {
                    brush_window_enabled = !brush_window_enabled;
                }

                if (ImGui::MenuItem("Tools", nullptr, tools_window_enabled)) {
                    tools_window_enabled = !tools_window_enabled;
                }
                
                // if (ImGui::MenuItem("Console")) {
                //     console_window_enabled = true;
                // }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}

void space::interface::draw(state_t& state) {
    draw_nav_bar(state);

    // if (connect_to_server_window_enabled) draw_connect_to_server_window(state);
    if (brush_window_enabled) show_brush_window(state);
    if (tools_window_enabled) show_tools_window(state);
    // if (console_window_enabled) draw_console_window(state);
}
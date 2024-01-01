#include "interface.h"

#include "connection.h"

#include <FunEngine2D/core/include/rpc/rpc.h>

namespace {
    bool brush_window_enabled = false;
    std::array<float, 3> selected_color = { 1.f, 1.f, 1.f };
    void show_brush_window(space::state_t&);

    bool tools_window_enabled = false;
    void show_tools_window(space::state_t&);

    void draw_nav_bar(space::state_t&);bool connect_to_server_window_enabled = false;

    char ip_input[32] = "localhost";
    char port_input[8] = "8001";
    void draw_connect_to_server_window(space::state_t&);
}

#pragma region(local)
void show_brush_window(space::state_t& state) {
    selected_color = state.tool.color.to_float();

    ImGui::Begin("Brush", &brush_window_enabled);

        if (ImGui::ColorPicker3("color picker", (float*)&selected_color)) {
            state.tool.color = fun::rgb_t::from_float(selected_color[0], selected_color[1], selected_color[2]);
        }

    ImGui::End();
}

void show_tools_window(space::state_t& state) {
    auto tool_button = [&state](const char* label, const char* text, space::tool_mode_t mode) {
        if (ImGui::Button(label, ImVec2(25.f, 0.f))) {
            state.tool.mode = mode;
        }

        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("%s", text);
        }
    };

    ImGui::Begin("Tools", &tools_window_enabled);

        tool_button("B", "brush", space::tool_mode_t::brush);
        tool_button("E", "eraser", space::tool_mode_t::eraser);
        tool_button("I", "eyedropper", space::tool_mode_t::eyedropper);
        tool_button("G", "bucket", space::tool_mode_t::bucket);
        tool_button("U", "rectangle", space::tool_mode_t::rectangle);
        tool_button("SU", "rectangle filled", space::tool_mode_t::rectangle_filled);
        tool_button("M", "rectangular selection", space::tool_mode_t::rectangular_selection);
        
        ImGui::SameLine();

    ImGui::End();
}

void draw_nav_bar(space::state_t& state) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("World")) {
            if (space::connection::is_connected(state.connection_data)) {
                if (ImGui::MenuItem("Disconnect")) {
                    space::connection::disconnect(state.connection_data);
                }
            } else {
                if (ImGui::MenuItem("Connect")) {
                    connect_to_server_window_enabled = true;
                }
            }

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

void draw_connect_to_server_window(space::state_t& state) {
    ImGui::Begin("Connect", &connect_to_server_window_enabled);

        ImGui::InputText(" Ip Address", ip_input, sizeof ip_input);
        ImGui::InputText(" Port", port_input, sizeof port_input);
        ImGui::NewLine();

        if (ImGui::Button("Join")) {
            if (space::connection::connect(state.connection_data, fun::rpc::addr_t(ip_input, port_input))) {
                connect_to_server_window_enabled = false;
                
                state.canvas.clear();
            }
        }

    ImGui::End();
}
#pragma endregion

void space::interface::draw(state_t& state) {
    ::draw_nav_bar(state);

    if (connect_to_server_window_enabled) ::draw_connect_to_server_window(state);
    if (brush_window_enabled) ::show_brush_window(state);
    if (tools_window_enabled) ::show_tools_window(state);
    // if (console_window_enabled) draw_console_window(state);
}
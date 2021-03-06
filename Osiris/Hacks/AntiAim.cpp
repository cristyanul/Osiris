#include "AntiAim.h"

#include "../imgui/imgui.h"

#include "../ConfigStructs.h"
#include "../SDK/Entity.h"
#include "../SDK/LocalPlayer.h"
#include "../SDK/UserCmd.h"
#include "../Memory.h"
#include "../SDK/GlobalVars.h"
#include "../SDK/Surface.h"
#include "../SDK/GameEvent.h"
#include "../SDK/Vector.h"

#if OSIRIS_ANTIAIM()

struct AntiAimConfig {
    bool enabled = false;
    bool pitch = false;
    bool yaw = false;
    float pitchAngle = 0.0f;
    float yawAngle = 0.0f;
} antiAimConfig;

void AntiAim::run(UserCmd* cmd, const Vector& previousViewAngles, const Vector& currentViewAngles, bool& sendPacket) noexcept
{
    //float a = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    bool tf = (rand() % 2) != 0;
    if (antiAimConfig.enabled) {
        if (!localPlayer)
            return;
        const auto activeWeapon = localPlayer->getActiveWeapon();
        if (!activeWeapon)
            return;
        if (!activeWeapon->isGrenade() && cmd->buttons & (UserCmd::IN_ATTACK | UserCmd::IN_ATTACK2))
            return;
        if (localPlayer->throwing(cmd))
            return;
        if (cmd->buttons & (UserCmd::IN_USE) || (localPlayer->moveType() == MoveType::LADDER) || (localPlayer->moveType() == MoveType::NOCLIP))
            return;
        if (cmd->buttons &= ~(UserCmd::IN_FORWARD | UserCmd::IN_BACK | UserCmd::IN_MOVERIGHT | UserCmd::IN_MOVELEFT))

        if (antiAimConfig.pitch && cmd->viewangles.x == currentViewAngles.x)
            cmd->viewangles.x = (antiAimConfig.pitchAngle);
        

        if (tf)
            // real
            cmd->viewangles.y += (antiAimConfig.yawAngle);
        else
            // fake
            cmd->viewangles.y -= (antiAimConfig.yawAngle);
   
   
    }
}

static bool antiAimOpen = false;

void AntiAim::menuBarItem() noexcept
{
    if (ImGui::MenuItem("Anti aim")) {
        antiAimOpen = true;
        ImGui::SetWindowFocus("Anti aim");
        ImGui::SetWindowPos("Anti aim", { 100.0f, 100.0f });
    }
}

void AntiAim::tabItem() noexcept
{
    if (ImGui::BeginTabItem("Anti aim")) {
        drawGUI(true);
        ImGui::EndTabItem();
    }
}

void AntiAim::drawGUI(bool contentOnly) noexcept
{
    if (!contentOnly) {
        if (!antiAimOpen)
            return;
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Anti aim", &antiAimOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    }
    ImGui::Checkbox("Enabled", &antiAimConfig.enabled);
    ImGui::Checkbox("##pitch", &antiAimConfig.pitch);
    ImGui::SameLine();
    ImGui::SliderFloat("Pitch", &antiAimConfig.pitchAngle, -89.0f, 89.0f, "%.2f");
    ImGui::Checkbox("Yaw", &antiAimConfig.yaw);
    ImGui::SliderFloat("yaw_angle", &antiAimConfig.yawAngle, -180.0f, 180.0f, "%.2f");
    if (!contentOnly)
        ImGui::End();
}

static void to_json(json& j, const AntiAimConfig& o, const AntiAimConfig& dummy = {})
{
    WRITE("Enabled", enabled);
    WRITE("Pitch", pitch);
    WRITE("Pitch angle", pitchAngle);
    WRITE("Yaw", yaw);
    WRITE("yaw_angle", yawAngle);
}

json AntiAim::toJson() noexcept
{
    json j;
    to_json(j, antiAimConfig);
    return j;
}

static void from_json(const json& j, AntiAimConfig& a)
{
    read(j, "Enabled", a.enabled);
    read(j, "Pitch", a.pitch);
    read(j, "Yaw", a.yaw);
    read(j, "Pitch angle", a.pitchAngle);
    read(j, "yaw_angle", a.yawAngle);
}

void AntiAim::fromJson(const json& j) noexcept
{
    from_json(j, antiAimConfig);
}

void AntiAim::resetConfig() noexcept
{
    antiAimConfig = { };
}

#else

namespace AntiAim
{
    void run(UserCmd*, const Vector&, const Vector&, bool&) noexcept {}

    // GUI
    void menuBarItem() noexcept {}
    void tabItem() noexcept {}
    void drawGUI(bool contentOnly) noexcept {}

    // Config
    json toJson() noexcept { return {}; }
    void fromJson(const json& j) noexcept {}
    void resetConfig() noexcept {}
}

#endif

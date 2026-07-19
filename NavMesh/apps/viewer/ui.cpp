#define RAYGUI_IMPLEMENTATION
#include "ui.hpp"

#include <raygui.h>

namespace {
constexpr float kPanelX = 1280.f - 260.f;
constexpr float kPanelY = 10.f;
constexpr float kPanelW = 250.f;
}

bool drawControlPanel(float& agentRadius, LayerToggles& toggles) {
    GuiPanel({kPanelX, kPanelY, kPanelW, 270.f}, "Controls");

    float before = agentRadius;
    GuiLabel({kPanelX + 10, kPanelY + 28, kPanelW - 20, 20}, TextFormat("Agent radius: %.0f", agentRadius));
    GuiSliderBar({kPanelX + 10, kPanelY + 50, kPanelW - 20, 20}, "0", "60", &agentRadius, 0.f, 60.f);

    float y = kPanelY + 84;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Input polygons", &toggles.showInput);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Inset result", &toggles.showInset);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Triangulation", &toggles.showTriangulation);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "A* visited", &toggles.showVisited);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Corridor", &toggles.showCorridor);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Portals crossed", &toggles.showPortals);
    y += 26;
    GuiCheckBox({kPanelX + 10, y, 18, 18}, "Funnel path", &toggles.showPath);

    return agentRadius != before;
}

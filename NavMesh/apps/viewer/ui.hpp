#pragma once

struct LayerToggles {
    bool showInput = true;
    bool showInset = true;
    bool showTriangulation = true;
    bool showVisited = true;
    bool showCorridor = true;
    bool showPortals = true;
    bool showPath = true;
};

bool drawControlPanel(float& agentRadius, LayerToggles& toggles);

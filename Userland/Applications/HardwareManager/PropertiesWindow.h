#pragma once

#include <LibGUI/Window.h>
#include "DeviceInfo.h"

class PropertiesWindow final : public GUI::Window {
    C_OBJECT(PropertiesWindow);

public:
    virtual ~PropertiesWindow() override = default;

private:
    PropertiesWindow(DeviceInfo device_info, Window* parent);
};

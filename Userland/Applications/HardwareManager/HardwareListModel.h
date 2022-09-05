#pragma once

#include <AK/Types.h>
#include <LibGUI/TreeViewModel.h>
#include "DeviceInfo.h"

class HardwareListNode final : public GUI::TreeViewModel::Node {
public:
    HardwareListNode(String text, Optional<GUI::Icon> icon, Node* parent_node, DeviceInfo device_info)
        : Node(move(text), move(icon), parent_node)
        , m_device_info(device_info)
    {
    }

    DeviceInfo const& device_info() const { return m_device_info; }

private:
    DeviceInfo m_device_info;
};

ErrorOr<NonnullRefPtr<GUI::TreeViewModel>> create_hardware_tree_model();

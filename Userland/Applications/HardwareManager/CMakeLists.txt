serenity_component(
    HardwareManager
    REQUIRED
    TARGETS HardwareManager
)

compile_gml(DevicePropertiesWindow.gml DevicePropertiesWindowGML.h device_properties_window_gml)

set(SOURCES
    DeviceInfo.cpp
    DeviceInfo.h
    DevicePropertiesWindow.gml
    HardwareListModel.cpp
    HardwareListModel.h
    main.cpp
    PropertiesWindow.cpp
    PropertiesWindow.h
)

serenity_app(HardwareManager ICON app-system-monitor)
target_link_libraries(HardwareManager LibPCIDB LibMain LibGUI)

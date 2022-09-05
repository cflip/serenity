serenity_component(
    HardwareManager
    TARGETS HardwareManager
)

set(SOURCES
    main.cpp
    HardwareListModel.h
    HardwareListModel.cpp
)

serenity_app(HardwareManager ICON app-system-monitor)
target_link_libraries(HardwareManager LibPCIDB LibMain LibGUI)

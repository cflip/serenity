#include <LibGUI/Application.h>
#include <LibGUI/Action.h>
#include <LibGUI/TreeView.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Menu.h>
#include <LibGUI/Window.h>
#include <LibMain/Main.h>

#include "HardwareListModel.h"
#include "PropertiesWindow.h"

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    auto app = TRY(GUI::Application::try_create(arguments));
    auto app_icon = GUI::Icon::default_icon("app-system-monitor"sv);
 
    auto window = GUI::Window::construct();
    window->set_title("Hardware Manager");
    window->set_icon(app_icon.bitmap_for_size(16));
    window->resize(530, 370);

    auto hardware_model = TRY(create_hardware_tree_model());
    auto hardware_tree = TRY(window->try_set_main_widget<GUI::TreeView>());
    hardware_tree->set_model(hardware_model);

    auto properties_action = GUI::CommonActions::make_properties_action([&](auto&) {
        auto selected_index = hardware_tree->selection().first();
        if (!selected_index.is_valid())
            return;

        auto& node = *static_cast<GUI::TreeViewModel::Node*>(selected_index.internal_data());
        if (!is<HardwareListNode>(node))
            return;

        auto device_info = static_cast<HardwareListNode&>(node).device_info();

        RefPtr<PropertiesWindow> properties_window = window->add<PropertiesWindow>(device_info, window);
        properties_window->set_title("PCI Device Properties");
        properties_window->resize(360, 420);
        properties_window->center_on_screen();
        //properties_window->set_resizable(false);
        properties_window->show();
    });

    auto context_menu = TRY(GUI::Menu::try_create());
    context_menu->add_action(properties_action);

    hardware_tree->on_context_menu_request = [&](auto&, auto& event) {
        context_menu->popup(event.screen_position());
    };

    window->show();
    return app->exec();
}

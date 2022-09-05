#include "PropertiesWindow.h"

#include <Userland/Applications/HardwareManager/DevicePropertiesWindowGML.h>

#include <LibGUI/Widget.h>
#include <LibGUI/Label.h>

#include <LibPCIDB/Database.h>

PropertiesWindow::PropertiesWindow(DeviceInfo device_info, Window* parent = nullptr)
	: GUI::Window(parent)
{
	auto& widget = set_main_widget<GUI::Widget>();
	widget.load_from_gml(device_properties_window_gml);

	auto pci_database = PCIDB::Database::open();

	StringView device_name = pci_database->get_device(device_info.vendor_id, device_info.device_id);
	StringView class_name = pci_database->get_class(device_info.class_id);
	StringView vendor_name = pci_database->get_vendor(device_info.vendor_id);
	StringView progif_name = pci_database->get_programming_interface(device_info.class_id, device_info.subclass_id, device_info.programming_interface_id);
	String location_name = String::formatted("PCI Bus {}, device {}, function {}", device_info.location.bus, device_info.location.device, device_info.location.function);
	
	widget.find_descendant_of_type_named<GUI::Label>("name")->set_text(device_name);
	widget.find_descendant_of_type_named<GUI::Label>("class")->set_text(class_name);
	widget.find_descendant_of_type_named<GUI::Label>("vendor")->set_text(vendor_name);
	widget.find_descendant_of_type_named<GUI::Label>("programming_interface")->set_text(progif_name);
	widget.find_descendant_of_type_named<GUI::Label>("location")->set_text(location_name);
}

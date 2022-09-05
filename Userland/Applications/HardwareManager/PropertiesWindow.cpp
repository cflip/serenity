#include "PropertiesWindow.h"

#include <Userland/Applications/HardwareManager/DevicePropertiesWindowGML.h>

#include <LibGUI/Widget.h>
#include <LibGUI/Label.h>
#include <LibGUI/Model.h>
#include <LibGUI/TableView.h>

#include <LibPCIDB/Database.h>

class DeviceResourceModel final : public GUI::Model {
public:
	DeviceResourceModel(u32* bars)
	{
		for (int i = 0; i < 6; i++) {
			u32 bar_value = bars[i];
			if (bar_value == 0)
				continue;

			Item item;
			item.bar_number = i;
			if ((bar_value & 1) == 0) {
				item.region_type = Item::RegionType::Memory;
				item.base_address = bar_value & 0xfffffff0;
			} else {
				item.region_type = Item::RegionType::IO;
				item.base_address = bar_value & 0xfffffffc;
			}
			m_items.append(item);
		}
	}

	enum Column {
		Index,
		RegionType,
		BaseAddress,
		__Count
	};

	struct Item {
		int bar_number;
		enum RegionType {
			Memory,
			IO
		} region_type;
		u32 base_address;
	};

	int row_count(GUI::ModelIndex const&) const override { return m_items.size(); }
	int column_count(GUI::ModelIndex const&) const override { return Column::__Count; }

	String column_name(int column) const override { 
		switch (column) {
			case Column::Index:
				return "BAR #";
			case Column::RegionType:
				return "Region Type";
			case Column::BaseAddress:
				return "Base Address";
			default:
				VERIFY_NOT_REACHED();
		}
	}

	GUI::Variant data(GUI::ModelIndex const& index, GUI::ModelRole role) const override
	{
		auto item = m_items[index.row()];
		if (role == GUI::ModelRole::Display) {
			switch (index.column())
			{
			case Column::Index:
				return item.bar_number;
			case Column::RegionType:
				return item.region_type == Item::RegionType::Memory ? "Memory Region" : "IO Region";
			case Column::BaseAddress:
				return String::formatted("{:p}", item.base_address);
			default:
				VERIFY_NOT_REACHED();
			}
		}

		return {};
	}
private:
	Vector<Item> m_items;
};

PropertiesWindow::PropertiesWindow(DeviceInfo device_info, Window* parent = nullptr)
	: GUI::Window(parent)
{
	auto& widget = set_main_widget<GUI::Widget>();
	widget.load_from_gml(device_properties_window_gml);

	auto pci_database = PCIDB::Database::open();

	StringView vendor_name = pci_database->get_vendor(device_info.vendor_id);
	StringView device_name = pci_database->get_device(device_info.vendor_id, device_info.device_id);
	StringView class_name = pci_database->get_class(device_info.class_id);
	StringView subclass_name = pci_database->get_subclass(device_info.class_id, device_info.subclass_id);
	StringView progif_name = pci_database->get_programming_interface(device_info.class_id, device_info.subclass_id, device_info.programming_interface_id);
	
	String location_name = String::formatted("PCI Bus {}, device {}, function {}", device_info.location.bus, device_info.location.device, device_info.location.function);
	
	widget.find_descendant_of_type_named<GUI::Label>("name")->set_text(String::formatted("{} (rev {})", device_name, device_info.revision_id));
	widget.find_descendant_of_type_named<GUI::Label>("class")->set_text(class_name);
	widget.find_descendant_of_type_named<GUI::Label>("subclass")->set_text(subclass_name);
	widget.find_descendant_of_type_named<GUI::Label>("vendor")->set_text(vendor_name);
	widget.find_descendant_of_type_named<GUI::Label>("programming_interface")->set_text(progif_name);
	widget.find_descendant_of_type_named<GUI::Label>("location")->set_text(location_name);

	auto table_view = widget.find_descendant_of_type_named<GUI::TableView>("resource_table_view");
	table_view->set_model(adopt_ref(*new DeviceResourceModel(device_info.bars)));
}

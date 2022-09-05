#include "HardwareListModel.h"

#include <AK/Vector.h>
#include <AK/HashMap.h>
#include <LibCore/DirIterator.h>
#include <LibCore/File.h>
#include <LibPCIDB/Database.h>


static HashMap<u32, Vector<DeviceInfo>> get_device_info_by_class()
{
	HashMap<u32, Vector<DeviceInfo>> device_info_by_class;

	Core::DirIterator di("/sys/bus/pci/", Core::DirIterator::SkipParentAndBaseDir);
    if (di.has_error()) {
        // FIXME: Show messagebox
    }

    while (di.has_next()) {
        auto dir = di.next_path();

		auto device_info_or_error = DeviceInfo::from_directory(dir);
		if (device_info_or_error.is_error())
			continue;
		auto device_info = device_info_or_error.value();

		auto device_info_for_this_class = device_info_by_class.get(device_info.class_id);
		if (device_info_for_this_class.has_value()) {
			device_info_for_this_class.value().empend(device_info);
		} else {
			device_info_by_class.set(device_info.class_id, { device_info });
		}
    }

	return device_info_by_class;
}

ErrorOr<NonnullRefPtr<GUI::TreeViewModel>> create_hardware_tree_model()
{
	auto hardware_tree_model = GUI::TreeViewModel::create();
	auto device_info_by_class = get_device_info_by_class();
	auto pci_database = PCIDB::Database::open();

	auto classes = device_info_by_class.keys();
	for (auto& class_id : classes) {
		auto class_node = hardware_tree_model->add_node(pci_database->get_class(class_id), {});

		auto device_info_list = device_info_by_class.get(class_id);
		if (device_info_list.has_value()) {
			for (auto& device_info : device_info_list.value()) {
				String device_name = pci_database->get_device(device_info.vendor_id, device_info.device_id);
				if (device_name.is_empty())
					device_name = String::formatted("Unknown device: {:02x}", device_info.device_id);
					
				(void)class_node->add_node<HardwareListNode>(device_name, {}, device_info);
			}
		}
	}

	return hardware_tree_model;
}

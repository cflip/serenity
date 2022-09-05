#include "DeviceInfo.h"

#include <AK/String.h>
#include <LibCore/File.h>

static u32 read_hex_string_from_bytebuffer(ByteBuffer const& buf)
{
    // FIXME: Propagate errors.
    return AK::StringUtils::convert_to_uint_from_hex(
        String(MUST(buf.slice(2, buf.size() - 2)).bytes()))
        .release_value();
}

static u32 convert_sysfs_value_to_uint(String const& value)
{
    if (auto result = AK::StringUtils::convert_to_uint_from_hex(value); result.has_value())
        return result.release_value();
    if (auto result = AK::StringUtils::convert_to_uint(value); result.has_value())
        return result.release_value();
    VERIFY_NOT_REACHED();
}

ErrorOr<DeviceInfo> DeviceInfo::from_directory(String dir)
{
    const auto read_property_from_file = [dir](String property_name) -> unsigned int {
        auto id_file = Core::File::construct(String::formatted("/sys/bus/pci/{}/{}", dir, property_name));
        if (!id_file->open(Core::OpenMode::ReadOnly)) {
            dbgln("Error: Could not open {}: {}", id_file->name(), id_file->error_string());
        }
        return read_hex_string_from_bytebuffer(id_file->read_all());
    };

	DeviceInfo device_info = {};

    auto domain_bus_device_parts = dir.split(':');
    VERIFY(domain_bus_device_parts.size() == 3);
	
    device_info.location.domain = convert_sysfs_value_to_uint(domain_bus_device_parts[0]);
    device_info.location.bus = convert_sysfs_value_to_uint(domain_bus_device_parts[1]);
    device_info.location.device = convert_sysfs_value_to_uint(domain_bus_device_parts[2].split('.')[0]);

    device_info.vendor_id = static_cast<u16>(read_property_from_file("vendor"));
    device_info.device_id = static_cast<u16>(read_property_from_file("device_id"));
    device_info.subvendor_id = static_cast<u16>(read_property_from_file("subsystem_vendor"));
    device_info.subdevice_id = static_cast<u16>(read_property_from_file("subsystem_id"));
    device_info.class_id = static_cast<u8>(read_property_from_file("class"));
    device_info.subclass_id = static_cast<u8>(read_property_from_file("subclass"));
    device_info.programming_interface_id = static_cast<u8>(read_property_from_file("progif"));
    device_info.revision_id = static_cast<u8>(read_property_from_file("revision"));

    for (size_t bar_index = 0; bar_index < 6; bar_index++) {
        u32 bar_value = read_property_from_file(String::formatted("bar{}", bar_index));
        device_info.bars[bar_index] = bar_value;
    }

	return device_info;
}

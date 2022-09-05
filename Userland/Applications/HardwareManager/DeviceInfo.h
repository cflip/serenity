#pragma once

#include <AK/Error.h>
#include <AK/StringView.h>
#include <AK/Types.h>

struct DeviceInfo {
    u16 vendor_id;
    u16 device_id;
	u16 subvendor_id;
	u16 subdevice_id;
    u8 class_id;
    u8 subclass_id;
	u8 programming_interface_id;
    u8 revision_id;
	struct {
		u32 domain;
		u32 bus;
		u32 device;
		u32 function;
	} location;

	static ErrorOr<DeviceInfo> from_directory(String dir);
};

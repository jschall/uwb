BOARD ?= org.proficnc.uwb_1.0

CSRC = $(shell find src -name "*.c")
INCDIR = ./include

MODULES_ENABLED = chibios_sys_init chibios_hal_init pubsub param uavcan flash dw1000 spi_device timing app_descriptor boot_msg can can_auto_init worker_thread lpwork_thread uavcan_param_interface uavcan_allocatee uavcan_restart system_event

MESSAGES_ENABLED = uavcan.protocol.NodeStatus uavcan.protocol.debug.LogMessage

-include omd_common/include.mk

omd_common/include.mk:
	git submodule init && git submodule update

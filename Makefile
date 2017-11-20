BOARD ?= org.proficnc.uwb_1.0

CSRC = $(shell find src -name "*.c")
INCDIR = ./include

MODULES_ENABLED = chibios_sys_init chibios_hal_init pubsub param uavcan flash driver_dw1000 spi_device ext_irq timing app_descriptor boot_msg can can_auto_init worker_thread lpwork_thread uavcan_param_interface uavcan_debug uavcan_allocatee uavcan_restart system uavcan_nodestatus_publisher uavcan_getnodeinfo_server uavcan_beginfirmwareupdate_server

MESSAGES_ENABLED = uavcan.protocol.debug.LogMessage uavcan.protocol.GetNodeInfo

-include omd_common/include.mk

omd_common/include.mk: PRE_BUILD_RULE
	git submodule init && git submodule update
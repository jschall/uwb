BOARD ?= org.proficnc.uwb_1.0

CSRC = $(shell find src -name "*.c")
INCDIR = ./include

MODULES_ENABLED = chibios_sys_init chibios_hal_init pubsub param uavcan flash dw1000 spi_device timing bootloader_compat can can_auto_init uavcan_allocatee worker_thread lpwork_thread

MESSAGES_ENABLED = uavcan.protocol.NodeStatus uavcan.protocol.debug.LogMessage uavcan.protocol.dynamic_node_id.Allocation

include omd_common/include.mk

.PHONY: update_submodules
PRE_BUILD_RULE: update_submodules
update_submodules:
	git submodule init && git submodule update

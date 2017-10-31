/*
 *    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "ch.h"
#include "hal.h"
#include <common/timing.h>
#include <common/helpers.h>
#include "dw1000.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <common/param.h>
#include <common/uavcan.h>
#include "uavcan_node.h"

#include <canard/uavcan.protocol.NodeStatus.h>
#include <canard/uavcan.protocol.debug.LogMessage.h>

static void status_topic_handler(size_t msg_size, const void* buf, void* ctx) {
    const struct uavcan_deserialized_message_s* wrapper = buf;
    const struct uavcan_protocol_NodeStatus_s* msg = (const struct uavcan_protocol_NodeStatus_s*)wrapper->msg;
    struct uavcan_protocol_debug_LogMessage_s log_message;
    log_message.level.value = UAVCAN_PROTOCOL_DEBUG_LOGLEVEL_DEBUG;
    log_message.source_len = 0;
    sprintf((char*)log_message.text, "%u %u %u %u %u %u", wrapper->source_node_id, msg->uptime_sec, msg->health, msg->mode, msg->sub_mode, msg->vendor_specific_status_code);
    log_message.text_len = strlen((char*)log_message.text);
    uavcan_broadcast(0, &uavcan_protocol_debug_LogMessage_descriptor, CANARD_TRANSFER_PRIORITY_LOW, &log_message);
}

int main(void) {
    uavcan_set_node_id(0, 42);

    struct pubsub_topic_s* status_topic = uavcan_get_message_topic(0, &uavcan_protocol_NodeStatus_descriptor);
    struct pubsub_listener_s status_listener;
    pubsub_init_and_register_listener(status_topic, &status_listener);
    pubsub_listener_set_handler_cb(&status_listener, status_topic_handler, NULL);

    struct uavcan_protocol_NodeStatus_s node_status;
    node_status.uptime_sec = 0;
    node_status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    node_status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    node_status.sub_mode = 0;
    node_status.vendor_specific_status_code = 0;

    while(true) {
        node_status.uptime_sec++;
        uavcan_broadcast(0, &uavcan_protocol_NodeStatus_descriptor, CANARD_TRANSFER_PRIORITY_LOW, &node_status);
        pubsub_listener_handle_until_timeout(&status_listener, MS2ST(1000));
    }

    return 0;
}

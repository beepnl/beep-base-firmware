var lib_iot_experimental =
[
    [ "Background DFU", "lib_background_dfu.html", "lib_background_dfu" ],
    [ "BSD Socket Interface", "lib_iot_socket.html", [
      [ "Blocking/Non-Blocking behavior", "lib_iot_socket.html#SOCKET_BLOCKORNOT", [
        [ "Blocking I/O", "lib_iot_socket.html#IOT_SOCKET_BLOCKING", null ],
        [ "Non-Blocking I/O", "lib_iot_socket.html#IOT_SOCKET_NONBLOCKING", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_socket.html#IOT_SOCKET_CONFIGURATION", null ],
      [ "References", "lib_iot_socket.html#IOT_SOCKET_REFERENCES", null ]
    ] ],
    [ "CoAP", "lib_iot_coap.html", "lib_iot_coap" ],
    [ "Commissioning", "lib_iot_commissioning.html", "lib_iot_commissioning" ],
    [ "Context Manager", "lib_iot_context_manager.html", [
      [ "6LoWPAN layer", "lib_iot_context_manager.html#IOT_CM_ARCH_1", null ],
      [ "IPv6 layer", "lib_iot_context_manager.html#IOT_CM_ARCH_2", null ],
      [ "Configuration parameters", "lib_iot_context_manager.html#IOT_MEM_MANAGER_CONF", [
        [ "IOT_CONTEXT_MANAGER_DISABLE_LOG", "lib_iot_context_manager.html#IOT_CONTEXT_MANAGER_DISABLE_LOG", null ],
        [ "IOT_CONTEXT_MANAGER_DISABLE_API_PARAM_CHECK", "lib_iot_context_manager.html#IOT_CONTEXT_MANAGER_DISABLE_API_PARAM_CHECK", null ],
        [ "IOT_CONTEXT_MANAGER_MAX_CONTEXTS", "lib_iot_context_manager.html#IOT_CONTEXT_MANAGER_MAX_CONTEXTS", null ],
        [ "IOT_CONTEXT_MANAGER_MAX_TABLES", "lib_iot_context_manager.html#IOT_CONTEXT_MANAGER_MAX_TABLES", null ]
      ] ]
    ] ],
    [ "IoT File", "lib_iot_file.html", [
      [ "Static memory access", "lib_iot_file.html#iot_file_static", null ],
      [ "Configuration parameters", "lib_iot_file.html#IOT_FILE_CONF", [
        [ "IOT_FILE_DISABLE_LOGS", "lib_iot_file.html#IOT_FILE_DISABLE_LOGS", null ],
        [ "IOT_FILE_DISABLE_API_PARAM_CHECK", "lib_iot_file.html#IOT_FILE_DISABLE_API_PARAM_CHECK", null ]
      ] ],
      [ "Specifics and limitations", "lib_iot_file.html#IOT_FILE_LIMITATIONS", [
        [ "Implemented features", "lib_iot_file.html#IOT_FILE_LIMITATION_Features", null ],
        [ "Limitations", "lib_iot_file.html#IOT_FILE_LIMITATION_limitations", null ]
      ] ]
    ] ],
    [ "IoT Timer", "lib_iot_timer.html", [
      [ "Code examples", "lib_iot_timer.html#IOT_TIMER_CODE", [
        [ "Initializing an application timer as a tick source for the IoT Timer", "lib_iot_timer.html#IOT_TIMER_CODE_1", null ],
        [ "Configuring clients for the IoT Timer", "lib_iot_timer.html#IOT_TIMER_CODE_2", null ],
        [ "Querying the IoT Timer for the wall clock value", "lib_iot_timer.html#IOT_TIMER_CODE_3", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_timer.html#IOT_TIMER_CONF", [
        [ "IOT_TIMER_RESOLUTION_IN_MS", "lib_iot_timer.html#IOT_TIMER_RESOLUTION_IN_MS", null ],
        [ "IOT_TIMER_DISABLE_API_PARAM_CHECK", "lib_iot_timer.html#IOT_TIMER_DISABLE_API_PARAM_CHECK", null ]
      ] ],
      [ "Implementation specific limitations", "lib_iot_timer.html#IOT_TIMER_LIMITATIONS", null ]
    ] ],
    [ "lwIP stack on nRF5", "iot_sdk_user_guides_lwip_stack.html", [
      [ "lwIP stack driver", "iot_sdk_user_guides_lwip_stack.html#iot_sdk_lwip_stack_driver", null ],
      [ "lwIP version and reference", "iot_sdk_user_guides_lwip_stack.html#iot_sdk_lwip_reference", null ]
    ] ],
    [ "LWM2M", "lib_iot_lwm2m.html", [
      [ "LWM2M and IPSO", "lib_iot_lwm2m.html#lib_iot_lwm2m_lwm2m_and_ipso", null ],
      [ "Objects", "lib_iot_lwm2m.html#lib_iot_lwm2m_resource_objects", null ],
      [ "Instances and resources", "lib_iot_lwm2m.html#lwm2m_resource_instances_and_resources", [
        [ "Disable Resource ID", "lib_iot_lwm2m.html#lwm2m_resource_instance_and_resources_disable_member", null ]
      ] ],
      [ "LWM2M CoAP integration", "lib_iot_lwm2m.html#lib_iot_lwm2m_coap", null ],
      [ "LWM2M API", "lib_iot_lwm2m.html#lib_iot_lwm2m_api", [
        [ "Initialization", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_init", null ],
        [ "Notifications", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_notification", null ],
        [ "Root handler", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_root", null ],
        [ "Object initialization", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_object_initialization", null ],
        [ "Instance initialization", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_instance_initialization", null ],
        [ "Named Objects", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_name_objects", null ],
        [ "Add an object", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_object_add", null ],
        [ "Delete an object", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_object_delete", null ],
        [ "Add an instance", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_instance_add", null ],
        [ "Delete an instance", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_instance_delete", null ],
        [ "Generate a Link Format string", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_gen_link_format", null ],
        [ "Encoding TLV", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_tlv_encoding", null ],
        [ "Decoding TLV", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_tlv_decoding", null ],
        [ "Respond with payload", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_respond_with_payload", null ],
        [ "Respond with code", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_respond_with_code", null ],
        [ "Initiate bootstrap", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_bootstrap", null ],
        [ "Register", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_register", null ],
        [ "Registration update", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_update", null ],
        [ "Deregister", "lib_iot_lwm2m.html#lib_iot_lwm2m_api_deregister", null ]
      ] ],
      [ "Features", "lib_iot_lwm2m.html#IOT_LWM2M_features", null ],
      [ "Configuration parameters", "lib_iot_lwm2m.html#IOT_LWM2M_CONF", [
        [ "LWM2M_DISABLE_LOGS", "lib_iot_lwm2m.html#LWM2M_DISABLE_LOGS", null ],
        [ "LWM2M_DISABLE_API_PARAM_CHECK", "lib_iot_lwm2m.html#LWM2M_DISABLE_API_PARAM_CHECK", null ],
        [ "LWM2M_MAX_SERVERS", "lib_iot_lwm2m.html#LWM2M_MAX_SERVERS", null ],
        [ "LWM2M_COAP_HANDLER_MAX_OBJECTS", "lib_iot_lwm2m.html#LWM2M_COAP_HANDLER_MAX_OBJECTS", null ],
        [ "LWM2M_COAP_HANDLER_MAX_INSTANCES", "lib_iot_lwm2m.html#LWM2M_COAP_HANDLER_MAX_INSTANCES", null ],
        [ "LWM2M_REGISTER_MAX_LOCATION_LEN", "lib_iot_lwm2m.html#LWM2M_REGISTER_MAX_LOCATION_LEN", null ]
      ] ]
    ] ],
    [ "Nordic's IPv6 stack", "lib_iot_stack.html", "lib_iot_stack" ],
    [ "Packet Buffer", "lib_iot_pbuffer.html", [
      [ "Allocating the packet buffer", "lib_iot_pbuffer.html#IOT_PBUFFER_OVERVIEW_1", null ],
      [ "Reallocating the packet buffer", "lib_iot_pbuffer.html#IOT_PBUFFER_OVERVIEW_2", null ],
      [ "Freeing the packet buffer", "lib_iot_pbuffer.html#IOT_PBUFFER_OVERVIEW_3", null ],
      [ "Code examples", "lib_iot_pbuffer.html#IOT_PBUFFER_CODE", [
        [ "Allocating CoAP packet, with 100 bytes payload", "lib_iot_pbuffer.html#IOT_PBUFFER_CODE_1", null ],
        [ "Reallocating the UDP packet buffer to 10 bytes more", "lib_iot_pbuffer.html#IOT_PBUFFER_CODE_2", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_pbuffer.html#IOT_PBUFFER_CONF", [
        [ "IOT_PBUFFER_DISABLE_LOG", "lib_iot_pbuffer.html#IOT_PBUFFER_DISABLE_LOG", null ],
        [ "IOT_PBUFFER_DISABLE_API_PARAM_CHECK", "lib_iot_pbuffer.html#IOT_PBUFFER_DISABLE_API_PARAM_CHECK", null ],
        [ "IOT_PBUFFER_MAX_COUNT", "lib_iot_pbuffer.html#IOT_PBUFFER_MAX_COUNT", null ]
      ] ]
    ] ],
    [ "Transport Layer Security on nRF5", "lib_iot_security.html", [
      [ "Overview", "lib_iot_security.html#lib_iot_security_overview", null ],
      [ "Nordic's TLS Abstraction Interface", "lib_iot_security.html#lib_iot_security_interface", [
        [ "Feature Configuration Summary", "lib_iot_security.html#lib_iot_security_config_table", null ],
        [ "Application Interface", "lib_iot_security.html#lib_iot_security_api", null ]
      ] ],
      [ "Version and Reference", "lib_iot_security.html#mbedtls", null ]
    ] ]
];
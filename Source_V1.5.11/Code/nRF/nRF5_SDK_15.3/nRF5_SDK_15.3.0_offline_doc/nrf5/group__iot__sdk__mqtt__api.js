var group__iot__sdk__mqtt__api =
[
    [ "mqtt_utf8_t", "structmqtt__utf8__t.html", [
      [ "p_utf_str", "structmqtt__utf8__t.html#a921e5972c5d5e3684e2c8a6847d63635", null ],
      [ "utf_strlen", "structmqtt__utf8__t.html#a828e592f8653eeeafcd6c8c27773a0e3", null ]
    ] ],
    [ "mqtt_binstr_t", "structmqtt__binstr__t.html", [
      [ "bin_strlen", "structmqtt__binstr__t.html#afad4c8f26558784a2d596b35dbd62105", null ],
      [ "p_bin_str", "structmqtt__binstr__t.html#a9ef7bd12d5a88cbb33bfb4e4814d17fd", null ]
    ] ],
    [ "mqtt_topic_t", "structmqtt__topic__t.html", [
      [ "qos", "structmqtt__topic__t.html#a654c8ae7a9c892c79d07af7febe83adc", null ],
      [ "topic", "structmqtt__topic__t.html#a117b21616c99a79224d62dde63fb7a66", null ]
    ] ],
    [ "mqtt_publish_message_t", "structmqtt__publish__message__t.html", [
      [ "payload", "structmqtt__publish__message__t.html#aae48741826077433e1f044271e3751a6", null ],
      [ "topic", "structmqtt__publish__message__t.html#a15fd203ea14c217a7b7423740a2ad8f1", null ]
    ] ],
    [ "mqtt_connack_param_t", "structmqtt__connack__param__t.html", [
      [ "return_code", "structmqtt__connack__param__t.html#a017cf2e7f4819ece8591686c87ce67e8", null ],
      [ "session_present_flag", "structmqtt__connack__param__t.html#a25e4588d7bdfca3a552f80b446a002a4", null ]
    ] ],
    [ "mqtt_puback_param_t", "structmqtt__puback__param__t.html", [
      [ "message_id", "structmqtt__puback__param__t.html#a88873cb62c6dc48ee1c1783876678cde", null ]
    ] ],
    [ "mqtt_pubrec_param_t", "structmqtt__pubrec__param__t.html", [
      [ "message_id", "structmqtt__pubrec__param__t.html#ab976dec35b3e55142bcb6f775bd346ac", null ]
    ] ],
    [ "mqtt_pubrel_param_t", "structmqtt__pubrel__param__t.html", [
      [ "message_id", "structmqtt__pubrel__param__t.html#a8ba9177ddb07ef50af5df8d52046a48f", null ]
    ] ],
    [ "mqtt_pubcomp_param_t", "structmqtt__pubcomp__param__t.html", [
      [ "message_id", "structmqtt__pubcomp__param__t.html#a2e46f0530d4e23dda450b31ae7d122e7", null ]
    ] ],
    [ "mqtt_suback_param_t", "structmqtt__suback__param__t.html", [
      [ "message_id", "structmqtt__suback__param__t.html#a417ea38335cbc81a8a9ff75bb2f4548f", null ]
    ] ],
    [ "mqtt_unsuback_param_t", "structmqtt__unsuback__param__t.html", [
      [ "message_id", "structmqtt__unsuback__param__t.html#ae564804b3a567d92c4ec4b53fc672f7c", null ]
    ] ],
    [ "mqtt_publish_param_t", "structmqtt__publish__param__t.html", [
      [ "dup_flag", "structmqtt__publish__param__t.html#aa8803537ecca8b327ab21611d35bb8fb", null ],
      [ "message", "structmqtt__publish__param__t.html#a46832c30c85f69561080a50cf5535fd5", null ],
      [ "message_id", "structmqtt__publish__param__t.html#a029adc3eae6162a855b07cc4e171dc58", null ],
      [ "retain_flag", "structmqtt__publish__param__t.html#ae30933998b6352c515234d797a765cf1", null ]
    ] ],
    [ "mqtt_subscription_list_t", "structmqtt__subscription__list__t.html", [
      [ "list_count", "structmqtt__subscription__list__t.html#a65b26e5025181a92099b23fb33a187fd", null ],
      [ "message_id", "structmqtt__subscription__list__t.html#a59116b8e18627dda2da8b3922700caef", null ],
      [ "p_list", "structmqtt__subscription__list__t.html#a641ab9844f2248ebbbc2ce580a2e0a82", null ]
    ] ],
    [ "mqtt_evt_param_t", "unionmqtt__evt__param__t.html", [
      [ "connack", "unionmqtt__evt__param__t.html#a66ad015c014ac2102a8f640f90dceb75", null ],
      [ "puback", "unionmqtt__evt__param__t.html#ae067991041dece60d3c1f03b44fa54ee", null ],
      [ "pubcomp", "unionmqtt__evt__param__t.html#a27d380631e077e3df1b04536c31b3270", null ],
      [ "publish", "unionmqtt__evt__param__t.html#ae53a9fbca3f893900d75273dc1ededd9", null ],
      [ "pubrec", "unionmqtt__evt__param__t.html#af7ca5a3a00c5b6ddad4e03ee285b52c6", null ],
      [ "pubrel", "unionmqtt__evt__param__t.html#a91dfea7e46ca05dcf06b5cfbb49bb794", null ],
      [ "suback", "unionmqtt__evt__param__t.html#a2704e1580bb62ed904ce10af489463b2", null ],
      [ "unsuback", "unionmqtt__evt__param__t.html#af5faa7950c14978806b0979ec0231c08", null ]
    ] ],
    [ "mqtt_evt_t", "structmqtt__evt__t.html", [
      [ "id", "structmqtt__evt__t.html#a38a1da954744a754a9bc764c5a068eaa", null ],
      [ "param", "structmqtt__evt__t.html#a81eff83d723c5d158d6c2fb331caf502", null ],
      [ "result", "structmqtt__evt__t.html#a4ecf1e1e609105a7fd8c1602546bca74", null ]
    ] ],
    [ "mqtt_client_t", "structmqtt__client__t.html", [
      [ "broker_addr", "structmqtt__client__t.html#a7ce49169dacd2143adef5c7f174b3627", null ],
      [ "broker_port", "structmqtt__client__t.html#a6b7632faa3cd77c1808470c2e67d40b5", null ],
      [ "clean_session", "structmqtt__client__t.html#aca3efa155cc5671e8b5aafd38e5bc014", null ],
      [ "client_id", "structmqtt__client__t.html#a76fedd05d8d8226aa7af346ea216a9d7", null ],
      [ "evt_cb", "structmqtt__client__t.html#a223291e72a9cabcee5ea5dade9c82ca3", null ],
      [ "last_activity", "structmqtt__client__t.html#a637718f0439910d68b2d14bdeb180fe9", null ],
      [ "p_packet", "structmqtt__client__t.html#ac75d43346edc978d349f43808ccb1b92", null ],
      [ "p_password", "structmqtt__client__t.html#a4b30969ab8929486170e495be79b4e6c", null ],
      [ "p_pending_packet", "structmqtt__client__t.html#acd862c4344e10aef1eaabfbc8468855b", null ],
      [ "p_security_settings", "structmqtt__client__t.html#a1537e57b48e2de67324ed121428fc585", null ],
      [ "p_user_name", "structmqtt__client__t.html#a3bfcb30e99d85eb980a3bb8f2e267049", null ],
      [ "p_will_message", "structmqtt__client__t.html#a63c3287203b5ee7203ea4c7d8d2ce00c", null ],
      [ "p_will_topic", "structmqtt__client__t.html#a8115711df2575eb531004caef85de423", null ],
      [ "pending_packetlen", "structmqtt__client__t.html#ac6918bc8f9876531fbc243272ca709a0", null ],
      [ "poll_abort_counter", "structmqtt__client__t.html#acf7806eb46b5c97cfde564470433bbb1", null ],
      [ "protocol_version", "structmqtt__client__t.html#a07000ac668a2981e122a4e27b824b58b", null ],
      [ "socket_fd", "structmqtt__client__t.html#ad743fa012ba8c9226ea3771b8a743b6a", null ],
      [ "state", "structmqtt__client__t.html#a73bd7274d4b053e0a6464a0f8bc2c8f3", null ],
      [ "tcp_id", "structmqtt__client__t.html#ac8f69a11775e169fdd6967ba70fbdf5f", null ],
      [ "tls_instance", "structmqtt__client__t.html#a5b20215bb041a3bfefbbb170cd88e8d0", null ],
      [ "transport_type", "structmqtt__client__t.html#a43afcd06aa6d41017597ee4965c5278e", null ],
      [ "will_retain", "structmqtt__client__t.html#a4493c796215152845b261ac6dc9aaf78", null ]
    ] ],
    [ "mqtt_client_id_t", "group__iot__sdk__mqtt__api.html#gabcac8ede1522f7982640d42f2a2f823e", null ],
    [ "mqtt_client_t", "group__iot__sdk__mqtt__api.html#ga3fa4eba06ca6187c7a8a8e7cb22b7e96", null ],
    [ "mqtt_evt_cb_t", "group__iot__sdk__mqtt__api.html#ga5421b53890e56473111a4d6f7bfa4331", null ],
    [ "mqtt_message_t", "group__iot__sdk__mqtt__api.html#ga32bdb5fa5ee14f0d10b174244e0a7bfb", null ],
    [ "mqtt_password_t", "group__iot__sdk__mqtt__api.html#ga04a1d17b1001b76b9d8dcacd8cf5dbbc", null ],
    [ "mqtt_username_t", "group__iot__sdk__mqtt__api.html#ga27b5a617160b34b268ae554e1f038bca", null ],
    [ "mqtt_will_message_t", "group__iot__sdk__mqtt__api.html#ga201fa1cc9941817ced64745db440be86", null ],
    [ "mqtt_conn_return_code_t", "group__iot__sdk__mqtt__api.html#gaec00f8261bf0c5543bde3063ac0a914f", [
      [ "MQTT_CONNECTION_ACCEPTED", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914fa2fdd911a8978b8df2254766c1e3fd050", null ],
      [ "MQTT_UNACCEPTABLE_PROTOCOL_VERSION", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914fa9195e642b0b9e594d96aff84f0696744", null ],
      [ "MQTT_IDENTIFIER_REJECTED", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914faa662f46a044e31ae26397adffee82141", null ],
      [ "MQTT_SERVER_UNAVAILABLE", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914fafe234e7084078871a567359819458df3", null ],
      [ "MQTT_BAD_USER_NAME_OR_PASSWORD", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914faa12d243cef12e3e7b1f4e245f64a1134", null ],
      [ "MQTT_NOT_AUTHORIZED", "group__iot__sdk__mqtt__api.html#ggaec00f8261bf0c5543bde3063ac0a914fa4d101dcf5a101d90ae26ba8fe54a27eb", null ]
    ] ],
    [ "mqtt_evt_id_t", "group__iot__sdk__mqtt__api.html#ga88151111124ef906adc9705f4ac23c2e", [
      [ "MQTT_EVT_CONNACK", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2eab6e626b5a1eda76b32053ccbb4b7b5f3", null ],
      [ "MQTT_EVT_DISCONNECT", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2ea8de64478f7ae463d7844fc2e786ce032", null ],
      [ "MQTT_EVT_PUBLISH", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2eaa893a345e05e796cfd28392c1c4d8cf9", null ],
      [ "MQTT_EVT_PUBACK", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2ea2f25d5d4ca704ab63439d4706d3587de", null ],
      [ "MQTT_EVT_PUBREC", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2ea1d5f6ba2524f935dd9625d85638eda87", null ],
      [ "MQTT_EVT_PUBREL", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2eab35ebaf4dcc6698471eb16a41c8252a2", null ],
      [ "MQTT_EVT_PUBCOMP", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2eafdba5e48987b4355f445d35b4dc056e8", null ],
      [ "MQTT_EVT_SUBACK", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2ea67caa558ae7975548b3c4e1c1de5f8fb", null ],
      [ "MQTT_EVT_UNSUBACK", "group__iot__sdk__mqtt__api.html#gga88151111124ef906adc9705f4ac23c2eaa5c333ddf366e44f932ec5545ed75bb2", null ]
    ] ],
    [ "mqtt_qos_t", "group__iot__sdk__mqtt__api.html#ga13b2e0f1afd11ab956c78ce2a20ef54c", [
      [ "MQTT_QoS_0_AT_MOST_ONCE", "group__iot__sdk__mqtt__api.html#gga13b2e0f1afd11ab956c78ce2a20ef54cae6d9626e997d7425ffae99625e6346fa", null ],
      [ "MQTT_QoS_1_ATLEAST_ONCE", "group__iot__sdk__mqtt__api.html#gga13b2e0f1afd11ab956c78ce2a20ef54caeb070f5af434dec5dae4398fdfa78151", null ],
      [ "MQTT_QoS_2_EACTLY_ONCE", "group__iot__sdk__mqtt__api.html#gga13b2e0f1afd11ab956c78ce2a20ef54ca4e6b7c0a6cf5b81299fca5389aa825ea", null ]
    ] ],
    [ "mqtt_transport_type_t", "group__iot__sdk__mqtt__api.html#ga1761007bc03507e6a967d491355d9a2c", [
      [ "MQTT_TRANSPORT_NON_SECURE", "group__iot__sdk__mqtt__api.html#gga1761007bc03507e6a967d491355d9a2ca981f7e2ca25c5e478bf658750e26972a", null ],
      [ "MQTT_TRANSPORT_SECURE", "group__iot__sdk__mqtt__api.html#gga1761007bc03507e6a967d491355d9a2ca4ee318fae90ea8c7da2bbf6de993c699", null ],
      [ "MQTT_TRANSPORT_MAX", "group__iot__sdk__mqtt__api.html#gga1761007bc03507e6a967d491355d9a2cae31ea053615b7f2b1fe6ff42eb903d46", null ]
    ] ],
    [ "mqtt_version_t", "group__iot__sdk__mqtt__api.html#ga23467de233c3e796fb416407db442171", [
      [ "MQTT_VERSION_3_1_0", "group__iot__sdk__mqtt__api.html#gga23467de233c3e796fb416407db442171ac93057183f274a064e118af6b4d681e7", null ],
      [ "MQTT_VERSION_3_1_1", "group__iot__sdk__mqtt__api.html#gga23467de233c3e796fb416407db442171ad2926cb14db1fcfb95a47f45d2f5b08e", null ]
    ] ],
    [ "mqtt_abort", "group__iot__sdk__mqtt__api.html#ga9d4cb94b91e6896bf2807800012712be", null ],
    [ "mqtt_client_init", "group__iot__sdk__mqtt__api.html#ga922edaad5c7e71d76cd73c6f30d79034", null ],
    [ "mqtt_connect", "group__iot__sdk__mqtt__api.html#ga3977d71d44f7fc6c6b83bab08db75d0e", null ],
    [ "mqtt_disconnect", "group__iot__sdk__mqtt__api.html#ga0830273e1f44bae321e2854c74f0690e", null ],
    [ "mqtt_init", "group__iot__sdk__mqtt__api.html#gaf46c7af2e70a250f7370e65ec65602ca", null ],
    [ "mqtt_input", "group__iot__sdk__mqtt__api.html#gaa07fb5fce3343261bc86de0bc2090b60", null ],
    [ "mqtt_live", "group__iot__sdk__mqtt__api.html#gad9864436ca06520c94a14f36e968f311", null ],
    [ "mqtt_publish", "group__iot__sdk__mqtt__api.html#gaa3e3238a382c98c5548178983c769e1d", null ],
    [ "mqtt_publish_ack", "group__iot__sdk__mqtt__api.html#ga395817fcbac43c510348cb553e1f4068", null ],
    [ "mqtt_publish_complete", "group__iot__sdk__mqtt__api.html#ga56a283fd55b04c626f74779110bb9c48", null ],
    [ "mqtt_publish_receive", "group__iot__sdk__mqtt__api.html#ga046e4d72a250c0b7ee8183b3adbd0d40", null ],
    [ "mqtt_publish_release", "group__iot__sdk__mqtt__api.html#ga4126ed6875b6ff049a7bac73dbd19ea4", null ],
    [ "mqtt_subscribe", "group__iot__sdk__mqtt__api.html#gaee34b1da364c99d343c12b20415a6ef2", null ],
    [ "mqtt_unsubscribe", "group__iot__sdk__mqtt__api.html#ga0abf765b94e575b98bb116cd039fbef0", null ]
];
var lib_iot_stack =
[
    [ "Architecture", "lib_iot_stack_arch.html", [
      [ "Architecture", "lib_iot_stack_arch.html#IOT_STACK_ARCH_ARCH", null ],
      [ "API Context", "lib_iot_stack_arch.html#IOT_STACK_API_CONTEXT", null ],
      [ "Timer context", "lib_iot_stack_arch.html#IOT_STACK_API_TIMER", null ],
      [ "IPv6 packets", "lib_iot_stack_arch.html#IOT_STACK_PACKETS", null ]
    ] ],
    [ "Memory management", "lib_iot_stack_memory.html", [
      [ "IoT SoftDevice", "lib_iot_stack_memory.html#LIB_IOT_ARCH_SS_1", null ],
      [ "IPSP", "lib_iot_stack_memory.html#LIB_IOT_ARCH_SS_2", null ],
      [ "6LoWPAN", "lib_iot_stack_memory.html#LIB_IOT_ARCH_SS_3", null ],
      [ "IPv6 stack driver", "lib_iot_stack_memory.html#LIB_IOT_ARCH_SS_4", null ],
      [ "IPv6 stack", "lib_iot_stack_memory.html#LIB_IOT_ARCH_SS_5", null ]
    ] ],
    [ "Internet Protocol version 6 (IPv6)", "lib_iot_stack_ipv6.html", [
      [ "Asynchronous Event Notification Callback", "lib_iot_stack_ipv6.html#IOT_IPV6_CALLBACK", null ],
      [ "Code examples", "lib_iot_stack_ipv6.html#IOT_IPV6_CODE", [
        [ "IPv6 initialization", "lib_iot_stack_ipv6.html#IOT_IPV6_CODE_1", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_stack_ipv6.html#IOT_IPV6_CONF", [
        [ "IPV6_DISABLE_LOGS", "lib_iot_stack_ipv6.html#IPV6_DISABLE_LOGS", null ],
        [ "IPV6_DISABLE_API_PARAM_CHECK", "lib_iot_stack_ipv6.html#IPV6_DISABLE_API_PARAM_CHECK", null ],
        [ "IPV6_MAX_INTERFACE", "lib_iot_stack_ipv6.html#IPV6_MAX_INTERFACE", null ],
        [ "IPV6_DEFAULT_HOP_LIMIT", "lib_iot_stack_ipv6.html#IPV6_DEFAULT_HOP_LIMIT", null ],
        [ "IPV6_ENABLE_UNSUPPORTED_PROTOCOLS_TO_APPLICATION", "lib_iot_stack_ipv6.html#IPV6_ENABLE_UNSUPPORTED_PROTOCOLS_TO_APPLICATION", null ]
      ] ]
    ] ],
    [ "Internet Control Message Protocol (ICMP)", "lib_iot_stack_icmp6.html", [
      [ "Asynchronous Event Notification Callback", "lib_iot_stack_icmp6.html#IOT_ICMP6_CALLBACK", null ],
      [ "Code examples", "lib_iot_stack_icmp6.html#IOT_ICMP6_CODE", [
        [ "Ping remote using link-local address", "lib_iot_stack_icmp6.html#IOT_ICMP6_CODE_1", null ],
        [ "Sending router solicitation to all router addresses", "lib_iot_stack_icmp6.html#IOT_ICMP6_CODE_2", null ],
        [ "Sending neighbor solicitation with ARO option to router node", "lib_iot_stack_icmp6.html#IOT_ICMP6_CODE_3", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_stack_icmp6.html#IOT_ICMP6_CONF", [
        [ "ICMP6_DISABLE_LOGS", "lib_iot_stack_icmp6.html#ICMP6_DISABLE_LOGS", null ],
        [ "ICMP6_DISABLE_API_PARAM_CHECK", "lib_iot_stack_icmp6.html#ICMP6_DISABLE_API_PARAM_CHECK", null ],
        [ "ICMP6_ENABLE_ND6_MESSAGES_TO_APPLICATION", "lib_iot_stack_icmp6.html#ICMP6_ENABLE_ND6_MESSAGES_TO_APPLICATION", null ],
        [ "ICMP6_ENABLE_ALL_MESSAGES_TO_APPLICATION", "lib_iot_stack_icmp6.html#ICMP6_ENABLE_ALL_MESSAGES_TO_APPLICATION", null ],
        [ "ICMP6_ENABLE_HANDLE_ECHO_REQUEST_TO_APPLICATION", "lib_iot_stack_icmp6.html#ICMP6_ENABLE_HANDLE_ECHO_REQUEST_TO_APPLICATION", null ]
      ] ]
    ] ],
    [ "User Datagram Protocol (UDP)", "lib_iot_stack_udp.html", [
      [ "Asynchronous Event Notification Callback on socket", "lib_iot_stack_udp.html#IOT_UDP_CALLBACK", null ],
      [ "Code examples", "lib_iot_stack_udp.html#IOT_UDP_CODE", [
        [ "UDP Echo application on port 7 and any address", "lib_iot_stack_udp.html#IOT_UDP_CODE_1", null ],
        [ "Creating socket with specific peer addresses and port", "lib_iot_stack_udp.html#IOT_UDP_CODE_2", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_stack_udp.html#IOT_UDP_CONF", [
        [ "UDP6_DISABLE_LOG", "lib_iot_stack_udp.html#UDP6_DISABLE_LOG", null ],
        [ "UDP6_DISABLE_API_PARAM_CHECK", "lib_iot_stack_udp.html#UDP6_DISABLE_API_PARAM_CHECK", null ],
        [ "UDP6_MAX_SOCKET_COUNT", "lib_iot_stack_udp.html#UDP6_MAX_SOCKET_COUNT", null ]
      ] ]
    ] ],
    [ "Domain Name System Client (DNS)", "lib_iot_stack_dns.html", [
      [ "Asynchronous Event Notification Callback on query", "lib_iot_stack_dns.html#IOT_DNS_CALLBACK", null ],
      [ "Code examples", "lib_iot_stack_dns.html#IOT_DNS_CODE", [
        [ "Initialization of the DNS Module", "lib_iot_stack_dns.html#IOT_DNS_CODE_1", null ],
        [ "DNS Query to obtain the IPv6 address of example.com domain.", "lib_iot_stack_dns.html#IOT_DNS_CODE_2", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_stack_dns.html#IOT_DNS_CONF", [
        [ "DNS6_DISABLE_LOGS", "lib_iot_stack_dns.html#DNS6_DISABLE_LOGS", null ],
        [ "DNS6_DISABLE_API_PARAM_CHECK", "lib_iot_stack_dns.html#DNS6_DISABLE_API_PARAM_CHECK", null ],
        [ "DNS6_MAX_PENDING_QUERIES", "lib_iot_stack_dns.html#DNS6_MAX_PENDING_QUERIES", null ],
        [ "DNS6_MAX_RETRANSMISSION_COUNT", "lib_iot_stack_dns.html#DNS6_MAX_RETRANSMISSION_COUNT", null ],
        [ "DNS6_RETRANSMISSION_INTERVAL", "lib_iot_stack_dns.html#DNS6_RETRANSMISSION_INTERVAL", null ]
      ] ]
    ] ],
    [ "Simple Network Time Protocol (SNTP)", "lib_sntp_client.html", [
      [ "Code examples", "lib_sntp_client.html#SNTP_CLIENT_CODE", [
        [ "Initializing the module", "lib_sntp_client.html#SNTP_CLIENT_CODE_1", null ],
        [ "Sending an SNTP query", "lib_sntp_client.html#SNTP_CLIENT_CODE_2", null ],
        [ "Getting the local Unix time", "lib_sntp_client.html#SNTP_CLIENT_CODE_3", null ],
        [ "Uninitializing the module", "lib_sntp_client.html#SNTP_CLIENT_CODE_4", null ]
      ] ],
      [ "Configuration parameters", "lib_sntp_client.html#SNTP_CLIENT_CONF", [
        [ "SNTP_MAX_RETRANSMISSION_COUNT", "lib_sntp_client.html#SNTP_MAX_RETRANSMISSION_COUNT", null ],
        [ "SNTP_RETRANSMISSION_INTERVAL", "lib_sntp_client.html#SNTP_RETRANSMISSION_INTERVAL", null ],
        [ "SNTP_CLIENT_DISABLE_API_PARAM_CHECK", "lib_sntp_client.html#SNTP_CLIENT_DISABLE_API_PARAM_CHECK", null ],
        [ "SNTP_CLIENT_DISABLE_LOGS", "lib_sntp_client.html#SNTP_CLIENT_DISABLE_LOGS", null ]
      ] ],
      [ "Implementation specific limitations", "lib_sntp_client.html#SNTP_CLIENT_LIMITATIONS", null ]
    ] ],
    [ "Trivial File Transfer Protocol (TFTP)", "lib_iot_stack_tftp.html", [
      [ "TFTP transfers", "lib_iot_stack_tftp.html#IOT_TFTP_ARCH_client", [
        [ "Negotiation of connection parameters", "lib_iot_stack_tftp.html#IOT_TFTP_ARCH_client_negotiation", null ],
        [ "File transfer", "lib_iot_stack_tftp.html#IOT_TFTP_ARCH_client_transfer", null ],
        [ "Controlling transmission", "lib_iot_stack_tftp.html#IOT_TFTP_ARCH_client_speed", null ],
        [ "Disconnecting", "lib_iot_stack_tftp.html#IOT_TFTP_ARCH_client_abort", null ]
      ] ],
      [ "Configuration parameters", "lib_iot_stack_tftp.html#IOT_TFTP_CONF", [
        [ "TFTP_DISABLE_LOGS", "lib_iot_stack_tftp.html#TFTP_DISABLE_LOGS", null ],
        [ "TFTP_DISABLE_API_PARAM_CHECK", "lib_iot_stack_tftp.html#TFTP_DISABLE_API_PARAM_CHECK", null ],
        [ "TFTP_MAX_RETRANSMISSION_COUNT", "lib_iot_stack_tftp.html#TFTP_MAX_RETRANSMISSION_COUNT", null ],
        [ "TFTP_MAX_INSTANCES", "lib_iot_stack_tftp.html#TFTP_MAX_INSTANCES", null ]
      ] ],
      [ "Specifics and limitations", "lib_iot_stack_tftp.html#IOT_TFTP_LIMITATIONS", [
        [ "Implemented features", "lib_iot_stack_tftp.html#IOT_TFTP_LIMITATION_Features", null ],
        [ "Limitations", "lib_iot_stack_tftp.html#IOT_TFTP_LIMITATION_limitations", null ]
      ] ],
      [ "References", "lib_iot_stack_tftp.html#IOT_TFTP_REFERENCES", null ]
    ] ]
];
var iot_ug_border_router =
[
    [ "Installing the Raspberry Pi", "iot_rpi_install.html", [
      [ "Required hardware", "iot_rpi_install.html#iot_rpi_hardware", null ],
      [ "Installing Raspbian OS", "iot_rpi_install.html#iot_rpi_os", null ]
    ] ],
    [ "Accessing the Raspberry Pi", "iot_rpi_access.html", [
      [ "With screen and keyboard connected", "iot_rpi_access.html#iot_rpi_accessing_screen", null ],
      [ "Remote connection", "iot_rpi_access.html#iot_rpi_accessing_remote", [
        [ "Advanced: Assigning a static IP", "iot_rpi_access.html#iot_rpi_static", null ]
      ] ]
    ] ],
    [ "Installing a 6LoWPAN enabled Linux kernel and required modules", "iot_rpi_kernel.html", [
      [ "Downloading the required packages", "iot_rpi_kernel.html#iot_rpi_download", null ],
      [ "Building Raspbian Kernel with bluetooth_6lowpan module", "iot_rpi_kernel.html#iot_rpi_compile_kernel", [
        [ "Option 1: Cross-compilation on x86/x64", "iot_rpi_kernel.html#kernel_compile_option1", null ],
        [ "Option 2: Native build", "iot_rpi_kernel.html#kernel_compile_option2", null ],
        [ "Installing the kernel on your Raspberry Pi", "iot_rpi_kernel.html#kernel_compile_install", null ]
      ] ]
    ] ],
    [ "Distributing a global IPv6 prefix", "iot_sdk_user_guides_radvd.html", [
      [ "Stateless auto-configuration", "iot_sdk_user_guides_radvd.html#iot_sdk_autoconfiguration", null ],
      [ "Router Advertisement Daemon", "iot_sdk_user_guides_radvd.html#iot_sdk_user_guides_radvd_intro_prog", null ],
      [ "Alternative methods", "iot_sdk_user_guides_radvd.html#iot_sdk_radvd_alternatives", null ]
    ] ],
    [ "Creating link-local IPv6 addresses", "iot_sdk_user_guides_ipv6_address_creation.html", [
      [ "Creating an IPv6 link-local address", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_build", null ],
      [ "Linux Kernel versions < 4.12 (Legacy IID derivation)", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_legacy", [
        [ "Public Bluetooth Device Address", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_public", null ],
        [ "Random Static Bluetooth Device Address", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_rs", null ]
      ] ],
      [ "Linux Kernel versions >= 4.12 (IID derivation according to RFC 7668)", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_rfc7668", [
        [ "Bluetooth Device Address (Public or Static Random)", "iot_sdk_user_guides_ipv6_address_creation.html#iot_sdk_user_guides_ipv6_address_creation_rfc7668_public", null ]
      ] ]
    ] ],
    [ "Connecting devices to the router", "iot_sdk_user_guides_linux_commands.html", [
      [ "Root access", "iot_sdk_user_guides_linux_commands.html#iot_sdk_root", null ],
      [ "Bluetooth 6LoWPAN module initialization", "iot_sdk_user_guides_linux_commands.html#iot_sdk_user_guides_linux_commands_init", [
        [ "debugfs file system", "iot_sdk_user_guides_linux_commands.html#iot_sdk_debugfs", null ],
        [ "6LoWPAN initialization", "iot_sdk_user_guides_linux_commands.html#iot_sdk_init_6lowpan", null ]
      ] ],
      [ "Bluetooth 6LoWPAN commands", "iot_sdk_user_guides_linux_commands.html#iot_sdk_user_guides_linux_commands_connect", null ],
      [ "HCI commands", "iot_sdk_user_guides_linux_commands.html#iot_sdk_user_guides_linux_commands_status", null ],
      [ "Linux interfaces", "iot_sdk_user_guides_linux_commands.html#iot_sdk_user_guides_linux_commands_ifconfig", null ],
      [ "Ping devices", "iot_sdk_user_guides_linux_commands.html#iot_sdk_user_guides_linux_commands_ping", null ]
    ] ]
];
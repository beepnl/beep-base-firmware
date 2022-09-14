var group__nrf__block__dev =
[
    [ "Empty implementation", "group__nrf__block__dev__empty.html", "group__nrf__block__dev__empty" ],
    [ "QSPI implementation", "group__nrf__block__dev__qspi.html", "group__nrf__block__dev__qspi" ],
    [ "Serial flash memory parameters", "group__nrf__serial__flash__params.html", "group__nrf__serial__flash__params" ],
    [ "RAM implementation", "group__nrf__block__dev__ram.html", "group__nrf__block__dev__ram" ],
    [ "SDC implementation", "group__nrf__block__dev__sdc.html", "group__nrf__block__dev__sdc" ],
    [ "nrf_block_req_t", "structnrf__block__req__t.html", [
      [ "blk_count", "structnrf__block__req__t.html#a736825ff509f19be26cd0020d01ea60d", null ],
      [ "blk_id", "structnrf__block__req__t.html#a10275670dc8d48216f31c1bf31f476c8", null ],
      [ "p_buff", "structnrf__block__req__t.html#ad43a7e9ad02d1b011b39b086ae5db168", null ]
    ] ],
    [ "nrf_block_dev_event_t", "structnrf__block__dev__event__t.html", [
      [ "ev_type", "structnrf__block__dev__event__t.html#a1118bb18b5f536e9059848d0d10bfe34", null ],
      [ "p_blk_req", "structnrf__block__dev__event__t.html#add41a6ef2f9d749a68caa12237adfef7", null ],
      [ "p_context", "structnrf__block__dev__event__t.html#a45959b988d683d4d49a10a0b26095d2b", null ],
      [ "result", "structnrf__block__dev__event__t.html#a09623241540db5c70097b39c389f7d06", null ]
    ] ],
    [ "nrf_block_dev_geometry_t", "structnrf__block__dev__geometry__t.html", [
      [ "blk_count", "structnrf__block__dev__geometry__t.html#ad8b0b4897bf5620d3be31090674ba29f", null ],
      [ "blk_size", "structnrf__block__dev__geometry__t.html#a6b0c00d078380aa0c911350333ad2c4c", null ]
    ] ],
    [ "nrf_block_dev_info_strings_t", "structnrf__block__dev__info__strings__t.html", [
      [ "p_product", "structnrf__block__dev__info__strings__t.html#a0fd3e4f980b3fe1c743c8187ad3bdaca", null ],
      [ "p_revision", "structnrf__block__dev__info__strings__t.html#a94e2dc01eebb4192b2f4ebdfa357b98a", null ],
      [ "p_vendor", "structnrf__block__dev__info__strings__t.html#a69b5db4920a22bdfdaf5aee535bde5c6", null ]
    ] ],
    [ "nrf_block_dev_s", "structnrf__block__dev__s.html", [
      [ "nrf_block_dev_ops_s", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html", [
        [ "geometry", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#a1323f9132ddbfc1a40372eaaf5d2c188", null ],
        [ "init", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#a6152bfd8b6bff60b1cea6fa7bbeb7922", null ],
        [ "ioctl", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#a0fb56a4c456fa68b1227378dd8386bc8", null ],
        [ "read_req", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#a97dd0ac6ca243709e46b1ecf22cdca19", null ],
        [ "uninit", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#af6c47f6e7c7a1a93bdd85aa128ed2c14", null ],
        [ "write_req", "structnrf__block__dev__s_1_1nrf__block__dev__ops__s.html#a02e07a8988494e28521fa62608e95150", null ]
      ] ],
      [ "p_ops", "structnrf__block__dev__s.html#a09f4e34fa6b95cba4922ae0491fe91a2", null ]
    ] ],
    [ "NFR_BLOCK_DEV_INFO_CONFIG", "group__nrf__block__dev.html#gae88b5eea86fc4fb6059b710faf0ec57c", null ],
    [ "NFR_BLOCK_DEV_INFO_CONFIG_EMPTY", "group__nrf__block__dev.html#ga0300ea3033b605c371c4d81d5b9106b9", null ],
    [ "NRF_BLOCK_DEV_REQUEST", "group__nrf__block__dev.html#ga2acdc511dca645a280588eea6b7d5325", null ],
    [ "NRF_BLOCKDEV_BASE_ADDR", "group__nrf__block__dev.html#gaa8e2d8723b9e5b2559086380c5a8aca3", null ],
    [ "nrf_block_dev_ev_handler", "group__nrf__block__dev.html#ga0cf6b29fca73b26f80e0caf23943484d", null ],
    [ "nrf_block_dev_ops_t", "group__nrf__block__dev.html#gab1935b3d0311f8de4dc44dc2e268490f", null ],
    [ "nrf_block_dev_t", "group__nrf__block__dev.html#ga9b17a54e2091b725cea7d797bed2671d", null ],
    [ "nrf_block_dev_event_type_t", "group__nrf__block__dev.html#gafc429cf638b287d73d9969dde9e6e0a3", [
      [ "NRF_BLOCK_DEV_EVT_INIT", "group__nrf__block__dev.html#ggafc429cf638b287d73d9969dde9e6e0a3a62363dd16a0341d02b8926f108e85fd8", null ],
      [ "NRF_BLOCK_DEV_EVT_UNINIT", "group__nrf__block__dev.html#ggafc429cf638b287d73d9969dde9e6e0a3a02ff5c32177288b35d03dd79926d39ef", null ],
      [ "NRF_BLOCK_DEV_EVT_BLK_READ_DONE", "group__nrf__block__dev.html#ggafc429cf638b287d73d9969dde9e6e0a3a26d87dbc858746cb264dc482b0f2ea65", null ],
      [ "NRF_BLOCK_DEV_EVT_BLK_WRITE_DONE", "group__nrf__block__dev.html#ggafc429cf638b287d73d9969dde9e6e0a3aeaa8c9d57c6b4eb54b07e486960b37c0", null ]
    ] ],
    [ "nrf_block_dev_ioctl_req_t", "group__nrf__block__dev.html#ga3e706791615b6a259ea5cd3a123b8220", [
      [ "NRF_BLOCK_DEV_IOCTL_REQ_CACHE_FLUSH", "group__nrf__block__dev.html#gga3e706791615b6a259ea5cd3a123b8220a541c87f696e4232a55436d729b7666c6", null ],
      [ "NRF_BLOCK_DEV_IOCTL_REQ_INFO_STRINGS", "group__nrf__block__dev.html#gga3e706791615b6a259ea5cd3a123b8220ad5a49f47275ec81007a56170d7dc32c1", null ]
    ] ],
    [ "nrf_block_dev_result_t", "group__nrf__block__dev.html#ga1b3185c2c415b4f68eb58ff24da07652", [
      [ "NRF_BLOCK_DEV_RESULT_SUCCESS", "group__nrf__block__dev.html#gga1b3185c2c415b4f68eb58ff24da07652a0e23dffd735def96aa13d1c2eabe7229", null ],
      [ "NRF_BLOCK_DEV_RESULT_IO_ERROR", "group__nrf__block__dev.html#gga1b3185c2c415b4f68eb58ff24da07652a0289c2de22b7497e98825a791eeab323", null ],
      [ "NRF_BLOCK_DEV_RESULT_TIMEOUT", "group__nrf__block__dev.html#gga1b3185c2c415b4f68eb58ff24da07652a27b8edcfa3674583e1273e346eaf4e5f", null ]
    ] ],
    [ "nrf_blk_dev_geometry", "group__nrf__block__dev.html#ga0a8149c61c621a6e7ac2e57f79b73487", null ],
    [ "nrf_blk_dev_init", "group__nrf__block__dev.html#ga03cd23aa9388f2e966d7a6317bed42f7", null ],
    [ "nrf_blk_dev_ioctl", "group__nrf__block__dev.html#ga522e19ebcfd6adc196e499e8df0aeba8", null ],
    [ "nrf_blk_dev_read_req", "group__nrf__block__dev.html#gac0e9d16bd2e157b190db1be6bdfa3661", null ],
    [ "nrf_blk_dev_uninit", "group__nrf__block__dev.html#ga50c56e42de4319ec2fb370e5acd44a2f", null ],
    [ "nrf_blk_dev_write_req", "group__nrf__block__dev.html#ga9abbef350d13e856d952bfd860ec0f15", null ]
];
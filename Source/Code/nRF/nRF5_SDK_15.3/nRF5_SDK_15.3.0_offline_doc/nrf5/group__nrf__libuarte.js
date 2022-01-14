var group__nrf__libuarte =
[
    [ "libUARTE library configuration", "group__nrf__libuarte__config.html", "group__nrf__libuarte__config" ],
    [ "nrf_libuarte_data_t", "structnrf__libuarte__data__t.html", [
      [ "length", "structnrf__libuarte__data__t.html#a2c0c79af3d17514eae195fceb4b9f88a", null ],
      [ "p_data", "structnrf__libuarte__data__t.html#a52e1eb974642cdad8d771a50542cdaea", null ]
    ] ],
    [ "nrf_libuarte_evt_t", "structnrf__libuarte__evt__t.html", [
      [ "data", "structnrf__libuarte__evt__t.html#aa4ea0e2f7cd9573399ee43b386071270", null ],
      [ "rxtx", "structnrf__libuarte__evt__t.html#a451131e4295e2c76f9fb80684d4df31a", null ],
      [ "type", "structnrf__libuarte__evt__t.html#a2690c60f0f1f79bc0b3d7394aaf54c26", null ]
    ] ],
    [ "nrf_libuarte_config_t", "structnrf__libuarte__config__t.html", [
      [ "baudrate", "structnrf__libuarte__config__t.html#a71fb99cf6a2e8ac5cf44db9eeb128242", null ],
      [ "cts_pin", "structnrf__libuarte__config__t.html#a5c16d0c232c941d34979610d43cfa323", null ],
      [ "endrx_evt", "structnrf__libuarte__config__t.html#ab68250f5cff40c294fc93b93aa8a9486", null ],
      [ "hwfc", "structnrf__libuarte__config__t.html#adea946b319a2f143238d8a018b2b0a94", null ],
      [ "irq_priority", "structnrf__libuarte__config__t.html#a7b6a7ca672f263945f1136f8a0f492b5", null ],
      [ "parity", "structnrf__libuarte__config__t.html#a1b747bae173022dad01c693b311fe2b0", null ],
      [ "rts_pin", "structnrf__libuarte__config__t.html#ac7e0c6e2c0c1ce9a42741d32619d737b", null ],
      [ "rx_pin", "structnrf__libuarte__config__t.html#a18b7a84c27234d3ab8aeee293320ed60", null ],
      [ "rxdone_tsk", "structnrf__libuarte__config__t.html#a966fa9518ee6adda0fb651b1f601b905", null ],
      [ "rxstarted_tsk", "structnrf__libuarte__config__t.html#a22f7ffa6f7e5b9ac6b36e1b8344b3231", null ],
      [ "startrx_evt", "structnrf__libuarte__config__t.html#aa11a219b9cced625ac264bd08b9c2656", null ],
      [ "tx_pin", "structnrf__libuarte__config__t.html#a3fc5b6fcfc35943e9fcf063ffc9c1be4", null ]
    ] ],
    [ "nrf_libuarte_ctrl_blk_t", "structnrf__libuarte__ctrl__blk__t.html", [
      [ "chunk_size", "structnrf__libuarte__ctrl__blk__t.html#abd462af396367c131578018015040ae2", null ],
      [ "context", "structnrf__libuarte__ctrl__blk__t.html#a220a00af2bb3750b94f48cade364e444", null ],
      [ "evt_handler", "structnrf__libuarte__ctrl__blk__t.html#a78a375bf12548c8aa34b35d833f0efaf", null ],
      [ "last_pin_rx_byte_cnt", "structnrf__libuarte__ctrl__blk__t.html#af933ada5050d0dd3c48c9e9a71475200", null ],
      [ "last_rx_byte_cnt", "structnrf__libuarte__ctrl__blk__t.html#aa928111951b6abfa0cc5adeb259db8cd", null ],
      [ "p_cur_rx", "structnrf__libuarte__ctrl__blk__t.html#a9cbd581815a5015c201ace0fd9c36fc8", null ],
      [ "p_next_next_rx", "structnrf__libuarte__ctrl__blk__t.html#af4ef86a3e14bd4dfa28d26a3b0b8cae4", null ],
      [ "p_next_rx", "structnrf__libuarte__ctrl__blk__t.html#a7acb3cbfea69214e345fc08c0699f7da", null ],
      [ "p_tx", "structnrf__libuarte__ctrl__blk__t.html#a6179c7c4e19e23ea2ca5e0eaea6e6d7f", null ],
      [ "ppi_channels", "structnrf__libuarte__ctrl__blk__t.html#a5d1ab22f6ad065fb9f81b1f2a791c298", null ],
      [ "ppi_groups", "structnrf__libuarte__ctrl__blk__t.html#ae05dc0263c04842f0e7d48ecb95899e1", null ],
      [ "tx_chunk8", "structnrf__libuarte__ctrl__blk__t.html#a6da46b5fbfa6e9bccb97137fd0fbedc0", null ],
      [ "tx_cur_idx", "structnrf__libuarte__ctrl__blk__t.html#a1e13320c2c48c3d711ca5a237c1a9dd2", null ],
      [ "tx_len", "structnrf__libuarte__ctrl__blk__t.html#a2259abaaf3b75fe781e25ab31ac284dc", null ]
    ] ],
    [ "nrf_libuarte_t", "structnrf__libuarte__t.html", [
      [ "ctrl_blk", "structnrf__libuarte__t.html#a8ec4194da55d4bb11512628570ab8f39", null ],
      [ "timer", "structnrf__libuarte__t.html#a3f8bc86009c80be6a473312dc4784f4f", null ],
      [ "uarte", "structnrf__libuarte__t.html#ac90e85fe6449ebc7da2d44aff30ec3ff", null ]
    ] ],
    [ "nrf_libuarte_evt_type_t", "group__nrf__libuarte.html#ga03464f6242778c62184d60e629d7c65c", [
      [ "NRF_LIBUARTE_EVT_RX_DATA", "group__nrf__libuarte.html#gga03464f6242778c62184d60e629d7c65caa57efd29f39aa91c2c71846fb64b3f9a", null ],
      [ "NRF_LIBUARTE_EVT_RX_BUF_REQ", "group__nrf__libuarte.html#gga03464f6242778c62184d60e629d7c65cabbaea68d9d44aef4b3c4cdf6adbd3c8b", null ],
      [ "NRF_LIBUARTE_EVT_TX_DONE", "group__nrf__libuarte.html#gga03464f6242778c62184d60e629d7c65ca50e9ed38296dfb154eca9e33fc89fbf4", null ],
      [ "NRF_LIBUARTE_EVT_ERROR", "group__nrf__libuarte.html#gga03464f6242778c62184d60e629d7c65cafaa5135387d4daf1baedd1a91821228a", null ]
    ] ],
    [ "nrf_libuarte_ppi_channel_t", "group__nrf__libuarte.html#ga60cfeaf5a39d6c3aa159f4a4649530f9", null ],
    [ "nrf_libuarte_ppi_group_t", "group__nrf__libuarte.html#ga1a1cddb20ff3391dd796d3c2d2467c24", [
      [ "NRF_LIBUARTE_PPI_GROUP_ENDRX_STARTRX", "group__nrf__libuarte.html#gga1a1cddb20ff3391dd796d3c2d2467c24a376f737d44736df1c32437e09714640e", null ],
      [ "NRF_LIBUARTE_PPI_GROUP_ENDRX_EXT_RXDONE_TSK", "group__nrf__libuarte.html#gga1a1cddb20ff3391dd796d3c2d2467c24a20928721645477dbd890f6b45d8220a4", null ]
    ] ],
    [ "nrf_libuarte_init", "group__nrf__libuarte.html#gaf13e9bc6c4ce047cc45109612a59d4fb", null ],
    [ "nrf_libuarte_rx_buf_rsp", "group__nrf__libuarte.html#gacdb9f7835852948d74ed17ecadf8616e", null ],
    [ "nrf_libuarte_rx_start", "group__nrf__libuarte.html#gaadcc91333fd08e21d82ab27d42ad0f61", null ],
    [ "nrf_libuarte_rx_stop", "group__nrf__libuarte.html#gadeb068dfaffce61f4ee8922bd9487f69", null ],
    [ "nrf_libuarte_tx", "group__nrf__libuarte.html#ga481301d3e200611e4148f15840bdd124", null ],
    [ "nrf_libuarte_uninit", "group__nrf__libuarte.html#gadfe21769e98501ba8452e4c17f8ebf8e", null ]
];
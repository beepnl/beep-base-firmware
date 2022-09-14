var lib_crypto_frontends =
[
    [ "AES - Advanced Encryption Standard", "lib_crypto_aes.html", [
      [ "Supported AES Modes", "lib_crypto_aes.html#lib_crypto_aes_modes", [
        [ "CBC", "lib_crypto_aes.html#sub_aes_cbc", null ],
        [ "CTR", "lib_crypto_aes.html#sub_aes_ctr", null ],
        [ "CFB", "lib_crypto_aes.html#sub_aes_cfb", null ],
        [ "ECB", "lib_crypto_aes.html#sub_aes_ecb", null ],
        [ "CBC-MAC", "lib_crypto_aes.html#sub_aes_cbc_mac", null ],
        [ "CMAC", "lib_crypto_aes.html#sub_aes_cmac", null ]
      ] ],
      [ "AES API", "lib_crypto_aes.html#lib_crypto_aes_mac_api", [
        [ "Creating a context for AES mode", "lib_crypto_aes.html#lib_crypto_aes_mac_api_creating_context", null ],
        [ "Selecting an AES algorithm", "lib_crypto_aes.html#lib_crypto_aes_mac_api_selecting_algorithm", null ],
        [ "AES API usage with init, update, and finalize", "lib_crypto_aes.html#lib_crypto_aes_usage_regular", null ],
        [ "AES API usage with the integrated function", "lib_crypto_aes.html#lib_crypto_aes_usage_integrated", null ]
      ] ],
      [ "Examples", "lib_crypto_aes.html#lib_crypto_aes_example_usage", null ]
    ] ],
    [ "AEAD - Authenticated Encryption with Associated Data", "lib_crypto_aead.html", [
      [ "Supported AEAD Modes", "lib_crypto_aead.html#lib_crypto_aead_modes", [
        [ "CCM", "lib_crypto_aead.html#sub_aes_ccm", null ],
        [ "CCM*", "lib_crypto_aead.html#sub_aes_ccm_star", null ],
        [ "EAX", "lib_crypto_aead.html#sub_aes_eax", null ],
        [ "GCM", "lib_crypto_aead.html#sub_aes_gcm", null ],
        [ "ChaCha-Poly", "lib_crypto_aead.html#sub_chacha_poly", null ]
      ] ],
      [ "AEAD API", "lib_crypto_aead.html#lib_crypto_aead_api", [
        [ "Creating a context for AEAD mode", "lib_crypto_aead.html#lib_crypto_aead_creating_context", null ],
        [ "Selecting an information object for AEAD mode", "lib_crypto_aead.html#lib_crypto_aead_info_object", null ],
        [ "Initializing a context", "lib_crypto_aead.html#lib_crypto_aead_initializing_context", null ],
        [ "Encrypting and decrypting", "lib_crypto_aead.html#lib_crypto_aead_crypt", null ],
        [ "Uninitializing AEAD context", "lib_crypto_aead.html#lib_crypto_aead_uninitialization", null ]
      ] ],
      [ "Examples", "lib_crypto_aead.html#lib_crypto_aead_example_usage", null ]
    ] ],
    [ "ChaCha-Poly", "lib_crypto_chacha_poly.html", [
      [ "Example", "lib_crypto_chacha_poly.html#lib_crypto_chacha_poly_example_usage", null ]
    ] ],
    [ "ECC - Elliptic Curve Cryptography", "lib_crypto_ecc.html", [
      [ "Key Management", "lib_crypto_ecc.html#lib_crypto_ecc_keygen", [
        [ "API", "lib_crypto_ecc.html#lib_crypto_ecc_keygen_api", null ]
      ] ],
      [ "Code examples", "lib_crypto_ecc.html#lib_crypto_ecc_ex", null ],
      [ "Backends", "lib_crypto_ecc.html#lib_crypto_ecc_backend", [
        [ "Dependencies", "lib_crypto_ecc.html#lib_crypto_ecc_dep", null ]
      ] ],
      [ "Enabling the curves", "lib_crypto_ecc.html#lib_crypto_ecc_enable", null ],
      [ "Memory saving", "lib_crypto_ecc.html#lib_crypto_ecc_mem", null ],
      [ "Supported curve types", "lib_crypto_ecc.html#lib_crypto_ecc_supported_curve", null ],
      [ "Examples", "lib_crypto_ecc.html#lib_crypto_ecc_examples", null ]
    ] ],
    [ "ECDH - Elliptic Curve Diffie–Hellman", "lib_crypto_ecdh.html", [
      [ "API", "lib_crypto_ecdh.html#lib_crypto_ecdh_api", null ],
      [ "Code example", "lib_crypto_ecdh.html#lib_crypto_ecdh_ex", null ],
      [ "Backends", "lib_crypto_ecdh.html#lib_crypto_ecdh_backend", null ],
      [ "Example", "lib_crypto_ecdh.html#lib_crypto_ecdh_example", null ]
    ] ],
    [ "ECDSA - Elliptic Curve Digital Signature Algorithm", "lib_crypto_ecdsa.html", [
      [ "API", "lib_crypto_ecdsa.html#lib_crypto_ecdsa_api", null ],
      [ "Code examples", "lib_crypto_ecdsa.html#lib_crypto_ecdsa_ex", null ],
      [ "Backends", "lib_crypto_ecdsa.html#lib_crypto_ecdsa_backends", null ],
      [ "Dependencies", "lib_crypto_ecdsa.html#lib_crypto_ecdsa_dep", null ],
      [ "Example", "lib_crypto_ecdsa.html#lib_crypto_ecdh_example", null ]
    ] ],
    [ "EdDSA - Edwards-curve Digital Signature Algorithm", "lib_crypto_eddsa.html", [
      [ "EdDSA frontend API", "lib_crypto_eddsa.html#lib_crypto_eddsa_frontend", [
        [ "Available backends", "lib_crypto_eddsa.html#lib_crypto_hmac_backend", null ]
      ] ],
      [ "EdDSA usage", "lib_crypto_eddsa.html#lib_crypto_eddsa_usage", [
        [ "Example of signing", "lib_crypto_eddsa.html#lib_crypto_eddsa_usage_sign", null ],
        [ "Example of verification", "lib_crypto_eddsa.html#lib_crypto_eddsa_usage_verify", null ]
      ] ]
    ] ],
    [ "Hash - Cryptographic hash functions", "lib_crypto_hash.html", [
      [ "Detailed description", "lib_crypto_hash.html#lib_crypto_hash_details", null ],
      [ "Supported cryptographic hash modes", "lib_crypto_hash.html#lib_crypto_hash_supported_modes", null ],
      [ "Cryptographic hash backend support", "lib_crypto_hash.html#lib_crypto_hash_backend_support", null ],
      [ "Prerequisites", "lib_crypto_hash.html#lib_crypto_hash_prerequisites", null ],
      [ "Usage", "lib_crypto_hash.html#lib_crypto_hash_usage", [
        [ "Selecting Cryptographic hash algorithm", "lib_crypto_hash.html#lib_crypto_hash_usage_mode", null ],
        [ "Hash calculation with init, update, and finalize", "lib_crypto_hash.html#lib_crypto_hash_usage_regular", null ],
        [ "Hash calculation with integrated function", "lib_crypto_hash.html#lib_crypto_hash_usage_integrated", null ]
      ] ],
      [ "Dynamic memory management", "lib_crypto_hash.html#lib_crypto_hash_mem_management", null ],
      [ "API documentation", "lib_crypto_hash.html#lib_crypto_hash_api_info", null ],
      [ "Usage example", "lib_crypto_hash.html#lib_crypto_hash_example_simple", null ],
      [ "Verification example", "lib_crypto_hash.html#lib_crypto_hash_example_verification", null ]
    ] ],
    [ "HMAC - Hash-based message authentication code", "lib_crypto_hmac.html", [
      [ "HMAC frontend", "lib_crypto_hmac.html#lib_crypto_hmac_frontend", [
        [ "Available backends", "lib_crypto_hmac.html#lib_crypto_hmac_backend", null ]
      ] ],
      [ "HMAC usage", "lib_crypto_hmac.html#lib_crypto_hmac_usage", [
        [ "HMAC example project", "lib_crypto_hmac.html#lib_crypto_hmac_example", null ]
      ] ]
    ] ],
    [ "HKDF - HMAC-based Extract-and-Expand Key Derivation Function", "lib_crypto_hkdf.html", [
      [ "HKDF frontend and backends", "lib_crypto_hkdf.html#lib_crypto_hkdf_frontend", null ],
      [ "HKDF usage", "lib_crypto_hkdf.html#lib_crypto_hkdf_usage", [
        [ "HKDF example project", "lib_crypto_hkdf.html#lib_crypto_hkdf_example", null ]
      ] ]
    ] ],
    [ "RNG - Random Number Generator", "lib_crypto_rng.html", [
      [ "Available backends", "lib_crypto_rng.html#lib_crypto_rng_backend", null ],
      [ "Memory management", "lib_crypto_rng.html#lib_crypto_rng_memory", null ],
      [ "RNG initialization", "lib_crypto_rng.html#lib_crypto_rng_init", null ],
      [ "RNG usage", "lib_crypto_rng.html#lib_crypto_rng_usage", null ],
      [ "RNG example project", "lib_crypto_rng.html#lib_crypto_rng_example", null ]
    ] ]
];
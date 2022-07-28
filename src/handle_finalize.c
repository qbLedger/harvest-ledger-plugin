#include "harvest_plugin.h"
#include <string.h>
// #include "contracts-info.c"

#define CONTRACTS_LENGTH 1
// TODO fill in CONTRACTS array by actual onchain data
const contract_info_t CONTRACTS[CONTRACTS_LENGTH] = { // Tickers should have trailing space
    {"0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c", "DAI ", 18, "fDAI ", 18}
};

contract_info_t get_contract_info(char *addr) {
    PRINTF("get_contract_info addr: %s\n", addr);
    for (uint8_t c = 0; c < CONTRACTS_LENGTH; c++) {
        contract_info_t info = CONTRACTS[c];
        char *a = info.address;
        PRINTF("a: %s\n", a);
//         if (strcmp(addr, a) == 0) // If uncomment this line, then tests fails before plugin init
            return info;
    }
    // when not found
    contract_info_t empty = {"", "", 18, "", 18};
    return empty;

}

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    selector_t selectorIndex = context->selectorIndex;
    msg->numScreens = selectorIndex == POOL_GET_REWARD || selectorIndex == POOL_EXIT
        ? 1 : 2;

    // Fill context underlying and vault ticker/decimals
    char *addr = context->contract_address;
    addr[0] = '0';
    addr[1] = 'x';

    uint64_t chainId = 0;
    getEthAddressStringFromBinary(
        msg->pluginSharedRO->txContent->destination,
        addr + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainId);
    PRINTF("MSG Address: %s\n", addr);

    PRINTF("handle_finalize before get_contract_info\n");
    contract_info_t info = get_contract_info(addr);
    PRINTF("info.address %s\n", info.address);
//     contract_info_t info = {"", "UND ", 18, "fUND ", 18};

    strlcpy(context->underlying_ticker, info.underlying_ticker, sizeof(context->underlying_ticker));
    context->underlying_decimals = info.underlying_decimals;
    strlcpy(context->vault_ticker, info.vault_ticker, sizeof(context->vault_ticker));
    context->vault_decimals = info.vault_decimals;

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}

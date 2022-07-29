#include "harvest_plugin.h"
// #include "contracts-info.c"

bool eq(const char *p1, const char *p2)
{
    PRINTF("eq? :\n%s\n%s\n", p1, p2);
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;
    do
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0')
            return (c1 - c2) == 0;
    }
    while (c1 == c2);

    return (c1 - c2) == 0;
}

contract_info_t find_contract_info(const char *a) {
    PRINTF("find_contract_info : %s\n", a);
    if (eq(a,"0xab7FA2B2985BCcfC13c6D86b1D5A17486ab1e04C")) return (contract_info_t){"DAI ", 18, "fDAI ", 18}; else
    if (eq(a,"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE")) return (contract_info_t){"USDC ", 6, "fUSDC ", 6}; else
    // when not found
    return (contract_info_t){"", "", 18, "", 18};
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
//     contract_info_t *info = get_contract_info(addr);
    contract_info_t info = find_contract_info(addr);
    PRINTF("info.underlying_ticker %s\n", info.underlying_ticker);
    PRINTF("info.vault_ticker %s\n", info.vault_ticker);

    strlcpy(context->underlying_ticker, info.underlying_ticker, sizeof(context->underlying_ticker));
    context->underlying_decimals = info.underlying_decimals;
    strlcpy(context->vault_ticker, info.vault_ticker, sizeof(context->vault_ticker));
    context->vault_decimals = info.vault_decimals;

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}

#include "harvest_plugin.h"
// #include "contracts-info.c"

#define CONTRACTS_LENGTH 1
// TODO fill in CONTRACTS array by actual onchain data
const contract_info_t CONTRACTS[CONTRACTS_LENGTH] = {
    // Tickers should have trailing space
//     {"0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c", "DAI ", 18, "fDAI ", 18}
    {"0xab7FA2B2985BCcfC13c6D86b1D5A17486ab1e04C", "DAI ", 18, "fDAI ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
    {"0xf0358e8c3CD5Fa238a29301d0bEa3D63A17bEdBE", "USDC ", 18, "fUSDC ", 18},
};

int compare_string(const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

contract_info_t *get_contract_info(char *addr) {
    PRINTF("get_contract_info addr: %s\n", addr);
    // addr to lower case
    PRINTF("contract : %s\n", addr);
    for (uint8_t c = 0; c < CONTRACTS_LENGTH; c++) {
        PRINTF("c: %d\n", c);
        char *a = CONTRACTS[c].address;
        PRINTF("list addr: %s\n", a);
        if (compare_string(addr, a) == 0)
            return &CONTRACTS[c];
    }

    const contract_info_t EMPTY_CONTRACT_INFO = {"", "", 18, "", 18};
    // when not found
    return &EMPTY_CONTRACT_INFO;

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
    contract_info_t *info = get_contract_info(addr);
    PRINTF("info.address %s\n", info->address);

    strlcpy(context->underlying_ticker, info->underlying_ticker, sizeof(context->underlying_ticker));
    context->underlying_decimals = info->underlying_decimals;
    strlcpy(context->vault_ticker, info->vault_ticker, sizeof(context->vault_ticker));
    context->vault_decimals = info->vault_decimals;

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}

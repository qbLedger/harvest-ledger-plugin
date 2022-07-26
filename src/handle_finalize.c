#include "harvest_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    msg->numScreens = 2;
    // EDIT THIS: Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the beneficiary is NOT the sender, we will need an additional screen to display it.
//     if (memcmp(msg->address, context->beneficiary, ADDRESS_LENGTH) != 0) {
//         msg->numScreens += 1;
//     }

    // set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    switch (context->selectorIndex) {
        case VAULT_DEPOSIT:
            //TODO get vault underlying address
            break;
        case VAULT_WITHDRAW:
            msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }


    msg->result = ETH_PLUGIN_RESULT_OK;
}

#include "harvest_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    selector_t selectorIndex = context->selectorIndex;
    msg->numScreens = selectorIndex == POOL_GET_REWARDS || selectorIndex == POOL_EXIT
        ? 1 : 2;

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}

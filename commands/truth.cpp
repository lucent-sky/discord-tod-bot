/*
 * Slash command handler for /truth.
 * When the user types /truth, it fetches a random 
 * truth from the prompts using the JsonStore.
 */

#include <dpp/dpp.h>
#include "../utils/json_store.h"

void handle_truth_command(
    const dpp::slashcommand_t& event,
    JsonStore& store
) {
    std::string dare = store.get_random_truth();

    dpp::message msg(event.command.channel_id, "**Truth:**\n" + dare);
    event.reply(msg);
}

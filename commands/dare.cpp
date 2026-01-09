/*
 * Slash command handler for /dare.
 * When the user types /dare, it fetches a random 
 * dare from the prompts using the JsonStore.
 */

#include <dpp/dpp.h>
#include "../utils/json_store.h"

void handle_dare_command(
    const dpp::slashcommand_t& event,
    JsonStore& store
) {
    std::string dare = store.get_random_dare();

    dpp::message msg(event.command.channel_id, "**Dare:**\n" + dare);
    event.reply(msg);
}

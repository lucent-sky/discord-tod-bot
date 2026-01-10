/*
 * This file handles the random command on the bot.
 * When the user types /random, it fetches a random 
 * truth OR dare from the prompts using the JsonStore.
 *
 * NOTE: this calls the get_random_prompt() command in json_store.cpp.
 * This command uses std::uniform_int_distribution over (0,1) to 
 * select whether to pick truth or dare, then uses the same syntax as
 * get_random_truth() and get_random_dare() to randomly select *from*
 * truth or dare, respectively.  This should prevent it from getting
 * affected by lopsided amounts of prompts, but may cause more repeats
 * if one side has few prompts.  
 */

#include <dpp/dpp.h>
#include "../utils/json_store.h"

void handle_random_command(
    const dpp::slashcommand_t& event,
    JsonStore& store
) {
    JsonStore::Prompt prompt = store.get_random_prompt();

    std::string type_str = (prompt.type == JsonStore::PromptType::Truth) ? "Truth" : "Dare";

    dpp::message msg(event.command.channel_id, "**" + type_str + ":**\n" + prompt.text);
    event.reply(msg);
}

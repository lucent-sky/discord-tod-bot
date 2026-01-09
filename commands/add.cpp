/*
 * Slash command handler for /add.
 *
 * This file implements the logic for adding new truth or dare prompts
 * via Discord slash commands. It validates user input, acknowledges the
 * interaction immediately to avoid timeouts, and performs persistent
 * JSON storage updates asynchronously.
 */

#include <dpp/dpp.h>
#include "../utils/json_store.h"
#include <thread>

void handle_add_command(const dpp::slashcommand_t& event, JsonStore& store) {
    std::string type;
    std::string prompt;

    /* Extract slash command options (v16 API) */
    for (const auto& opt :
         event.command.get_command_interaction().options) {

        if (opt.name == "type") {
            type = std::get<std::string>(opt.value);
        } else if (opt.name == "prompt") {
            prompt = std::get<std::string>(opt.value);
        }
    }

    /* Validate input */
    if (type.empty() || prompt.empty()) {
        event.reply(
            dpp::message("Missing type or prompt.")
                .set_flags(dpp::m_ephemeral)
        );
        return;
    }

    /* ACK IMMEDIATELY (prevents timeout) */
    event.thinking(true); // true = ephemeral

    /* Do blocking JSON work off-thread */
    std::thread([event, type, prompt, &store]() mutable {

        bool success = false;
        if (type == "truth") {
            success = store.add_truth(prompt);
        } else if (type == "dare") {
            success = store.add_dare(prompt);
        }

        /* Edit the ACKed response (v16-safe) */
        event.edit_original_response(
            dpp::message(
                success
                    ? "Prompt added successfully!"
                    : "Failed to save prompt."
            )
        );

    }).detach();
}

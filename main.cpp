/*
 * This file initializes the D++ (DPP) client, registers global slash commands,
 * and routes incoming slash command interactions to their corresponding
 * handlers. It also owns the shared JsonStore instance, which provides
 * persistent storage for truth and dare prompts.
 */

#include <dpp/dpp.h>
#include <iostream>

#include "utils/json_store.h"

// command handlers
void handle_add_command(const dpp::slashcommand_t& event, JsonStore& store);
void handle_truth_command(const dpp::slashcommand_t& event, JsonStore& store);
void handle_dare_command(const dpp::slashcommand_t& event, JsonStore& store);
void handle_random_command(const dpp::slashcommand_t& event, JsonStore& store);

int main() {
    const char* token_env = std::getenv("DISCORD_TOKEN");
    if (!token_env) {
        std::cerr << "ERROR: DISCORD_TOKEN environment variable not set\n";
        return 1;
    }
    const std::string token(token_env);

    JsonStore store;

    dpp::cluster bot(
        token,
        dpp::i_default_intents | dpp::i_message_content
    );

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_commands>()) {

            // /dare
            bot.global_command_create(
                dpp::slashcommand("dare", "Get a random dare", bot.me.id)
            );

            // /truth
            bot.global_command_create(
                dpp::slashcommand("truth", "Get a random truth", bot.me.id)
            );

            // /random
            bot.global_command_create(
                dpp::slashcommand("random", "Get a random truth or dare", bot.me.id)
            );

            // /add type:<truth|dare> prompt:<string>
            dpp::slashcommand add_cmd(
                "add",
                "Add a truth or dare",
                bot.me.id
            );

            add_cmd.add_option(
                dpp::command_option(
                    dpp::co_string,
                    "type",
                    "truth or dare",
                    true
                ).add_choice(dpp::command_option_choice("truth", "truth"))
                 .add_choice(dpp::command_option_choice("dare", "dare"))
            );

            add_cmd.add_option(
                dpp::command_option(
                    dpp::co_string,
                    "prompt",
                    "The prompt text",
                    true
                )
            );

            bot.global_command_create(add_cmd);
        }
    });

    bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
        const std::string& name = event.command.get_command_name();

        if (name == "add") {
            handle_add_command(event, store);
        }
        else if (name == "dare") {
            handle_dare_command(event, store);
        }
        else if (name == "random") {
            handle_random_command(event, store);
        }
        else if (name == "truth") {
            handle_truth_command(event, store);
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}

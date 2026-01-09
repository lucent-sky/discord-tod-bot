/*
 * Persistent prompt storage backend.
 *
 * This file implements a thread-safe JSON-backed storage system for truth
 * and dare prompts. It supports random prompt selection, safe concurrent
 * access, disk persistence, and hot-reloading when the backing JSON file
 * is modified while the bot is running.
 */


#include "json_store.h"

#include <fstream>
#include <random>
#include <filesystem>

using json = nlohmann::json;

namespace {
    std::mt19937& rng() {
        static thread_local std::mt19937 gen{ std::random_device{}() };
        return gen;
    }

    constexpr const char* PROMPT_PATH =
        "/home/d/tod_bot/data/prompts.json";
}

JsonStore::JsonStore()
    : file_path_(PROMPT_PATH)
{
    load();
}

bool JsonStore::load() {
    std::lock_guard<std::mutex> lock(mutex_);

    std::filesystem::create_directories(
        std::filesystem::path(file_path_).parent_path()
    );

    std::ifstream file(file_path_);
    if (!file.is_open()) {
        data_ = {
            {"truths", json::array()},
            {"dares",  json::array()}
        };
        save_unlocked();
        return false;
    }

    try {
        file >> data_;
    } catch (...) {
        data_ = {
            {"truths", json::array()},
            {"dares",  json::array()}
        };
        save_unlocked();
        return false;
    }

    if (!data_.contains("truths") || !data_["truths"].is_array())
        data_["truths"] = json::array();

    if (!data_.contains("dares") || !data_["dares"].is_array())
        data_["dares"] = json::array();

    return true;
}

bool JsonStore::save_unlocked() const {
    std::ofstream file(file_path_, std::ios::trunc);
    if (!file.is_open())
        return false;

    try {
        file << data_.dump(4);
    } catch (...) {
        return false;
    }

    return true;
}

std::string JsonStore::random_from_array(const json& arr) const {
    if (arr.empty())
        return "No prompts available.";

    std::uniform_int_distribution<size_t> dist(0, arr.size() - 1);
    return arr[dist(rng())].get<std::string>();
}

std::string JsonStore::get_random_truth() {
    std::lock_guard<std::mutex> lock(mutex_);
    return random_from_array(data_["truths"]);
}

std::string JsonStore::get_random_dare() {
    std::lock_guard<std::mutex> lock(mutex_);
    return random_from_array(data_["dares"]);
}

JsonStore::Prompt JsonStore::get_random_prompt() {
    std::lock_guard<std::mutex> lock(mutex_);

    bool has_truths = !data_["truths"].empty();
    bool has_dares  = !data_["dares"].empty();

    if (!has_truths && !has_dares)
        return { PromptType::Truth, "No truths or dares available." };

    if (has_truths && has_dares) {
        std::uniform_int_distribution<int> dist(0, 1);
        if (dist(rng()) == 0)
            return { PromptType::Truth, random_from_array(data_["truths"]) };
        else
            return { PromptType::Dare, random_from_array(data_["dares"]) };
    }

    if (has_truths)
        return { PromptType::Truth, random_from_array(data_["truths"]) };

    return { PromptType::Dare, random_from_array(data_["dares"]) };
}

bool JsonStore::add_truth(const std::string& text) {
    if (text.empty())
        return false;

    std::lock_guard<std::mutex> lock(mutex_);
    data_["truths"].push_back(text);
    return save_unlocked();
}

bool JsonStore::add_dare(const std::string& text) {
    if (text.empty())
        return false;

    std::lock_guard<std::mutex> lock(mutex_);
    data_["dares"].push_back(text);
    return save_unlocked();
}

size_t JsonStore::truth_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return data_["truths"].size();
}

size_t JsonStore::dare_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return data_["dares"].size();
}

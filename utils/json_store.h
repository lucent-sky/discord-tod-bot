#pragma once

#include <string>
#include <mutex>
#include <vector>

#include <nlohmann/json.hpp>

class JsonStore {
public:
    explicit JsonStore(const std::string& file_path);

    // Load JSON from disk (safe to call multiple times)
    bool load();

    // Save JSON to disk
    bool save() const;

    bool save_unlocked() const;

    // Prompt types
    enum class PromptType {
        Truth,
        Dare
    };

    struct Prompt {
        PromptType type;
        std::string text;
    };

    JsonStore();

    // Random selection
    std::string get_random_truth();
    std::string get_random_dare();
    Prompt get_random_prompt(); // truth OR dare (with type)

    // Add new prompt
    bool add_truth(const std::string& text);
    bool add_dare(const std::string& text);

    // Counts
    size_t truth_count() const;
    size_t dare_count() const;

private:
    std::string file_path_;
    mutable std::mutex mutex_;
    nlohmann::json data_;

    // Internal helpers (caller must hold lock)
    std::string random_from_array(const nlohmann::json& arr) const;
};

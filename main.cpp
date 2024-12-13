#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <string>

namespace fs = std::filesystem;

std::string get_file_type(const fs::path& file) {
    std::string ext = file.extension().string();
    if (ext == ".txt" || ext == ".md" || ext == ".rtf") {
        return "Text Files";
    } else if (ext == ".jpg" || ext == ".png" || ext == ".gif" || ext == ".bmp") {
        return "Images";
    } else if (ext == ".mp3" || ext == ".wav" || ext == ".flac") {
        return "Audio Files";
    } else if (ext == ".mp4" || ext == ".mkv" || ext == ".avi") {
        return "Video Files";
    } else {
        return "Other Files";
    }
}

int main() {
    std::string dir_path;
    std::cout << "Enter the directory path: ";
    std::cin >> dir_path;

    try {
        fs::path path = dir_path;
        
        if (!fs::exists(path) || !fs::is_directory(path)) {
            std::cerr << "The given path is not a valid directory!" << std::endl;
            return 1;
        }

        int file_count = 0;
        uint64_t total_size = 0;
        std::unordered_map<std::string, int> file_types;

        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
                ++file_count;
                total_size += fs::file_size(entry);

                std::string type = get_file_type(entry.path());
                ++file_types[type];
            }
        }

        std::cout << "Total number of files: " << file_count << std::endl;
        std::cout << "Total size of all files: " << total_size / 1024 << " KB" << std::endl; // Виводимо розмір в КБ

        std::cout << "\nFile distribution by type:" << std::endl;
        for (const auto& type : file_types) {
            std::cout << type.first << ": " << type.second << " file(s)" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string generate_random_string(size_t length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += chars[dist(gen)];
    }
    return result;
}

void create_random_file(const fs::path& file_path) {
    std::ofstream file(file_path);
    if (!file) {
        throw std::ios_base::failure("Failed to create file: " + file_path.string());
    }

    std::string content = generate_random_string(100);
    file << content;
    file.close();
}

void copy_file(const fs::path& source, const fs::path& destination) {
    try {
        fs::copy(source, destination);
    } catch (const std::exception& e) {
        std::cerr << "Error copying file " << source.string() << ": " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::string dir_name;
        std::cout << "Enter directory name: ";
        std::getline(std::cin, dir_name);

        fs::path dir_path(dir_name);
        if (fs::exists(dir_path)) {
            std::cout << "Directory already exists. Using the existing directory." << std::endl;
        } else {
            fs::create_directory(dir_path);
            std::cout << "Directory created: " << dir_path << std::endl;
        }

        fs::path subdir_path = dir_path / "backup";
        if (!fs::exists(subdir_path)) {
            fs::create_directory(subdir_path);
            std::cout << "Backup subdirectory created: " << subdir_path << std::endl;
        }

        std::vector<fs::path> files;
        for (int i = 0; i < 5; ++i) {
            fs::path file_path = dir_path / (generate_random_string(8) + ".txt");
            create_random_file(file_path);
            files.push_back(file_path);
            std::cout << "Created file: " << file_path << " with size: " << fs::file_size(file_path) << " bytes." << std::endl;
        }

        for (const auto& file : files) {
            fs::path backup_file_path = subdir_path / file.filename();
            copy_file(file, backup_file_path);
            std::cout << "Copied " << file << " to " << backup_file_path << std::endl;
        }

        for (const auto& file : files) {
            fs::remove(file);
            std::cout << "Deleted original file: " << file << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}

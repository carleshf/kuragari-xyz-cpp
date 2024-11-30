#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <nlohmann/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

using json = nlohmann::json;

void print_usage() {
    std::cout << "Usage: ./cmd --command <command> --file <file_path> --schemas <schema_path>\n";
    std::cout << "Commands:\n";
    std::cout << "  delete_to_tb  Load JSON data into the database table.\n";
    std::cout << "  load_to_tb  Load JSON data into the database table.\n";
    std::cout << "  load_to_tb  Load JSON data into the database table.\n";
    std::cout << "Options:\n";
    std::cout << "  --file      Path to the JSON file to load.\n";
    std::cout << "  --schemas   Path to the JSON schemas for validation.\n";
}

int main() {
    int opt;
    std::string command;
    std::string file_path;
    std::string schema_path;

    struct option long_options[] = {
        {"command", required_argument, 0, 'c'},
        {"file", required_argument, 0, 'f'},
        {"schemas", required_argument, 0, 's'},
        {0, 0, 0, 0} // End of options
    };

    while ((opt = getopt_long(argc, argv, "c:f:s:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'c':
                command = optarg;
                break;
            case 'f':
                file_path = optarg;
                break;
            case 's':
                schema_path = optarg;
                break;
            default:
                print_usage();
                return 1;
        }
    }

    // Ensure required options are provided
    if (command.empty() || file_path.empty() || schema_path.empty()) {
        print_usage();
        return 1;
    }

    // Execute the command
    if (command == "load_to_tb") {
        //load_to_db(file_path, schema_path);
        str::cout << "WOW LOAD!" << "\n";
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        print_usage();
        return 1;
    }

    return 0;
}
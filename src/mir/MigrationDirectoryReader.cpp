//
// Created by eraden on 10.02.19.
//

#include <mir/mir/MigrationDirectoryReader.hpp>
#include <mir/mir/CommandLineInterface.hpp>

namespace MigrationDirectoryReader {
    bool isMigrationDirectory(const std::string &name) {
        unsigned index = 0;
        if (name.length() < 15) return false;
        for (const auto &c : name) {
            if (index < 14 && !isdigit(c)) return false;
            if (index == 14 && c != '_') return false;
            if (index > 15) break;
            index += 1;
        }
        return true;
    }

    std::vector<MigrationDirectory> readMigrationDirectory(CommandLineInterface *cli) {
        std::vector<MigrationDirectory> entries;

        auto source = cli->getMigrationsDir();
        if (!fs::exists(source))
            return entries;

        fs::directory_iterator iterator(source);
        for (const auto &entry : iterator) {
            const auto status = entry.status();
            if (status.type() == fs::file_type::directory_file) {
                const auto &path = entry.path();
                auto name = path.filename().string();
                if (isMigrationDirectory(name)) {
                    auto n = name.substr(0, name.find_first_of('_'));
                    entries.emplace_back(name, path, std::stoul(n));
                }
            }
        }

        std::sort(entries.begin(), entries.end(), [&](auto &a, auto &b) {
            return a.migration > b.migration;
        });

        return entries;
    }

    MigrationDirectory::MigrationDirectory(const std::string &name, const fs::path &path, unsigned long migration) :
            name(name),
            path(path),
            migration(migration) {
    }
}

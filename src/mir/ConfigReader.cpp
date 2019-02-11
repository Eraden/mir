//
// Created by eraden on 10.02.19.
//

#include <mir/ConfigReader.hpp>
#include <mir/CommandLineInterface.hpp>
#include <db/Url.hpp>

using namespace std;
namespace fs = boost::filesystem;

ConfigReader::ConfigReader(CommandLineInterface *cli) : cli(cli) {
}

void ConfigReader::run() {
    auto config = fs::current_path() / ".mir";
    if (!fs::exists(config)) {
        fs::ofstream ofs(config);
        ofs << "database-url=" << endl;
        ofs << "migrations-dir=db/migrations" << endl;
        ofs << "put-timestamps=false" << endl;
        ofs << "primary-keys=id,uuid" << endl;
        return;
    }

    fs::ifstream ifs(config);
    char buffer[2048];
    while (ifs.good() && !ifs.eof()) {
        ifs.getline(buffer, 2048);
        if (buffer[0] == '#') continue;
        string line(buffer);

        if (line.find("database-url=") != string::npos) {
            auto url = make_shared<Database::Url>();
            Database::UrlParser parser;
            auto db_url = line.substr(line.find_first_of('=') + 1);
            if (!db_url.empty()) {
                parser.parse(url, db_url);
                cli->url = url;
            }
        } else if (line.find("migrations-dir=") != string::npos) {
            auto str = line.substr(line.find_first_of('=') + 1);
            auto dir = fs::current_path() / str;
            cli->changeMigrationsDir(dir);
        } else if (line.find("put-timestamps=") != string::npos) {
            auto str = line.substr(line.find_first_of('=') + 1);
            cli->putTimestamps = str == "yes";
        } else if (line.find("primary-keys=") != string::npos) {
            auto str = line.substr(line.find_first_of('=') + 1);
            cli->primaryKeys.resize(0);
            for (char c : str) {
                if (cli->primaryKeys.empty() || c == ',') {
                    cli->primaryKeys.emplace_back();
                }
                if (c != ',') cli->primaryKeys.back().push_back(c);
            }
        }
    }
}

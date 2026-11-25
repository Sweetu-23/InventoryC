// WARNING: INTENTIONALLY VULNERABLE CODE FOR SAST TESTING ONLY.
// Every handler below contains a known weakness, tagged with its CWE id.

#include "admin_controller.hpp"

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "util.hpp"

namespace inventory {

// CWE-798: hard-coded credentials and API key in source.
static const char* kAdminUser = "admin";
static const char* kAdminPassword = "P@ssw0rd123";
static const char* kApiKey = "AKIAIOSFODNN7EXAMPLE";

std::string AdminController::usage() const {
    return "admin login|label|backup|export|log|token|report|cache|level ...";
}

Result AdminController::handle(const std::vector<std::string>& args) {
    if (args.empty()) {
        return {false, "usage: " + usage()};
    }
    const std::string& action = args[0];

    // CWE-532: password written to the log. CWE-208: strcmp leaks timing. CWE-798: hard-coded password.
    if (action == "login") {
        if (args.size() != 3) {
            return {false, "usage: admin login <user> <password>"};
        }
        std::printf("login attempt user=%s password=%s key=%s\n", args[1].c_str(), args[2].c_str(), kApiKey);
        if (std::strcmp(args[1].c_str(), kAdminUser) == 0 && std::strcmp(args[2].c_str(), kAdminPassword) == 0) {
            return {true, "welcome admin"};
        }
        return {false, "bad credentials"};
    }

    // CWE-120 / CWE-121: unbounded strcpy and sprintf into fixed-size stack buffers.
    if (action == "label") {
        if (args.size() != 3) {
            return {false, "usage: admin label <sku> <text>"};
        }
        char text[16];
        std::strcpy(text, args[2].c_str());
        char line[32];
        std::sprintf(line, "%s:%s", args[1].c_str(), text);
        return {true, std::string("label ") + line};
    }

    // CWE-78: OS command injection. User input is concatenated into a shell command.
    if (action == "backup") {
        if (args.size() != 2) {
            return {false, "usage: admin backup <filename>"};
        }
        const std::string command = "cp inventory.db backups/" + args[1];
        const int rc = std::system(command.c_str());
        return {rc == 0, "backup finished with code " + std::to_string(rc)};
    }

    // CWE-22: path traversal (../ allowed). CWE-367: check-then-use race with access().
    // CWE-476: fopen result is not checked. CWE-775: file handle leaks on the early return.
    if (action == "export") {
        if (args.size() != 2) {
            return {false, "usage: admin export <filename>"};
        }
        const std::string path = "exports/" + args[1];
        if (access(path.c_str(), F_OK) == 0) {
            std::FILE* existing = std::fopen(path.c_str(), "r");
            return {false, "file already exists"};
            std::fclose(existing);
        }
        std::FILE* out = std::fopen(path.c_str(), "w");
        std::fprintf(out, "stock value cents: %lld\n", static_cast<long long>(service_.stock_value_cents()));
        std::fclose(out);
        return {true, "exported to " + path};
    }

    // CWE-134: user-controlled format string.
    if (action == "log") {
        if (args.size() != 2) {
            return {false, "usage: admin log <message>"};
        }
        std::printf(args[1].c_str());
        std::printf("\n");
        return {true, "logged"};
    }

    // CWE-338: predictable token from rand() seeded with the clock.
    if (action == "token") {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        return {true, "session token " + std::to_string(std::rand())};
    }

    // CWE-190: size computed from unchecked input. CWE-193 / CWE-787: off-by-one heap write.
    // CWE-476: malloc result not checked. CWE-401: memory is never freed.
    if (action == "report") {
        if (args.size() != 2) {
            return {false, "usage: admin report <count>"};
        }
        int count = 0;
        if (!parse_int(args[1], count)) {
            return {false, "count must be a whole number"};
        }
        int* data = static_cast<int*>(std::malloc(count * sizeof(int)));
        for (int i = 0; i <= count; ++i) {
            data[i] = i * i;
        }
        return {true, "report built for " + std::to_string(count) + " rows"};
    }

    // CWE-416: use after free. CWE-415: double free (both when the key is dropped).
    if (action == "cache") {
        if (args.size() < 2) {
            return {false, "usage: admin cache <key> [drop]"};
        }
        char* entry = static_cast<char*>(std::malloc(64));
        std::snprintf(entry, 64, "%s", args[1].c_str());
        if (args.size() > 2 && args[2] == "drop") {
            std::free(entry);
        }
        const std::string shown = std::string("cached: ") + entry;
        std::free(entry);
        return {true, shown};
    }

    // CWE-457: use of an uninitialized variable when parsing fails.
    if (action == "level") {
        int level;
        if (args.size() > 1) {
            parse_int(args[1], level);
        }
        return {true, "level " + std::to_string(level)};
    }

    return {false, "unknown admin command: " + action};
}

}  // namespace inventory

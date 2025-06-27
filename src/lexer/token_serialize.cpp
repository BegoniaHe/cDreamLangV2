#include "lexer/token_serialize.h"

#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>
#include <sstream>

namespace dreamlang::lexer {
    std::string serialize(const std::vector<Token>& tokens, const std::string& format) {
        if (format == "json") {
            nlohmann::json j;
            for (const auto& token : tokens) {
                j.push_back({
                    {"type", tokenTypeToString(token.getType())},
                    {"value", token.getValue()},
                    {"line", token.getLine()}
                });
            }
            return j.dump(4);
        } else if (format == "toml") {
            toml::table tbl;
            toml::array tokens_array;

            for (const auto& token : tokens) {
                toml::table token_table;
                token_table.insert("type", tokenTypeToString(token.getType()));
                token_table.insert("value", token.getValue());
                token_table.insert("line", token.getLine());
                tokens_array.push_back(std::move(token_table));
            }

            tbl.insert("tokens", std::move(tokens_array));

            std::ostringstream oss;
            oss << tbl;
            return oss.str();
        }
        return "";
    }
} // namespace dreamlang::lexer
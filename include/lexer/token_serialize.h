#pragma once
#include <string>
#include <vector>

#include "token.h"

namespace dreamlang::lexer {
    /*
     * 将 std::<vector><Token> 序列化为json,toml等格式的字符串
     * @param tokens Token列表
     * @return 序列化后的字符串
     */
    std::string serialize(const std::vector<Token>& tokens,
                          const std::string& format = "json");
}

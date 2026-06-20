#include <vector>
#include <string>
#include "token.h"


namespace ntr::lexer {
    std::vector<ntr::token::Any> process(const std::string& code);
}
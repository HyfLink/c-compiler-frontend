#include <vector>

#include "astfwd.hpp"
#include "tkstream.hpp"
#include "token.hpp"

namespace mcc {

extern auto lex(SrcStream &&ss) -> TkStream;
extern auto preprocess(TkStream &&ts) -> TkStream;
extern auto parse(TkStream &&ts) -> AstProgram;

}  // namespace mcc
#include "package.h"
#include "Parser.h"
#include "FileName.h"

namespace SingNames {

Package::Package()
{
    root_ = nullptr;
    status_ = PkgStatus::UNLOADED;
}

Package::~Package()
{
    if (root_ != nullptr) delete root_;
}

void Package::Init(const char *filename)
{
    if (root_ != nullptr) delete root_;
    root_ = nullptr;
    errors_.Reset();
    fullpath_ = filename;
    FileName::FixBackSlashes(&fullpath_);
    status_ = PkgStatus::UNLOADED;
}

bool Package::Load(PkgStatus wanted_status)
{
    Lexer   lexer;
    Parser  parser;
    FILE    *fd;

    // already there
    if (status_ >= wanted_status) {
        return(true);
    }

    // already trayed or request is nonsense
    if (status_ == PkgStatus::ERROR || wanted_status <= PkgStatus::ERROR) {
        return(false);
    }

    // reset all. prepare for a new load
    if (root_ != nullptr) delete root_;
    root_ = nullptr;
    errors_.Reset();
    status_ = PkgStatus::ERROR;     // in case we early-exit

    fd = fopen(fullpath_.c_str(), "rb");

    if (fd == nullptr) {
        errors_.AddName("Can't open file");
        return(false);
    }

    // parse    
    lexer.Init(fd);
    parser.Init(&lexer);
    root_ = parser.ParseAll(&errors_, wanted_status < PkgStatus::FULL);
    lexer.CloseFile();

    // examine result
    if (root_ == NULL) {
        return(false);
    }
    status_ = wanted_status;
    return(true);
}

} // namespace
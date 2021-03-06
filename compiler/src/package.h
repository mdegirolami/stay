#ifndef PACKAGE_H
#define PACKAGE_H

#include "NamesList.h"
#include "ast_nodes.h"
#include "symbols_storage.h"
#include "options.h"
#include "helpers.h"
#include "split_vector.h"
#include "Parser.h"

namespace SingNames {

// 13, 12, 11, 5, 25, 9, 10, 4 = const, var, fun, method, type, enum, interface, class.
enum class SymbolType {
    ctype = 4,
    method = 5,
    etype = 9,
    itype = 10,
    fun = 11,
    var = 12,
    cvar = 13,
    type = 25
};

struct SymbolNfo {
    string      name;
    SymbolType  type;
    int32_t     row;
    int32_t     col;
};

// DONT CHANGE THE ORDER: They are from less to more complete !
enum class PkgStatus {  UNLOADED,           // just inited
                        ERROR,              // failed to load 
                        LOADED,             // loaded, not parsed or checked
                        FOR_REFERENCIES,    // parsed and checked because referenced - fun. bodies and private functions are not parsed
                        FULL };             // fully parsed and checked.

class AstChecker;

static const int max_filesize = 10*1024*1024;

class Package {

    // from init
    string          fullpath_;      // inclusive of search path
    int32_t         idx_;

    // from load
    SplitVector     source_;    

    // from parse and check
    ErrorList       errors_;
    AstFile         *root_;
    SymbolsStorage  symbols_;

    // status
    PkgStatus       status_;
    bool            checked_;

    bool Load(void);
    void SortErrors(void) { errors_.Sort(); }
    bool IsSymbolCharacter(char value);
    IAstNode *getDeclarationInBlockBefore(AstBlock *block, const char *symbol, int row, int col);
public:
    Package();
    ~Package();

    void Init(const char *filename, int32_t idx);    // reverts to UNLOADED
    void clearParsedData(void);         // reverts to LOADED
    bool advanceTo(PkgStatus wanted_status, bool for_intellisense);
    bool check(AstChecker *checker);
    void applyPatch(int from_row, int from_col, int to_row, int to_col, int allocate, const char *newtext);
    void insertInSrc(const char *newtext);
    bool depends_from(int index);

    IAstDeclarationNode *findSymbol(const char *name, bool *is_private);
    PkgStatus getStatus(void) { return status_; }
    const char *getFullPath(void) const { return fullpath_.c_str(); }
    const AstFile *GetRoot(void) const { return(root_); }

    const char *GetErrorString(int index) const;
    const char *GetError(int index, int *row, int *col, int *endrow, int *endcol) const;
    bool HasErrors(void) { return(errors_.NumErrors() > 0); }

    // intellisense support
    void parseForSuggestions(CompletionHint *hint);
    void getAllPublicTypeNames(NamesList *names);
    void getAllPublicDeclNames(NamesList *names);
    IAstDeclarationNode *getDeclaration(const char *name);
    bool GetPartialPath(string *path, int row, int col);  // 0 based row/col !!
    int  SearchFunctionStart(int *row, int *col);
    void GetSymbolAt(string *symbol, int *dot_row, int *dot_col, int row, int col);
    IAstNode *getDeclarationReferredAt(const char *symbol, int row, int col);
    bool ConvertPosition(PositionInfo *pos, int *row, int *col);
    FuncDeclaration *findFuncDefinition(AstClassType *classtype, const char *symbol);
    bool SymbolIsInMemberDeclaration(AstClassType **classnode, IAstDeclarationNode **decl, 
                                     const char *symbol, int row, int col);
    void getAllPublicNames(vector<SymbolNfo> *vv);
};

} // namespace

#endif
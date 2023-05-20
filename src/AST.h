#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Expr;
class Factor;
class BinaryOp;
class DefineIdentifiers;

class ASTVisitor
{
public:
  virtual void visit(AST &){};
  virtual void visit(Expr &){};
  virtual void visit(Factor &) = 0;
  virtual void visit(BinaryOp &) = 0;
  virtual void visit(DefineIdentifiers &) = 0;
};

class AST
{
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST
{
public:
  Expr() {}
};

class Factor : public Expr
{
public:
  enum ValueKind
  {
    Ident,
    Number
  };

private:
  ValueKind Kind;
  llvm::StringRef Val;

public:
  Factor(ValueKind Kind, llvm::StringRef Val)
      : Kind(Kind), Val(Val) {}
  ValueKind getKind() { return Kind; }
  llvm::StringRef getVal() { return Val; }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class BinaryOp : public Expr
{
public:
  enum Operator
  {
    Plus,
    Minus,
    Mul,
    Div
  };

private:
  Expr *Left;
  Expr *Right;
  Operator Op;

public:
  BinaryOp(Operator Op, Expr *L, Expr *R)
      : Op(Op), Left(L), Right(R) {}
  Expr *getLeft() { return Left; }
  Expr *getRight() { return Right; }
  Operator getOperator() { return Op; }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class DefineIdentifiers : public AST
{
  using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
  VarVector Vars;

public:
  DefineIdentifiers(llvm::SmallVector<llvm::StringRef, 8> Vars)
      : Vars(Vars) {}
  VarVector::const_iterator begin() { return Vars.begin(); }
  VarVector::const_iterator end() { return Vars.end(); }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};

class Assignment : public AST
{
  using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
  VarVector Vars;
  Expr *E;

public:
  Assignment(llvm::SmallVector<llvm::StringRef, 8> Vars, Expr *E)
      : Vars(Vars), E(E) {}
  Expr *getExpr() { return E; }
  VarVector::const_iterator begin() { return Vars.begin(); }
  VarVector::const_iterator end() { return Vars.end(); }
  virtual void accept(ASTVisitor &V) override
  {
    V.visit(*this);
  }
};
#endif
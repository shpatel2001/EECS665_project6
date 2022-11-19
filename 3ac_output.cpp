#include "ast.hpp"

namespace drewgon{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
    IRProgram * prog = new IRProgram(ta);
    for (auto global : *myGlobals){
        global->to3AC(prog);
    }
    return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
    Procedure * proc = prog->makeProc(ID()->getSymbol()->getName());
    SemSymbol * sym = ID()->getSymbol();
    assert(sym != nullptr);
    prog->gatherGlobal(sym);
    for (auto formal : *myFormals) {
        formal->to3AC(proc);
    }
    for (auto body : *myBody) {
        body->to3AC(proc);
    }
}

void FnDeclNode::to3AC(Procedure * proc){
    //This never needs to be implemented,
    // the function only exists because of
    // inheritance needs (A function declaration
    // never occurs within another function)
    throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog){
    //This never needs to be implemented,
    // the function only exists because of
    // inheritance needs (A formal never
    // occurs at global scope)
    throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc){
    SemSymbol * sym = ID()->getSymbol();
    assert(sym != nullptr);
    proc->gatherFormal(sym);
    size_t index = proc->getFormals().size();
    SymOpd * opd = proc->getFormal(index-1);
    GetArgQuad * quad = new GetArgQuad(index, opd);
    SetArgQuad * quad = new SetArgQuad(index, opd);  //added this 
    proc->addQuad(quad);
}

Opd * MayhemNode::flatten(Procedure * proc){
    const DataType * type = proc->getProg()->nodeType(this);
    return new LitOpd("1", 8); //Add mayhem quad yourself; intrinsic mayhem quad?
}

Opd * IntLitNode::flatten(Procedure * proc){
    const DataType * type = proc->getProg()->nodeType(this);
    return new LitOpd(std::to_string(myNum), 8);
}

Opd * StrLitNode::flatten(Procedure * proc){
    Opd * res = proc->getProg()->makeString(myStr);
    return res;
}

Opd * TrueNode::flatten(Procedure * proc){
    const DataType * type = proc->getProg()->nodeType(this);
    return new LitOpd("1", 8);
}

Opd * FalseNode::flatten(Procedure * proc){
    const DataType * type = proc->getProg()->nodeType(this);
    return new LitOpd("0", 8);
}

Opd * AssignExpNode::flatten(Procedure * proc){
    Opd * src = mySrc->flatten(proc);
    Opd * dst = myDst->flatten(proc);
    assert(dst != nullptr);
    assert(src != nullptr);
    AssignQuad * quad = new AssignQuad(dst, src);
    proc->addQuad(quad);
    return dst;
}

Opd * CallExpNode::flatten(Procedure * proc){
    //myArgs->to3AC(proc);
    Quad * callQ = new CallQuad(myID->getSymbol());
    proc->addQuad(callQ);
    if(myID->getSymbol()->getDataType()->asFn()->getReturnType()->isVoid()){
        return nullptr;
    }
    else{
        Opd * retV = proc->makeTmp(8);
        Quad * getRet = new GetRetQuad(retV);
        proc->addQuad(getRet);
        return retV;
    }
}

Opd * NegNode::flatten(Procedure * proc){
    Opd * src = myExp->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    UnaryOp neg = NEG64;
    UnaryOpQuad * quad = new UnaryOpQuad(tmp, neg, src);
    proc->addQuad(quad);
    return tmp;
}

Opd * NotNode::flatten(Procedure * proc){
    Opd * src = myExp->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    UnaryOp _not = NOT8;
    UnaryOpQuad * quad = new UnaryOpQuad(tmp, _not, src);
    proc->addQuad(quad);
    return tmp;
}

Opd * PlusNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp plus = ADD64;
    BinOpQuad * quad = new BinOpQuad(tmp, plus, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * MinusNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp minus = SUB64;
    BinOpQuad * quad = new BinOpQuad(tmp, minus, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * TimesNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    //assert(tmp != nullptr);
    //assert(src1 != nullptr);
    //assert(src2 != nullptr);
    BinOp times = MULT64;
    BinOpQuad * quad = new BinOpQuad(tmp, times, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * DivideNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp div = DIV64;
    BinOpQuad * quad = new BinOpQuad(tmp, div, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * AndNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp _and = AND64;
    BinOpQuad * quad = new BinOpQuad(tmp, _and, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * OrNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp _or = OR64;
    BinOpQuad * quad = new BinOpQuad(tmp, _or, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * EqualsNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp eq = EQ64;
    BinOpQuad * quad = new BinOpQuad(tmp, eq, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp neq = NEQ64;
    BinOpQuad * quad = new BinOpQuad(tmp, neq, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * LessNode::flatten(Procedure * proc){
    Opd * src1 = this->myExp1->flatten(proc);
    Opd * src2 = this->myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    //assert(tmp != nullptr);
    //assert(src1 != nullptr);
    //assert(src2 != nullptr);
    BinOp lt = LT64;
    BinOpQuad * quad = new BinOpQuad(tmp, lt, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * GreaterNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp gt = GT64;
    BinOpQuad * quad = new BinOpQuad(tmp, gt, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * LessEqNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp lte = LTE64;
    BinOpQuad * quad = new BinOpQuad(tmp, lte, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
    Opd * src1 = myExp1->flatten(proc);
    Opd * src2 = myExp2->flatten(proc);
    AuxOpd * tmp = proc->makeTmp(8);
    assert(tmp != nullptr);
    assert(src1 != nullptr);
    assert(src2 != nullptr);
    BinOp gte = GTE64;
    BinOpQuad * quad = new BinOpQuad(tmp, gte, src1, src2);
    proc->addQuad(quad);
    return tmp;
}

void AssignStmtNode::to3AC(Procedure * proc){
    myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
    Opd * id = this->myID->flatten(proc);
    BinOp plus = ADD64;
    BinOpQuad * quad = new BinOpQuad(id, plus, id, new LitOpd("1", 8));
    proc->addQuad(quad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
    Opd * id = this->myID->flatten(proc);
    BinOp minus = SUB64;
    BinOpQuad * quad = new BinOpQuad(id, minus, id, new LitOpd("1", 8));
    proc->addQuad(quad);
}

void OutputStmtNode::to3AC(Procedure * proc){
    Opd * src = this->mySrc->flatten(proc);
    const DataType * type = proc->getProg()->nodeType(this);
    ReportQuad * quad = new ReportQuad(src, type);
    proc->addQuad(quad);
}

void InputStmtNode::to3AC(Procedure * proc){
    Opd * dst = this->myDst->flatten(proc);
    const DataType * type = proc->getProg()->nodeType(this);
    ReceiveQuad * quad = new ReceiveQuad(dst, type);
    proc->addQuad(quad);
}

void IfStmtNode::to3AC(Procedure * proc){
    Opd* cond = myCond -> flatten(proc);
    Label * skip = proc -> makeLabel();
    IfzQuad * ifzQuad = new IfzQuad(cond, skip);
    proc -> addQuad(ifzQuad);
    for (auto stmt : *myBody) {
        stmt -> to3AC(proc);
    }
    NopQuad * nop = new NopQuad();
    nop -> addLabel(skip);
    proc -> addQuad(nop);
}

void IfElseStmtNode::to3AC(Procedure * proc){
    Opd* cond = myCond -> flatten(proc);
    Label * skip = proc -> makeLabel();
    Label * end = proc -> makeLabel();
    IfzQuad * ifzQuad = new IfzQuad(cond, skip);
    proc -> addQuad(ifzQuad);

    for (auto stmt : *myBodyTrue) {
        stmt -> to3AC(proc);
    }

    GotoQuad * gotoQuad = new GotoQuad(end);
    proc -> addQuad(gotoQuad);

    NopQuad * nop = new NopQuad();
    nop -> addLabel(skip);
    proc -> addQuad(nop);

    for (auto stmt : *myBodyFalse) {
        stmt -> to3AC(proc);
    }

    NopQuad * nop2 = new NopQuad();
    nop2 -> addLabel(end);
    proc -> addQuad(nop2);
}

void WhileStmtNode::to3AC(Procedure * proc){
    Label * start = proc -> makeLabel();
    Label * end = proc -> makeLabel();
    NopQuad * nop1 = new NopQuad();

    nop1 -> addLabel(start);
    proc -> addQuad(nop1);

    Opd* cond = myCond -> flatten(proc);
    IfzQuad * ifzQuad = new IfzQuad(cond, end);
    proc -> addQuad(ifzQuad);

    for (auto stmt : *myBody) {
        stmt -> to3AC(proc);
    }

    GotoQuad * gotoQuad = new GotoQuad(start);
    NopQuad * nop2 = new NopQuad();
    proc -> addQuad(gotoQuad);
    nop2 -> addLabel(end);
    proc -> addQuad(nop2);
}

void ForStmtNode::to3AC(Procedure * proc){ //havent tested this yet
    myInit->to3AC(proc);
	auto startLb1 = proc->getProg()->makeLabel();
	auto endLb1 = proc->getProg()->makeLabel();

	Quad * nop1Quad = new NopQuad();
	nop1Quad->addLabel(startLb1);
	proc->addQuad(nop1Quad);

	Quad * condQuad = new IfzQuad(myCond->flatten(proc), endLb1);
	proc->addQuad(condQuad);

	for(auto stmt : * myBody)
	{
        stmt-> to3AC(proc);
	}

	myItr->to3AC(proc);

	Quad * gotoQuad = new GotoQuad(startLb1);
	proc->addQuad(condQuad);

	Quad * nop2Quad = new NopQuad();
	nop2Quad -> addLabel(endLb1);
	proc -> addQuad(nop2Quad);
}

void CallStmtNode::to3AC(Procedure * proc){
    Opd * exp = myCallExp->flatten(proc);
    if(exp != nullptr){
        Quad * last = proc->popQuad();
    }
}

void ReturnStmtNode::to3AC(Procedure * proc){
    if (myExp != nullptr) {
        Opd * src = myExp -> flatten(proc);
        SetRetQuad * srQuad = new SetRetQuad(src);
        proc -> addQuad(srQuad);
    }
    GotoQuad * gotoQuad = new GotoQuad(proc -> getLeaveLabel());
    proc -> addQuad(gotoQuad);
}

void VarDeclNode::to3AC(Procedure * proc){
    SemSymbol * sym = ID()->getSymbol();
    assert(sym != nullptr);
    proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog){
    SemSymbol * sym = ID()->getSymbol();
    assert(sym != nullptr);
    //prog->gatherGlobal(sym);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IDNode::flatten(Procedure * proc){
    SemSymbol * sym = mySymbol;
    return proc->getSymOpd(sym);
}

}

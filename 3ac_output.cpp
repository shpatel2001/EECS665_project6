#include "ast.hpp"

namespace cshanty{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta) {
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals) {
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog) {
	Procedure * proc = prog->makeProc(ID()->getName());
	for(auto Formal : *myFormals)
	{
		Formal->to3AC(proc);
	}
	for(auto stmt : *myBody)
	{
		stmt->to3AC(proc);
	}
}

void FnDeclNode::to3AC(Procedure * proc) {
	//This never needs to be implemented,
	// the function only exists because of 
	// inheritance needs (A function declaration
	// never occurs within another function)
	throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog) {
	//This never needs to be implemented,
	// the function only exists because of 
	// inheritance needs (A formal never 
	// occurs at global scope)
	throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc) {
	proc->gatherFormal(ID()->getSymbol());
	auto formalList = proc->getFormals();
	GetArgQuad * getQuad = new GetArgQuad(formalList.size(), formalList.back());
	proc->addQuad(getQuad);
}

void RecordTypeDeclNode::to3AC(IRProgram * prog) {

}

void RecordTypeDeclNode::to3AC(Procedure * proc) {

}

Opd * IntLitNode::flatten(Procedure * proc) {
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(myNum), 8);
}

Opd * StrLitNode::flatten(Procedure * proc) {
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * TrueNode::flatten(Procedure * proc) {
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(1), 8);
}

Opd * FalseNode::flatten(Procedure * proc) {
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(0), 8);
}

Opd * AssignExpNode::flatten(Procedure * proc) {
	Opd * srcOpd = mySrc->flatten(proc);
	Opd * dstOpd = myDst->flatten(proc);
	AssignQuad * assign = new AssignQuad(dstOpd, srcOpd);
	proc->addQuad(assign);
	return dstOpd;
}

Opd * LValNode::flatten(Procedure * proc) {
	TODO("what");
}

Opd * CallExpNode::flatten(Procedure * proc) {
	for(auto args : *myArgs)
	{
		Opd * argOpd = args->flatten(proc);
		SetArgQuad * setarg = new SetArgQuad(argOpd->getWidth(), argOpd);
		proc->addQuad(setarg);
	}
	SemSymbol * IdSym = myID->getSymbol();
	CallQuad * call = new CallQuad(IdSym);
	proc->addQuad(call);
	return proc->getSymOpd(IdSym);
}

Opd * NegNode::flatten(Procedure * proc) {
	Opd * src = myExp->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	UnaryOpQuad * quad = new UnaryOpQuad(temp, NEG64, src);
	proc->addQuad(quad);
	return temp;
}

Opd * NotNode::flatten(Procedure * proc) {
	Opd * src = myExp->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	UnaryOpQuad * quad = new UnaryOpQuad(temp, NOT64, src);
	proc->addQuad(quad);
	return temp;
}

Opd * PlusNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, ADD64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * MinusNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, SUB64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * TimesNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, MULT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * DivideNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, DIV64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * AndNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, AND64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * OrNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, OR64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * EqualsNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, EQ64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * NotEqualsNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, NEQ64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * LessNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, LT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * GreaterNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, GT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * LessEqNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, LTE64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * GreaterEqNode::flatten(Procedure * proc) {
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, GTE64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

void AssignStmtNode::to3AC(Procedure * proc) {
	myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc) {
	Opd * opd = myLVal->flatten(proc);
	LitOpd * lopd = new LitOpd("1", 8);
	BinOpQuad * boq = new BinOpQuad(opd, ADD64, opd, lopd);
	proc->addQuad(boq);
}

void PostDecStmtNode::to3AC(Procedure * proc) {
	Opd * opd = myLVal->flatten(proc);
	LitOpd * lopd = new LitOpd("1", 8);
	BinOpQuad * boq = new BinOpQuad(opd, SUB64, opd, lopd);
	proc->addQuad(boq);
}

void ReceiveStmtNode::to3AC(Procedure * proc) {
	Opd * DstOpd = myDst->flatten(proc);
	auto type = proc->getProg()->nodeType(myDst);
	ReceiveQuad * receive = new ReceiveQuad(DstOpd, type);
	proc->addQuad(receive);
}

void ReportStmtNode::to3AC(Procedure * proc) {
	Opd * srcOpd = mySrc->flatten(proc);
	auto type = proc->getProg()->nodeType(mySrc);
	ReportQuad * report = new ReportQuad(srcOpd, type);
	proc->addQuad(report);
}

void IfStmtNode::to3AC(Procedure * proc) {
	Opd * CondOpd = myCond->flatten(proc);
	Label * end_of_if = proc->makeLabel();
	IfzQuad * ifz = new IfzQuad(CondOpd,end_of_if);
	proc->addQuad(ifz);
	for(auto stmt : *myBody)
	{
		stmt->to3AC(proc);
	}
	NopQuad * endofif = new NopQuad();
	endofif->addLabel(end_of_if);
	proc->addQuad(endofif);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	Opd * CondOpd = myCond->flatten(proc);
	
	Label * end_of_true_body = proc->makeLabel();
	Label * end_of_if = proc->makeLabel();
	
	IfzQuad * ifz = new IfzQuad(CondOpd,end_of_true_body);
	proc->addQuad(ifz);
	for(auto stmt : *myBodyTrue)
	{
		stmt->to3AC(proc);
	}
	GotoQuad * goto_end = new GotoQuad(end_of_if);
	proc->addQuad(goto_end);

	NopQuad * endoftrue = new NopQuad();
	endoftrue->addLabel(end_of_true_body);
	proc->addQuad(endoftrue);
	for(auto stmt : *myBodyFalse)
	{
		stmt->to3AC(proc);
	}
	NopQuad * endofif = new NopQuad();
	endofif->addLabel(end_of_if);
	proc->addQuad(endofif);
	//not sure about how to jump to the end of the if
}


void WhileStmtNode::to3AC(Procedure * proc){
	Opd * CondOpd = myCond->flatten(proc);
	
	Label * start_of_while = proc->makeLabel();
	Label * end_of_while = proc->makeLabel();

	NopQuad * startofwhile = new NopQuad();
	startofwhile->addLabel(start_of_while);
	proc->addQuad(startofwhile);

	IfzQuad * ifz = new IfzQuad(CondOpd,end_of_while);
	proc->addQuad(ifz);

	for(auto stmt : *myBody)
	{
		stmt->to3AC(proc);
	}

	GotoQuad * goto_start = new GotoQuad(start_of_while);
	proc->addQuad(goto_start);

	NopQuad * endofwhile = new NopQuad();
	endofwhile->addLabel(end_of_while);
	proc->addQuad(endofwhile);
	//help
}

void CallStmtNode::to3AC(Procedure * proc) {
	myCallExp->flatten(proc);
	auto aux = proc->makeTmp(8);
	auto getRet = new GetRetQuad(aux);
	proc->addQuad(getRet);
}

void ReturnStmtNode::to3AC(Procedure * proc) {
	auto expOpd = myExp->flatten(proc);
	auto setret = new SetRetQuad(expOpd);
	proc->addQuad(setret);
	auto * goto_end = new GotoQuad(proc->getLeaveLabel());
    proc->addQuad(goto_end);
}

void VarDeclNode::to3AC(Procedure * proc) {
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog) {
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	prog->gatherGlobal(sym);
}

Opd * IndexNode::flatten(Procedure * proc) {
	SymOpd * recordOpd = proc->getLocals().at(myBase->getSymbol());
	auto record = myBase->getSymbol()->getDataType()->asRecord();
	int offset = record->getOffset(myIdx->getName());
	AddrOpd * addr = proc->makeAddrOpd(8);
	LitOpd * lit = new LitOpd(to_string(offset), 8);
	IndexQuad * quad = new IndexQuad(addr, recordOpd, lit);
	proc->addQuad(quad);
	return addr;
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent) 
Opd * IDNode::flatten(Procedure * proc) {
	return proc->getSymOpd(mySymbol);
}

}
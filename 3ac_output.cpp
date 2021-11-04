#include "ast.hpp"

namespace cshanty{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
	Procedure * proc = prog->makeProc(this->ID()->getName());
	for(auto Formal : *myFormals)
	{
		TODO(Implement me)
		Formal->to3AC(proc);
	}
	for(auto stmt : *myBody)
	{
		stmt->to3AC(proc);
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
	//might need to change this
	Opd * IDOpd = this->ID()->flatten(proc);
	// GetArgQuad(IDOpd, IDOpd->getWidth());
}

void RecordTypeDeclNode::to3AC(IRProgram * prog){
	TODO(Implement me)
}

void RecordTypeDeclNode::to3AC(Procedure * proc){
	TODO(Implement me)
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
	return new LitOpd(std::to_string(1), 8);
}

Opd * FalseNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	return new LitOpd(std::to_string(0), 8);
}

Opd * AssignExpNode::flatten(Procedure * proc){
	Opd * srcOpd = this->mySrc->flatten(proc);
	Opd * dstOpd = this->myDst->flatten(proc);
	AssignQuad * assign = new AssignQuad(dstOpd, srcOpd);
	proc->addQuad(assign);
	return dstOpd;
}

Opd * LValNode::flatten(Procedure * proc){
	TODO("how did you get here?");
}

Opd * CallExpNode::flatten(Procedure * proc){
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

Opd * NegNode::flatten(Procedure * proc){
	Opd * src = myExp->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	UnaryOpQuad * quad = new UnaryOpQuad(temp, NEG64, src);
	proc->addQuad(quad);
	return temp;
}

Opd * NotNode::flatten(Procedure * proc){
	Opd * src = myExp->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	UnaryOpQuad * quad = new UnaryOpQuad(temp, NOT8, src);
	proc->addQuad(quad);
	return temp;
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, ADD64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * MinusNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, SUB64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * TimesNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, MULT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * DivideNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, DIV64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * AndNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, AND64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * OrNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, OR64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * EqualsNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, EQ64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, NEQ64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * LessNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, LT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * GreaterNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, GT64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * LessEqNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, LTE64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	Opd * lOpd = myExp1->flatten(proc);
	Opd * rOpd = myExp2->flatten(proc);
	AuxOpd * temp = proc->makeTmp(8);
	BinOpQuad * boq = new BinOpQuad(temp, GTE64, lOpd, rOpd);
	proc->addQuad(boq);
	return temp;
}

void AssignStmtNode::to3AC(Procedure * proc){
	this->myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void PostDecStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void ReceiveStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void ReportStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfElseStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void WhileStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void CallStmtNode::to3AC(Procedure * proc){
	this->myCallExp->flatten(proc);
}

void ReturnStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sym = ID()->getSymbol();
	assert(sym != nullptr);
	prog->gatherGlobal(sym);
}

Opd * IndexNode::flatten(Procedure * proc){
	TODO(Implement me)
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent) 
Opd * IDNode::flatten(Procedure * proc) {
	return proc->getSymOpd(this->mySymbol);
}

}
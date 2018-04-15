#ifndef __SYNTACTIC_ANALYSER__
#define __SYNTACTIC_ANALYSER__
#include <string>

using namespace std;
void Syntactic_analyser();
void program();

void Operator_s(bool(*stop)(string));
void Operator();

void variable_definition();
void expression();
void unempty_expression();
void Priority2();
void Priority3();
void Priority4();
void Priority5();
void Priority6();
void Priority7();
void Priority8();
void Priority9();
void Priority10();
void Priority11();
void Priority12();
void Priority13();
void Priority14();
void Priority15();

void functioncall();
void parameters();

void assignable();
void variable();
void name();

void block();

void output_operator();

void special_operator();

void select_operator();


void conditional_operator();
void Else();
void Else1();

void cycle_operator();
void While();
void DoWhile();
void For();
void For_condition(int& Foradd1, int& Foradd0, int& Foradd2);

void function_definition();
void arguments();
void gotos_children();

static ifstream in("Files/Lexout.txt");
struct lexem {
	int id;
	string name;
};
static lexem lex;
int line = 1;
int local = 0;

istream& operator >> (istream& in, lexem & lex) {
	in >> lex.id;
	while (lex.id == 7 && !in.eof()) {
		line++;
		while (in.get() != '\n' && !in.eof());
		in >> lex.id;
	}
	char c;
	in.get();
	lex.name = "";

	c = in.get();
	if (c == '"') {
		lex.name += c; c = in.get();
		while (c != '"') {
			lex.name += c;
			c = in.get();
		}
		lex.name += c;
		in.get();
	}
	else {
		while (c != '\n' && !in.eof()) {
			lex.name += c;
			c = in.get();
		}
	}

	return in;
}
bool mark = false;
void ERROR(string str) {
	throw(str);
}

//  TID
enum Type {
	integer,
	real,
	boolean,
	str
};
void Case(Type type);
struct Variable {
	Type type;
	string name;
	string value;
};

vector<Variable> TID;// read from the end
void deltill(int till) {
	for (int i = TID.size() - 1; i >= till; i--)
		TID.pop_back();
}

void addId(Type type, string name, string value) {
	bool exists = false;
	for (int i = TID.size() - 1; i >= local; i--) {
		if (name == TID[i].name) {
			exists = true;
			break;
		}
	}
	if (exists) {
		throw(1);
		return;
	}
	Variable var;// = Variable();  //  ??
	var.type = type;
	var.name = name;
	var.value = value;
	TID.push_back(var);
}

Type checkId(string name/*, Type type*/) {
	for (int i = TID.size() - 1; i >= 0; i--) {
		if (name == TID[i].name) {
			return TID[i].type;
		}
	}
	throw(2);
}


//  stack
struct stackContent {
	Type type;
	string opr = "";
};

stack<stackContent> stacky;

void clearst() {
	while (!stacky.empty()) {
		stacky.pop();
	}
}
void pushType(Type type) {
	stackContent stc = stackContent();
	stc.type = type;
	stacky.push(stc);
}

void pushOp(string operation) {
	stackContent stc = stackContent();
	stc.opr = operation;
	stacky.push(stc);
}

void checkOp() {
	stackContent stc = stacky.top();
	stacky.pop();
	Type type1 = stc.type;

	stc = stacky.top();
	stacky.pop();
	string opr = stc.opr;

	stc = stacky.top();
	stacky.pop();
	Type type2 = stc.type;

	if (opr == "=") {
		if (type1 == type2) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(3);
	}
	if (opr == "+") {
		if (type1 != type2) {
			if (type1 == integer && type2 == real ||
				type1 == real && type2 == integer) {
				stc.type = real;
				stacky.push(stc);
				return;
			}
			throw(3);
		}
		if (type1 == boolean) {
			throw(4);
		}
		stc.type = type1;
		stacky.push(stc);
		return;
	}
	if (opr == "-" || opr == "*") {
		if (type1 != type2) {
			if (type1 == integer && type2 == real ||
				type1 == real && type2 == integer) {
				stc.type = real;
				stacky.push(stc);
				return;
			}
			throw(3);
		}
		if (type1 == str || type1 == boolean) {
			throw(4);
		}
		stc.type = type1;
		stacky.push(stc);
		return;
	}
	if (opr == "&&" || opr == "||") {
		if (type1 == type2 && type1 == boolean) {
			stc.type = boolean;
			stacky.push(stc);
			return;
		}
		throw(3);
	}
	if (opr == "/") {
		if (type1 == type2 && (type1 == integer || type1 == real)) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		if (type1 != type2) {
			if ((type1 == integer || type1 == real) && (type2 == integer || type2 == real)) {
				stc.type = real;
				stacky.push(stc);
				return;
			}
		}
		throw(4);
	}
	if (opr == "|" || opr == "&" || opr == "^")
	{
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "%") {
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "==" || opr == "!=" || opr == "===" || opr == "!==" || opr == "<=>") {
		if (type1 == type2) {
			stc.type = boolean;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "<" || opr == ">" || opr == "<=" || opr == ">=") {
		if (type1 == type2 && (type1 == integer || type1 == real)) {
			stc.type = boolean;
			stacky.push(stc);
			return;
		}
	}
	if (opr == "<<" || opr == ">>") {
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
	}
	if (opr == "**") {
		if ((type1 == real || type1 == real) && type2 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "/=") {
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		if ((type1 == integer || type1 == real) && (type2 == integer && type2 == real)) {
			stc.type = real;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "%=") {
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == ".=") {
		if (type1 == type2 && type1 == str) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
	if (opr == "&=" || opr == "<<=" || opr == ">>=" || opr == "^=" || opr == "|=") {
		if (type1 == type2 && type1 == integer) {
			stc.type = type1;
			stacky.push(stc);
			return;
		}
		throw(4);
	}
}

void checkNot() {
	stackContent stc = stacky.top();
	stacky.pop();
	string opr;
	Type type;
	if (stc.opr == "++" || stc.opr == "--") {
		stc = stacky.top();
		stacky.pop();
		if (stc.type == integer) {
			stacky.push(stc);
			return;
		}
	}
	else {
		type = stc.type;
		stc = stacky.top();
		stacky.pop();
		opr = stc.opr;
	}

	if (opr == "++" || opr == "--") {
		if (type == integer) {
			stc.type = integer;
			stacky.push(stc);
			return;
		}
	}
	if (opr == "!") {
		if (type == boolean) {
			stc.type = type;
			stacky.push(stc);
			return;
		}

	}
	throw(3);
}
string sign_1[13] = { "=","+=","-=","*=","**=","/=",".=","%=","&=","|=","^=","<<=",">>=" };
bool Check_sign1(string str) {
	bool flag = false;
	for (int i = 0; i < 13; i++)
		if (str == sign_1[i])
			flag = true;
	return flag;
}
struct Pol {
	string name = "";
	int add = 0;
};
vector<Pol> Police;
Pol a;



// stops for Operator_s() function
bool stop1(string lex) {
	if (lex == ">") return true;
	else return false;
}
bool stop2(string lex) {
	if (lex == "}") return true;
	else return false;
}
bool stop3(string lex) {
	if (lex == "case" || lex == "default" || lex == "}" || lex == "endswitch") return true;
	else return false;
}
bool stop4(string lex) {
	if (lex == "else") return true;
	else return false;
}
bool stop5(string lex) {
	if (lex == "endif") return true;
	else return false;
}
bool stop6(string lex) {
	if (lex == "endwhile") return true;
	else return false;
}
bool stop7(string lex) {
	if (lex == "endfor") return true;
	else return false;
}

void Check(string str) {
	if (lex.name == str) { in >> lex; }
	else
		ERROR(str);

}
struct Marky {
	string name="";
	int add = 0, gotoadd = 0;
	Marky* next = nullptr;
};
	Marky* M =nullptr, *End = nullptr;

	struct FUN {
		string name = "";
		int add = 0;
		FUN* next=nullptr;
	};
	FUN* F = nullptr, *endF = nullptr;
#endif

#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
using namespace std;
/*posfix opertaions checknot?????
local variabl??
#yet without functions in polise
#without in/output
# a+=b=c doesnt work! mistake in checkop
#runpolice TID read from end
# !F !G
*/
#include"Syntactic_analyser.h"
ofstream outPol("Files/Police.txt");
void program() {
	a.name = "{";
	Police.push_back(a);
	in >> lex;
	Check("<");
	Check("?");
	Check("php");
	Operator_s(stop1);
	if (lex.name != ">")
		cout << "WOOOPS! '>' expected.\n";
	a.name = "}";
	Police.push_back(a);
}

void Operator_s(bool(*stop)(string)) {
	while (!stop(lex.name) && !in.eof()) {
		Operator();
	}
}
void Operator() {
	/*** expression | spec.opertr | func_defin ***/
	if (lex.name == "switch" || lex.name == "if" || lex.name == "while" ||
		lex.name == "do" || lex.name == "for" || lex.name == "return" ||
		lex.name == "break" || lex.name == "continue" || lex.name == "goto" || lex.name == "echo") {
		special_operator();
	}
	else
		if (lex.name == "function") {
			in >> lex;
			function_definition();
		}
		else
			if (lex.name == "int" || lex.name == "double" || lex.name == "bool" ||
				lex.name == "string" || lex.name == "char") {
				variable_definition();
				clearst();
			}
			else {
				expression();
				clearst();
				if (!mark) { 
					Check(";"); 
					a.name = ";";
					Police.push_back(a);
				}
				else
					mark = false;
			}
}
void variable_definition() {
	Type type;
	if (lex.name == "int") {
		type = integer;
		pushType(type);
	}
	else if (lex.name == "double") {
		type = real;
		pushType(type);
	}
	else if (lex.name == "bool") {
		type = boolean;
		pushType(type);
	}
	else if (lex.name == "string") {
		type = str;
		pushType(type);
	}
	do {
		in >> lex;
		Check("$");
		if (lex.id != 2) ERROR("name");
		addId(type, lex.name, "");
		a.name = "$" + lex.name;
			Police.push_back(a);
		
		in >> lex;
		if (lex.name == "=") {
			pushOp("=");
			string oper = "=";
			in >> lex;
			unempty_expression();
			a.name = oper;
				Police.push_back(a);
			
			clearst();
		}
		a.name = lex.name;
		Police.push_back(a);

	} while (lex.name == ",");
	if (!stacky.empty())
		stacky.pop();
	Check(";");
}
void expression() {
	if (lex.name == "$" || lex.name == "(" || lex.id == 3 || lex.id == 2)
		unempty_expression();

}
void unempty_expression() {
	vector<string> signs;
	Priority2();
	while (Check_sign1(lex.name)) {
		pushOp(lex.name);
		string oper = lex.name;
		signs.push_back(lex.name);
		in >> lex;
		Priority2();
		checkOp();
	}
	for (int i = signs.size() - 1; i >= 0; i--) {
		a.name = signs[i];
		Police.push_back(a);
	}
}
void Priority2() {
	Priority3();
	while (lex.name == "||") {
		pushOp(lex.name);
		in >> lex;
		Priority3();
		checkOp();
		a.name = "||";
		Police.push_back(a);
	}
}
void Priority3() {
	Priority4();
	while (lex.name == "&&") {
		pushOp(lex.name);
		in >> lex;
		Priority4();
		checkOp();
		a.name = "&&";
		Police.push_back(a);
	}
}
void Priority4() {
	Priority5();
	while (lex.name == "|") {
		pushOp(lex.name);
		in >> lex;
		Priority5();
		checkOp(); a.name = "|";
		Police.push_back(a);
	}
}
void Priority5() {
	Priority6();
	while (lex.name == "^") {
		pushOp(lex.name);
		in >> lex;
		Priority6();
		checkOp();
		a.name = "^";
		Police.push_back(a);
	}
}
void Priority6() {
	Priority7();
	while (lex.name == "&") {
		pushOp(lex.name);
		in >> lex;
		Priority7();
		checkOp();
		a.name = "&";
		Police.push_back(a);
	}
}
void Priority7() {
	Priority8();
	while (lex.name == "==" ||
		lex.name == "!=" ||
		lex.name == "===" ||
		lex.name == "!==" ||
		lex.name == "<=>") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority8();
		checkOp();
		a.name = oper;
		Police.push_back(a);
	}
}
void Priority8() {
	Priority9();
	while (lex.name == "<" ||
		lex.name == ">" ||
		lex.name == "<=" ||
		lex.name == ">=") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority9();
		checkOp();
		a.name = oper;
		Police.push_back(a);
	}
}
void Priority9() {
	Priority10();
	while (lex.name == "<<" ||
		lex.name == ">>") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority10();
		checkOp();
		a.name = oper;
		Police.push_back(a);
	}
}
void Priority10() {
	Priority11();
	while (lex.name == "+" ||
		lex.name == "-" ||
		lex.name == ".") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority11();
		checkOp();
		a.name = oper;
		Police.push_back(a);
	}
}
void Priority11() {
	Priority12();
	while (lex.name == "*" ||
		lex.name == "/" ||
		lex.name == "%") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority12();
		checkOp();
		a.name = oper;
		Police.push_back(a);
	}
}
void Priority12() {
	if (lex.name == "!") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority13();
		checkNot();
		a.name = oper;
		Police.push_back(a);
	}
	else
		Priority13();
}
void Priority13() {
	if (lex.name == "++" ||
		lex.name == "--") {
		pushOp(lex.name);
		string oper = lex.name;
		in >> lex;
		Priority14();
		checkNot();
		a.name = "pre" + oper;
		Police.push_back(a);
	}
	else {
		Priority14();
		if (lex.name == "++" ||
			lex.name == "--") {
			pushOp(lex.name);
			string oper = lex.name;
			in >> lex;
			checkNot();
			a.name = "post" + oper;
			Police.push_back(a);
		}
	}
}
void Priority14() {
	Priority15();
	while (lex.name == "**") {
		pushOp(lex.name);
		in >> lex;
		Priority15();
		checkOp();
		a.name = "**";
		Police.push_back(a);
	}
}
void Priority15() {
	if (lex.name == "$") {
		assignable();
	}
	else
		if (lex.name == "(") {
			in >> lex;
			unempty_expression();
			Check(")");
		}
		else
			if (lex.id == 3) { //const
				if (lex.name[0] == '"')
					pushType(str);
				else
					if (lex.name == "true" || lex.name == "false") {
						pushType(boolean);
					}
					else {
						for (int i = 0; i < lex.name.size(); i++) {
							if (lex.name[i] == '.') {
								pushType(real);
								in >> lex;
								return;
							}
						}
						pushType(integer);
					}
					a.name = lex.name;
						Police.push_back(a);
				
					in >> lex;
			}
			else
				if (lex.id == 2) { // if name
					a.name = lex.name;
					Police.push_back(a);
					in >> lex;
					if (lex.name == ":") {
						Police[Police.size() - 1].name += ":";
						in >> lex;
						mark = true;
					}
					else
						functioncall();
				}
				else ERROR("an operator"); //??!!
}


void functioncall() {
	Check("(");
	parameters();
	Check(")");
}
void parameters() {
	unempty_expression();
	clearst();
	while (lex.name == ",") {
		in >> lex;
		unempty_expression();
		clearst();
	}
}

void assignable() {
	variable();
}
void variable() {
	Check("$");
	if (lex.id != 2) ERROR("name");
	pushType(checkId(lex.name));
	a.name = "$" + lex.name;
	Police.push_back(a);
	in >> lex;
}


void block() {
	if (lex.name == "{") {
		int local_1 = local;
		local = TID.size();

		a.name = "{";
		Police.push_back(a);
		in >> lex;
		Operator_s(stop2);
		deltill(local);
		local = local_1;

		a.name = "}";
		Police.push_back(a);
		in >> lex;
		
	}
	else
		Operator();
}

void output_operator() {
	in >> lex;
	expression();
	clearst();
	Check(";");
}

void special_operator() {
	if (lex.name == "switch")
		select_operator();
	else if (lex.name == "if")
		conditional_operator();
	else if (lex.name == "while" || lex.name == "do" || lex.name == "for")
		cycle_operator();
	else if (lex.name == "echo") {
		output_operator();
	}
	else gotos_children();
}

void select_operator() {
	in >> lex;
	Check("(");
	unempty_expression();
	Type typesw = stacky.top().type;
	clearst();
	Check(")");
	if (lex.name == "{") {
		in >> lex;
		Case(typesw);
		while (lex.name == "case" || lex.name == "default")
			Case(typesw);
		Check("}");
	}
	else {
		Case(typesw);
		while (lex.name == "case" || lex.name == "default")
			Case(typesw);
		Check("endswitch");
		Check(";");
	}
}
void Case(Type type1) {
	Type type2;
	int local_1 = local;
	local = TID.size();
	if (lex.name == "default") {
		in >> lex;
		Check(":");
		if (lex.name == "case" || lex.name == "default" || lex.name == "}" || lex.name == "endswitch") ERROR("an operator");
		Operator_s(stop3);
		deltill(local);
		local = local_1;
	}
	else {
		Check("case");
		if (lex.id != 3) ERROR("const");
		if (lex.name[0] == '"') {
			type2 = str;
		}
		else if (lex.name == "true" || lex.name == "false") {
			type2 = boolean;
		}
		else {
			type2 = integer;
			for (int i = 0; i < lex.name.size(); i++) {
				if (lex.name[i] == '.') {
					type2 = real;
				}
			}

		}
		if (type1 == type2 || type1 == real && type2 == integer) {
			in >> lex;
			Check(":");
			if (lex.name == "case" || lex.name == "default" || lex.name == "}" || lex.name == "endswitch") ERROR("an operator");
			Operator_s(stop3);
			deltill(local);
			local = local_1;
		}
		else {
			throw(3);
		}
	}
}
void conditional_operator() {
	int address;
	in >> lex;
	Check("(");
	unempty_expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Check(")");
	address = Police.size();
	a.name = "@";
	Police.push_back(a);
	a.name = "!F";
	Police.push_back(a);
	if (lex.name == "{") {
		block();
		Police[address].add = Police.size()+2;
		address = Police.size();
		a.name = "@";
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
		Else();
		Police[address].add = Police.size();

	}
	else {
		if (lex.name == "else") ERROR(";");
		Operator_s(stop4);
		Police[address].add = Police.size() + 2;
		address = Police.size();
		a.name = "@";
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
		Else1();
		Police[address].add = Police.size();
	}
}
void Else() {
	Check("else");
	int local_1 = local;
	local = TID.size();
	block();
	deltill(local);
	local = local_1;
}
void Else1() {
	in >> lex;
	int local_1 = local;
	local = TID.size();
	if (lex.name == "endif") ERROR("an operator");
	Operator_s(stop5);
	deltill(local);
	local = local_1;
	Check("endif");
	Check(";");
	a.name = ";";
		Police.push_back(a);
}

void cycle_operator() {
	if (lex.name == "while") {
		in >> lex;
		While();
	}
	else if (lex.name == "do") {
		in >> lex;
		DoWhile();
	}
	else {
		in >> lex;
		For();
	}
}
void While() {
	Check("(");
	unempty_expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Check(")");
	int local_1 = local;
	local = TID.size();
	if (lex.name == "{") {
		block();
		deltill(local);
		local = local_1;
	}
	else {
		if (lex.name == "endwhile") ERROR("an operator");
		Operator_s(stop6);
		deltill(local);
		local = local_1;
		Check("endwhile");
		Check(";");
	}
}
void DoWhile() {
	int local_1 = local;
	local = TID.size();
	block();
	deltill(local);
	local = local_1;
	Check("while");
	Check("(");
	unempty_expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Check(")");
}
void For() {
	int local_1 = local;
	local = TID.size();
	Check("(");
	For_condition();
	Check(")");
	if (lex.name == "{") {
		block();
		deltill(local);
		local = local_1;
	}
	else {
		if (lex.name == "endwhile") ERROR("an operator");
		Operator_s(stop7);
		deltill(local);
		local = local_1;
		Check("endfor");
		Check(";");
	}
}
void For_condition() {
	expression();
	clearst();
	Check(";");
	expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Check(";");
	expression();
	clearst();
}

void function_definition() {
	if (lex.id == 2)
		in >> lex;
	Check("(");
	int local_1 = local;
	local = TID.size();
	arguments();
	Check(")");
	block();
	deltill(local);
	local = local_1;
}
void arguments() {
	Type type;
	if (lex.name == "int" || lex.name == "double" || lex.name == "bool" ||
		lex.name == "string" || lex.name == "char") {
		if (lex.name == "int") {
			type = integer;
		}
		else if (lex.name == "double") {
			type = real;
		}
		else if (lex.name == "bool") {
			type = boolean;
		}
		else if (lex.name == "string") {
			type = str;			
		}
		in >> lex;
		
		Check("$");
		if (lex.id != 2) ERROR("name");
		addId(type,lex.name,"");
		in >> lex;
		
		while (lex.name == ",") {
			in >> lex;
			if (lex.name == "int" || lex.name == "double" || lex.name == "bool" ||
				lex.name == "string" || lex.name == "char") {
				if (lex.name == "int") {
					type = integer;
				}
				else if (lex.name == "double") {
					type = real;
				}
				else if (lex.name == "bool") {
					type = boolean;
				}
				else if (lex.name == "string") {
					type = str;
				}
				in >> lex;
				Check("$");
				if (lex.id != 2) ERROR("name");
				addId(type, lex.name, "");
				in >> lex;
			}
			else ERROR("type");
		}
	}
	else ERROR("type");
}


void gotos_children() {
	if (lex.name == "return") {
		in >> lex;
		expression();
	}
	else if (lex.name == "break") in >> lex;
	else if (lex.name == "continue") in >> lex;
	else {
		in >> lex;   // name
		if (lex.id != 2) ERROR("name");
		in >> lex;
	}
}

void Syntactic_analyser() {
	try { program(); }
	catch (string str) {
		cout << "WOOOPS! Here is '" << lex.name << "' in the " << line
			<< " line. Expected '" << str << "' !\n";
		in.close();
		return;
	}
	catch (int i) {
		if (i == 1) cout << "Duplicate declaration of a variable ";
		if (i == 2) cout << "Variable is not declared ";
		if (i == 3) cout << "Type missmatch ";
		if (i == 4) cout << "Type and operation missmatch ";
		cout << "in the " << line << " line\n";
		return;
	}
	cout << "BUILD SUCCEEDED!\n";
	
	for (int i = 0; i < Police.size(); i++) {
		outPol << Police[i].name << " ";
		if (Police[i].name == ";")
			outPol << "\n";
		if (Police[i].name == "@")
			outPol << Police[i].add<<" ";
	}

}
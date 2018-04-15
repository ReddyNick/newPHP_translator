#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
using namespace std;
/*posfix opertaions checknot?????
local variabl??
#yet without functions in polise
#without in/output output echo operation
# a+=b=c doesnt work! mistake in checkop(there is no "+=" )
#  "The number is " . $i . "<br>" (there is no "." )
#runpolice TID read from end
# !F !E
#without CASE, ?? endfor ...,endwhile and so on
# without $c=f($a,$b);
# dont work for(int $i=1; with "int"
*/




/*COOOL
	goto yeahhh
*/
#include"Syntactic_analyser.h"
void f() {
	cout << 6;
}
struct Pol {
	string name = "";
	int add = 0;
};
vector<Pol> Police;
Pol a;
int hh = 1234;
ofstream outPol("C:\\Users\\tretiakova\\Source\\Repos\\newPHP_translator\\PHP_translator\\PHP_translator\\Files\\Police.txt");
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
			a.name = "init";
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

	if (lex.name == "$" || lex.name == "(" || lex.id == 3 || lex.id == 2 || lex.name == "--" || lex.name == "++") {
		unempty_expression();
	}

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
					//Police.push_back(a);
					in >> lex;
					if (lex.name == ":") {
						if (M == nullptr) {
							M = new Marky;
							M->name = a.name;
							M->add = Police.size();
							End = M;
						}
						else {
							
							Marky *ptr = M;
							while (ptr != nullptr && ptr->name != a.name)
								ptr = ptr->next;
							
							if (ptr == nullptr) {
								ptr = new Marky;
								ptr->name = a.name;
								ptr->add = Police.size();
								End->next = ptr;
								End = ptr;
							}
							else {
								//cout << ptr->gotoadd<<"ty";
								Police[ptr->gotoadd].add = Police.size();
							}
							
							//ptr = nullptr;
							//delete ptr;
						}
							
						in >> lex;
						mark = true;
					}
					else {
						functioncall();
						
					}
				}
				else ERROR("an operator"); //??!!
}


void functioncall() {
	
	FUN*ptr = F;
	while (ptr->name != a.name)
		ptr = ptr->next;
	
	a.name = "@";
	a.add = ptr->add;
	Police.push_back(a);
	a.name = "Fcall";
	Police.push_back(a);
	Check("(");
	parameters();
	Check(")");

	a.name = "@";
	a.add = Police.size() + 1;
	Police.push_back(a);

}
void parameters() {
	unempty_expression();
	clearst();
	while (lex.name == ",") {
		a.name = ",";
		Police.push_back(a);
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
	a.name = "echo";
	Police.push_back(a);
	clearst();
	Check(";");
	a.name = ";";
	Police.push_back(a);
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
int cycledepth = 0;
vector<int> cyclebreak;
vector<int> cyclecontinue;
void cycle_operator() {
	int cycontinue = Police.size();
	cycledepth++;
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
	cycledepth--;
	if (!cyclebreak.empty()) {
		Police[cyclebreak[cyclebreak.size() - 1]].add = Police.size();
		cyclebreak.pop_back();
	}
	if (!cyclecontinue.empty()) {
		Police[cyclecontinue[cyclecontinue.size() - 1]].add = cycontinue;
		cyclecontinue.pop_back();
	}
}
void While() {
	Check("(");
	int whileadd = Police.size();
	unempty_expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Check(")");
	int local_1 = local;
	local = TID.size();

	int address = Police.size();
	a.name = "@";
	Police.push_back(a);
	a.name = "!F";
	Police.push_back(a);
	if (lex.name == "{") {
		block();
		deltill(local);
		local = local_1;
		Police[address].add = Police.size() + 2;
		a.name = "@";
		a.add = whileadd;
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
	}
	else {
		if (lex.name == "endwhile") ERROR("an operator");
		Operator_s(stop6);
		deltill(local);
		local = local_1;
		Check("endwhile");
		Police[address].add = Police.size() + 2;
		a.name = "@";
		a.add = whileadd;
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
		Check(";");
		a.name = ";";
		Police.push_back(a);
	}
}
void DoWhile() {
	int local_1 = local;
	local = TID.size();
	int address = Police.size();
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
	a.name = "@";
	a.add = address;
	Police.push_back(a);
	a.name = "!F";
	Police.push_back(a);

}

void For() {
	int Foradd1, Foradd0, Foradd2;
	int local_1 = local;
	local = TID.size();
	Check("(");
	For_condition(Foradd1,Foradd0,Foradd2);
	Check(")");
	Police[Foradd0].add = Police.size();
	if (lex.name == "{") {
		block();
		deltill(local);
		local = local_1;
		a.name = "@";
		a.add = Foradd2;
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
	}
	else {
		if (lex.name == "endwhile") ERROR("an operator");
		Operator_s(stop7);
		deltill(local);
		local = local_1;
		a.name = "@";
		a.add = Foradd2;
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
		Check("endfor");
		Check(";");
		a.name = ";";
		Police.push_back(a);
	}
	Police[Foradd1].add = Police.size();
}

void For_condition(int& Foradd1, int& Foradd0, int& Foradd2) {
	expression();
	clearst();
	Check(";");
	a.name = ";";
	Police.push_back(a);
	int ForaddE=Police.size();
	expression();
	if (!stacky.empty())
		if (stacky.size() != 1 || stacky.top().type != boolean) throw(3);
	while (!stacky.empty())
		stacky.pop();
	Foradd1 = Police.size();
	a.name = "@";
	Police.push_back(a);
	a.name = "!F";
	Police.push_back(a);
	Foradd0 = Police.size();
	a.name = "@";
	Police.push_back(a);
	a.name = "!E";
	Police.push_back(a);

	Check(";");
	a.name = ";";
	Police.push_back(a);
	Foradd2 = Police.size();
	expression();
	a.name = "@";
	a.add = ForaddE;
	Police.push_back(a);
	a.name = "!E";
	Police.push_back(a);
	clearst();
}

void function_definition() {
	FUN *ptr;
	if (F == nullptr) {
		F = new FUN;
		F->name = lex.name;
		F->add = Police.size() + 2;
		endF = F;
	}
	else {
		ptr = new FUN;
		ptr->name = lex.name;
		ptr->add = Police.size() + 2;
		endF->next = ptr;
		endF = ptr;
	}
	int end=Police.size();
	a.name = "@";
	Police.push_back(a);
	a.name = "!E";
	Police.push_back(a);
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
	a.name = "endf";
	Police.push_back(a);
	Police[end].add = Police.size();

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
		a.name = "$" + lex.name;
		Police.push_back(a);
		a.name = "initF";
		Police.push_back(a);
		in >> lex;
		
		while (lex.name == ",") {
			a.name = ",";
			Police.push_back(a);
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
				a.name = "$" + lex.name;
				Police.push_back(a);
				a.name = "initF";
				Police.push_back(a);
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
		else 
			if (lex.name == "break") { in >> lex; 
		if (cycledepth > 0) {
			cyclebreak.push_back(Police.size());
			a.name = "@";
			Police.push_back(a);
			a.name = "!E";
			Police.push_back(a);
		}
	}
	else 
		if (lex.name == "continue") { in >> lex; 
	if (cycledepth > 0) {
		cyclecontinue.push_back(Police.size());
		a.name = "@";
		Police.push_back(a);
		a.name = "!E";
		Police.push_back(a);
	}
	}
	else {
		
		in >> lex;   // name
		if (lex.id != 2) ERROR("name");
		
		Marky*ptr = M;
		
		while (ptr != nullptr && ptr->name != lex.name ) {
			ptr = ptr->next;
		}

		if (ptr!=nullptr) {
			a.name = "@";
			a.add = ptr->add;
			Police.push_back(a);
			a.name = "!E";
			Police.push_back(a);
		}
		else {
			if (M == nullptr) {
				
				ptr = new Marky;
				ptr->name = lex.name;
				ptr->gotoadd = Police.size();
				a.name = "@";
				//a.add = ptr->add;
				Police.push_back(a);
				a.name = "!E";
				Police.push_back(a);
				M = ptr;
				End = M;
			}
			else {
				ptr = new Marky;
				ptr->name = lex.name;
				ptr->gotoadd = Police.size();
				a.name = "@";
				//a.add = ptr->add;
				Police.push_back(a);
				a.name = "!E";
				Police.push_back(a);
				End->next = ptr;
				End = ptr;
			}
		}
		//delete ptr;
		in >> lex;
		Check(";");
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
		/*if (Police[i].name == "@")
			outPol << Police[i].add<<" ";*/
	}
	
	while (M != nullptr) {
		Marky *ptr;
		ptr = M;
		M = M->next;
		delete ptr;
	}

	while (F != nullptr) {
		FUN *ptrf;
		ptrf = F;
		F = F->next;
		
		delete ptrf;
	}

	//cout << "name: " << F->name << "\n addre: " << F->add;
	
}

vector<Variable> ptid;
vector<int> depth;

int find_variable(string name) {
	for (int i = ptid.size() - 1; i >= 0; i--) {
		if (ptid[i].name == name) {
			return i;
		}
	}
	return -1;
}

stack<Variable> expr;
void clean_stack_expr() {
	while (!expr.empty()) {
		expr.pop();
	}
}

int convert_to_int(string number) {
	int ans = 0;
	bool negative = false;
	if (number[0] == '-') {
		negative = true;
	}
	for (int i = 0; i < number.length(); i++) {
		if (negative) {
			continue;
		}
		ans *= 10;
		ans += (number[i] - '0');
	}
	if (negative) {
		ans *= -1;
	}
	return ans;
}

vector<string> func_values;
int after_func_address;
int func_iterator = 0;

void Run_police() {
	for (int i = 0; i < Police.size(); i++) {
		if (Police[i].name == "Fcall") {
			int func_address = Police[i - 1].add;
			depth.push_back(0);
			i++;
			while (i < Police.size() && Police[i].name != "@") {
				if (Police[i].name != ",") {
					if (Police[i].name[0] <= '9' && Police[i].name[0] >= '0') {
						func_values.push_back(Police[i].name);
					}
					else {
						func_values.push_back(ptid[find_variable(Police[i].name)].value);
					}
				}
				i++;
			}
			for (int j = 0; j < func_values.size(); j++) {
				outPol << func_values[j] << " ";
			}
			after_func_address = Police[i].add;
			i = func_address - 1;
			continue;
		}
		if (Police[i].name == "initF") {
			
			depth[depth.size() - 1]++;
			Variable vrb = expr.top();
			vrb.value = func_values[func_iterator];
			ptid.push_back(vrb);
			func_iterator++;
			continue;
		}
		if (Police[i].name == "endf") {
			func_iterator = 0;
			while (depth[depth.size() - 1] > 0) {
				ptid.pop_back();
				depth[depth.size() - 1]--;
			}
			depth.pop_back();
			clean_stack_expr();
			i = after_func_address - 1;
			/*for (int j = 0; j < func_values.size(); j++) {
				func_values.pop_back();
			}*/
			continue;
		}
		if (Police[i].name == "{") {
			depth.push_back(0);
			continue;
		}
		if (Police[i].name == "}") {
			outPol << '\n';
			for (int j = 0; j < ptid.size(); j++) {
				outPol << ptid[j].name << "=" << ptid[j].value << "; ";
			}
			while (depth[depth.size() - 1] > 0) {
				ptid.pop_back();
				depth[depth.size() - 1]--;
			}
			depth.pop_back();
			clean_stack_expr();
			continue;
		}
		if (Police[i].name == "<") {
			Variable vrb2 = expr.top();
			expr.pop();
			Variable vrb1 = expr.top();
			expr.pop();
			if (convert_to_int(vrb1.value) < convert_to_int(vrb2.value)) {
				vrb1.value = "true";
			}
			else {
				vrb1.value = "false";
			}
			expr.push(vrb1);
		}
		if (Police[i].name == ">") {
			Variable vrb2 = expr.top();
			expr.pop();
			Variable vrb1 = expr.top();
			expr.pop();
			if (convert_to_int(vrb1.value) > convert_to_int(vrb2.value)) {
				vrb1.value = "true";
			}
			else {
				vrb1.value = "false";
			}
			expr.push(vrb1);
		}
		if (Police[i].name == "<=") {
			Variable vrb2 = expr.top();
			expr.pop();
			Variable vrb1 = expr.top();
			expr.pop();
			string value1 = ptid[find_variable(vrb1.name)].value;
			if (convert_to_int(vrb1.value) <= convert_to_int(vrb2.value)) {
				vrb1.value = "true";
			}
			else {
				vrb1.value = "false";
			}
			expr.push(vrb1);
			continue;
		}
		if (Police[i].name == ">=") {
			Variable vrb2 = expr.top();
			expr.pop();
			Variable vrb1 = expr.top();
			expr.pop();
			if (convert_to_int(vrb1.value) >= convert_to_int(vrb2.value)) {
				vrb1.value = "true";
			}
			else {
				vrb1.value = "false";
			}
			expr.push(vrb1);
		}
		if (Police[i].name == "==") {
			Variable vrb2 = expr.top();
			expr.pop();
			Variable vrb1 = expr.top();
			expr.pop();
			if (vrb1.value == vrb2.value) {
				vrb1.value = "true";
			}
			else {
				vrb1.value = "false";
			}
			expr.push(vrb1);
		}
		if (Police[i].name[0] == '$') {
			Variable vrb;
			vrb.name = Police[i].name;
			if (find_variable(vrb.name) != -1) {
				vrb.value = ptid[find_variable(vrb.name)].value;
			}
			expr.push(vrb);
			continue;
		}
		if (Police[i].name == "init") {
			depth[depth.size() - 1]++;
			Variable vrb = expr.top();
			ptid.push_back(vrb);
			continue;
		}
		if (Police[i].name == "@") {
			if (Police[i + 1].name == "!E") {
				i = Police[i].add - 1;  //  ??
				clean_stack_expr();
				continue;  //  ??
			}
			if (Police[i + 1].name == "!F") {
				if (expr.top().value == "false") {
					i = Police[i].add - 1;  //  ??
					clean_stack_expr();
					continue;
					expr.pop();
					clean_stack_expr();
				}
				continue;
			}
		}
		if (Police[i].name == ",") {
			if (!expr.empty())
				expr.pop();
			continue;
		}
		if (Police[i].name == ";") {
			clean_stack_expr();
			continue;
		}
		if (Police[i].name == "+") {
			Variable vrb;
			vrb = expr.top();
			string value2 = ptid[find_variable(vrb.name)].value;
			expr.pop();
			vrb = expr.top();
			vrb = ptid[find_variable(vrb.name)];
			expr.pop();
			vrb.value = to_string(convert_to_int(vrb.value) 
					+ convert_to_int(value2));
			expr.push(vrb);
			continue;
		}
		if (Police[i].name == "post++") {
			Variable vrb = expr.top();
			int target = find_variable(vrb.name);
			ptid[target].value = to_string(convert_to_int(ptid[target].value)
				+ 1);
		}
		if (Police[i].name == "pre++") {
			Variable vrb = expr.top();
			expr.pop();
			int target = find_variable(vrb.name);
			ptid[target].value = to_string(convert_to_int(ptid[target].value)
				+ 1);
			expr.push(ptid[target]);
		}
		if (Police[i].name == "post--") {
			Variable vrb = expr.top();
			int target = find_variable(vrb.name);
			ptid[target].value = to_string(convert_to_int(ptid[target].value)
				- 1);
		}
		if (Police[i].name == "pre--") {
			Variable vrb = expr.top();
			expr.pop();
			int target = find_variable(vrb.name);
			ptid[target].value = to_string(convert_to_int(ptid[target].value)
				- 1);
			expr.push(ptid[target]);
		}
		if (Police[i].name == "-") {
			Variable vrb;
			vrb = expr.top();
			string value2 = ptid[find_variable(vrb.name)].value;
			expr.pop();
			vrb = expr.top();
			vrb = ptid[find_variable(vrb.name)];
			expr.pop();
			vrb.value = to_string(convert_to_int(vrb.value)
				- convert_to_int(value2));
			expr.push(vrb);
			continue;
		}
		if (Police[i].name == "*") {
			Variable vrb;
			vrb = expr.top();
			string value2 = ptid[find_variable(vrb.name)].value;
			expr.pop();
			vrb = expr.top();
			vrb = ptid[find_variable(vrb.name)];
			expr.pop();
			vrb.value = to_string(convert_to_int(vrb.value)
				* convert_to_int(value2));
			expr.push(vrb);
			continue;
		}
		if (Police[i].name == "/") {
			Variable vrb;
			vrb = expr.top();
			string value2 = ptid[find_variable(vrb.name)].value;
			expr.pop();
			vrb = expr.top();
			vrb = ptid[find_variable(vrb.name)];
			expr.pop();
			vrb.value = to_string(convert_to_int(vrb.value)
				/ convert_to_int(value2));
			expr.push(vrb);
			continue;
		}
		if (Police[i].name == "=") {
			string value2;
			value2 = (expr.top()).value;
			expr.pop();
			Variable vrb;
			vrb = expr.top();
			expr.pop();
			int target_variable = find_variable(vrb.name);
			ptid[target_variable].value = value2;
			ptid[target_variable].name = vrb.name;
			expr.push(ptid[target_variable]);
			continue;
		}
		if (Police[i].name[0] >= '0' && Police[i].name[0] <= '9') {
			Variable vrb;
			vrb.value = Police[i].name;
			expr.push(vrb);
		}
	}
	
}
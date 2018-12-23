// 2017203044 Han Hyung Seok

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

char operationAt[3][3] = { { 'a','b','c' },{ 'b','c','a' },{ 'c','a','b' } };
char operationSharp[3][3] = { { 'a','c','b' },{ 'b','a','c' },{ 'c','b','a' } };
char operationAnd[3][3] = { { 'a','a','a' },{ 'a','b','c' },{ 'a','c','b' } };

using namespace std;

// Set operation
void setOperation() {
	ifstream readf("operations.txt");
	char temp;

	// Set @
	readf >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			readf >> temp;
			operationAt[i][j] = temp;
		}
	} 

	// Set #
	readf >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			readf >> temp;
			operationSharp[i][j] = temp;
		}
	}

	// Set &
	readf >> temp;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			readf >> temp;
			operationAnd[i][j] = temp;
		}
	}

	// Print operation arrays' elements
	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << operationAt[i][j] << ' ';
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << operationSharp[i][j] << ' ';
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cout << operationAnd[i][j] << ' ';
		cout << endl;
	}*/
}

// Check the equation has right pair of parenthesis
bool isValidEq(const string src) {
	stack<char> ckStack;
	int length = (int)src.length();
	//int numCount=0;
	char nextChar, prevChar, pair;

	for (int i = 0; i < length; i++) {
		switch (src.at(i)) {
		/*case 'a':
		case 'b':
		case 'c':
			numCount++; break;*/
		case '(':
		case '{':
		case '[':
			ckStack.push(src.at(i)); break;
		case ')':
		case '}':
		case ']':
			if (ckStack.empty()) return false;
			switch (src.at(i)) {
			case ')': pair = '('; break;
			case '}': pair = '{'; break;
			case ']': pair = '['; break;
			}
			//for (; ckStack.top() == '@' || ckStack.top() == '#' || ckStack.top() == '&'; ckStack.pop());
			if (ckStack.top() != pair) return false;
			ckStack.pop(); break;
		case '@':
		case '#':
		case '&':
			if (i < 1 || i + 1 >= length) return false; // Start or end with operator
			prevChar = src.at(i - 1);
			nextChar = src.at(i + 1);
			//if (ckStack.empty()) ckStack.push(src.at(i));
			if (prevChar == '(' || prevChar == '{' || prevChar == '[') return false; // Missing left operand
			else if (prevChar == '@' || prevChar == '#' || prevChar == '&') return false; // Consecutive operator
			else if (nextChar == ')' || nextChar == '}' || nextChar == ']') return false; // Missing right operand
			//else ckStack.push(src.at(i));
			break;
		}
	}

	//if (numCount <= 1) return false;
	if (ckStack.empty()) return true;
	/*else {
		nextChar = ckStack.top();
		while (!ckStack.empty() && (nextChar == '@' || nextChar == '#' || nextChar == '&')) {
			ckStack.pop();
			if (!ckStack.empty()) nextChar = ckStack.top();
		}
		if (ckStack.empty()) return true;
		else return false;
	}*/
	else return false;
}

void newCalc(stack<char>& numStack, char op) {
	int left, right;
	right = (int)(numStack.top() - 'a');
	numStack.pop();
	left = (int)(numStack.top() - 'a');
	numStack.pop();

	switch (op) {
	case '@':
		numStack.push(operationAt[left][right]);
		break;
	case '#':
		numStack.push(operationSharp[left][right]);
		break;
	case '&':
		numStack.push(operationAnd[left][right]);
		break;
	}
}

void makePostfix(const string src, char* buf) {
	stack<char> op;
	char cursor;
	int iter = 0;
	for (int i = 0; i<src.length(); i++) {
		cursor = src.at(i);
		if (cursor >= 'a'&&cursor <= 'c') { buf[iter] = cursor; iter++; }
		else if (cursor == '@' || cursor == '#' || cursor == '&') {
			if (op.empty() || op.top() == '('
				|| op.top() == '{' || op.top() == '[') {
				op.push(cursor);
			}
			else if (cursor == '&') { // &
				if (op.top() == '&') {
					buf[iter] = cursor;
					iter++;
					op.pop();
					op.push(cursor);
				}
				else op.push(cursor);
			}
			else { // #, @
				buf[iter] = op.top();
				iter++;
				op.pop();
				op.push(cursor);
			}
		}
		else if (cursor == '(' || cursor == '{' || cursor == '[') op.push(cursor);
		else if (cursor == ')' || cursor == '}' || cursor == ']') {
			char pair = 0;
			switch (cursor) {
			case ')': pair = '('; break;
			case '}': pair = '{'; break;
			case ']': pair = '['; break;
			}
			for (;;) { // Pop until top() is pair
				if ((char)op.top() == pair) { op.pop(); break; }
				buf[iter] = op.top();
				iter++;
				op.pop();
			}
		}
	}
	if (op.empty()) return;
	for (; op.empty() != true; iter++) {
		buf[iter] = op.top();
		op.pop();
	}
}

void calcPostfix(const char* src, int length) {
	stack<char> num;
	char cursor;
	for (int i = 0; i<length; i++) {
		cursor = src[i];
		switch (cursor) {
		case 'a':
		case 'b':
		case 'c':
			num.push(cursor); break;
		case '@':
		case '#':
		case '&':
			newCalc(num, cursor);
			break;
		}
	}
	cout << num.top() << endl;
}

void Calculate(const string src) {
	if (!isValidEq(src)) {
		cout << "Error!" << endl;
	}
	else {
		char* postfix = (char*)malloc(src.length());
		for (int i = 0; i<src.length(); i++)
			postfix[i] = 0;
		makePostfix(src, postfix);
		/*for (int i = 0; i<src.length() && postfix[i] != 0; i++) cout << postfix[i];
		cout << endl;*/
		calcPostfix(postfix, (int)strlen(postfix));
		free(postfix);
	}
}


int main(int argc, const char * argv[]) {
	vector<string> eq;
	string tmp;
	setOperation();

	// Get equations
	while (1) {
		cin >> tmp;
		if (tmp == "EOI") break;
		eq.push_back(tmp);
	}

	// Calculate
	for (int i = 0; i<eq.size(); i++)
		Calculate(eq[i]);

	return 0;
}


//for (int i = 0; i<length; i++) {
//	if (src.at(i) == '(' || src.at(i) == '{' || src.at(i) == '[') { // If left parenthesis
//		ckStack.push(src.at(i));
//	}
//	else if (src.at(i) == ')' || src.at(i) == '}' || src.at(i) == ']') { // If right parenthesis
//		if (src.at(i) == ')') {
//			if (ckStack.empty()) return false;
//			else if (ckStack.top() != '(') return false;
//		}
//		else if (src.at(i) == '}') {
//			if (ckStack.empty()) return false;
//			else if (ckStack.top() != '{') return false;
//		}
//		else {
//			if (ckStack.empty()) return false;
//			else if (ckStack.top() != '[') return false;
//		}
//		ckStack.pop();
//	}
//}
#include <iostream>
#include <string>
#include <cstdlib>
#include <utility>

#include "node.h"
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"

using namespace std;

Token token;
NFInfo result;
int a0;
int a1;
int depth;

Node* parser(char* file, char* baseFile, int valC){
	//Token token;
	File info;
	string totalInstance = "";
	int line = 1;
	if(token.line == 0){
		token.line = line;
	}
	token = scanner(file, baseFile, totalInstance, token.line);
	totalInstance += token.instance;
	info.file = file;
	info.bFile = baseFile;
	info.ttlInst = totalInstance;
	info.ln = token.line;
	if(token.type == "ERR01" || token.type == "ERR02" || token.type == "NL") {
		cout << "ERROR: Line Number: " << token.line << " Token: " << token.instance << endl;
	}
	// Call Nonterminals in Parser.cpp
	// Store ID for Node with Nonterminal or Terminal
	result = S(info);
	Node* root = result.node;
	return root;
}

// Nonterminals in order
NFInfo S(File info){
	// Production Rule: CD
	Node* node = createNewNode(0);
	//cout << "S" << endl;
	result = C(info);
	node->ch1 = result.node;
	result = D(result.info);
	node->ch2 = result.node;
	result.node = node;
	result.info = info;
	return result;
}
NFInfo A(File info){
	// Production Rule: F AP
	Node* node = createNewNode(1);
	//cout << "A" << endl;
	//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
	result = F(info);
	node->ch1 = result.node;
	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
	info.ttlInst += token.instance;
	result = AP(result.info);
	node->ch2 = result.node;
	result.node = node;
	result.info;
	return result;
}
// AP = A' = A Prime
NFInfo AP(File info){
	// Production Rule: F ?$ | .
	Node* node = createNewNode(10);
	//cout << "AP" << endl;
	a0 = token.instance[0];
        //cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
	//a1 = token.instance[1];
	if(a0 == 46){ // "."
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.node = node;
		result.info = info;
		//result.count += 1;
		return result;
	}else{
		result = F(result.info);
		node->ch1 = result.node;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                info.ttlInst += token.instance;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		int a01 = token.instance[0];
        	int a02 = token.instance[1];
		if(a01 == 63 && a02 == 36){//token.instance == "?$ "){
			node->t2 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			//cout << "Result: " << result.count << endl;
			result.count += 1;
			//cout << "Result: " << result.count << endl;
			result.node = node;
			result.info = info;	
	//		cout << "result" << endl;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <A>" << endl;
}
NFInfo B(File info){
	// Production Rule: .t2A!
	Node* node = createNewNode(2);
	//cout << "B" << endl;
	if(token.instance == ". "){
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		if(token.type == "T2"){
			node->t2 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result = A(info);
			node->ch3 = result.node;
			//cout << "Token: " << token.type << "\t{" << token.instance << "}\t" << token.line << endl;
			int a01 = token.instance[0];
			cout << a01 << " / " << token.instance[0] << endl;
			if(a01 == 33){
				node->t4 = token;
				//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
				token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
				info.ttlInst += token.instance;
				result.node = node;
				result.info = info;
				return result;
			}
		}
	}
	cout << "PARSER ERROR: Unexpected Token{" << token.instance << "} inside nonterminal <B>" << endl;
}
NFInfo C(File info) {
	// Production Rule: t2*
	Node* node = createNewNode(3);
	//cout << "C" << endl;
	if(token.type == "T2"){
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		if(token.type == "T3" && token.instance == "* "){
			node->t2 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.node = node;
			result.info = info;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <C>" << endl;
}
NFInfo D(File info){
	// Production Rule: DH? | empty
	Node* node = createNewNode(4);
	//cout << "D" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	//cout << "Token: " << token.type << "\t{" << token.instance << "}\t" << token.line << endl;
	//cout << a0 << " + " << a1 << endl;
	if(token.type == "NLTk" || token.type == "EOFTk" || token.instance == " "){
		node->t1 = token;
		//cout << "Here" << endl;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		result.node = node;
		result.info = info;
		return result;
	}else if(token.instance == "?" || token.instance == ". " || a0 == 44 || (a0 == 44 && a1 == 59) || (a0 == 42 && a1 == 34) || token.type == "T2"){
		result = H(info);
		node->ch1 = result.node;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		// Catch the token inside of D up to the token that was inside of H
		int count = result.count;
		for(int i = 0; i < count; i++){
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			//cout << i << ": " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		}
		//cout << "What" << endl;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		if(token.instance == "?"){
			node->t2 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			//cout << "Calling D" << endl;
			result = D(result.info);
			node->ch3 = result.node;
			result.node = node;
			result.info = info;
			return result;
		}
	}else{
		cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <D>" << endl;
	}
}
NFInfo E(File info){
	// Production Rule: ,AAH | ,;FH
	Node* node = createNewNode(5);
	//cout << "E" << endl;
	a0 = token.instance[0];
        a1 = token.instance[1];
	if(token.type == "T3"){
		if(a0 == 44 && a1 == 59){
			node->t1 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			result.count += 1;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.count += 1;
			result = F(info);
			node->ch2 = result.node;
			result = H(result.info);
			node->ch3 = result.node;
			result.node = node;
			result.info = info;
			return result;
		}else if(a0 == 44){
                        node->t1 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
                        result.count += 1;
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
                        result.count += 2;
                        result = A(info);
                        node->ch2 = result.node;
                        result.count += 2;
                        // Catch A up to prev A
                        //cout << "1: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			//cout << "2: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
			//cout << "3: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
			//cout << "4: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			result = A(result.info);
                        node->ch3 = result.node;
                        result = H(result.info);
                        node->ch4 = result.node;
                        result.node = node;
                        result.info = info;
                        return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <E>" << endl;
}
NFInfo F(File info){
	// Production Rule: t1 | t2
	Node* node = createNewNode(6);
	//cout << "F" << endl;
	if(token.type == "T1"){
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.node = node;
		result.info = info;		
		return result;
	}else if(token.type == "T2"){
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <F>" << endl;
}
NFInfo G(File info){
	// Production Rule: B | C | J
	Node* node = createNewNode(7);
	//cout << "G" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	if(token.instance == ". "){
		result.count += 6;
		result = B(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;	
		return result;
	}else if(token.type == "T2"){
		result.count += 2;
		result = C(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;
		return result;
	}else if(a0 == 42 && a1 == 34){
		result.count += 4;
		result = J(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <G>" << endl;
}
NFInfo H(File info){
	// Production Rule: E? | G. | empty
	Node* node = createNewNode(8);
	//cout << "H" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	if(a0 == 44 || (a0 == 44 && a1 == 59)){
		result = E(info);
		node->ch1 = result.node;
		if(token.instance == "? "){
			node->t2 = token;
		//	cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			result.count += 1;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
		
			result.node = node;
			result.info = info;
			return result;
		}
	}else if(token.instance == ". " || token.type == "T2" || a0 == 42 && a1 == 34){
		result = G(info);
		node->ch1 = result.node;
		int a01 = token.instance[0];
		//cout << a01 << " / " << token.instance << " / " << token.instance[0] << endl;
		//for(int i = 0; i < result.count-1; i++){	
		//	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                //	info.ttlInst += token.instance;
		//}
		//cout << a01 << " / " << token.instance << " / " << token.instance[0] << endl;
		if(a01 == 46){
			node->t2 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			result.count += 1;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
		}
		result.node = node;
		result.info = info;	
		return result;
	}else{
		//node->t3 = "empty";
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <H>" << endl;
}
NFInfo J(File info){
	// Production Rule: *" A.
	Node* node = createNewNode(9);
	//cout << "J" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	//cout << a0 << endl;
	//cout << a1 << endl; 
	if(a0 == 42 && a1 == 34){
		node->t1 = token;
		//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result = A(info);
		node->ch2 = result.node;
		if(token.instance == ". "){
			node->t3 = token;
			//cout << "Token: " << token.type << "\t" << token.instance << "\t" << token.line << endl;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.node = node;
			result.info = info;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{" << token.instance << "} inside nonterminal <J>" << endl;
}

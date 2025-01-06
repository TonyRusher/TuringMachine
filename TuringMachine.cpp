#include <bits/stdc++.h>

using namespace std;

class Rules{
	public:
	int nextState;
	char char2write; 
	char move; // I: izquierda, D: derecha
	Rules() : nextState(0), char2write('$'), move('$') {}
	
	Rules(int nextState, char char2write, char move){
		this->nextState = nextState;
		this->char2write = char2write;
		this->move = move;
	}
};

class TuringMachine{
	
	set<char> gammaTape; // alfabeto de la cinta
	set<int> finalStates; // estados finales
	vector<map<char, Rules>> delta; // funcion de trancision

	char blank; // símbolo blanco
	int startState; // estado inicial
	int numStates; // numero de estados
	
	public:
	TuringMachine(int numStates, int startState, char blank, set<char> gammaTape, set<int> finalStates){
		this->numStates = numStates;
		this->startState = startState;
		this->blank = blank;
		this->gammaTape = gammaTape;
		this->finalStates = finalStates;
		delta.resize(numStates + 1);// 1-indexed
	}
	
	void addTransition(int currState, char c, Rules rule){
		delta[currState][c] = rule;
	}
	
	void printTape(list<char> tape, int state, int index){
		system("clear");
		for(auto c : tape)
			cout << c;
		cout << '\n';
		for(int i = 0; i < index; i++)
			cout << ' ';
		cout << '|' << '\n';
		
		for(int i = 0; i < index; i++)
			cout << ' ';
		cout << "q_" << state << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	
	void run(string input){
		int index = 0;
		list<char> tape (input.begin(), input.end());
		int currState = startState;
		list<char>::iterator it = tape.begin();
		printTape(tape, currState, index);
		while(true){
			if(delta[currState].find(*it) == delta[currState].end()){
				cout << "No se encontro transicion para el estado " << currState << " y el simbolo " << *it << '\n';
				cout << "Fin de la simulacion" << '\n';
				break;
			}
			Rules rule = delta[currState][*it];
			*it = rule.char2write;
			currState = rule.nextState;
			if(rule.move == 'I'){// izquierda
				if(it == tape.begin()){
					tape.push_front(blank);
					index = 0; 	
				}else{
					index--;
				}
				it--;
			}else {//derecha
				it++;
				index++;
				if(it == tape.end()){
					tape.push_back(blank);
					it = tape.end();
					it--;
				}
			}
			printTape(tape, currState, index);
			if( finalStates.find(currState) != finalStates.end()){
				cout << "Se llego a un estado final" << '\n';
				break;
			}
		}
	}
};

void procesarCadenas(TuringMachine TM){
	int opc2 = 1;
	while(opc2){
		cout << "Ingrese una cadena a procesar" << endl;
		string input; cin >> input;
		TM.run(input);
		cout << "Desea probar otra cadena ? " << '\n';
		cin >> opc2;
	}
	
}

TuringMachine readMachine(ifstream &inFile){
	int numStates, startState;
	char blank;
	int sizeGammaTape;
	int numRules;
	int numFinalStates;
	set<char> gammaTape;
	set<int> finalStates;
	
	inFile >> sizeGammaTape; 
	for(int i = 0; i < sizeGammaTape; i++){
		char aux; inFile >> aux;
		gammaTape.insert(aux);
	}
	
	inFile >> blank;
	inFile >> numStates;
	inFile >> startState;
	inFile >> numFinalStates;
	// cout << "blank =" << blank << endl;
	// cout << "numStates =" << numStates << endl;
	// cout << "startState =" << startState << endl;
	// cout << "numFinalStates =" << numFinalStates << endl;
	
	
	for(int i = 0; i < numFinalStates; i++){
		int aux; inFile >> aux;
		finalStates.insert(aux);
	}
	
	TuringMachine TM = TuringMachine(numStates, startState, blank, gammaTape, finalStates);
	
	int qi,     qf;
	char    ci,     cf, dir;
	while(inFile >> qi){
		inFile >> ci >> qf >> cf >> dir;
		TM.addTransition(qi, ci, Rules(qf, cf, dir));
		// cout << "ciclo" << '\n';
	}
	return TM;
	
}

void menu(){
	int cont = 1;
	while(cont){
		cout << "Simulador maquina de Turing" << endl;
		cout << "Elija una maquina de Turing" << endl;
		cout << "1. Compara dos cadenas ( sigma = {0,1} )" << endl;
		cout << "2. Ver si una cadena es palindromo ( sigma = {0,1} )" << endl;
		cout << "3. Duplicar una cadena ( sigma = {0,1} )" << endl;
		cout << "4. Elimina los ceros de una cadena ( sigma = {0,1} )" << endl;
		cout << "5. Pone 1 si tienen la misma longitud y 0 en caso contrario  ( sigma = {0,1} )" << endl;
		
		
		int opc = 0; cin >> opc;
		ifstream inFile("maquina" + to_string(opc) + ".txt");
		
		TuringMachine TM = readMachine(inFile);
		
		procesarCadenas(TM);
		
		cout << "Desea probar otra maquina de Turing ? " << '\n';
		cin >> cont;
	}
}

int main() {
	menu();
	return 0;
}
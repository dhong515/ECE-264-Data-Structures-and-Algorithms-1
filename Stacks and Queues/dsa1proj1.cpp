// Danny Hong DSA I Assignment 1
// The following program asks the user for the name of an input text file and output text file. The input file will contain a list of commands (one each line), and each command will tell the program to either create a stack or queue, pop a value from a stack or queue(dequeue), or push a value into a stack or queue (enqueue). The program will then read through each line of the input file andeither execute it or output an specific error message to indicate one of the few problems that may occur while attempting to run the program. The program will then write the output of the program onto an output text file specificed by the user.  

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string>
#include <sstream>
#include <map>

using namespace std;

//DataStructures Class in which the Stack and Queue classes are derived from. All the classes are placed in the same map object.
class DataStructures {
public:
	virtual void* pop() = 0;
	virtual void push(void*) = 0;
};

//Abstract SimpleList class that creates Stacks and Queues
template <typename Data>
class SimpleList: public DataStructures{
private:
        class Node{ 
	public:
                Data* data;
                Node* next; 
	public:
		Node(Data* d, Node* n = NULL) { 
			data = d, 
			next = n; 
		};
		Data* getData() { 
			return data; 
		};
		Node* getNext() { 
			return next; 
		};
		void setNext(Node* n) { 
			next = n; 
		};
	};
	Node* head; //pointer head points to the start of the node
	Node* tail; // pointer tail points to the end of the node

public: 
	SimpleList(){
		head = NULL,
		tail = NULL;
	};
	~SimpleList();
	virtual void* pop()=0;
	virtual void push(void*)=0;
	bool isEmpty();

protected:
	void removeNode(); //Takes away the node at the start
	void append(Data*);
	void input(Data*);
	Node* getHead(){
		return head;
	}
	Node* getTail(){
		return tail;
	}
};

template <typename Data>
void SimpleList<Data>::removeNode(){
	if(head && head->getNext()){
		Node* tmpNode=head;
		head = head->getNext();
		delete tmpNode;
	}
	else{
		head=NULL,
		tail=NULL;
	}
}

template <typename Data>
SimpleList<Data>::~SimpleList(){
	while(!this->isEmpty()){
		removeNode();
	}
}

template <typename Data>
void SimpleList <Data>::input(Data* d){
	SimpleList::Node* newNode = new SimpleList::Node((Data*) d, head);
	head=newNode;
}
	
template <typename Data>
void SimpleList<Data>::append(Data *d){
	SimpleList::Node* newNode = newNode = new SimpleList::Node((Data*) d);
	if(!head){
		head=newNode;
	}
	if(tail){
		tail->setNext(newNode);
	}
	tail=newNode;
}
template <typename Data>
bool SimpleList<Data>::isEmpty(){
	if(!head && !tail){
		return true;
	}
	else{
		return false;
	}
}

template <typename Data>
class Stack: public SimpleList<Data>{
public:
	void push(void* d){
		SimpleList<Data>:: input((Data*) d);
	}
	void* pop(){
		if(this-> isEmpty()){
			return NULL;
			}
		Data* tmp;
		tmp = this->getHead()->getData();
		this->removeNode();
		return tmp;
	}	
};

template <typename Data>
class Queue: public SimpleList<Data>{
public:
	void push(void* d){
		SimpleList<Data>:: append((Data*) d);
	}
	void* pop(){
		if(this->isEmpty()){
			return NULL;
		}
		Data* tmp = this->getHead()->getData();
		this->removeNode();
		return tmp;
	}
};

template <template<typename> typename S, typename T>
void createDataStruct(std:: string*);

template <typename T>
void print(std:: string*, ofstream*);

std:: map<string, DataStructures*> DataStructMap;

int main(int i, char* c[]){
	
	//User-program interaction
	std:: string input; //"input.txt"=std:: string input (user input)
	std:: string output; //"output.txt"=std:: string output (program output)
	std:: cout << "Enter the name of the input file: ";
	std:: cin >> input;
	std:: cout << "Enter the name of the output file: ";
	std:: cin >> output;

	ifstream inputF; //input Stream class that reads the command lines from the input file
	ofstream outputF; //output Stream class that writes the results onto the output file
	
	//prints an error message for possible failure in opening input and output files.
	try{
		inputF.open(input);
	}
	catch (ifstream:: failure & e){
		cerr << "ERROR: Failure opening input file." << typeid(e).name()<< endl;
		exit(20);
	}
	try {
                outputF.open(output);
        }
        catch (ofstream:: failure & e) {
                cerr << "ERROR: Failure opening output file." << typeid(e).name() << endl;
                exit(20);
        }
	
	string command;
	getline(inputF, command);
	while(inputF){
		istringstream strStream(command); //converts a line from the file to a stream

		//input commands are broken up into parts (words)
		std:: string part1;
		std:: string part2;
		std:: string part3;
		
		strStream >> part1;
		strStream >> part2;
		strStream >> part3;
		outputF<< "PROCESSING COMMAND: "<< command << endl;
		
		//Program proceeds to read each line part by part.
		if(part1 == "create"){ //creates the data structure
			if(DataStructMap.find(part2)== DataStructMap.end()){
				if(part3 == "stack"){
					//Program creates stack based on data type (integer,string, or double)
					if (part2.substr(0,1) == "i"){
						createDataStruct<Stack, int>(&part2);
					}
					else if(part2.substr(0,1) == "s"){
						createDataStruct<Stack, std:: string>(&part2);
					}
					else if(part2.substr(0,1) == "d"){
						createDataStruct<Stack, double>(&part2);
					}					
				}
				else if(part3=="queue"){
					//Program creates queue based on data type (integer,string, or double)
					if(part2.substr(0,1) == "i"){
						createDataStruct<Queue, int>(&part2);
					}
					else if(part2.substr(0,1)=="s"){
						createDataStruct<Queue, std:: string>(&part2);
					}
					else if(part2.substr(0,1) =="d"){
						createDataStruct<Queue, double>(&part2);
					}						
				}			
			}
			else{ 
				outputF << "ERROR: This name already exists!" << endl;
			}
		}
        	else if (part1 == "pop"){ //looks for the specific data structure and pops an element from it.
            		if (DataStructMap.find(part2) != DataStructMap.end()) {
                		if (part2.substr(0, 1) == "i"){
                    			print<int>(&part2, &outputF);
				}
                		else if (part2.substr(0, 1) == "s"){
                    			print<std::string>(&part2, &outputF);
				}
                		else if (part2.substr(0, 1) == "d"){
                    			print<double>(&part2, &outputF);
				}
                	}
                	else {
                    		outputF << "ERROR: This name does not exist!" << endl;
                	}
		}
            	else if (part1 == "push"){ //looks for the specific data structure and pushes an element into it.
			if(DataStructMap.find(part2)!= DataStructMap.end()){
				if(part2.substr(0,1)=="i"){
					int* num = new int(stoi(part3)); //stoi is a C++ 11 feature which converts a string into a integer.
					DataStructMap[part2]->push(num);
				}
				else if(part2.substr(0,1) =="s"){
					string* tmp = new string(part3);
					DataStructMap[part2]->push(tmp);
				}
				else if(part2.substr(0,1) =="d"){
					double *num = new double(stod(part3)); //stod is a C++ 11 feature which converts a string into a double.
					DataStructMap[part2]->push(num);
				}		
			}                               
                	else {
                        	outputF << "ERROR: This name does not exist!" << endl;
                	}
		}
                getline(inputF, command);
        }
	
	
	DataStructMap.clear();//removes all elements from the map container
        inputF.close();//closes input document
        outputF.close();//closes output document

        return 0;
}

//constructs the corresponding data structure
template<template<typename> typename S, typename T>
void createDataStruct(std:: string *name){
	S<T>* newDataStruc = new S<T>();
	DataStructMap.insert(pair <string, DataStructures*>(*name, newDataStruc));
}

//prints the value thats being popped using the correct type
template <typename T>
void print(std::string *key, ofstream *outputFile){
	T* tmp = (T*)(DataStructMap[*key]->pop());
	if (tmp){
                *outputFile << "Value popped: "<< *tmp << endl;
	}
        else{
                *outputFile << "ERROR: This list is empty!"<< endl;
	}
}

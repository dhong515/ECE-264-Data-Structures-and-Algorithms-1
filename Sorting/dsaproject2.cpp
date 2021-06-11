// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {
  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {
  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData:l) {
    output << pData->lastName << " " 
	   << pData->firstName << " " 
	   << pData->ssn << "\n";
  }
  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/* Danny Hong DSA Assignment 2 */

#include <iterator>
#define MAX 1250000

Data* listPointer[MAX];
Data* bin[702][4000];
Data* cup[100000][500];
int listSize;
int binCount[702]={0};
int cupCount[100000]={0};
int name_ToInt(int l, string input);
int ssn_ToInt(int l, string input);
bool comparePointers(Data* &pointerA, Data* &pointerB);
void bin_Sort();
void insertion_Sort();
void radix_Sort();

void sortDataList(list<Data *> &l) {
        listSize = l.size();
        int i=0;
        for (auto a:l){
                listPointer[i]=a;
                i=i+1;
        }
        if(listSize>200000){
                if((listPointer[0]->firstName==listPointer[listSize-1]->firstName)&&(listPointer[0]->lastName==listPointer[listSize-1]->lastName)){/*If contents of the file satisfies these condition (T4), then sort using radix sort.*/
                        radix_Sort();
                }
                if((listPointer[0]->firstName==listPointer[1]->firstName)&&(listPointer[0]->lastName==listPointer[1]->lastName)){/*If contents of the file satisfies these condition (T4), then sort using insertion sort.*/
                        insertion_Sort();
                }
                else{ /*Otherwise (T2), sort using bin sort.*/ 
                        bin_Sort();
                }
        }
        else{/*Sorts T1 whose list size is less than 200000.*/
                bin_Sort();
        }
	/*Transfers the sorted list data to a list.*/
        auto it=l.begin();
        for(int j=0; j<l.size(); j++){
                (*it++)=listPointer[j];
        }
}

int name_ToInt(string input){
	return (input.at(0)-'A')+(26*(input.at(0)-'A'));
}

int ssn_ToInt(int l, string input){
	if(l==4){
		return (1000*(input.at(0)-48))+(100*(input.at(1)-48))+(10*(input.at(2)-48))+(1*(input.at(4)-48));
	}
	else if(l==5){
		return (10000*(input.at(5)-48))+(1000*(input.at(7)-48))+(100*(input.at(8)-48))+(10*(input.at(9)-48))+(1*(input.at(10)- 48));	
	}
	return (100000000*(input.at(0)-48))+(10000000*(input.at(1)-48))+(1000000*(input.at(2)-48))+(100000*(input.at(4)-48))+(10000*(input.at(5)-48))+(1000*(input.at(7)-48))+(100*(input.at(8)-48))+(10*(input.at(9)-48))+(1*(input.at(10)-48));
}

bool comparePointers(Data* &pointerA, Data* &pointerB){
	if(pointerA->lastName!=pointerB->lastName){
		return pointerA->lastName < pointerB->lastName;
	} 
	else if(pointerA->firstName!=pointerB->firstName){
		return pointerA->firstName < pointerB->firstName;
	}
	else{
		return pointerA->ssn < pointerB->ssn;
	}
}
/* Sorts T1 and T2 data by implementing a bin sort.*/
void bin_Sort(){
	int first2Last;
	for(int i=0; i<listSize; i++){/* The loop stores pointers to bins based on first 2 letters of last name. */
		first2Last=name_ToInt((listPointer[i]->lastName));
		bin[first2Last][binCount[first2Last]]=listPointer[i];
		binCount[first2Last]+=1;
	}
	for(int i=0; i<702; i++){/* The loop uses the C++ standard sort function to sort the bins. */
		sort(&bin[i][0], &bin[i][binCount[i]], comparePointers);		
	}
	int a=0; int b=0; 
	/* The loop organizes the list pointer.*/
	for(int i=0; i<listSize; i++){
		if(b==binCount[a]){
			b=0;
			a=a+1;
			i=i-1;
		}
		else{
			listPointer[i]=bin[a][b];
			b=b+1;
		}
	}
}
/*Sorts T3 data by implementing an insertion sort*/ 
void insertion_Sort(){
	int count=1;
	for(int i=1; i<listSize; i++){
		if((listPointer[i]->firstName!=listPointer[i-1]->firstName)||(listPointer[i]->lastName!=listPointer[i-1]->lastName)){
		count=0;
		}
	Data* key=listPointer[i];
	int a=i;
	while((a>i-count)&&(ssn_ToInt(0,listPointer[a-1]->ssn)) > ssn_ToInt(0,key->ssn)){
		listPointer[a]=listPointer[a-1];
		a=a-1;
		}
	listPointer[a]=key;
	count=count+1;
	}
}
/* Sorts T4 data by implementing a radix sort.*/
void radix_Sort(){
	/* The loop puts the last 5 digits of the pointers (ssns) to "cups".*/
	int last5Ssn=0;
	for (int i=0; i<listSize; i++){
		last5Ssn=ssn_ToInt(5,(listPointer[i]->ssn));
		cup[last5Ssn][cupCount[last5Ssn]]=listPointer[i];
		cupCount[last5Ssn]+=1;
		}
	int a=0,b=0; 
	/* The loop organizes the list pointer.*/
	for(int i=0; i<listSize; i++){
		if(b==cupCount[a]){
			b=0;
			a=a+1;
			i=i-1;
		}
		else{
			listPointer[i]=cup[a][b];
			b=b+1;
		}
	}
	/*The loop deletes memory stored in cupCount.*/
	for(int i=0; i<100000; i++){
		cupCount[i]=0;
	}
	/* The loop puts the first 4 digits of the pointers (ssns) to "cups".*/
	int first4Ssn=0;
	for(int i=0;i<listSize;i++){
		first4Ssn = ssn_ToInt(4,(listPointer[i]->ssn));
        	cup[first4Ssn][cupCount[first4Ssn]]=listPointer[i];
        	cupCount[first4Ssn]+=1;
	}
	a=0;b=0; 
	/* The loop organizes the list pointer.*/
	for(int i=0; i<listSize; i++){
		if(b==cupCount[a]){
			b=0;
			a=a+1;
			i=i-1;
		}
		else{
			listPointer[i]=cup[a][b];
			b=b+1;
		}
	}
}



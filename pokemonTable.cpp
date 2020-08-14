#include "pokemonTable.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <regex>
#include <iomanip>

using namespace std;

// default constructor
pokemonTable::pokemonTable()
{}

// constructor
pokemonTable::pokemonTable(int key)
{
	this->bucket = key;		// set bucket number
	table.resize(bucket);	// initilize table with bucket number
}

// function to setup the hash table with the 
// initial data from the input file
void pokemonTable::setData(int ID, std::string data)
{
	int i = hash(ID);	// call hash function to get hash key
	pokemon temp;		// create temp obj
	temp.ID = ID;	// set id
	temp.data = data;	// set data
	table[i].push_back(temp);	// push back temp obj onto table vectors
}

// function to insert a new entry in the table
// takes in data and sets up the entry
// in the original hash table
void pokemonTable::insert(std::string tuple, std::string tableName)
{
	stringstream ss(tuple);	// stringstring to parse tuple string
	string ID, data;	// strings to hold id and data
	getline(ss, ID, '|');	// parse tuple to get id
	int i = hash(stoi(ID));	// call hash function to get hash key
	pokemon temp;	// create temp obj
	temp.ID = stoi(ID);	// set id
	temp.data = tuple;	// set data
	table[i].push_back(temp);	// push back temp obj onto table vectors
}

// function to display the data that is stored in the
// hash table it will format the data using setw 
// to have a uniform layout
void pokemonTable::display()
{
	// output the table names
	cout << left << setw(10) << "ID"
		<< left << setw(15) << "NAME"
		<< left << setw(25) << "TYPE"
		<< left << setw(35) << "STRONG_TO"
		<< left << setw(30) << "WEAK_TO" << endl;
	
	// loop through the hash table
	for (int i = 0; i < bucket; i++)
	{
		// loop through the items in each bucket
		for (auto p : table[i])
		{
			// make sure the bucket is not empty
			if (p.ID > -1)
			{
				stringstream ss(p.data);	// stringstream to parse data
				string ID, name, type, strongTo, weakTo;	// strings to hold data
				getline(ss, ID, '|');		// get ID
				getline(ss, name, '|');		// get name
				getline(ss, type, '|');		// get type
				getline(ss, strongTo, '|');	// get strongTo
				getline(ss, weakTo, '|');	// get weakTo

				// output the data with formatting
				cout << left << setw(10) << ID
					<< left << setw(15) << name
					<< left << setw(25) << type
					<< left << setw(35) << strongTo
					<< left << setw(30) << weakTo << endl;
			}
		}
	}
}

// function to update an entry in the table
// it takes in the x,y pair to find the correct entry
// x = hash key, y = index in inner vector 
void pokemonTable::update(int inner, int outer, string tuple)
{
	outer--;	// decrement the outer count
	table[inner][outer].data = tuple;	// change the data to updated value
}

// function to search for a specific value in the table
// this works by separating the given string into it's indiviudal
// values ie: name, type, etc
// it then loops through all values in hash table looking
// for each of the values given to search for
// the function returns a vector with an x,y pair
// giving the location of the entry in the table
vector<int> pokemonTable::find(std::string tuple, std::string tableName)
{
	// remove all characters before ((
	string y = tuple.substr(tuple.find("(") + 1);
	string x = y.substr(y.find("(") + 1);
	stringstream ss(x);	// create stringstring to parse data
	string a, b, c, d, e, f;	// variables to hold parsed data
	getline(ss, a, '|');	// get id
	getline(ss, b, '|');	// get name
	getline(ss, c, '|');	// get type
	getline(ss, d, '|');	// get strengths
	getline(ss, e, '|');	// get weaknesses

	// remove last part of string "),pokemon)"
	regex r("(\\),pokemon\\))+");	
	string repl = "";
	string e2 = regex_replace(e, r, repl);

	vector <string> test;	// temp vector to hold strings to search for
	test.push_back(a);	// push back values to search for
	test.push_back(b);
	test.push_back(c);
	test.push_back(d);
	test.push_back(e2);

	// loop through the hash table
	for (int i = 0; i < bucket; i++)
	{
		int inner = 0;	// int to hold inner value
		// loop through the items in each bucket
		for (auto p : table[i])
		{
			inner++;	//increment inner value
			if (stoi(test[0]) == p.ID)
			{
				for (auto x : test)	//for each item in test
				{
					// look for values in entries
					if (p.data.find(x) != string::npos)
					{
						vector<int> xy;		// vector to hold x,y pair
						xy.push_back(i);	// push back outer index
						xy.push_back(inner);	// push back inner index
						return xy;	// return vector
					}
				}
			}
		}
	}

	vector<int> xy;	// if value isn't found create vector
	xy.push_back(-1);	// and push back -1 to let main function
	return xy;			// know the value was not found
						// and return the vector
}

// function to delete specific entry
// takes in the location (x,y) of the 
// entry in the table and deletes the data
// in that entry
void pokemonTable::deleteEntry(int inner, int outer)
{
	outer--;	// decrement the outer count
	table[inner][outer].ID = -1;	// set the id to null
	table[inner][outer].data = "";	// set the data to ""
}

// function to redirect the output 
// to a file
void pokemonTable::write()
{
	ofstream file;	// file
	file.open("out__pokemon.txt");	// open file

	file << "ID\n" << "ID, NAME, TYPE, STRONG_TO, WEAK_TO\n";	// output table names	
	// loop through each bucket in hastable
	for (int i = 0; i < bucket; i++)
	{
		// loop through each item in the inner vector
		for (auto p : table[i])
		{
			if (p.ID > -1)	// make sure the item isn't empty
				file << p.data << endl;		// output to file
		}
	}
	file.close();	// close file
}

// hash function using mid-square hasing
int pokemonTable::hash(int ID)
{
	// square ID
	ID = ID * ID;
	string l = to_string(ID);	// convert id to str
	int length = l.length();	// get length of str
	
	string r;	// create new str to get middle digits

	// get middle digits of str depending on 
	// number of digits
	if (length == 2)
	{
		r = l.substr(1, 1);
	}
	if (length == 3)
	{
		r = l.substr(1, 1);
	}
	if (length == 4)
	{
		r = l.substr(1, 2);
	}
	if (length == 5)
	{
		r = l.substr(2, 3);
	}

	// convert back to int
	int R = stoi(r);

	// perform mod operation to get key
	int key = R % bucket;

	return key;		// return key
}

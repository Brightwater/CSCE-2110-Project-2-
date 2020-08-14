#include "trainerTable.h"

#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <fstream>

using namespace std;

// default constructor
trainerTable::trainerTable()
{}

// constructor
trainerTable::trainerTable(int size)
{
	bucket = size;	// set number of buckets
	
	// initilize the hash table with
	// size entrys
	for (int i = 0; i < size; i++)
	{
		trainer temp;	// create temp obj
		int key = -1;	// set key to -1
		temp.data = "";		// set data to ""
		temp.firstname = "";	// set names to ""
		temp.lastname = "";

		trainers.push_back(temp);	// push back temp
	}
}

// function to setup the hash table with the 
// initial data from the input file
void trainerTable::setData(std::string firstname, std::string lastname, std::string data)
{
	int key = hash(firstname, lastname);	// hash function returns hash key

	int probed = 0;	// probed index
	// prevent infinite loop
	while (probed < bucket)
	{
		if (trainers[key].key <= -1)	// make sure entry is not empty
		{
			trainers[key].key = key;	// set key
			trainers[key].data = data;	// set data
			trainers[key].firstname = firstname;	// set names
			trainers[key].lastname = lastname;
			
			return;	// return if set
		}

		// go to next key if not found
		key = (key + 1) % bucket;
		probed++;
	}

	// should not get here
	cout << "Error\n";
}

// function to insert a new entry in the table
// takes in data and sets up the entry
// in the original hash table
void trainerTable::insert(std::string tuple, std::string tableName)
{
	stringstream ss(tuple);	// stringstring to parse tuple string
	string firstName, lastName, data;	// strings to hold data and names
	getline(ss, firstName, '|');	// parse tuple to get names
	getline(ss, lastName, '|');	
	int i = hash(firstName, lastName);	// hash function returns hash key

	int probed = 0;	// probed index
	// prevent infinite loop
	while (probed < bucket)
	{
		if (trainers[i].key == i)	// make sure entry is not empty
		{
			trainers[i].key = i;	// set key
			trainers[i].firstname = firstName;	// set
			trainers[i].lastname = lastName;	// set names
			trainers[i].data = tuple;			// set data
			return;	// return if set
		}
		// go to next key if not found
		i = (i + 1) % bucket;
		probed++;
	}

	// should not get here?
	cout << "Error inserting\n";
}

// function to display the data that is stored in the
// hash table it will format the data using setw 
// to have a uniform layout
void trainerTable::display()
{
	// output the table names
	cout << left << setw(15) << "FIRSTNAME"
		<< left << setw(15) << "LASTNAME"
		<< left << setw(10) << "AGE"
		<< left << setw(60) << "BADGES_HELD"
		<< left << setw(35) << "POKEMON_OWNED" << endl;

	// loop through the hash table
	for (int i = 0; i < bucket; i++)
	{
		if (trainers[i].firstname != "")	// make sure the bucket is not empty
		{
			stringstream ss(trainers[i].data);	// stringstream to parse data
			string firstname, lastname, age, badges, pokemon;	// strings to hold data
			getline(ss, firstname, '|');// get firstname
			getline(ss, lastname, '|');	// get lastname
			getline(ss, age, '|');		// get age
			getline(ss, badges, '|');	// get badges_held
			getline(ss, pokemon, '|');	// get pokemon_owned

			// output the data with formatting
			cout << left << setw(15) << firstname
				<< left << setw(15) << lastname
				<< left << setw(10) << age
				<< left << setw(60) << badges
				<< left << setw(35) << pokemon << endl;
		}
	}
}

// function to update an entry in the table
// it takes in the x value which is the location
// of the entry in the table
void trainerTable::update(int key, std::string tuple)
{
	// remove all characters before ((
	string y = tuple.substr(tuple.find("(") + 1);
	string x = y.substr(y.find("(") + 1);
	stringstream ss(x);	// create stringstring to parse data
	string a, b;	// variables to hold parsed data
	getline(ss, a, '|');	// get firstname
	getline(ss, b, '|');	// get lastname

	// remove last part of string "),gyms)"
	regex r("(\\),trainers\\))+");
	string repl = "";
	string e2 = regex_replace(x, r, repl);
	
	// push back values to search for
	trainers[key].data = e2;
	trainers[key].firstname = a;
	trainers[key].lastname = b;
}

// function to search for a specific value in the table
// this works by separating the given string into it's indiviudal
// values ie: name, type, etc
// it then loops through all values in hash table looking
// for each of the values given to search for
// the function returns an int
// giving the location of the entry in the table
int trainerTable::find(std::string tuple, std::string tableName)
{
	// remove all characters before ((
	string y = tuple.substr(tuple.find("(") + 1);
	string x = y.substr(y.find("(") + 1);

	stringstream ss(x);	// create stringstring to parse data
	string a, b, c, d, e, f;	// variables to hold parsed data
	getline(ss, a, '|');	// parse and get data for
	getline(ss, b, '|');	// each item
	getline(ss, c, '|');
	getline(ss, d, '|');
	getline(ss, e, '|');

	// remove last part of string "),trainers)"
	regex r("(\\),trainers\\))+");
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
		// check if entry is already taken
		if (trainers[i].firstname != "")
		{
			for (auto x : test)	// for each item in test	
			{
				if (trainers[i].data.find(x) != string::npos)	// look for values in entries	
					return i;	// return location of entry
			}
		}
	}

	return -1;	// return -1 if not found
}

// function to delete specific entry
// takes in the location x of the 
// entry in the table
void trainerTable::deleteEntry(int x)
{
	trainers[x].key = -1;	// set entry to null
	trainers[x].data = "";	// set data to ""
	trainers[x].firstname = "";
	trainers[x].lastname = "";
}

// function to redirect the output 
// to a file
void trainerTable::write()
{
	ofstream file;	// file
	file.open("out__trainers.txt");	// open file

	file << "FIRSTNAME, LASTNAME\n" << "FIRSTNAME, LASTNAME, AGE, BADGES_HELD, POKEMON_OWNED\n";	// output table names and key
	// loop through each bucket in hashtable
	for (int i = 0; i < bucket; i++)
	{
		if (trainers[i].key > -1)	// make sure the item isn't empty
			file << trainers[i].data << endl;	// output to file
	}
	file.close();	// close file
}


// addititve string hash function
int trainerTable::hash(std::string firstname, std::string lastname)
{
	int num = 0; // var to hold additive asci values

	// loop through every char in firstname
	for (unsigned int i = 0; i < firstname.size(); i++)
	{
		num += firstname[i];	// add asci values in firstname string
	}
	// loop through every char in lastname
	for (unsigned int i = 0; i < lastname.size(); i++)
	{
		num += lastname[i];	// add asci values in lastname string
	}

	int key = num % bucket;	// hash function

	return key;	// return key
}

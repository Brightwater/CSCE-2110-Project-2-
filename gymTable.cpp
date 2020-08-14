#include "gymTable.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <regex>
#include <iomanip>
#include <fstream>

using namespace std;

// default constructor
gymTable::gymTable()
{}

// constructor
gymTable::gymTable(int size)
{
	bucket = size;	// set number of buckets

	// initilize the hash table with
	// size entrys
	for (int i = 0; i < size; i++)
	{
		gym temp;	// creat temp obj
		int key = -1;	// set key to -1
		temp.data = "";// set data to ""

		gyms.push_back(temp);	// push back temp obj
	}
}

// function to setup the hash table with the 
// initial data from the input file
void gymTable::setdata(int ID, std::string data)
{
	int key = hash(ID);	// hash function returns hash key
	
	gyms[key].gymID = ID;	// set id
	gyms[key].data = data;	// set data
}

// function to insert a new entry in the table
// takes in data and sets up the entry
// in the original hash table
void gymTable::insert(std::string tuple, std::string tableName)
{
	stringstream ss(tuple);	// stringstream to parse data

	string ID, data;	// variables to hold data and id
	getline(ss, ID, '|');	// parse tuple to get id
	int i = hash(stoi(ID));	// hash function returns hash key
	gyms[i].gymID = stoi(ID);	// set gymid
	gyms[i].data = tuple;		// set data
}

// function to display the data that is stored in the
// hash table it will format the data using setw 
// to have a uniform layout
void gymTable::display()
{
	// output the table names
	cout << left << setw(10) << "GYMID"
		<< left << setw(25) << "NAME"
		<< left << setw(20) << "LOCATION"
		<< left << setw(30) << "LEADER"
		<< left << setw(30) << "BADGE" << endl;

	// loop through the hash table
	for (int i = 0; i < bucket; i++)
	{
		if (gyms[i].gymID > -1)// make sure the bucket is not empty
		{
			stringstream ss(gyms[i].data);	// stringstream to parse data
			string ID, name, location, leader, badge;	// strings to hold data
			getline(ss, ID, '|');	// get id
			getline(ss, name, '|');	// get name
			getline(ss, location, '|');	// get location
			getline(ss, leader, '|');	// get leader
			getline(ss, badge, '|'); // get badge

			// output the data with formatting
			cout << left << setw(10) << ID
				<< left << setw(25) << name
				<< left << setw(20) << location
				<< left << setw(30) << leader
				<< left << setw(30) << badge << endl;
		}
	}
}

// function to update an entry in the table
// it takes in the x value which is the location
// of the entry in the table
void gymTable::update(int key, std::string tuple)
{
	gyms[key].data = tuple;	// change data value
}

// function to search for a specific value in the table
// this works by separating the given string into it's indiviudal
// values ie: name, type, etc
// it then loops through all values in hash table looking
// for each of the values given to search for
// the function returns an int
// giving the location of the entry in the table
int gymTable::find(std::string tuple, std::string tableName)
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

	// remove last part of string "),gyms)"
	regex r("(\\),gyms\\))+");
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
		if (gyms[i].gymID > -1 && stoi(test[0]) == gyms[i].gymID)
		{
			for (auto x : test)	// for each item in test	
			{
				if (gyms[i].data.find(x) != string::npos)	// look for values in entries	
				{
					return i;	// return location of entry
				}
			}
		}
	}
	return -1;	// return -1 if not found
}

// function to delete specific entry
// takes in the location x of the 
// entry in the table
void gymTable::deleteEntry(int x)
{
	gyms[x].gymID = -1;	// set entry to null
	gyms[x].data = "";	// set data to ""
}

// function to redirect the output 
// to a file
void gymTable::write()
{
	ofstream file;	// file
	file.open("out__gyms.txt");	// open file

	file << "GYMID\n";	// output table key
	file << "GYMID\n" << "GYMID,NAME,LOCATION,LEADER,BADGE\n";	 //output table names
	// loop through each bucket in hashtable
	for (int i = 0; i < bucket; i++)
	{	// make sure the item isn't empty
		if (gyms[i].gymID > -1)
		{
			file << gyms[i].data << endl;	// output to file
		}
	}
	file.close(); // close file
}

// function to find the nearest prime number to the
// number of buckets in the hashtable
// this is used for the second hash function
int gymTable::getPrime(int ID)
{
	while (ID >= 0)
	{
		int num = ID - 1;	// make id less than num
		if (isPrime(num) == true)
		{
			return num;
		}
		else
			ID++;	// increment id if id - 1 is not prime
	}

	// should not get here
	cout << "Prime error\n";
	return -1;
}

// function to check if number is prime
bool gymTable::isPrime(int num)
{
	// not prime if it is 1 or less
	if (num <= 1)
		return false;

	// check if it is prime using
	// modulo
	for (int i = 2; i < num; i++)
	{
		if (num % i == 0)	// mod operation
			return false;	// false if not prime
		
		// return true if number is prime
		return true;
	}
	return false;
}

// double hashing function
int gymTable::hash(int ID)
{
	// get prime number to be used
	// in second hashing function
	int prime = getPrime(ID);

	int hash1 = (ID % bucket);	// first hashing function
	int hash2 = (prime + (ID % prime));	// second hash function using prime number

	int i = 0; // index
	int key;
	while (1)
	{
		cout << "test\n";
		key = (hash1 + i * hash2) % bucket;	// double hashing function

		if (gyms[key].data == "")	// make sure bucket is empty
		{
			break;	// return hash key
		}
		i++;	// if bucket is not empty increment i
	}		
	return key;
}

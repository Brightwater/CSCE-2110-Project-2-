/* Jeremiah Spears | CSCE 2110 | 7/16/2019
	~ Project 2: Gotta Catch 'Em All ~
This program will take input files given by
the user.  These files hold tables for
pokemon gyms, pokemon, and trainers.
These files will be read in and stored
in hashtables.  The hashtables will support
a variety of commands to be run on them to
manipulate/read the data. These commands are:
select, delete, insert, display and update. */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

#include "pokemonTable.h"
#include "trainerTable.h"
#include "gymTable.h"

using namespace std;

// function declarations
pokemonTable pokemonRead(string fileString, int insertCount);
trainerTable trainersRead(string fileString, int insertCount);
gymTable gymRead(string fileString, int insertCount);
string strip(string i);

// main function
int main()	
{
	// variables
	ifstream fileName;			// file name
	string fName;				// file name string
	string pokemonFile;			// pokemon input file
	string trainersFile;		// trainers input file
	string gymsFile;			// gyms input file
	vector <string> commands;	// vector of commands

	// prompt the user for input
	cout << "Input the file name (and extension) of the file you want to use.\n";
	cin >> fName;
	//fName = "test.txt";

	// open file
	fileName.open(fName);
	// Make sure file is open 
	// and read in file
	if (fileName.is_open())
	{
		string line;	// string to hold each line in file

		// loop three times for the three input lines/files
		for (int i = 0; i < 3; i++)
		{
			getline(fileName, line);	// getline and set it to line
			stringstream ss(line);		// create stringstream to parse line
			string temp;				// create temp strings for parsing
			string temp2;

			getline(ss, temp, ' ');		// parse line by ' '
			getline(ss, temp2, ' ');

			// setup file names from file
			// check for table names
			if (temp2 == "pokemon")
				pokemonFile = temp;	// file for pokemon
			if (temp2 == "trainers")
				trainersFile = temp;	// file for trainers
			if (temp2 == "gyms")
				gymsFile = temp;	// file for gyms
		}
		getline(fileName, line);	// skip the extra space in file
		
		// read the rest of the lines in the file
		while (getline(fileName, line))
		{
			// don't read the final line that's empty
			if (line != " ")
			{
				// add command to commands vector for each line
				commands.push_back(line);
			}
		}
	}
	// Let the user know they entered incorrect input
	else
	{
		cout << "File does not exist! Please try again.\n";
		return 0;
	}

	// count the number of insert commands given to make sure 
	// that the hash tables have enough buckets
	// (this is for edgecases where a large number of inserts
	// are given in the input)
	int insertCount = 0;
	// run through commands
	for (auto &i : commands)
	{
		// check for insert commands
		if (i.find("INSERT") != string::npos)
		{
			// increase insertCount
			insertCount++;
		}
	}

	// read in individual files and call their read functions
	pokemonTable pokemon = pokemonRead(pokemonFile, insertCount);		// pokemon file
	trainerTable trainers = trainersRead(trainersFile, insertCount);	// trainer file
	gymTable gyms = gymRead(gymsFile, insertCount);						// gym file

	// run through all commands given in input file
	for (auto &i : commands) 
	{
		string p = strip(i);	// call parse function
		// check for display commands
		if (i == "DISPLAY()")
		{
			cout << endl;			// skip a line
			cout << "DISPLAY\n";	// output display
			trainers.display();		// call trainers display function
			cout << endl;			// skip a line
			pokemon.display();		// call pokemon display function
			cout << endl;			// skip a line
			gyms.display();			// call gyms display function
			cout << endl;			// skip a line
		}
		// check for insert commands
		if (i.find("INSERT") != string::npos)
		{
			// check command for pokemon string
			// if pokemon string exists run command
			// for pokemon table
			if (i.find("pokemon") != string::npos)
			{
				// call find function
				// returns an x, y pair for row, col in table
				vector<int> xy = pokemon.find(i, "");
				// check if entry was found
				if (xy[0] == -1)
				{
					// if entry not found 
					// insert new entry in table
					pokemon.insert(p, "");		// if pokemon string exists run command
					cout << "Inserted " << p;	// for pokemon table			
				}
				if (xy[0] > -1)	// if entry found
					cout << "Failed to insert " << p;	// output msg
				cout << " In the pokemon table\n";		// output msg
			}
			// check command for gyms string
			// if gyms string exists run command
			// for gyms table
			if (i.find("gyms") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = gyms.find(i, "");
				if (x == -1)
				{
					// if entry not found
					// insert new entry in table
					gyms.insert(p, "");			// if gym string exists run command
					cout << "Inserted " << p;	// for gym table
				}
				if (x > -1)	// if entry found
					cout << "Failed to insert " << p;	// output msg
				cout << " In the gyms table\n";			// output msg
			}
			// check command for trainers string
			// if trainers string exists run command
			// for trainers table
			if (i.find("trainers") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = trainers.find(i, "");
				if (x == -1)
				{
					// if entry not found
					// insert new entry in table
					trainers.insert(p, "");			// if trainer string exists run command
					cout << "Inserted " << p;	// for trainer table
				}
				if (x > -1) // if entry found
					cout << "Failed to insert " << p; // output msg
				cout << " In the trainers table\n";
			}
		}

		// check for update commands
		if (i.find("UPDATE") != string::npos)
		{
			// check command for pokemon string
			// if pokemon string exists run command
			// for pokemon table
			if (i.find("pokemon") != string::npos)
			{
				// call find function
				// returns an x, y pair for row, col in table
				vector<int> xy = pokemon.find(i, "");
				if (xy[0] == -1)	// if entry not found
					cout << "Failed to update " << p;	// don't update
				if (xy[0] > -1)
				{	// if entry found
					pokemon.update(xy[0], xy[1], p);	// update entry
					cout << "Updated " << p;			// output msg
				}
				cout << " In the pokemon table\n";		// output msg
			}
			// check command for gyms string
			// if gyms string exists run command
			// for gyms table
			if (i.find("gyms") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = gyms.find(i, "");
				if (x == -1)	// if entry not found
					cout << "Failed to update " << p;	// dont update
				if (x > -1)		// if entry found
				{
					gyms.update(x, p);			// update entry
					cout << "Updated " << p;	// output msg
				}
				cout << " In the gyms table\n";	// output msg
			}
			// check command for trainers string
			// if trainers string exists run command
			// for trainers table
			if (i.find("trainers") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = trainers.find(i, "");
				if (x == -1)	// if entry not found;
					cout << "Failed to update " << p; // don't update
				if (x > -1)		// if entry found
				{
					trainers.update(x, i);	// 
					cout << "Updated " << p;
				}
				cout << " In the trainers table\n";
			}
		}
		// check for select commands
		if (i.find("SELECT") != string::npos && i.find("PROJECT") == string::npos)
		{
			// check command for pokemon string
			// if pokemon string exists run command
			// for pokemon table
			if (i.find("pokemon") != string::npos)
			{
				// call find function
				// returns an x, y pair for row, col in table
				vector<int> xy = pokemon.find(i, "");
				if (xy[0] == -1)	// if entry not found
					cout << "Failed to find " << p;	// output msg
				if (xy[0] > -1)	// if entry found 
					cout << "Found " << p;			// output msg
				cout << " In the pokemon table\n";	// output msg
			}
			// check command for gyms string
			// if gyms string exists run command
			// for gyms table
			if (i.find("gyms") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = gyms.find(i, "");
				if (x == -1)	// if entry not found
					cout << "Failed to Find " << p;	// output msg
				if (x > -1)		// if entry found
					cout << "Found " << p;			// output msg
				cout << " In the gyms table\n";		// output msg
			}
			// check command for trainers string
			// if trainers string exists run command
			// for trainers table
			if (i.find("trainers") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = trainers.find(i, "");
				if (x == -1)	// if entry not found
					cout << "Failed to find " << p; // output msg
				if (x > -1)
					cout << "Found " << p;			// output msg;
				cout << " In the trainers table\n";
			}
		}
		// check for delete commands
		if (i.find("DELETE") != string::npos)
		{
			// check command for pokemon string
			// if pokemon string exists run command
			// for pokemon table
			if (i.find("pokemon") != string::npos)
			{
				// call find function
				// returns an x, y pair for row, col in table
				vector<int> xy = pokemon.find(i, "");
				if (xy[0] == -1)	// if entry not found
					cout << "Failed to delete " << p;	// don't delete
				if (xy[0] > -1)		// if entry found
				{	// delete entry function
					pokemon.deleteEntry(xy[0], xy[1]);
					cout << "Deleted " << p;		// output msg
				}
				cout << " In the pokemon table\n";	// output msg
			}
			// check command for gyms string
			// if gyms string exists run command
			// for gyms table
			if (i.find("gyms") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = gyms.find(i, "");
				if (x == -1)		// if entry not found
					cout << "Failed to delete " << p;	// don't delete
				if (x > -1)			// if entry found
				{	// delete entry function
					gyms.deleteEntry(x);
					cout << "Deleted " << p;		// output msg
				}
				cout << " In the gyms table\n";		// output msg
			}
			// check command for trainers string
			// if trainers string exists run command
			// for trainers table
			if (i.find("trainers") != string::npos)
			{
				// call find function
				// returns int x for table key
				int x = trainers.find(i, "");
				if (x == -1)		// if entry not found
					cout << "Failed to delete " << p;	// don't delte
				if (x > -1)			// if entry found
				{	// delete entry function
					trainers.deleteEntry(x);
					cout << "Deleted " << p;		// output msg
				}
				cout << " In the trainers table\n";
			}
		}
		// check for write commands
		if (i == "WRITE()")
		{
			cout << "WRITE\n";	// output msg
			pokemon.write();	// write pokemon file
			trainers.write();	// write trainer file
			gyms.write();		// write gym file
			cout << "Processing complete!\n";	// output msg
		}
	}
	
	return 0;
}


/* function to parse data
for example given 
DELETE((1 | Pewter Gym | Pewter City | Brock | Boulder Badge), gyms)
it will remove all characters that are not part of the actual pokemon
data
It will change the given example to
1 | Pewter Gym | Pewter City | Brock | Boulder Badge */
string strip(string tuple)
{
	// remove all characters before ((
	string y = tuple.substr(tuple.find("(") + 1);
	string x = y.substr(y.find("(") + 1);
	stringstream ss(x);	// create stringstring to parse data

	// check for pokemon string
	if (x.find("pokemon") != string::npos)
	{
		// remove last part of string "),pokemon)"
		regex r("(\\),pokemon\\))+");
		string repl = "";
		string p = regex_replace(x, r, repl);
		// return stripped string
		return p;
	}

	// check for gyms string
	if (x.find("gyms") != string::npos)
	{
		// remove last part of string "),gyms)"
		regex r("(\\),gyms\\))+");
		string repl = "";
		string p = regex_replace(x, r, repl);
		// return stripped string
		return p;
	}

	// check for trainers string
	if (x.find("trainers") != string::npos)
	{
		// remove last part of string "),trainers)"
		regex r("(\\),trainers\\))+");
		string repl = "";
		string p = regex_replace(x, r, repl);
		// return stripped string
		return p;
	}
		
	// should not get here
	return "Error";
}

// function to read in gym file
// it then creates a new instance of gym
// class and sets up the hashtable
// the function returns the gym class object
gymTable gymRead(string fileString, int insertCount)
{
	ifstream fileName;			// fileName
	fileName.open(fileString);	// open file

	// check if file is open
	if (fileName.is_open())
	{
		string line;		// string to hold each line in file
		int lineNum = 0;	// var to count number of lines in file

		// get second line of file
		getline(fileName, line);
		getline(fileName, line);
		// count how many lines are in the file
		while (getline(fileName, line))
		{
			if (line != " ")	// don't count last line
			{
				lineNum++; // increment 
			}
		}

		// set bucketCount
		// add the number of insert commands so that 
		// there guranteed be enough buckets
		int bucketCount = ((int)(lineNum * 1.75)) + insertCount;

		gymTable gyms(bucketCount);	// setup gyms class
		
		// reset the file back to the start
		fileName.clear();				// clear eof			
		fileName.seekg(0, ios::beg);	// return to beginning of file

		// skip a line
		getline(fileName, line);
		string key = line;		// line string
		getline(fileName, line);	// skip a line
		// read the rest of the file line by line
		while (getline(fileName, line))
		{
			// don't read the final line that's empty
			if (line != " ")
			{
				stringstream ss(line);	// stringstream to parse line
				string gymID;			// string to hold gymID

				getline(ss, gymID, '|'); // parse to get gymID
				// convert gymId to int
				int ID = stoi(gymID);

				// hash function
				gyms.setdata(ID, line);
			}
		}
		return gyms;	// return gyms object
	}
	gymTable temp;	// 200iq play
	return temp;	// return temp to prevent warning
}

// function to read in trainer file
// it then creates a new instance of trainer
// class and sets up the hashtable
// the function returns the trainer class object
trainerTable trainersRead(string fileString, int insertCount)
{
	ifstream fileName;			// fileName
	fileName.open(fileString);	// open file
	
								// check if file is open
	if (fileName.is_open())
	{
		string line;			// string to hold each line in file
		int lineNum = 0;		// var to count number of lines in file

		// get second line of file
		getline(fileName, line);
		getline(fileName, line);
		// count how many lines are in the file
		while (getline(fileName, line))
		{
			if (line != " ")	// don't count last line
			{
				lineNum++;		// increment linenum
			}
		}

		// set bucketCount
		// add the number of insert commands so that 
		// there guranteed be enough buckets
		int bucketCount = ((int)(lineNum * 1.75)) + insertCount;

		trainerTable trainers(bucketCount); // setup trainers class

		// reset the file back to the start
		fileName.clear();				// clear eof			
		fileName.seekg(0, ios::beg);	// return to beginning of file
		
		// skip a line
		getline(fileName, line);
		string key = line;		// line string
		getline(fileName, line);	// skip a line
		// read the rest of the file line by line

		while (getline(fileName, line))
		{
			// don't read the final line that's empty
			if (line != " ")
			{
				stringstream ss(line);			// stringstream to parse line
				string firstname, lastname, data;	// strings to hold data

				getline(ss, firstname, '|');	// parse line to get names
				getline(ss, lastname, '|');

				// hash function
				trainers.setData(firstname, lastname, line);
			}
		}
		return trainers;	// return trainers object
	}
	trainerTable temp;	// return temp to prevent warning
	return temp;
}

// function to read in pokemon file
// it then creates a new instance of pokemon
// class and sets up the hashtable
// the function returns the pokemon class object
pokemonTable pokemonRead(string fileString, int insertCount)
{
	ifstream fileName;	// fileName
	fileName.open(fileString);	// open file

	// check if file is open
	if (fileName.is_open())
	{
		string line;	// string to hold each line in file
		int lineNum = 0;	// var to count number of lines in file

		// get second line of file
		getline(fileName, line);
		getline(fileName, line);
		// count how many lines are in the file
		while (getline(fileName, line))
		{
			if (line != " ")	// don't count last line
			{
				lineNum++;	// increment 
			}
		}

		// set bucketCount
		// add the number of insert commands so that 
		// there guranteed be enough buckets
		int bucketCount = ((int)(lineNum * 1.75)) + insertCount;
		
		pokemonTable pokemon(bucketCount);	// setup pokemon class

		// reset the file back to the start
		fileName.clear();				// clear eof			
		fileName.seekg(0, ios::beg);	// return to beginning of file

		// skip a line
		getline(fileName, line);
		string key = line;		// line string
		getline(fileName, line);	// skip a line
		// read the rest of the file line by line
		while (getline(fileName, line))
		{
			// don't read the final line that's empty
			if (line != " ")
			{
				stringstream ss(line);	// stringstream to parse line
				string ID;	// string to hold gymID

				getline(ss, ID, '|');		// get ID
				// convert gymId to int
				int id = stoi(ID);

				// hash function
				pokemon.setData(id, line);
				
			}
		}
		return pokemon;	// return gyms object
	}
	pokemonTable temp;
	return temp;	// return temp to prevent warning
}
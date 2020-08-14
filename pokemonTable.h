#include <string>
#include <vector>
#include <tuple>

#pragma once

// hash table class using chain strategy
// this class holds the pokemon table
class pokemonTable
{
private:
	int bucket;	// bucket count

	// struct to hold pokemon data and ID
	struct pokemon
	{
		int ID;	// pokemon ID
		std::string data;	// pokemon data
	};
	
	std::vector<std::vector<pokemon>> table;	// 2d vector to hold pokemon structs

public:
	pokemonTable();	// default constructor
	pokemonTable(int key);	// constructor

	// setter function
	void setData(int ID, std::string data);

	// functions (explained in .cpp)
	void insert(std::string tuple, std::string tableName);
	void display();
	void update(int x, int y, std::string data);
	std::vector<int> find(std::string tuple, std::string tableName);
	void deleteEntry(int inner, int outer);
	void write();

	// hash function
	int hash(int ID);
};
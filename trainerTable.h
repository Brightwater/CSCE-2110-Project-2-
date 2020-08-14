#include <string>
#include <vector>

#pragma once

// struct to hold trainer data
struct trainer
{
	int key;	// hash key
	std::string data;	// full entry data
	std::string firstname;	// trainer firstname
	std::string lastname;	// lastname firstname
};

// hash table class using addititve string hash and linear probing
class trainerTable
{
private:
	std::vector <trainer> trainers;	// vector of trainers to hold data
	int bucket;	// number of buckets
	
public:
	trainerTable();	// default constructor
	trainerTable(int size); // constructor

	// function declarations
	void setData(std::string firstname, std::string lastname, std::string data);
	
	// operation functions (explained in .cpp)
	void insert(std::string tuple, std::string tableName);
	void display();
	void update(int x, std::string data);
	int find(std::string tuple, std::string tableName);
	void deleteEntry(int x);
	void write();

	// hash function
	int hash(std::string firstname, std::string lastname);

};
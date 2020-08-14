#include <string>
#include <vector>

#pragma once

// struct to hold trainer data
struct gym
{
	int gymID;	// ID of gym
	std::string data;	// full entry data
};

// hash table class using double hashing
class gymTable
{
private:
	std::vector <gym> gyms; // vector of gyms to hold data
	int bucket;	// number of buckets

public:
	gymTable();	// default constructor
	gymTable(int size);	// constructor

	// setter function
	void setdata(int ID, std::string data);
	
	// function declarations (explained in .cpp)
	void insert(std::string tuple, std::string tableName);
	void display();
	void update(int x, std::string tuple);
	int find(std::string tuple, std::string tableName);
	void deleteEntry(int x);
	void write();
	int getPrime(int ID);
	bool isPrime(int num);

	// hash function
	int hash(int ID);
};

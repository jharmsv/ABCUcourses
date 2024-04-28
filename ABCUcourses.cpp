//============================================================================
// Name        : ABCUcourses.cpp
// Author      : Jared Harms
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <cctype>
using namespace std;

string csvPath = "";


// define a structure to hold course information
struct Course {
	string courseNumber;
	string courseName;
	string prerequisites;
	vector<string> prerequisitesVec;
};

// internal structure for the tree node
struct Node {
	Course course;
	Node *left;
	Node *right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseNumber);
};

BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

void BinarySearchTree::InOrder() {
	inOrder(root);
}

// insert node into tree structure using addNode
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course);
	} else {
		addNode(root, course);
	}
}

// search for given node using courseNumber, return course
Course BinarySearchTree::Search(string courseNumber) {
	Node *cur = root;

	while (cur != nullptr) {
		if (cur->course.courseNumber.compare(courseNumber) == 0) {
			return cur->course;
		} else if (courseNumber.compare(cur->course.courseNumber) < 0) {
			cur = cur->left;
		} else {
			cur = cur->right;
		}
	}
	Course course;
	return course;
}

// add node to tree structure recursively
void BinarySearchTree::addNode(Node* node, Course course) {
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		} else {
			addNode(node->left, course);
		}
	} else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		} else {
			addNode(node->right, course);
		}
	}
}

// traverse tree in order, outputting formatted course information
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
}

// parse courses information from csv file
void loadCourses(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	fstream userFile;

	userFile.open(csvPath);

	for (std::string line; getline(userFile, line); ) {

		stringstream ss;
		Course course;

		// get size of each line
		int lineSize = 0;
		try {
			while (line.at(lineSize)) {
				lineSize++;
			}
		} catch (exception e){
			string nothing = "do nothing";
		}

		// determine each property for course structure and create course using these properties
		string lineString = line.substr(0, lineSize);

		string courseNumber = lineString.substr(0,7);
		course.courseNumber = courseNumber;

		unsigned int firstCommaIdx = 7;

		string restOfLine = lineString.substr(firstCommaIdx + 1, lineSize - firstCommaIdx);

		unsigned int secondCommaIdx = restOfLine.find(',');

		string courseName;
		string prerequisites;

		if (secondCommaIdx == string::npos) {
			courseName = restOfLine;
			course.courseName = courseName;
			course.prerequisites = "non";
		} else {
			courseName = restOfLine.substr(0, secondCommaIdx);
			course.courseName = courseName;
			prerequisites = lineString.substr(firstCommaIdx + secondCommaIdx + 2, lineSize - firstCommaIdx - secondCommaIdx);
			course.prerequisites = prerequisites;
		}

		// insert course into tree
		bst->Insert(course);
	}

	cout << "\n";
	userFile.close();
}

// check if course is in courseNumbers vector
bool locateCourse(string courseNumber, vector<string> courseNumbers) {
	for (unsigned int idx = 0; idx < courseNumbers.size(); idx++) {
		if (courseNumbers.at(idx) == courseNumber) {
			return true;
		}
	}
	return false;
}

int main() {

	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	// get file name from user input and open file
	cout << "beep boop" << endl;
	cout << "please enter the file name" << endl;
	cin >> csvPath;

	fstream userFile;

	userFile.open(csvPath);

	// check if user specified file is opened or does not exist
	if (!userFile) {
		cout << "this file does not exist, goodbye" << endl;
		return 0;
	} else {
		cout << "found it...checking file format" << endl;
	}

	// Iterate through each line in file and create courseNumbers vector to hold each courseNumber
	vector<string> courseNumbers;
	for (std::string line; getline(userFile, line); ) {
		stringstream ss;
		int idx = 0;
		while (line.at(idx) != ',') {
			if (idx > 8) {
				cout << "error in parsing file -- courseNumber not valid" << endl;
			}
			ss << line.at(idx);
			idx++;
		}
		string courseNumber = ss.str();
		courseNumbers.push_back(courseNumber);
	}

	cout << "...looks good\n" << endl;

	userFile.close();

	// display menu
	int choice = 1;
	while ((choice == 1) || (choice == 2) || (choice == 3)) {
		cout << "Welcome to the course planner.\n" << endl;
		cout << " 1. Load Courses" << endl;
		cout << " 2. Display All Courses" << endl;
		cout << " 3. Find and Display Course" << endl;
		cout << " Enter Any Other Choice To Exit\n" << endl;
		cout << "What would you like to do? ";
		cin >> choice;

		switch(choice) {

		case 1:
			// load courses
			loadCourses(csvPath, bst);
			cout << "Courses successfully loaded\n" << endl;
			break;

		case 2:
			// display all courses
			bst->InOrder();
			break;

		case 3:
			// find and display course
			cout << "What course do you want to know about? " << endl;
			string courseNumber;
			cin >> courseNumber;
			bool exists = locateCourse(courseNumber, courseNumbers);

			if (exists == false) {
				cout << "This course does not exist.\n" << endl;
			} else {
				course = bst->Search(courseNumber);


				cout << course.courseNumber;
				cout << ", " << course.courseName << endl;
				cout << "Prerequisites: ";
				cout << course.prerequisites << "\n" << endl;
			}
			break;

		}
	}

	cout << "Thank you for using the course planner. See you again, soon!" << endl;
	return 0;

}

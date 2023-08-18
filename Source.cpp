using namespace std;
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>

class Course {
public:
    string courseNumber; // or courseId
    string courseName;   // Spelled out name
    vector<string> coursePrereqs; // vector with all of the prerequites
    // Constructors and methods here
    Course() = default;
    Course(string &courseNumber, string &courseName, vector<string> &coursePrereqs);
    string getCourseNumber();
    string getCourseName();
    vector<string> getPrereqs();
};

struct Node {
    Course course;
    Node *left;
    Node *right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

class CoursesBST {
private: 
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void CoursesBSTDestructor(Node* node);
    Node* removeNode(Node* node, string courseNumber);

public:
    CoursesBST();
    virtual ~CoursesBST();
    void InOrder();
    void Remove(string courseNumber);
    void Insert(Course course);
    Course Search(string courseNumber);
};

//============================================================//
//==================    Courses Methods     ==================//
//============================================================//

Course::Course(string& courseNumber, string& courseName, vector<string>& prerequisites) {
    this->courseNumber = courseNumber;
    this->courseName = courseName;
    this->coursePrereqs = prerequisites;
}
string Course::getCourseNumber() {
    return this->courseNumber;
}
string Course::getCourseName() {
    return this->courseName;
}
vector<string> Course::getPrereqs() {
    return this->coursePrereqs;
}

//============================================================//
//===================     BST Methods     ====================//
//============================================================//

CoursesBST::CoursesBST() {
    root = nullptr;
}

CoursesBST::~CoursesBST() {
    CoursesBSTDestructor(root);

}

void CoursesBST::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}

void CoursesBST::Remove(string courseNumber) {
    this->removeNode(root, courseNumber);
}

void CoursesBST::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

Course CoursesBST::Search(string courseNumber) {
    // set current node equal to root
    // Transform the input to match uppercase
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    Node *cur = root;
    while (cur != nullptr) {
        if (cur->course.getCourseNumber().compare(courseNumber) == 0) {   // Checks on match to the current courseNumber
            return cur->course;
        }
        else if (cur->course.getCourseNumber().compare(courseNumber) > 0) { // Checks if courseNumber is MORE than current node
            cur = cur->left;
        }
        else {                                                              // Only option remaining, so current node goes to the right
            cur = cur->right;
        }
    }
    // Get the course information and return it.
    Course course;
    return course;
}

void CoursesBST::addNode(Node* node, Course course) {
    // if node is not null and smaller then move to left
    if (node != nullptr && (node->course.getCourseNumber().compare(course.getCourseNumber()) > 0)) {
        // Check if null
        if (node->left == nullptr) {
            // If null then becomes new node
            node->left = new Node(course);
            return;
        }
        // recurse down the left node and check again
        else {
            this->addNode(node->left, course);
        }
    }
    // this will go to the right
    else if (node != nullptr && (node->course.getCourseNumber().compare(course.getCourseNumber()) < 0)) {
        // check if right node is null
        if (node->right == nullptr) {
            // set to new node
            node->right = new Node(course);
            return;
        }
        // if node not null, go to the right
        else {
            // recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

void CoursesBST::inOrder(Node* node) {
    if (node != nullptr) {
        // The furthest left is the lowest alphanumeric. So they print
        // Off from there recursively
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseName << endl;
        inOrder(node->right);
    }
}

Node* CoursesBST::removeNode(Node* node, string courseNumber) {

    if (node == nullptr) {                                              // Checks if Root is empty. If it is, means the whole tree is empty, return nullptr
        return node;
    }
    if (courseNumber.compare(node->course.courseNumber) < 0) {           // The courseNumber is less than in the current node's courseNumber
        node->left = removeNode(node->left, courseNumber);              // Move the node to the left, call removeNode again.
    }
    else if (courseNumber.compare(node->course.courseNumber) > 0) {
        node->right = removeNode(node->right, courseNumber);            // Move the node to the right and call recursively
    }
    else {                                                              // Case where node must be removed
        if (node->left == nullptr && node->right == nullptr) {
            delete node;                                                // Delete the node and set the node to null
            node = nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;                                           // Make a temp node to store current node
            node = node->right;                                          // point the node at the right node
            delete temp;                                                 // delete our temp node
        }
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else {                                                            // has 2 children
            Node* successor = node->right;                                // go right once, then check the leftmost child
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            node->course = successor->course;                               // copies the new info so we can move it.
            node->right = removeNode(node->right, successor->course.courseNumber);
        }
    }
    return node;
}

void CoursesBST::CoursesBSTDestructor(Node* node) {
    // Check not null
    if (node != nullptr) {
        // Call destructor on all of the right, then all of the left
        CoursesBSTDestructor(node->right);
        CoursesBSTDestructor(node->left);
        delete node;
        node = nullptr;
    }
}

//============================================================//
//=============     Load and Display Courses     =============//
//============================================================//

void displayCourse(Course course) {
    // Takes in the "course" from search
    // Make a local var of the prereqs.
    // prerequisites will be appended to contain the vector prereqs in string form
    vector<string> coursePrereq = course.getPrereqs();
    string prerequisites;
    // If there are prereqs, then for loop to extract else just say "none"
    if (coursePrereq.size() >= 1) {
        for (int i = 0; i < coursePrereq.size() - 1; ++i) {
            prerequisites += coursePrereq[i] + ", ";
        }
        prerequisites += coursePrereq.back();
    }
    else {
        prerequisites = "none";
    }
    cout << course.getCourseNumber() << ", " << course.getCourseName() << endl <<
        "Prerequisites: " << prerequisites << endl;
    return;
}

void loadCourses(const string& inputFilePath, CoursesBST* coursesBST) {
    cout << "Loading input file " << inputFilePath << endl;
    // Initialize file input for stream
    ifstream file(inputFilePath);
    if (file) // Check if the file is empty.
    {
        file.peek();
        if (file.eof())
        {
            cout << "file opened but it is empty or invalid content" << endl;
        }
    }
    string currentLine;
    // First item of the line will be the courseNumber
    // Second item will be the courseName
    // Third or more will be the Prerequisites.    
    try {
        while (getline(file, currentLine)) {
            stringstream ss(currentLine);
            string word;            // Placekeeper
            string courseNumber;
            string courseName;
            vector<string> coursePrereqs;
            int index = 0;
            while (!ss.eof()) {
                getline(ss, word, ',');
                // Regex will change every line break to nothing.
                word = regex_replace(word, regex(R"(\r|\n|\r\n)"), "");
                //Before the first comma
                if (index == 0) {
                    courseNumber = word;
                }
                // Before the 2nd comma
                else if (index == 1) {
                    courseName = word;
                }  
                // If there are more after the 2nd comma appended to coursePrereqs
                else {                                                          
                    coursePrereqs.push_back(word); // Append to the vector coursePrereqs
                }
                ++index;                            // Increment index.
            }
            // Make new Course objects. Then insert into our BST
            Course course = Course(courseNumber, courseName, coursePrereqs);
            coursesBST->Insert(course);
        }
    }
    // If is an error, throw failure message
    catch (ifstream::failure& e) {
        cerr << e.what() << std::endl;
    }
    // Close the file at the end.
    file.close();
}

int main() {
    // Create new BST 
    CoursesBST* coursesBST;
    coursesBST = new CoursesBST();
    Course course;
    string inputFilePath;
    string courseKey;

        int choice = 0;
        cout << "Welcome to the course planner." << endl;
        while (choice != 9) {
            cout << endl;
            cout << "  1. Load Data Structure" << endl;
            cout << "  2. Print Course List." << endl;
            cout << "  3. Print Course" << endl;
            cout << "  9. Exit" << endl;
            cout << endl;
            cout << "What would you like to do? ";
            cin >> choice;

            switch (choice) {
            case 1:
                // Loading data structure. Requires correct full file name.
                cout << "Enter the course file name ";
                cin >> inputFilePath;
                cout << endl;
                loadCourses(inputFilePath, coursesBST);
                break;

            case 2:
                // Prints everything inOrder from the BST
                cout << "Here is a sample schedule:" << endl;
                cout << endl;
                coursesBST->InOrder();
                break;

            case 3:
                // Prompts for course
                cout << "What course do you want to know about? ";
                cin >> courseKey;
                // Calls Search, will print if found.
                course = coursesBST->Search(courseKey);
                // If search finds the "course" then will send to displayCourse
                if (!course.courseNumber.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "course Number " << courseKey << " not found." << endl;
                }
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                // Error check, will say "not valid" then clear cin.
                // Then back to waiting for input
                cout << choice << " is not a valid option." << endl;
                cin.clear();
                cin.ignore();
            }
        }

        cout << "Thank you for using the course planner!" << endl;

        return 0;
    }

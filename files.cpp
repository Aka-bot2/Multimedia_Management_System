#include<iostream>
#include<fstream>
#include<sstream>
#include <vector>
#include <limits>
using namespace std;

class DepartmentRepository{
    int Total_storage,Used_storage;
    public:
        void View_Events(int choice)
        {
            fstream fin("release.csv", ios::in); 
            //creates file stream object named "fin", 
            // open "release.csv" in read mode, 
            // and then input mode

            if (!fin) //check if file opened
            {
                cout << "Error opening file.\n";
                return;
            }

            string line; 
            //stores each line from file
            bool found = false;
            //tracks whether selected event was found

            while (getline(fin, line)) //reads one line at a time from CSV
            {
                stringstream ss(line); 
                //converts line into stream object 
                vector<string> row;
                //stores each row of current line
                string word;
                //temp string for each column

                while (getline(ss, word, ',')) //read until comma, stores each part into vector
                {
                    row.push_back(word);
                }

                if (row.size() < 2) //minimum two rows should be there
                    continue;

                int eventID = stoi(row[0]); //stoi - convet string to integer
                string eventName = row[1];
                int fileCount = row.size() - 2;

                // CASE 1 → Just display all events
                if (choice == 0)
                {
                    cout << eventID << ") "
                        << eventName
                        << " (" << fileCount << " files)"
                        << endl;
                    found=true;
                }

                // CASE 2 → Display files of selected event
                else if (eventID == choice)
                {
                    found = true;

                    cout << "\nEvent: " << eventName << endl;
                    cout << "Files:\n";

                    for (int i = 2; i < row.size(); i++)
                    {
                        cout << i - 1 << ") " << row[i] << endl;
                    }

                    cout << endl;
                    break;
                }
            }

            if (choice != 0 && !found)
            {
                cout << "Event not found.\n";
                cout << "Select Event (0 to go back): ";
                cin >> choice;

                View_Events(choice);
            }

            fin.close();
        }
        
};


class UserStorage{
    int User_total_storage,User_storage_used;
    public:
        void List_files(long long owner_id)
            {
             int row_num=-1;
             ifstream inFile("userfiles.csv",ios::in);//opening a file in read only mode
                vector<vector<string>> data;//this is equivalent to a 2d array in c which can store strings
                string line, word;//temporary variables

                // Read file
                while (getline(inFile, line))//while loop runs till end of file which will be indicated by a '/0'
                 {
                    stringstream s(line);//converts line to a stream because getline only works with streams not strings
                    vector<string> row;//an list/array of strings

                    while (getline(s, word, ',')) //used to seperate a words in a string when a , is met, then the all the characters before the , is a word
                    {
                        row.push_back(word);//adds the word to the row vector
                    }

                    data.push_back(row);//adds the row to 2d list/array 
                }

                inFile.close();//close the read only file

             for (int i = 0; i < (int)data.size(); i++)
                if (!data[i].empty() && stoll(data[i][0]) == owner_id)//stoll is converting from string to long long (int) and checking if that row has the reg no
                    row_num=i;
                if (row_num == -1 || (int)data[row_num].size() <= 1)//if it is not found or if no pictures are there
                    {cout << "You have no copied files.\n";
                     return;
                    }
                cout<<"Your copied file are:"<<endl;
                for (int j = 1; j < (int)data[row_num].size(); j++)
                if (!data[row_num][j].empty()) {
                cout << "  " << j << ") " << data[row_num][j] << "\n";//prints all the data in a row i.e all the files saved by an user
             }

            }
                    
            void Delete_files(long long owner_id)
                {
                  int row_num=-1;
                ifstream inFile("userfiles.csv");//opening a file in read only mode
                vector<vector<string>> data;//this is equivalent to a 2d array in c which can store strings
                string line, word;//temporary variables

                // Read file
                while (getline(inFile, line)) //while loop runs till end of file which will be indicated by a '/0'
                {
                    stringstream s(line);//converts line to a stream because getline only works with streams not strings
                    vector<string> row;//an list/array of strings


                    while (getline(s, word, ','))//used to seperate a words in a string when a , is met, then the all the characters before the , is a word
                     {
                        row.push_back(word);//adds the word to the row vector
                    }

                    data.push_back(row);//adds the row to 2d list/array 
                }

                inFile.close();//close the read only file

                
                 List_files(owner_id);
                
                
                int x=1; 
                
                while (1) {
                    for (int i = 0; i < (int)data.size(); i++)
                    if (!data[i].empty() && stoll(data[i][0]) == owner_id)//stoll is converting from string to long long (int) and checking if that row has the reg no
                        row_num=i;  
                    if (row_num == -1 || (int)data[row_num].size() <= 1)//if row doesnt exist 
                        {cout << "You have no copied files to delete.\n";
                        return;
                        }
                    cout << "Enter file number to delete (0 to cancel): ";
                    cin >> x;
                    if(x==0)
                        return;
                    if (row_num < 0 || row_num >= (int)data.size()) {
                        cout << "Invalid row.\n"; 
                        return;
                    }
                    if (x < 1 || x >= (int)data[row_num].size() || data[row_num][x].empty()) {
                        cout << "Invalid number. Try again: ";
                        continue; // prompt again instead of returning
                    }
                    cout << "'" << data[row_num][x] << "' removed from your repository.\n";
                    data[row_num].erase(data[row_num].begin() + x);

                    if ((int)data[row_num].size() <= 1) 
                        {
                         cout << "You have no copied files to delete.\n";
                         break; // exit if row is empty
                        }

                    
                    ofstream outFile("userfiles.csv");//opening a write file

                    for (int i = 0; i < data.size(); i++) {//loop to add values from data vector to the file

                        for (int j = 0; j < data[i].size(); j++) {

                            outFile << data[i][j];//overwriting into the file

                            if (j != data[i].size() - 1)
                                outFile << ",";//adding , till the last data in the row
                        }

                        outFile << endl;//adding an \n to indicate eof
                    }

                    outFile.close();//closing file
                

                    List_files(owner_id);
                }
            }   
            void Check_storage(long long owner_id)
                {
                int row_num=-1;
                ifstream inFile("userfiles.csv",ios::in);//opening file in read only mode
                vector<vector<string>> data;//2d array of vectors
                string line, word;

                // Read file
                while (getline(inFile, line)) {//loops till eof
                    stringstream s(line);//converting string to istream
                    vector<string> row;//list of strings

                    while (getline(s, word, ',')) {//getting each word/value from the row
                        row.push_back(word);//adding the word to the row
                    }

                    data.push_back(row);//adding row to the 2d list/array
                }

                inFile.close();//closing the read only file

                for (int i = 0; i < (int)data.size(); i++)//looping through to check the no of files
                    if (!data[i].empty() && stoll(data[i][0]) == owner_id)//checking if that record belongs to the current user
                        row_num=i;//storing the row which has current user information
                int count=0;
                for (int j = 1; j < (int)data[row_num].size(); j++)//looping through that row of the current user
                    if (!data[row_num][j].empty()) {//if that 
                    count++;
                    }
                cout<<"You have used "<<count<<"/20 pictures."<<endl;
                }

};

class Equipment{
    int Equipment_ID;
    string Name, Available,Condition;
    float Cost;

    public:
        void Is_available()
            {
                
            }
        int Current_cost()
            {
            
            }
};

class EquipmentIssue{ //this would be a friend class with equipment class and the member class
    string Issue_date,Return_date;
    public:
        void Show_issues()
            {

            }
        void Return_equipment()
            {

            }
        void Issue_duration()
            {

            }
        
};

class Project {
    friend class Admin;//friend class

    private:
        void Show_members()
            {
             ifstream inFile("projects.csv");//opening a file in read only mode
                vector<vector<string>> data;//this is equivalent to a 2d array in c which can store strings
                string line, word;//temporary variables

                // Read file
                while (getline(inFile, line)) //while loop runs till end of file which will be indicated by a '/0'
                {
                    stringstream s(line);//converts line to a stream because getline only works with streams not strings
                    vector<string> row;//an list/array of strings


                    while (getline(s, word, ','))//used to seperate a words in a string when a , is met, then the all the characters before the , is a word
                     {
                        row.push_back(word);//adds the word to the row vector
                    }

                    data.push_back(row);//adds the row to 2d list/array 
                }

                inFile.close();//close the read only file
                for (int i=0;i<(int)data.size(); i++)
                    {
                        cout<<i+1<<") "<<data[i][1]<<endl;
                    }//printing all the project names
                int choice=0;
                cout<<"Enter the project number you have to list member from: ";
                cin>>choice;
                int row_num=-1;
                for (int i = 0; i < (int)data.size(); i++)
                    if (!data[i].empty() && stoll(data[i][0]) == choice)//stoll is converting from string to long long (int) and checking if that row has the reg no
                        row_num=i;  
                if (row_num == -1 || (int)data[row_num].size() <= 2)//if row doesnt exist or no members in proj
                    {cout << "No members in project.\n";
                     return;
                    }
                for (int i=0;i<(int)data[row_num].size()-2; i++)
                    {
                        cout<<i+1<<") "<<data[row_num][i+2]<<endl;
                    }
            }
        void Add_member() {
            ifstream inFile("projects.csv");
            if(!inFile)
                {
                 cout<<"File cannot be accessed"<<endl;
                 return;
                }
                vector<vector<string>> data;
                string line, word;

                // Read file and prints events
                while (getline(inFile, line)) {
                    stringstream s(line);
                    vector<string> row;

                    while (getline(s, word, ',')) {
                        row.push_back(word);
                    }
                    data.push_back(row);
                }
                //cout<<"done reading";
                int choice=0,found=0;
                for (int i=0;i<(int)data.size(); i++)
                    {
                        cout<<i+1<<") "<<data[i][1]<<endl;
                    }//printing all the project names
                inFile.close();
                cout<<"Which project do you want to a member in: ";
                cin>>choice;
                if (choice==0)
                    return;
                string name;
                //cout<<Reg_No;
                vector<string> row;
                cout<<"Enter name of the project member to add (DONT KEEP SPACES): ";
                cin>>name;
                for (int i = 0; i < data.size(); i++) {
                    if (stoll(data[i][0]) == choice) {
                        data[i].push_back(name);
                        found=1;
                        break;
                    }
                }
                

                // Rewrite file
                ofstream outFile("projects.csv");

                for (int i = 0; i < data.size(); i++) {

                    for (int j = 0; j < data[i].size(); j++) {

                        outFile << data[i][j];

                        if (j != data[i].size() - 1)
                            outFile << ",";
                    }

                    outFile << endl;
                }

                outFile.close();
                if(found==1)
                    {
                     cout<<"Member has been added successfully!!!"<<endl;
                    }
                else
                    cout<<"Invalid choice"<<endl;
        }
        void Remove_member() {
            
                ifstream inFile("projects.csv");//opening a file in read only mode
                vector<vector<string>> data;//this is equivalent to a 2d array in c which can store strings
                string line, word;//temporary variables

                // Read file
                while (getline(inFile, line)) //while loop runs till end of file which will be indicated by a '/0'
                {
                    stringstream s(line);//converts line to a stream because getline only works with streams not strings
                    vector<string> row;//an list/array of strings


                    while (getline(s, word, ','))//used to seperate a words in a string when a , is met, then the all the characters before the , is a word
                     {
                        row.push_back(word);//adds the word to the row vector
                    }

                    data.push_back(row);//adds the row to 2d list/array 
                }

                inFile.close();//close the read only file
                for (int i=0;i<(int)data.size(); i++)
                    {
                        cout<<i+1<<") "<<data[i][1]<<endl;
                    }//printing all the project names
                int choice=0;
                cout<<"Enter the project number you have to remove member from: ";
                cin>>choice;
                int row_num=-1;
                for (int i = 0; i < (int)data.size(); i++)
                    if (!data[i].empty() && stoll(data[i][0]) == choice)//stoll is converting from string to long long (int) and checking if that row has the reg no
                        row_num=i;  
                if (row_num == -1 || (int)data[row_num].size() <= 2)//if row doesnt exist or no members in proj
                    {cout << "No members in project to remove.\n";
                     return;
                    }
                for (int i=0;i<(int)data[row_num].size()-2; i++)
                    {
                        cout<<i+1<<") "<<data[row_num][i+2]<<endl;
                    }
                cout << "Enter member number to remove (0 to cancel): ";
                int x; 
                cin >> x;
                if (x == 0) return;
                if (x < 1 || x >= (int)data[row_num].size() || data[row_num][x].empty()) {
                    cout << "Invalid number.\n"; return;
                }
                cout << "'" << data[row_num][x+1] << "' removed.\n";
                data[row_num].erase(data[row_num].begin() + (x+1));// .begin sets a pointer at the starting index of the row and we are doing + x to go to the index in which the file to be deleted is located
                //.erase deletes the data and shifts the data to the left (to avoid blank spaces)

                ofstream outFile("projects.csv");

                for (int i = 0; i < data.size(); i++) {

                    for (int j = 0; j < data[i].size(); j++) {

                        outFile << data[i][j];

                        if (j != data[i].size() - 1)
                            outFile << ",";
                    }

                    outFile << endl;
                }

                outFile.close();
        }
};

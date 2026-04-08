#include<iostream> 
#include<fstream>//for file I/O
#include<sstream> //for coverting strings to istream, used when reading from files and storing it into a vector<string> or vector<vector<string>>
#include<vector> // for storing data from a file
#include<limits> //for clearing keyboard buffer, used in main function
#include<stdexcept>//for using standard exceptions like bad_alloc and runtime error
#include "files.cpp"//naive programming
using namespace std;
using namespace storage; // user defined namespace
// What we have used till now: Inheritance, Polymorphism(function overloading), abstract classes, virtual function, file handling, friend function/class, vector, composition, constructors, parameterized constructors, runtime polymorphism,operator overloading, naive programming , Namespaces

class User{ // Abstract Class and virtual function
    public:
        long long Reg_No;
        string Name,Department;
        DepartmentRepository d;
        UserStorage u;
        // User()
        //     {
        //      Reg_No=0;
        //      Name="";
        //      Department="";
        //     }
        User(long long id =0 , string n ="", string d= "" )
            {
             Reg_No=id;
             Name=n;
             Department=d;
            }
        static User* login();//static function
        static int count;
        // void Open_folder()
        //     {

        //     }
        void View_file()//lists the files in an event
            {   
                DepartmentRepository repo; // compostition

                repo.View_Events(0);

                int choice = 0;

                cout << "Select the Event: ";
                cin >> choice;

                repo.View_Events(choice);
            }
            
        void Copy_file()//there will be another file called userfiles.csv when he copies a valid file then it will be pasted next to his name
        {
            DepartmentRepository repo;

            //Step 1: Show the events
            repo.View_Events(0);

            int event_choice;
            cout << "Select Event: ";
            cin >> event_choice;

            //Step 2: Show files of the selected event
            repo.View_Events(event_choice);

            cout << "Enter file numbers you want to copy (0 to stop):\n";

            //Read userfiles.csv
            ifstream inFile("userfiles.csv");
            if(!inFile)
                throw runtime_error("Error: Cannot Open Userfiles.\n");
            vector<vector<string>> data;
            string line, word;

            while (getline(inFile, line))
            {
                stringstream s(line); // converting string to istream
                vector<string> row; // temp row vector

                while (getline(s, word, ','))//seperating words by , 
                {
                    row.push_back(word);//adding them to the vector
                }

                data.push_back(row);//adding them to the 2d vector
            }
            /*this upper loop will convert THIS :
            1, pic1, pic2
            2, pic1, pic2
            TO
            data[0] = ["1", "pic1", "pic2"]
            data[1] = ["2", "pic1", "pic2"]
            */


            inFile.close();//closing the read only file

            //Find user's row
            int row_num = -1;

            for (int i = 0; i < (int)data.size(); i++)//iterating through the 2d array
            {
                if (!data[i].empty() && stoll(data[i][0]) == Reg_No)//trying to find the row of the user
                {
                    row_num = i;
                    break;
                }
            }

            //Storage check
            //"data[row_num].size() - 1 >= 20" this "-1" beacuse index is from 0 to 19
            if ((int)data[row_num].size() - 1 >= 20) //here we caan change the file limit (now: 20)
            {
                cout << "Storage Full (20 files limit).\n";
                cout << "Please delete files first.\n";
                return;
            }

            //Read event file list
            ifstream eventFile("release.csv");//opening release.csv in read only mode.
            if(!eventFile)
                throw runtime_error("Error: Cannot Open Release.\n");
            vector<string> event_files;//vector to store all the files in an event

            while (getline(eventFile, line))//till eof take every line of release.csv as line
            {
                stringstream s(line);//converting line string to a istream
                vector<string> row;// to store data in one record

                while (getline(s, word, ','))//extracting data from s using , into word.
                    row.push_back(word);//adding that word to row vector

                if (!row.empty() && stoi(row[0]) == event_choice)//if desired eventid is the eventid of the current row
                {
                    for (int i = 2; i < (int)row.size(); i++)//i starts from 2 because the the 0 and 1st indices are not files of that event.
                        event_files.push_back(row[i]);//storing files of that event in the event_files vector.

                    break;//breaking after desired event is found
                }
            }

            eventFile.close();//closing the read only file

            //Copy loop
            while (true)//user can copy multiple files at once
            {
                int x;
                cin >> x;

                if (x == 0)//for user to stop copying
                    break;

                if (x < 1 || x > (int)event_files.size())//copying from event_files vector but 
                {
                    cout << "Invalid file number.\n";
                    continue;
                }

                if ((int)data[row_num].size() - 1 >= 20)//if user already has 20 files copied
                {
                    cout << "Storage limit reached.\n";
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clears the keyboard buffer

                    break;
                }

                data[row_num].push_back(event_files[x - 1]);//copying the file into user storage

                cout << "'" << event_files[x - 1] << "' copied.\n";
            }

            //Rewrite userfiles.csv
            ofstream outFile("userfiles.csv");//opening userfiles as a write file.
            if(!outFile)
                throw runtime_error("Error: Cannot Open Userfiles.\n");
            for (int i = 0; i < (int)data.size(); i++)//looping through all the vals in the 2d vector (data) to overwrite userfiles.
            {
                for (int j = 0; j < (int)data[i].size(); j++)
                {
                    outFile << data[i][j];

                    if (j != data[i].size() - 1)
                        outFile << ",";
                }

                outFile << endl;//adding endl at the eof to indicate eof
            }

            outFile.close();//closing userfiles
        }

        void change_password()
            {
                ifstream inFile("users.csv");//opening a file in read only mode
                vector<vector<string>> data;//this is equivalent to a 2d array in c which can store strings
                string line, word;//temporary variables

                // Read file
                while (getline(inFile, line)) {//reading the file until eof
                    stringstream s(line);//coverting the string to a stream, so that getline can access that particular line(row)
                    vector<string> row;// created to store multiple strings 

                    while (getline(s, word, ',')) {//used to seperate a words in a string when a , is met, then the all the characters before the , is a word
                        row.push_back(word);//adds the word to the row vector
                    }

                    data.push_back(row);//adding that row vector to the 2d vector
                }

                inFile.close();//closing the read only file

                string newpass;
                //cout<<Reg_No;
                cout<<"Enter new password: ";
                cin>>newpass;
                for (int i = 0; i < data.size(); i++) {//looping through the 2d array
                    if (stoll(data[i][0]) == Reg_No) {//checking if that current line's 0th index is the same as Reg_No
                        data[i][1] = newpass;//changing password in the 2d array.
                    }
                }

                // Rewrite file
                ofstream outFile("users.csv");//opening file as write file
                if(!outFile)
                throw runtime_error("Error: Cannot Open Users.\n");
                for (int i = 0; i < data.size(); i++) {

                    for (int j = 0; j < data[i].size(); j++) {

                        outFile << data[i][j];//storing(overwriting) data from 2d vector to the file(users.csv) 

                        if (j != data[i].size() - 1)//to make sure there is no , after the last data in the row
                            outFile << ",";
                    }

                    outFile << endl;//endl at the eof, to indicate no more data left
                }

                outFile.close();//closing write file.

                cout << "Password updated successfully.\n";

            }
        virtual void logout() =0;//user
        virtual void show_menu()
            {
             cout<<"********************************"<<endl;
             cout<<"1) Change Password"<<endl;
             cout<<"2) Logout"<<endl;
             cout<<"3) View Files"<<endl;
             cout<<"4) Copy File"<<endl;
             cout<<"5) List_User_Files"<<endl;
             cout<<"6) Delete_User_Files"<<endl;
             cout<<"7) Check_User_Storage"<<endl;
            }
        virtual void handleop(int op) //used to handle special operations if applicable
            {
             cout<<"Invalid option\n";
            }
        void List_User_Files()
            {
              u.List_files(Reg_No);
            }
        void Delete_User_Files()
            {
             u.Delete_files(Reg_No);
            }
        void Check_User_Storage()
            {
             u.Check_storage(Reg_No);
            }
        void menu()//takes menu option input from the user
            {
                int op=-1;

                while(op)
                    {
                     show_menu();
                     cout<<endl<<"What do you want to do? : ";
                     cin>>op;

                     switch (op)
                     {
                     case 1:
                        change_password();
                        break;
                     case 2:
                        logout();
                        op=0;
                        break;
                     case 3:
                        View_file();
                        break;
                     case 4:
                        Copy_file();
                        break;
                     case 5:
                        List_User_Files();
                        break;
                     case 6:
                        Delete_User_Files();
                        break;
                     case 7:
                        Check_User_Storage();
                        break;
                     default:
                        handleop(op);
                        break;
                     }   
                    }
            }
        virtual ~User() {}
};

int User:: count =0;

class NonMember: virtual public User{
    public:
        NonMember()
            {
             
            }
        NonMember(long long id, string n, string d)
            :User(id,n,d){}

        void show_menu()
            {
             User::show_menu();
             cout<<"********************************"<<endl;          
            }
        void handleop(int op) 
            {
             cout<<"Invalid option\n";
            }        
        void logout()//function overloading (polymorphism done)
            {
             cout<<"See you again."<<endl;
             cout<<"You logged out from a non member account with ID: "<<Reg_No<<endl;
            }
        
};

class Member: virtual public User{
    public:
        string joiningDate,role;
        EquipmentIssue e;
        Member()
            {}
        Member(long long id, string n, string d)
        : User(id,n,d) {}
        void Edit_file()
            {
             int choice1=0,choice2=0;
             d.View_Events(0);
             cout<<"Which event do you want to edit a file in : "<<endl;
             cin>>choice1;
             d.View_Events(choice1);
             cout<<"Which file do you want to edit : "<<endl;
             cin>>choice2;
             ifstream inFile("release.csv");
             if(!inFile)
                throw runtime_error("Error: Cannot Open Release.\n");
                vector<vector<string>> data;
                string line, word;

                // Read file
                while (getline(inFile, line)) {
                    stringstream s(line);
                    vector<string> row;

                    while (getline(s, word, ',')) {
                        row.push_back(word);
                    }

                    data.push_back(row);
                }

                inFile.close();

                string newname;
                //cout<<Reg_No;
                cout<<"Enter new name: ";
                cin>>newname;
                for (int i = 0; i < data.size(); i++) {
                    //cout << "Row " << i << " col[0] = " << data[i][0] << endl;
                    if (stoll(data[i][0]) == choice1) {
                        data[i][choice2+1]=newname;//to skip the first two cols, if you dont enter a correct pic id then anything may happen
                        break;
                    }
                }

                // Rewrite file
                ofstream outFile("release.csv");
                if(!outFile)
                throw runtime_error("Error: Cannot Open Userfiles.\n");
                for (int i = 0; i < data.size(); i++) {

                    for (int j = 0; j < data[i].size(); j++) {

                        outFile << data[i][j];

                        if (j != data[i].size() - 1)
                            outFile << ",";
                    }

                    outFile << endl;
                }

                outFile.close();

                cout << "File edited successfully.\n";
            }
        void Delete_file()
            {
                int row_num=-1;
                ifstream inFile("release.csv");//opening a file in read only mode
                if(!inFile)
                    throw runtime_error("Error: Cannot Open Userfiles.\n");
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
                d.View_Events(0);
                int choice=0;
                cout<<"Enter the event number you have to delete from: ";
                cin>>choice;
                for (int i = 0; i < (int)data.size(); i++)
                    if (!data[i].empty() && stoll(data[i][0]) == choice)//stoll is converting from string to long long (int) and checking if that row has the reg no
                        row_num=i;  
                if (row_num == -1 || (int)data[row_num].size() <= 1)//if row doesnt exist 
                    {cout << "No files in event to delete.\n";
                     return;
                    }
                d.View_Events(choice);
                cout << "Enter file number to delete (0 to cancel): ";
                int x; 
                cin >> x;
                if (x == 0) return;
                if (x < 1 || x >= (int)data[row_num].size() || data[row_num][x].empty()) {
                    cout << "Invalid number.\n"; return;
                }
                cout << "'" << data[row_num][x+1] << "' deleted.\n";
                data[row_num].erase(data[row_num].begin() + (x+1));// .begin sets a pointer at the starting index of the row and we are doing + x to go to the index in which the file to be deleted is located
                //.erase deletes the data and shifts the data to the left (to avoid blank spaces)

                ofstream outFile("release.csv");
                if(!outFile)
                    throw runtime_error("Error: Cannot Open Release.\n");
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
        void Add_event()
            {
             string name;
             fstream fout("release.csv",ios::app);
             cout<<"Enter the event name that you want to add: ";
             cin>>name;
             int newID = 1;
             ifstream inFile("release.csv");
             if(!inFile)
                throw runtime_error("Error: Cannot Open Release.\n");
             string line;

             while (getline(inFile, line)) {
                 if (!line.empty())
                    newID++;  // just count lines
                 }
             inFile.close();
             fout<<newID<<","<<name;
            }
        void Upload_file()
            {
             d.View_Events(0);
             ifstream inFile("release.csv");
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
                
                inFile.close();
                cout<<"Which event do you want to add files in: ";
                cin>>choice;
                if (choice==0)
                    return;
                string filename;
                //cout<<Reg_No;
                vector<string> row;
                cout<<"Enter name of the file to be added (DONT KEEP SPACES): ";
                cin>>filename;
                for (int i = 0; i < data.size(); i++) {
                    if (stoll(data[i][0]) == choice) {
                        data[i].push_back(filename);
                        found=1;
                        break;
                    }
                }
                

                // Rewrite file
                ofstream outFile("release.csv");
                if(!outFile)
                    throw runtime_error("Error: Cannot Open Release.\n");
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
                     cout<<"File has been added successfully!!!"<<endl;
                    }
                else
                    cout<<"Invalid choice"<<endl;
                    
            }
        void Show_Equipment() {
                if (equipmentList.empty()) {
                    cout << "No equipment available.\n";
                    return;
                }

                for (int i = 0; i < (int)equipmentList.size(); i++) {
                    cout << i + 1 << ") ";
                    equipmentList[i].display();
                }
            }
        void logout()  
            {
             cout<<"See you again."<<endl;
             cout<<"You logged out from a member account with ID: "<<Reg_No<<endl;
            }
        void show_menu()
            {
             User::show_menu();
             cout<<"8) Edit File"<<endl;
             cout<<"9) Delete File"<<endl;
             cout<<"10) Upload File"<<endl;
             cout<<"11) Add Event"<<endl;
             cout<<"12) Issue Equipment"<<endl;
             cout<<"13) Return Equipment"<<endl;
             cout<<"14) Issue Summary"<<endl;
             cout<<"15) Show equipments"<<endl;
             //cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             switch (op)
                     {
                     case 8:
                        Edit_file();
                        break;
                     case 9:
                        Delete_file();
                        break;
                     case 10:
                        Upload_file();
                        break;
                     case 11:
                        Add_event();
                        break;
                     case 12:
                        e.Issue_equipment();
                        break;
                     case 13:
                        e.Return_equipment();
                        break;
                     case 14:
                        e.show_summary();
                        break;
                     case 15:
                        Show_Equipment();
                        break;
                     }   
            }

};
class Studio: virtual public NonMember{//inherits non member
    string Role;
    int Experience;
    public:
        Studio()
            {}
        Studio(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as an Studio member."<<endl;
            }
        void show_menu()
            {
             User::show_menu();
             cout<<"8) Give Song"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             switch (op)
                     {
                     case 8:
                        Give_song();
                        break;
                     default:
                        cout<<"Enter a valid number"<<endl<<endl;
                        break;
                     }   
            }
        void Give_song()
            {
             cout<<"Song given."<<endl;
            }
        ~Studio()
            {}
};
class Artist: virtual public NonMember{//inherits non member
    int Card_experience;
    public:
        Artist()
            {
            }
        Artist(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as an Artist."<<endl;
            }
        void show_menu()
            {
             User::show_menu();
             cout<<"8) Give Design"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             switch (op)
                     {
                     case 8:
                        Give_Design();
                        break;
                     default:
                        cout<<"Enter a valid number"<<endl<<endl;
                        break;
                     }   
            }

        void Give_Design()
            {
             cout<<"Design given"<<endl;
            }

        // ~Artist()
        //     {}
};

class Other: public NonMember{//inherits non member
    public:
        Other()
            {
            }
        Other(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as a Non Member."<<endl;
            }
        void show_menu()
            {
             User::show_menu();
             cout<<"********************************"<<endl;          
            }
        void handleop(int op) 
            {
             cout<<"Invalid option\n";
            }  
        ~Other()
            {}
};

class Editor: virtual public Member,virtual public Studio{//inherits member
    public:
        Editor()
            {}
        Editor(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as an Editor."<<endl;
            }
        void Edit_Media()
            {

            }
        void show_menu()
            {
             Member::show_menu();
             cout<<"16) Make Premovie"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 16)
                Edit_Media();
             else
                Member::handleop(op);
            }
        ~Editor()
            {}
        void logout()
            {
             Member::logout();
            }
};

class Designer: virtual public Member, virtual public Artist{//inherits member and artist
    public:
        Designer()
            {}
        Designer(long long id, string n,string d)
            {
                Member::Reg_No=id;
                Member::Name=n;
                Member::Department=d;

                cout<<"Hi!! "<<Member::Name<<" from "<<Member::Department<<endl;
                cout<<"You logged in as an Designer."<<endl;
            }
        void Make_Poster()
            {

            }
        void show_menu()
            {
             Member::show_menu();
             cout<<"16) Make Poster"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 16)
                Make_Poster();
             else
                Member::handleop(op);
            }
        void logout()
            {
             Member::logout();
            }
};

class Admin: public Member{//inherits member
    public:
        Project proj;//composition
        Admin(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as an Admin."<<endl;
            }
        void Add_proj()
            {
             string name;
             fstream fout("projects.csv",ios::app);
             cout<<"Enter the project name that you want to add: ";
             cin>>name;
             int newID = 1;
             ifstream inFile("projects.csv");
             string line;

             while (getline(inFile, line)) {
                 if (!line.empty())
                    newID++;  // just count lines
                 }
             inFile.close();
             fout<<newID<<","<<name;
            }
        void Add_Equipment() {
            string name;
            float cost;

            int id = equipmentList.size() + 1; // auto ID

            cout << "Enter Equipment Name: ";
            cin >> name;

            cout << "Enter Cost per day: ";
            cin >> cost;

            equipmentList.push_back(Equipment(id, name, cost));
            Save_Equipment_To_File();
            cout << "Equipment added with ID: " << id << endl;
        }
        void show_menu()
            {
             Member::show_menu();
             cout<<"16) Add Project"<<endl;
             cout<<"17) Show Members"<<endl;
             cout<<"18) Add member"<<endl;
             cout<<"19) Remove member"<<endl;
             cout<<"20) Add Equipment"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 16)
                {
                 Add_proj();
                }
            else if(op==17)
                {
                 proj.Show_members();
                }
             else if(op==18)
                {
                 proj.Add_member();
                }
             else if(op==19)
                {
                 proj.Remove_member();
                }
             else if(op==20)
                {
                 Add_Equipment();
                }
             else
                Member::handleop(op);
            }
        ~Admin()
            {
             
            }
};

class Photographer: public Member{//inherits member
    public:
        Photographer(long long id, string n,string d)
            {
                Reg_No=id;
                Name=n;
                Department=d;

                cout<<"Hi!! "<<Name<<" from "<<Department<<endl;
                cout<<"You logged in as a Photographer."<<endl;
            }
        void Click_Pic()
            {
                cout<<"Picture Clicked!!!"<<endl;
            }  
        void show_menu()
            {
             Member::show_menu();
             cout<<"16) Click Picture"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 16)
                Click_Pic();
             else
                Member::handleop(op);
            }   
};

class GraphicEditor: public Designer, public Editor{//inherits designer and editor
    public:
        GraphicEditor(long long id, string n,string d)
            {
                Reg_No=id;
                Editor::Name=n;
                Editor::Department=d;

                cout<<"Hi!! "<<Editor::Name<<" from "<<Editor::Department<<endl;
                cout<<"You logged in as an Admin."<<endl;
            }
        void Make_Graphics()
            {

            }
        void logout() { Editor::logout(); }
        void show_menu() { Editor::show_menu(); }
        void handleop(int op) { Editor::handleop(op); }
}; 

//using a static function here because i need to call this function without creating an object of this class
User* User::login() { // definition of the static login function in the User class
    long long id,targetID,rcount=0,pecount=0;
    string password;
    // File object
    fstream fin;

    // Open an existing file in read only mode



    cout << "Enter ID: ";
    cin >> targetID;
    

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;
while(1){
    fin.open("users.csv", ios::in);
    if (!fin) {
    cout << "Error opening file.\n";
    return NULL;
    }
    while (getline(fin, line))
    {  
        row.clear();
        stringstream s(line);//makes s behave like istream, cuz getline only can work with streams.

        while (getline(s, word, ','))
        {
            row.push_back(word);// adds to the string vector row
        }

        if (row.size() < 5)//if the row is not complete then this ignores the row and goes to the next one.
            continue;

        id = stoll(row[0]);//coverts string in csv file to a long long so we can compare user entry and the file
        if (id == targetID)
        {
            rcount = 1;
            cout << "Enter password: ";
            cin >> password;

            while(password != row[1] && pecount < 3) // gives 4 tries to enter correct password
            {
                cout << "Wrong password. (" << 3-pecount << " tries left): ";
                cin >> password;
                pecount++;
            }

            if (pecount == 3)//if user takes more than 4 tries
            {
                cout << "Bye...\n";
                return NULL;
            }
            if (row[3] == "Artist") //did this to implement runtime polymorphism
                {
                User::count++;
                return new Artist(id,row[2],row[4]);
                
            }
            else if (row[3] == "Photographer")
                {
                User::count++;
                return new Photographer(id,row[2],row[4]);
    
            }
            else if (row[3] == "Editor")
                {
                User::count++;
                return new Editor(id,row[2],row[4]);
               
            }
            else if (row[3]=="Admin")
                {
                User::count++;
                return new Admin(id,row[2],row[4]);
               
            }
            else if (row[3]=="Studio")
                {
                User::count++;
                return new Studio(id,row[2],row[4]);
        
            }
            else if (row[3]=="Designer")
                {   User::count++;
                    return new Designer(id,row[2],row[4]);
      
                }
            else if (row[3]=="Graphic Editor")
                {
                User::count++;
                return new GraphicEditor(id,row[2],row[4]);
            }
            else
                {User::count++;
                 return new Other(id,row[2],row[4]);
                }
        }
     
    }
    cout << "Incorrect ID\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<"Enter ID: ";
    cin>>targetID;
    fin.close();//closing read only file
}
    
    return NULL;//returning NULL if user was not found

}

int main()
    {
     //cout<<"********************************"<<endl;
     while(1){
     cout<<"Do you want to login? (Y/N): ";


     char resp;
     cin>>resp;
     cin.ignore(numeric_limits<streamsize>::max(), '\n'); // makes sure it ignores whatever is there in the buffer till the newline char, clears the buffer.
     /*cin.ignore(): This is a member function of the std::istream class used to extract and discard characters from the input stream.
    std::numeric_limits<std::streamsize>::max(): This part specifies the maximum number of characters to ignore.
    std::streamsize is a type used to represent the size of a stream. 
    Using its maximum possible value effectively tells cin to ignore characters "without limit" (up to a very large, finite number),
    ensuring that all characters leading up to the delimiter are considered for discarding.
    '\n': This is the delimiter character. cin.ignore() will stop discarding characters once it encounters this character, 
    or when it reaches the specified character limit (which is essentially infinite), or the end of the file (EOF). The newline character itself is also discarded. */

     try{//using exception handling here.
     if(resp=='N' || resp=='n')
         throw 1;
     else if(resp=='y'||resp=='Y')
        throw 'a';
     else
        throw false;
     }
     catch(int a){
        cout<<"The user did not want to login"<<endl;
        cout<<User::count<<" users logged in !!!"<<endl;
        return 0;
     }
     catch(char e)
        {
         try{
            User* currentUser = User::login();//runtime polymorphism
        
            if(currentUser)
            {   Load_Equipment_From_File();
                currentUser->menu();
            }
        }
            catch (const bad_alloc& e) {//malloc error (wont happen mostly)
            cout << "Memory allocation failed: " << e.what() <<endl;
            }
            catch(const runtime_error &e){//this error is shown when the csv file cannot be accessed
                cout << "Login error: " << e.what() << "\n";
            }
        }
    catch(bool x)
        {
            cout<<"User did not enter Y/N, failed to launch program."<<endl;
            return 0;
        }
    }
    
    }
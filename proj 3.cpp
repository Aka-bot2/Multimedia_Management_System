#include<iostream>
#include<fstream>
#include<sstream>
#include <vector>
#include <limits>
#include "files.cpp"
using namespace std;
// What we have used till now: Inheritance, Polymorphism(function overloading), abstract classes, virtual function, file handling, friend function/class, vector, composition, constructors, parameterized constructors, runtime polymorphism
// What we have to use: Generic (Templates), Namespaces (Easy only, we can keep all the file classes in a diff file),operator overloading IDK What else is there.....

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
        static User* login();
        // void Open_folder()
        //     {

        //     }
        void View_file()//lists the files in an event
            {   
                DepartmentRepository repo;

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
            vector<vector<string>> data;
            string line, word;

            while (getline(inFile, line))
            {
                stringstream s(line);
                vector<string> row;

                while (getline(s, word, ','))
                {
                    row.push_back(word);
                }

                data.push_back(row);
            }
            /*this upper loop will convert THIS :
            1, pic1, pic2
            2, pic1, pic2
            TO
            data[0] = ["1", "pic1", "pic2"]
            data[1] = ["2", "pic1", "pic2"]
            */


            inFile.close();

            //Find user's row
            int row_num = -1;

            for (int i = 0; i < (int)data.size(); i++)
            {
                if (!data[i].empty() && stoll(data[i][0]) == Reg_No)
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
            ifstream eventFile("release.csv");

            vector<string> event_files;

            while (getline(eventFile, line))
            {
                stringstream s(line);
                vector<string> row;

                while (getline(s, word, ','))
                    row.push_back(word);

                if (!row.empty() && stoi(row[0]) == event_choice)
                {
                    for (int i = 2; i < (int)row.size(); i++)
                        event_files.push_back(row[i]);

                    break;
                }
            }

            eventFile.close();

            //Copy loop
            while (true)
            {
                int x;
                cin >> x;

                if (x == 0)
                    break;

                if (x < 1 || x > (int)event_files.size())
                {
                    cout << "Invalid file number.\n";
                    continue;
                }

                if ((int)data[row_num].size() - 1 >= 20)
                {
                    cout << "Storage limit reached.\n";
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clears the keyboard buffer

                    break;
                }

                data[row_num].push_back(event_files[x - 1]);

                cout << "'" << event_files[x - 1] << "' copied.\n";
            }

            //Rewrite userfiles.csv
            ofstream outFile("userfiles.csv");

            for (int i = 0; i < (int)data.size(); i++)
            {
                for (int j = 0; j < (int)data[i].size(); j++)
                {
                    outFile << data[i][j];

                    if (j != data[i].size() - 1)
                        outFile << ",";
                }

                outFile << endl;
            }

            outFile.close();
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
        virtual void handleop(int op) 
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
        void menu()
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
                    cout << "Row " << i << " col[0] = " << data[i][0] << endl;
                    if (stoll(data[i][0]) == choice1) {
                        data[i][choice2+1]=newname;//to skip the first two cols, if you dont enter a correct pic id then anything may happen
                        break;
                    }
                }

                // Rewrite file
                ofstream outFile("release.csv");

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
             string line;

             while (getline(inFile, line)) {
                 if (!line.empty())
                    newID++;  // just count lines
                 }
             inFile.close();
             fout<<endl<<newID<<","<<name;
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
             cout<<"12) Make Premovie"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 12)
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
             cout<<"12) Make Poster"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 12)
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
             fout<<endl<<newID<<","<<name;
            }
        void show_menu()
            {
             Member::show_menu();
             cout<<"12) Add Project"<<endl;
             cout<<"13) Show Members"<<endl;
             cout<<"14) Add member"<<endl;
             cout<<"15) Remove member"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 12)
                {
                 Add_proj();
                }
            else if(op==13)
                {
                 proj.Show_members();
                }
             else if(op==14)
                {
                 proj.Add_member();
                }
             else if(op==15)
                {
                 proj.Remove_member();
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

            }  
        void show_menu()
            {
             Member::show_menu();
             cout<<"12) Click Picture"<<endl;
             cout<<"********************************"<<endl;
            }
        void handleop(int op)
            {
             if (op == 12)
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

User* User::login() {
    long long id,targetID,count=0,pecount=0;
    string password;
    // File pointer
    fstream fin;

    // Open an existing file in read only mode
    fin.open("users.csv", ios::in);
    if (!fin) {
    cout << "Error opening file.\n";
    return NULL;
    }


    cout << "Enter ID: ";
    cin >> targetID;
    

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word, temp;

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
            count = 1;
            cout << "Enter password: ";
            cin >> password;

            while(password != row[1] && pecount < 3)
            {
                cout << "Wrong password. (" << 3-pecount << " tries left): ";
                cin >> password;
                pecount++;
            }

            if (pecount == 3)
            {
                cout << "Bye...\n";
                return NULL;
            }
            if (row[3] == "Artist") //did this to implement runtime polymorphism
                return new Artist(id,row[2],row[4]);
            else if (row[3] == "Photographer")
                return new Photographer(id,row[2],row[4]);
            else if (row[3] == "Editor")
                return new Editor(id,row[2],row[4]);
            else if (row[3]=="Admin")
                return new Admin(id,row[2],row[4]);
            else if (row[3]=="Studio")
                return new Studio(id,row[2],row[4]);
            else if (row[3]=="Designer")
                return new Designer(id,row[2],row[4]);
            else if (row[3]=="Graphic Editor")
                return new GraphicEditor(id,row[2],row[4]);
            else
                return new Other(id,row[2],row[4]);
        }
    }
    if (count == 0)
        cout << "Incorrect ID\n";
    fin.close();
    return NULL;

}

int main()
    {
     //cout<<"********************************"<<endl;
     cout<<"Do you want to login? (Y/N): ";


     char resp;
     cin>>resp;
     cin.ignore(numeric_limits<streamsize>::max(), '\n'); // makes sure it ignores whatever is there in the buffer till the newline char, clears the buffer.
     if(resp=='N' || resp=='n')
         return 0;
     User* currentUser = User::login();//runtime polymorphism
     
     if(currentUser)
     {  currentUser->menu();
     }
    return 0;


    }
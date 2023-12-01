#include <vector>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <regex>
#include<thread>
#include<mutex>
std::mutex mtx;
// namespace fs=std::filesystem;
using namespace std;
// using namespace std::filesystem;
#define Username "a"
#define Password "b"
class Command{
public:
string Directory;
Command(string curr_dir){
    string usr,pass;
    cout<<"Username : ";
    cin>>usr;
    cout<<"\nPassword : ";
    cin>>pass;
    getchar();
    if(usr==Username && pass==Password){
        Directory=curr_dir;
    }
    else{
        cout<<"Wrong Login credentials";
        exit(0);
    }

}
void ls(string& current, bool recursive) {
    int col = 0;

    if (recursive) {
        for (const auto& entry : filesystem::recursive_directory_iterator(current)) {
            if (col % 5 == 0 && col != 0) {
                cout << endl;
            }
            cout << entry.path().filename() << "\t";
            col++;
        }
    } else {
        for (const auto& entry : filesystem::directory_iterator(current)) {
            if (col % 5 == 0 && col != 0) {
                cout << endl;
            }
            cout << entry.path().filename() << "\t";
            col++;
        }
    }

    cout << endl;
}

void cd(string& dir){
    const char* directory=dir.c_str();
    if(chdir(directory)==0){
        Directory=(filesystem::current_path()).string();
        printf("\x1B[31m");
        cout<<"Your Working Directory is : "<<Directory<<endl;
         printf("\x1B[32m");
        // cout<<Directory<<"% ";
    }
    else{
        cout<<"no such file or directory : "<<dir;
    }
}
void rm(string& path,char& flag){
    if(flag=='f' || flag=='F'){
        if(filesystem::exists(path)){
            filesystem::remove(path);
            cout<<path<<" is successfully removed\n";
        }
        else{
            cout<<"No such file found\n";
        }
    }
    if(flag=='r' || flag=='R'){
        if(filesystem::exists(path)){
            filesystem::remove_all(path);
            cout<<path<<" is successfully removed\n";
        }
        else{
            cout<<"No such directory found\n";
        }
        

    }
}

void help(string comm){
    
    printf("\x1B[31m");
    ifstream help;
    string line;
    if(comm=="cd "){
        help.open("help_cd.txt");
        if(!help.is_open()){
            cerr<<"Error in opening file\n";
            return;
        }
        while(getline(help,line)){
            cout<<line<<endl;
        }
        
    }
    else if(comm=="ls "){
        help.open("help_ls.txt");
        if(!help.is_open()){
            cerr<<"Error in opening file\n";
            return;
        }
        while(getline(help,line)){
            cout<<line<<endl;
        }
        
    }
    else if(comm=="rm "){
        help.open("help_rm.txt");
        if(!help.is_open()){
            cerr<<"Error in opening file\n";
            return;
        }
        while(getline(help,line)){
            cout<<line<<endl;
        }
        
    }
    else if(comm=="mv "){
        help.open("help_mv.txt");
        if(!help.is_open()){
            cerr<<"Error in opening file\n";
            return;
        }
        while(getline(help,line)){
            cout<<line<<endl;
        }
        
    }
    else if(comm=="cp "){
        help.open("help_cp.txt");
        if(!help.is_open()){
            cerr<<"Error in opening file\n";
            return;
        }
        while(getline(help,line)){
            cout<<line<<endl;
        }
        
    }
    return;
}

void lsWithWildcard(string& directory, string wildcardPattern) {
    try {
        // Converting wildcard pattern to a regex pattern
        string regexPattern = "^" + regex_replace(wildcardPattern, regex("\\*"), ".*") + regex_replace(wildcardPattern, regex("\\?"), ".?") + "$";
        regex pattern(regexPattern);

        for (const auto& entry : filesystem::directory_iterator(directory)) {
            std::smatch match;
            std::string filename = entry.path().filename().string();

            if (std::regex_match(filename, match, pattern)) {
                std::cout << filename << "\t";
            }
        }

        std::cout << std::endl;
    } catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}





};


void cp(const string& sourcei, const string& destination,const bool recursive) {
    scoped_lock lock(mtx);
    // filesystem::path sourcePath = source;
                filesystem::path source=filesystem::path(sourcei);

    // filesystem::path destinationPath = destination;
    try {
    
        if (filesystem::is_directory(destination)) {
            filesystem::path destinationPath = filesystem::path(destination) / filesystem::path(sourcei).filename();

        if (recursive) {
            filesystem::copy(source,destinationPath, filesystem::copy_options::recursive | filesystem::copy_options::overwrite_existing);
            cout << source << "' successfully copied to '" << destination << "'.\n";
        } else {
            filesystem::copy_file(source, destinationPath, filesystem::copy_options::overwrite_existing);
            cout<< source << "' successfully copied to '" << destination << "'.\n";
        }
    }
    else{
        filesystem::copy_file(source, destination, filesystem::copy_options::overwrite_existing);
        cout<< source << "' successfully copied to '" << destination << "'.\n";
    }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Error copying: " << e.what() << endl;
    }
}
void cpW(string& source,string& destination,bool recursive){
    
    if(source.size()>=2 && source.substr(source.size()-2)=="/*"){
        source.pop_back();
        for(const auto& it : filesystem::directory_iterator(source)){
            string file=it.path().string();
            thread(cp,std::ref(file),std::ref(destination),recursive).detach();
        }
    }
    else{
       
            thread(cp,(source),(destination),recursive).detach();
        
    }
}


void mv(const string source,const string destination,const bool flag) {
    scoped_lock lock(mtx);
    if (filesystem::exists(destination) && filesystem::exists(source)) {
        if (filesystem::is_directory(destination)) {
            // If destination is a directory, appending the source file name to the destination path to create the full destination path
            filesystem::path destinationpath = filesystem::path(destination) / filesystem::path(source).filename();
            if(flag){
            auto source_time = filesystem::last_write_time(source);
            auto destination_time = filesystem::last_write_time(destination);
            if (source_time <= destination_time) {
                cout << "Source file is not newer than the destination. Operation canceled.\n";
                return;
            }
            else{
                filesystem::rename(source, destinationpath);
                cout << "File '" << source << "' successfully moved\n";
            }
            }
            else{
                filesystem::rename(source, destinationpath);
                cout << "File '" << source << "' successfully moved \n";
            }
            } 
            else{
            filesystem::rename(source, destination);
            cout << "File '" << source << "' successfully moved\n";
            }
    }
    else{
        cout<<"No such file or directory\n";
    }
    
    
}
void mvW(string& source, string& destination, bool flag){
    if (source.size() >= 2 && source.substr(source.size() - 2) == "/*") {
            source.pop_back(); // Removing the trailing *
            // Iterating over files in the source directory
            for (const auto& entry : filesystem::directory_iterator(source)) {
                string file = entry.path().string();
                thread(mv,file,destination,flag).detach();
                
            }
        }
        else {
           
                thread(mv,source,destination,flag).detach();
        
}
}
void Input_Command(Command &comm,string comm_input){
    
    if(comm_input.size()>=3 && comm_input.substr(3)=="--help"){
        string given_comm=comm_input.substr(0,3);
        // cout<<given_comm;
        comm.help(given_comm);
    }
    else if(comm_input.substr(0,3)=="cd "){

        string dest=comm_input.substr(3);
        comm.cd(dest);
    }
    else if(comm_input == "ls -R"){
    bool recursive = true;
    string current = (filesystem::current_path()).string();
    comm.ls(current, recursive);
}

     else if (comm_input.substr(0,5) == "ls -w") {
        string wildcardPattern = comm_input.substr(6);
        string current = (filesystem::current_path()).string();
        comm.lsWithWildcard(current,wildcardPattern);
    }
    // else if(comm_input.substr(0,4)=="ls -i"){
    //     bool showInode=true;
    //     string current=(filesystem::current_path()).string();
    //     comm.ls(current,showInode); 
    // }
    else if(comm_input.substr(0,2)=="ls"){
        bool recursive=false;
        // cout<<comm_input.substr(0,2);
        string current=(filesystem::current_path()).string(); 
        comm.ls(current,recursive);
    }
    
    else if(comm_input.substr(0,3)=="rm "){
        if(comm_input.size()==2){
            cout<<"usage: rm [-f | -i] [-dIPRrvWx] file ...\nunlink [--] file";

        }
        // cout<<comm_input.substr(6)<<endl;
        // cout<<comm_input.substr(3,5);
        string path=comm_input.substr(6);
        char flag=comm_input.at(4);
        comm.rm(path,flag);
    }
    
    else if(comm_input.substr(0,3)=="mv "){
        string source,destination;
        bool temp=false;
        if(comm_input.size()==2){
        cout<<"usage: mv [-f | -i | -n] [-hv] source target\nmv [-f | -i | -n] [-v] source ... directory\n";
        }
        char flag=comm_input.at(4);
        if(flag=='u'){
        temp=true;
        source=comm_input.substr(6,comm_input.find(" ",6)-6);
        destination=comm_input.substr(comm_input.find(" ",6)+1);
        }
        else{
            source = comm_input.substr(3, comm_input.find(" ", 3) - 3);
            destination = comm_input.substr(comm_input.find(" ", 3) + 1);
        }
        // cout<<source<<endl<<destination;
        mvW(source,destination,temp);
    }
    
    else if(comm_input.substr(0,3)=="cp "){
        bool recursive=false;
        string source,destination;
        if(comm_input.size()==2){
            cout<<"usage: cp [-R [-H | -L | -P]] [-fi | -n] [-aclpSsvXx] source_file target_file\ncp [-R [-H | -L | -P]] [-fi | -n] [-aclpSsvXx] source_file ... target_directory\n";
        }
        char flag=comm_input.at(4);
        if (flag=='R')
        {
            source=comm_input.substr(6,comm_input.find(" ",6)-6);
            destination=comm_input.substr(comm_input.find(" ",6)+1);
            recursive=true;
        }
        else{
        source=comm_input.substr(3,comm_input.find(" ",3)-3);
        destination=comm_input.substr(comm_input.find(" ",3)+1);
        }
        // cin>>temp>>source>>destination;
        // cout<<source<<endl<<destination;
        
        cpW(source,destination,recursive);
    }
    else{
        cout<<"\nNo matching command with your input!!!!!"<<endl;
    }
    
}
int main(){
    system("clear");
    string curr_path=(filesystem::current_path()).string();
    
    Command comm(curr_path);
     while(true){
        printf("\x1B[32m");
        string comm_input;
        cout<<Username<<comm.Directory<<"% ";
        
        // getchar();
        getline(cin,comm_input);
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        auto t1 = high_resolution_clock::now();
       Input_Command(comm,comm_input);
        auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
     printf("\x1B[31m");
    cout<<"\nTime Duration for "<<comm_input<<" command : ";
    
    cout <<ms_double.count() << "ms\n";
     printf("\x1B[32m");
     cout<<endl;
    };
}
// List all files with the extension ".jpg" in the current directory:
//     ls -w *.jpg
// List all files starting with "doc" and having any extension:
//     ls -w doc*
// List files with names that start with "file" and end with ".txt":
//     ls -w file*.txt
// List files with names that contain exactly three characters before the extension:
// ls -w ???.*
// List files with names that start with a vowel:
// ls -w [aeiou]*



     
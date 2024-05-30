#include <windows.h>
#include <bits/stdc++.h>
#include <string.h>
#include <vector>
#include <thread>
#include <chrono>
#include <tlhelp32.h>
#include <tchar.h>
#include <debugapi.h>
#include <processthreadsapi.h>
#include <fstream>
#include <algorithm>
#include "bibrary.h"
#include "messenger.h"
#include "concurentCommandBuilder.h"

using namespace std;

const string EXIT_COMMAND = "exit"; //
string currentDirectory = "";
vector<string> history;
HANDLE hForeProcess;

void runable(string command)
//Switch case command => run
{
	vector<string> eList = elementList(command);
    void time1();
    void date();
    void time2();
	
    if (command.compare("help") == 0)
    {
        help();
    }
    
    else if (command.compare("history") == 0)
    {
        int i = 0;
        for (string s: history){
            if (s != "history"){
                i++;
                cout << i << ". "<< s << endl;
            }
        }
        if (i == 0)
            cout << "Empty history!" << endl;
    }

    else if (command.compare("clean") == 0)
    {
        history.clear();
        cout << "Your history has been cleaned thoroughly!" << endl;
    }

    else if (command.compare("exit") == 0)
    {
        GOODBYE;
        printf("Sending signal to kill all child processes ...\n");
        kill_All();
        Sleep(1);
        //this_thread::sleep_for(chrono::milliseconds(800));
        exit(0);
    }

    else if ( eList[0].compare("wordpad") == 0)
    {
    	if  (eList.size() == 1 ||
    		(eList[1].compare("back") == 0 && eList.size() == 2) ||
    		(eList[1].compare("fore") == 0 && eList.size() == 2) ) 
    	{
    		string command_;
			if( eList.size() == 1)
		    	command_ = "wordpad fore";
		    else
		    	command_ = eList[0] + " " + eList[1];
	    	printf("Hello, wordpad is running\n");
			openInBackOrFore(command_, "C:/Program Files/Windows NT/Accessories/wordpad.exe"); //C:/Windows/System32/calc.exe
		}
		else
		{
			printf("Illegal command!\n");
        	command.append(" -> Illegal command");
		}
			
    }

    else if (eList[0].compare("dir") == 0 && eList.size() <= 2)
    {
    	if(eList.size() == 1)
        	listOfCurrent();//system("dir")
        else
        	listOfPath(eList[1]);
    }

    else if (command.compare("date") == 0){
        HANDLE h1;
        DWORD ThreadId;
        h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)date, NULL, 0, &ThreadId);
        cin.get();
        TerminateThread(h1, 0);
        CloseHandle(h1);
    }

    else if (command.compare("time") == 0) {
        HANDLE h1;
        DWORD ThreadId;
        h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)time1, NULL, 0, &ThreadId);
        cin.get();
        TerminateThread(h1, 0);
        CloseHandle(h1);
    }

    else if (command.compare("clock") == 0) {
        HANDLE h1;
        DWORD ThreadId;
        h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)time2, NULL, 0, &ThreadId);
        cin.get();
        TerminateThread(h1, 0);
        CloseHandle(h1);
    }

//	else if (command.compare("countdown") == 0 || command.compare("countdown fore") == 0 || command.compare("countdown back") == 0)
//    {
//    	if(command.compare("countdown") == 0)
//    		command += " fore";
//        printf("Hello, countdown clock is running\n");
//        string s = currentDirectory + "\\countDownClock.exe";
//        openInBackOrFore(command, s);
//    }

    else if (command.compare("list") == 0)
    {
        list1();
    }

    else if (command.compare("clear") == 0)
    {
        system("cls");
    }
    
    else if (eList[0].compare("cd") == 0)
    {
    	if(eList.size() == 2)
    	{
    		string s = eList[1];
        	cd(s);
		}
		else
		{
			printf("Illegal command!\n");
        	command.append(" -> Illegal command");
		}
    }

    else if (eList[0].compare("kill") == 0 && eList.size() == 2)
    {
        if (eList[1].compare("-1") == 0)
        {
            kill_All();
        }
        else
        {
			string s = eList[1];
	        kill(s);
        }
        
    }

    else if (eList[0].compare("stop") == 0) {
        string s = eList[1];
        stop(s); 
    }

    else if (eList[0].compare("resume") == 0) {
        string s = eList[1];
        resume(s); 
    }

    else if (eList[0].compare("env") == 0 && eList.size() <= 2){
    	if (eList.size() == 1) {
    		read_env(NULL);
		}
		else 
		{
			char * envname = (char *) eList[1].c_str();
			read_env(envname);
		}
	}
	
	else if (eList[0].compare("addenv") == 0 && eList[2].compare("=") == 0 && eList.size() == 4){
		char *name = (char*) eList[1].c_str();
		char *value = (char*) eList[3].c_str();
		add_env(name, value);
	}
	
	else if (eList[0].compare("delenv") == 0 && eList.size() == 2){
		char *envname = (char *) eList[1].c_str();
		del_env(envname);
	}
	
	else if( eList[0].compare("run") == 0)
	{
		if  ( eList[1].find(".exe") != std::string::npos &&
			( eList.size() == 2 ||
			( eList.size() == 3 && (eList[2].compare("back") || eList[2].compare("fore"))))) {
    		if( eList.size() == 2)
    			command = eList[1] + " fore";
    		else
    			command = eList[1] + " " + eList[2];
    		runExe(command);
    	}
    	
    	else if (eList[1].find(".bat") != std::string::npos && eList.size() == 2){
    		string s;
    		if( eList[1].find(":") != std::string::npos)
    			s = eList[1];
    		else
				s = currentDirectory + "\\" + eList[1];
    		runBat(s);
		}
		else
		{
			printf("Illegal command!\n");
            command.append(" -> Illegal command");
		}
		
	}

	else if(eList[0].compare("mkdir") == 0 && eList.size() == 2){
		string folderName = eList[1];
		mk_dir(folderName);
	}
	
	else if(eList[0].compare("rmdir") == 0 && eList.size() == 2){
		string folderPath;
		if(eList[1].find(":\\") != std::string::npos)
			folderPath = eList[1];
		else
			folderPath = currentDirectory + "\\" + eList[1];
		rm_dir(folderPath);
	}
	
	else if(eList[0].compare("color") == 0 && eList.size() <= 2){
		if(eList.size() == 1)
			colortext("white");
		else
			colortext(eList[1]);
	}
	
	else if(command.length() == 0) {
		
	}
    else
    {
        printf("Illegal command!\n");
        command.append(" -> Illegal command");
    }
    history.push_back(command);
}

void run(string command)
{
    vector<string> jobs;
    jobs = createJobs(command);
    for (int i = 0; i < jobs.size(); ++i)
    {
        runable(trim(jobs[i]));
    }
}

int main()
{
    WELCOME(); // message
    string command;
//    char *buffer1 = _getcwd(NULL, 0);// get current working directory
//    currentDirectory = buffer1;
//    
    while (true)
    {
    	char *buffer = _getcwd(NULL, 0);
        currentDirectory = buffer;
	    
    	printf("%s", buffer); cout << ">";
        getline(cin, command);
        trim(command);
    							
        if (command == EXIT_COMMAND) // exit
        {
            GOODBYE(); //message
            printf("Killing all processes...\n");
            kill_All();
            this_thread::sleep_for(chrono::milliseconds(2000)); // tam dung thuc thi luong hien tai trong 2s
            return 0;
        }
        else
        {
            run(command);
        }
    }

}

//Thời gian tiến trình
void time1()
{
    int sec_prev=0;
    while(1)
    {
        int seconds, minutes, hours;
        string str;

        //storing total seconds
        time_t total_seconds=time(0);

        //getting values of seconds, minutes and hours
        struct tm* ct = localtime(&total_seconds); // lay thoi gian hien tai

        seconds=ct->tm_sec;
        minutes=ct->tm_min;
        hours=ct->tm_hour;

        //converting it into 12 hour format
        /*if(hours>=12)
            str="PM";
        else
            str="AM";
        hours=hours>12?hours-12:hours;  */


        //printing the result
        if(seconds==sec_prev+1 || (sec_prev==59 && seconds==0))
        {
            cout << '\r'; //nhay ve dau dong
            cout<< (hours<10?"0":"") << hours <<":" << (minutes<10?"0":"") << minutes << ":" << (seconds<10?"0":"") << seconds << " " << str;
        }

        sec_prev=seconds;

    }
}

void date()
{
    int sec_prev=0; //save previous second
    while(1){
        int seconds, minutes, hours;
        int days, months, years;
        int weekdays;
        string wday[] = {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"
        };
        string str;

        //storing total seconds
        time_t total_seconds=time(0);

        //getting values of seconds, minutes and hours
        struct tm* ct = localtime(&total_seconds);

        seconds=ct->tm_sec;
        minutes=ct->tm_min;
        hours=ct->tm_hour;
        days = ct->tm_mday;
        months = ct->tm_mon + 1;
        years = ct ->tm_year + 1900;    //in struct ct, tm_year beginning at 1900
        weekdays = ct->tm_wday; //beginning at Sunday

        //converting it into 12 hour format
        if(hours>=12)
            str="PM";
        else
            str="AM";
        hours=hours>12?hours-12:hours;

        //printing the result
        if(seconds==sec_prev+1 || (sec_prev==59 && seconds==0))
        {
            cout << '\r';
            cout << wday[weekdays] << " ";
            cout<< (hours<10?"0":"") << hours <<":" << (minutes<10?"0":"") << minutes << ":" << (seconds<10?"0":"") << seconds << " " << str;
            cout<<'\t' <<(days<10?"0":"") << days <<"/" << (months<10?"0":"") << months << "/" << (years<10?"0":"") << years;
        }

        sec_prev=seconds;
    }
}

//print clock
void time2()
{
    int sec_prev = 0;
    while (1)
    {
        int seconds, minutes, hours;
        string str;

        time_t total_seconds = time(0);            // storing total seconds
        struct tm *ct = localtime(&total_seconds); // getting values of seconds, minutes and hours

        seconds = ct->tm_sec;
        minutes = ct->tm_min;
        hours = ct->tm_hour;

        //printing the result

        if (seconds == sec_prev + 1 || (sec_prev == 59 && seconds == 0))
        {
            system("cls");
            cout << "       ,--.-----.--." << endl;
            cout << "       |--|-----|--|" << endl;
            cout << "       |--|     |--|" << endl;
            cout << "       |  |-----|  |" << endl;
            cout << "     _|--|     |--|_" << endl;
            cout << "    /  |  |-----|  |  \\" << endl;
            cout << "   /   \\__|-----|__/   \\" << endl;
            cout << "  /   _____---_____   \\/\\" << endl;
            cout << " /   /               \\   \\/" << endl;
            cout << "{   /      ROLEX      \\   }" << endl;
            cout << "|  {                   }  |-," << endl;

            cout << (hours < 10 ? "|  |    0" : "|  |    ") << hours << (minutes < 10 ? " : 0" : " : ") << minutes
                 << (seconds < 10 ? " : 0" : " : ") << seconds << "   |  | |" << endl;

            cout << "|  {                   }  |-'" << endl;
            cout << "{   \\                 /   }" << endl;
            cout << " \\   `------___------'   /\\ " << endl;
            cout << "  \\     _|-----|_     /\\/" << endl;
            cout << "   \\   /  |-----|  \\   /" << endl;
            cout << "    \\  |--|     |--|  /" << endl;
            cout << "     --|  |-----|  |--" << endl;
            cout << "       |--|     |--|" << endl;
            cout << "       |--|-----|--|" << endl;
            cout << "       `--'-----`--'" << endl;
        }
        sec_prev = seconds;
    }
}


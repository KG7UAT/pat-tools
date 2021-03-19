#include <iostream>
#include <ios>
#include <dirent.h>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

bool replace(string& str, const string& from, const string& to) {
	size_t start_pos = str.find(from);
	if(start_pos == string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void tokenize(string s, string del = " ") {
	int start = 0;
	int end = s.find(del);
	while (end != -1) {
		cout << s.substr(start, end - start) << endl;
		start = end + del.size();
		end = s.find(del, start);
	}

	cout << s.substr(start, end - start);
}

vector<string> split(string str,string sep) {
	char* cstr=const_cast<char*>(str.c_str());
	char* current;
	vector<string> arr;
	current=strtok(cstr,sep.c_str());

	while(current!=NULL){
		arr.push_back(current);
		current=strtok(NULL,sep.c_str());
	}

	return arr;
}

string exec(string command) {
	char buffer[128];
	string result = "";

	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe) {
		return "popen failed!";
	}

	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

	pclose(pipe);
	return result;
}

int main(int argc, char *argv[]) {
	string args[argc];

	for(int i = 0; i < argc; i++) {
		args[i] = argv[i];
	}

	if (args[1] == "list-all") {
		if (argc == 2) {
			system("pat rmslist -m ardop");
		} else {
			system(("pat rmslist -m ardop | grep " + args[2]).c_str());
		}
	} else if (args[1] == "new-list") {
		string listName;

		if (argc == 2) {
			cout << "Type a name for your new list: ";
			cin >> listName;
			cout << endl;
		} else if (argc == 3) {
			listName = args[2];
		}

		ofstream MyFile("/usr/local/bin/lists/" + listName + ".list");
		MyFile << "";
		MyFile.close();

		cout << "Created list: " << listName << endl;
	} else if (args[1] == "add") {
		string call;
		string frequency;

		cout << "Enter callsign: ";
		cin >> call;
		cout << "Enter dial frequency in kHz: ";
		cin >> frequency;

		string myText;
		string list;

		ifstream MyReadFile("/usr/local/bin/lists/" + args[2] + ".list");

		while (getline (MyReadFile, myText)) {
			list += myText + "\n";
		}

		MyReadFile.close();

		myText = call + "," + frequency + "\n";
		list += myText;

		ofstream MyFile("/usr/local/bin/lists/" + args[2] + ".list");
		MyFile << list;

		MyFile.close();
	} else if (args[1] == "remove") {
		string myText;
		string entries;
		int num = 1;
		int inputNum;
		ifstream MyReadFile("/usr/local/bin/lists/" + args[2] + ".list");

		while (getline (MyReadFile, myText)) {
			replace(myText, ",", "?freq=");
			string s = to_string(num);
			system(("printf '" + s + "   ' && pat rmslist -m ardop | grep " + myText).c_str());
			num += 1;
		}

		MyReadFile.clear();
		MyReadFile.seekg(0);
		num = 1;

		cout << endl << endl << "Enter the number of the entry that you'd like to delete: ";
		cin >> inputNum;

		while (getline (MyReadFile, myText)) {
			if (num != inputNum) {
				entries += myText + "\n";
			}
			num += 1;
		}

		ofstream MyFile("/usr/local/bin/lists/" + args[2] + ".list");

		MyFile << entries;

		cout << endl << endl << "Entry deleted" << endl;

		MyReadFile.close();
		MyFile.close();
	} else if (args[1] == "delete-list") {
		string listName;

		if (argc == 2) {
			cout << "Type the name of the list you'd like to delete: ";
			cin >> listName;
			cout << endl;
		} else if (argc == 3) {
			listName = args[2];
		}

		system(("rm /usr/local/bin/lists/" + listName + ".list").c_str());

		cout << "Deleted list: " << listName << endl;
	} else if (args[1] == "connect") {
		string bands[12][3] = {{"1800", "2000", "160m"}, {"3500", "4000", "80m"}, {"7000", "7300", "40m"}, {"10100", "10150", "30m"}, {"14000", "14350", "20m"}, {"18068", "18168", "17m"}, {"21000", "21450", "15m"}, {"24890", "24990", "12m"}, {"28000", "29700", "10m"}, {"50000", "54000","6m"}, {"144000", "148000", "2m"}, {"420000", "450000", "70cm"}};
		string frequency[2];

		if (argc == 4) {
			for (int i = 0; i < 12; i++) {
				if (args[3] == bands[i][2]) {
					frequency[0] = bands[i][0];
					frequency[1] = bands[i][1];
				}
			}
		}

		string myText;
		string slash = "/";
		ifstream MyReadFile("/usr/local/bin/lists/" + args[2] + ".list");

		if (argc == 3) {
			while (getline (MyReadFile, myText)) {
				replace(myText, ",", "?freq=");
				system(("pat connect ardop:" + slash + slash + slash + myText).c_str());
				cout << endl;
			}
		} else if (argc == 4) {
			while (getline (MyReadFile, myText)) {
				std::string s = myText;
				std::string delimiter = ",";

				size_t pos = 0;
				std::string token;
				while ((pos = s.find(delimiter)) != std::string::npos) {
					token = s.substr(0, pos);
					s.erase(0, pos + delimiter.length());
				}

				int a = stoi(frequency[0]);
				int b = stoi(frequency[1]);
				int c = stoi(s);

				if (a <= c && b >= c) {
					replace(myText, ",", "?freq=");
					system(("pat connect ardop:" + slash + slash + slash + myText).c_str());
					cout << endl;
				}
			}
		}

		MyReadFile.close();
	} else if (args[1] == "list") {
		string bands[12][3] = {{"1800", "2000", "160m"}, {"3500", "4000", "80m"}, {"7000", "7300", "40m"}, {"10100", "10150", "30m"}, {"14000", "14350", "20m"}, {"18068", "18168", "17m"}, {"21000", "21450", "15m"}, {"24890", "24990", "12m"}, {"28000", "29700", "10m"}, {"50000", "54000","6m"}, {"144000", "148000", "2m"}, {"420000", "450000", "70cm"}};
		string frequency[2];

		if (argc == 4) {
			for (int i = 0; i < 12; i++) {
				if (args[3] == bands[i][2]) {
					frequency[0] = bands[i][0];
					frequency[1] = bands[i][1];
				}
			}
		}

		if (argc == 2) {
			string files;
			string output;

			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir ("/usr/local/bin/lists/")) != NULL) {
				while ((ent = readdir (dir)) != NULL) {
					output = ent->d_name;
					if (output != "." && output != "..") {
						files = files + output + "\n";
						replace(files, ".list", "");
					}
				}

				cout << files;

				closedir (dir);
			}
		} else if (argc >= 3) {
			string myText;
			ifstream MyReadFile("/usr/local/bin/lists/" + args[2] + ".list");

			if (argc == 3) {
				while (getline (MyReadFile, myText)) {
					replace(myText, ",", "?freq=");
					system(("pat rmslist -m ardop | grep " + myText).c_str());
				}
			} else if (argc == 4) {
				while (getline (MyReadFile, myText)) {
					std::string s = myText;
					std::string delimiter = ",";

					size_t pos = 0;
					std::string token;
					while ((pos = s.find(delimiter)) != std::string::npos) {
						token = s.substr(0, pos);
						s.erase(0, pos + delimiter.length());
					}

					int a = stoi(frequency[0]);
					int b = stoi(frequency[1]);
					int c = stoi(s);

					if (a <= c && b >= c) {
						replace(myText, ",", "?freq=");
						system(("pat rmslist -m ardop | grep " + myText).c_str());
					}
				}
			}

			MyReadFile.close();
		}
	} else if (args[1] == "compile-list") {
		string bands[12][3] = {{"1800", "2000", "160m"}, {"3500", "4000", "80m"}, {"7000", "7300", "40m"}, {"10100", "10150", "30m"}, {"14000", "14350", "20m"}, {"18068", "18168", "17m"}, {"21000", "21450", "15m"}, {"24890", "24990", "12m"}, {"28000", "29700", "10m"}, {"50000", "54000","6m"}, {"144000", "148000", "2m"}, {"420000", "450000", "70cm"}};
		char letters[4] = {'A', 'K', 'N', 'W'};
		int num = 0;
		string homeDir = getenv("HOME");
		system(("pat rmslist -m ardop >> " + homeDir + "/pat-tools/stations.txt").c_str());

		int minFreq;
		int maxFreq;

		for (int i = 0; i < 12; i++) {
			if (args[3] == bands[i][2]) {
				stringstream a(bands[i][0]);
				stringstream b(bands[i][1]);

				a >> minFreq;
				b >> maxFreq;
			}
		}

		string myText;
		ifstream MyReadFile(homeDir + "/pat-tools/stations.txt");
		while (getline (MyReadFile, myText)) {
			for (int i = 0; i < 4; i++) {
				if (letters[i] == myText[0]) {
					string stations;
					string callsign;
					string frequency;
					bool two = false;
					for (int a = 76; a < myText.length(); a++) {
						stations += myText[a];
					}

					replace(stations, "ardop:", "");
					replace(stations, "/", "");
					replace(stations, "/", "");
					replace(stations, "/", "");
					replace(stations, "?freq=", ",");

					string line = stations;
					vector <string> tokens;
					stringstream check1(line);
					string intermediate;

					while(getline(check1, intermediate, ' ')) {
						tokens.push_back(intermediate);
					}

					for(int i = 0; i < tokens.size(); i++) {
						vector<string> arr;
						arr=split(tokens[i],",");

						stringstream c(arr[1]);
						int x = 0;
						c >> x;

						if ((x <= maxFreq) && (x >= minFreq)) {
							cout << num << ". " << arr[0] << "	@	" << arr[1] << endl;

							string slash = "/";

							string cmd = exec(("pat connect ardop:" + slash + slash + slash + arr[0] + "?freq=" + arr[1]).c_str());
							if (cmd.find("Unable to establish connection to remote") != std::string::npos) {
								cout << "Connection failed to " << arr[0] << endl;

							} else {
								cout << "Connection succeeded to " << arr[0] << endl;

								ofstream list("/usr/local/bin/lists/" + args[2] + ".list", ios_base::app | ios_base::out);
								list << arr[0] << "," << arr[1] << endl << endl;;
							}
						}
					}

					num += 1;
				}
			}
		}

		system("cd ~/pat-tools && rm stations.txt");
	} else if (args[1] == "version") {
		cout << "Version 0.2" << endl;
	} else {
		cout << "Error: command not recongized" << endl;
	}

	return 0;
}

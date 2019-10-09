#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

typedef struct Row {
	int id;
	string col1;
    string col2;
    string col3;
} Row;

int Num_Row;
vector <string> header;
vector <Row> df1;
vector <Row> df2;

int main(void) {
    string tmp;
    
    scanf("%d", &Num_Row);
    cin.clear();
    getline( cin, tmp );
    header.push_back(tmp);
    // while(!cin.eof()) {
    //     getline( cin, tmp );
    //     header.push_back(tmp);
    // }
    
    // for(int i=2; i<=Num_Row; i++) {
    //     for(int j=0; j<header.size(); j++) {
    //         getline( cin, tmp );
    //     }
    // }

    for (int i = 0; i<header.size(); i++ ){
        cout << header[i] << endl;
     }    
}
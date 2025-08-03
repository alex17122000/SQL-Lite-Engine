#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
using namespace std;

class Column {
private:
    string ColumnName = "";
    string ColumnType = "";
    string DefaultValue = "";

public:
    Column() { }
    Column(Column& name) {
        this->ColumnName = name.ColumnName;
        this->ColumnType = name.ColumnType;
        this->DefaultValue = name.DefaultValue;
    }
    Column(string columnName, string columnType, string value) {
        setColumnName(columnName);
        setColumnType(columnType);
        setValue(value);
    }

    void setColumnName(string cName) {
        this->ColumnName = cName;
    }
    void setColumnType(string cType) {
        this->ColumnType = cType;
    }
    void setValue(string cValue) {
        this->DefaultValue = cValue;
    }

    string getColumnName() {
        return this->ColumnName;
    }


    string obtainColumns(int noCols, Column col[]) {
        string columns;
        for (int i = 0; i < noCols; i++)
        {
            columns = columns + col[i].ColumnName;
            columns = columns + col[i].ColumnType;
            columns = columns + col[i].DefaultValue;
        }
        return columns;

    }

};

class Table {

    string Name = "";
    string columns = "";


public:

    Table(string name) {
        this->Name = name;
    }

    Table(string name, Column** cols)
    {
        this->Name = name;

    }


    Table(Table& table) {
        this->Name = table.Name;
        this->columns = columns;
    }

    ~Table() {

    }

    string getTableName() {
        return this->Name;
    }

    string getColumns() {
        if (this->columns == "") return "No columns for this table";
        return this->columns;
    }

    void setColumns(string columns) {
        this->columns = columns;
    }
};

class Util {
public:
    /*  static void readString(ifstream& file, string& value) {
          file >> value;
          if (value == "") { cout << "no table created (message from Util class)" << endl; }
      }

      static void writeString(ofstream& file, string value) {
          int dim = value.size() + 1;
          file.write(value.c_str(), dim * sizeof(char));
      }*/
};


class tablesSearch {


};

class INSERT {
private:
    string tableline = " ";
    int tableExists = 0;
    const string insertDelimiter = ",";
    int insertPos = 0;
    string insertParameter = "";
    string insertParameters[50];
    int j = 0;
    vector<string>  insertArray;
    vector<string>  newInsertArray;
    int p = 0;
public:


    void insertFromSQLCommand(string tableName, string sqlCommand)
    {
        //CHECK IF TABLE EXISTS, GETTING THE PARAMS IN INSERT ARRAY, 
        ifstream fisier2("tables.txt", ios_base::in);
        getline(fisier2, tableline);
        do {
            if (tableline == tableName) {
                this->tableExists = 1;
            }
        } while (getline(fisier2, tableline, '\n') && (this->tableExists == 0));
        fisier2.close();

        int openBracket = sqlCommand.find("(") + 1;
        int closeBracket = sqlCommand.find(")") - 1;
        cout << endl;
        string columnData = sqlCommand.substr(openBracket, closeBracket);

        while ((insertPos = columnData.find(insertDelimiter)) != string::npos) {
            insertParameter = columnData.substr(0, insertPos+1);
            insertParameters[j] = insertParameter;
            insertArray.push_back(insertParameter);
            columnData.erase(0, insertPos + insertDelimiter.length());
            j++;
        }
        insertParameters[j] = columnData; j++;
        insertArray.push_back(columnData);
        //CHECK IF TABLE EXISTS, GETTING THE PARAMS IN INSERT ARRAY,

        //CHECK THE "tables.txt" FILE, 
        ofstream fisier3("tables.txt", ios_base::out | ios_base::app);
        string searchTableName;
        ifstream inFile;
        string line;

        inFile.open("tables.txt");

        if (!inFile) {
            cout << "Unable to open file" << endl;
            exit(1);
        }
        searchTableName = tableName;
        int tablePosition;      // before it was    size_t

         //CHECK THE "tables.txt" FILE, 


        if(inFile.good())  //AICI ERA WHILE
        {
            getline(inFile, line); // get line from file
            tablePosition = line.find(searchTableName); // search for the tablename
            if (tablePosition == string::npos) // string::npos is returned if string is not found
            {
                cout << "Table was not found while trying to insert!";
                //break;
            }
            else
            {
               
                    cout <<endl<< "Table position: "+ tablePosition<<endl;
               
                ofstream fout(tableName, ios::out | ios::binary);

                fout.write((char*)&insertArray, sizeof(insertArray));
                fout.close();

                ifstream fin(tableName, ios::in | ios::binary);
                if (!fin.good()) {
                    cout << "Error occurred at reading time!" << endl;

                }
                else {
                    while (fin.read((char*)&newInsertArray, sizeof(newInsertArray)))
                    {

                        while (j != p)
                        {
                            cout << endl;

                            cout << newInsertArray[p] << " ";
                            p++;

                        }

                        cout << endl;
                    }
                }
            }
            fisier3.close();
        }
        //  BINARYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
        ofstream fout(tableName, ios::out | ios::binary);

        fout.write((char*)&insertArray, sizeof(insertArray));
        fout.close();

        ifstream fin(tableName, ios::in | ios::binary);
        if (!fin.good()) {
            cout << "Error occurred at reading time!" << endl;

        }
        else {
            while (fin.read((char*)&newInsertArray, sizeof(newInsertArray)))
            {
                cout << "Values from INSERT (.bin)" << " ";
                while (j != p)
                {
                    cout << endl;

                    cout << newInsertArray[p] << " ";
                    p++;

                }

                cout << endl;
            }
        }


        fin.close();

    }
};

class  CREATE_TABLE {
public:

    int noCols = 0;
    size_t pos = 0;
    string columnParameter;
    size_t commaDelimiterPosition = 0;
    vector<string> columnParametersArray;
    const string commaDelimiter = ",";
    Column** cols = new Column * [50];
    string columns;
    Table** tables = new Table * [50];
    int noTables; //ASTA ERA STATIC AICI INAINTE********CAUSES LINKER ERROR***************************************************

    int createFromSQLCommand(string tableName, string sqlCommand) {


        int i = 0;
        string fileParameters[50];
        this->noCols = 0;
        cout << "the table name is " << endl << tableName << endl;
        cout << "the comand introduced is: " << sqlCommand;
        while ((this->pos = sqlCommand.find(")")) != string::npos)
        {
            this->noCols++;

            unsigned openBracket = sqlCommand.find("(") + 1;
            unsigned closeBracket = sqlCommand.find(")") - 1;

            cout << endl;

            string columnData = sqlCommand.substr(openBracket, closeBracket);
            int columnDataLength = columnData.length();
            cout << columnData;

            fileParameters[i] = columnData;
            while ((commaDelimiterPosition = columnData.find(commaDelimiter)) != string::npos) {
                columnParameter = columnData.substr(0, commaDelimiterPosition);
                fileParameters[i] = columnParameter;
                columnParametersArray.push_back(columnParameter);
                columnData.erase(0, commaDelimiterPosition + commaDelimiter.length());
                i++;
            }

            fileParameters[i] = columnData; i++;
            columnParametersArray.push_back(columnData);

            cols[noCols - 1] = new Column(columnParametersArray[0], columnParametersArray[1], columnParametersArray[2]);

            cout << endl;


            columnParametersArray.clear();

            columnData.erase(0, commaDelimiterPosition + commaDelimiter.length());

            cout << endl;

            sqlCommand.erase(0, columnDataLength + 3);

        }


        string filename = tableName + ".txt";
        string search;
        string line;

        ofstream ofstreamTables("tables.txt", fstream::in | fstream::out | fstream::app);

        ifstream ifstreamTables;

        search = tableName;
        if (!ifstreamTables) {
            cout << "Unable to open file" << endl;
            exit(1);
        }

        size_t pos;
        ifstreamTables.close();
        ofstreamTables << tableName << endl;




        ofstream fisier1;
        fisier1.open(filename.c_str(), fstream::in | fstream::out | fstream::app);
        if (!fisier1)
            cout << "\nCould not create FISIER\n";
        else {
            int noOfParams = sizeof(fileParameters) / sizeof(fileParameters[0]);
            for (int i = 0; i <= noOfParams; i++)
            {
                if (fileParameters[i] == "" || fileParameters[i].length() == 0)
                    break;

                fisier1 << fileParameters[i] << endl;

            }

            fisier1.close();
            ofstreamTables.close();

        }


    }



};



class DISPLAY_TABLE {
private:
    string Name;
public:


    void setColumnName(string cName) {
        this->Name = cName;
    }
    void getvalues() {
        cout << "The table " << /*Name */ " doesn't exist";
        cout << endl;
    }

    void displayTableFunction(string name) {

        string tableName = name;

        // cout << endl << "Am intrat in functia display "<<endl;
        string filename = tableName + ".txt";
        string search;
        string line;

        ifstream ofstreamTable(filename, fstream::in | fstream::out | fstream::app);
        if (ofstreamTable.is_open()) {
            cout << endl << "The columns of the table " << tableName << " are:  " << endl;
            while (ofstreamTable >> line) {
                cout << line << endl;
            }
            ofstreamTable.close();
        }
        else cout << endl << "The table to be displayed couldn't be opened" << endl;
    }
};



class CREATE_INDEX {
private:
    int pk;
    string ColumnName;
public:
    CREATE_INDEX(string Name) {

    }
    void setColumnName(string cName) {
        this->ColumnName = cName;
    }
    void getvalues() {
        cout << this->ColumnName << " ";

    }
};

class DROP_INDEX {
private:
    int pk;
    string ColumnName;
};



class UPDATE {

};
class DELETE {

};



class DROP_TABLE {

public:

    DROP_TABLE dropTableObject();


    int deletingTxtandBin(string theTable)
    {
        string tableName = theTable;
        cout << "Table to drop : " << tableName << endl;
        string tables[50];
        int noTables = 0;
        int pozitieTabelDropTable = -1;

        string readedTable;
        ifstream fisier2("tables.txt", ios_base::in | fstream::out | fstream::app);


        if (fisier2.is_open()) {

            while (fisier2 >> readedTable) {

                noTables++;
                tables[noTables] = readedTable;
                if (readedTable == tableName && pozitieTabelDropTable == -1)
                {
                    pozitieTabelDropTable = noTables;
                    //deleting the file and txt file:
                    string filename = readedTable + ".txt";
                    remove(filename.c_str());
                    string binfilename = readedTable;
                    remove(binfilename.c_str());
                    if (remove(filename.c_str()) != 0)
                        cout << endl << "Error deleting the table's file" << endl;
                    else  cout << endl << "File successfully deleted" << endl;

                }
            }
            fisier2.close();
            if (pozitieTabelDropTable == -1) {
                cout << "There are no created tables or the table doesn't exist" << endl; return 0;
            }


        }
        else cout << "the tables.txt file couldn't open";

    }



    void deletingFromTables(string theTable)
    {

        string line, name;
        cout << "Please Enter the name of record you want to delete: ";
        name = theTable;
        ifstream myfile;
        ofstream temp;
        myfile.open("tables.txt");
        temp.open("temp.txt");
        while (getline(myfile, line))
        {
            if (line != name)
                temp << line << endl;
        }
        cout << "The record with the name " << name << " has been deleted if it exsisted" << endl;
        myfile.close();
        temp.close();
        remove("tables.txt");
        rename("temp.txt", "tables.txt");
    }



};

class UserInput {

public:
    int userChoice() {
        INSERT insertObject;
        // DISPLAY_TABLE displayObject;



        cout << "***********************************************************************************************************";

        cout << endl;
        cout << "                                       Welcome to MySQLveryLite engine 2.0!"; cout << endl;
        cout << "                                            (it doesn't have a menu!)"; cout << endl; cout << endl;


        string sqlText;
        string line;
        ifstream myfile("comenzi.txt");



        cout << "Insert a command (or EXIT to exit):";
        if (myfile)
        {
            getline(myfile, line);

        }
        else cout << "We' ve got a problem \n";
        do {
            sqlText = line;


            cout << endl;
            string spaceDelimiter = " ";
            string token;

            size_t pos = 0;
            vector<string> commandArray;

            while ((pos = sqlText.find(spaceDelimiter)) != string::npos) {
                token = sqlText.substr(0, pos);
                commandArray.push_back(token);
                sqlText.erase(0, pos + spaceDelimiter.length());
            }
            commandArray.push_back(sqlText);

            string command = commandArray[0] + " " + commandArray[1];

            if (commandArray.size() <= 2)
            {
                cout << "Not a valid sql command";
                return 0;
            }

            if (command == "EXIT") { cout << "press X"; return 0; }

            else if (command == "CREATE TABLE")
            {
                string tabelele[50];
                for (int i = 0; i < 50; i++) tabelele[i] = "nothing";
                int i = 0;

                ifstream fisTabele("tables.txt", ios::in);
                if (fisTabele.is_open()) {
                    while (fisTabele >> tabelele[i]) { i++; }
                    fisTabele.close();
                }
                else {
                    cout << endl << "****** The file is not opened*****";
                }

                string tableName = commandArray[2];
                int ok = 1;

                for (int j = 0; j < i; j++) {
                    if (tabelele[j] == commandArray[2]) {
                        ok = 0; cout << endl << "the table already exists "; break;
                    }
                }

                if (ok == 1) {

                    CREATE_TABLE tableObject;
                    tableObject.createFromSQLCommand(commandArray[2], commandArray[3]);

                }
            }

            else if (command == "CREATE INDEX")
            {
                string tableName = commandArray[2];

            }

            else if (command == "DROP TABLE")
            {
                DROP_TABLE dropTableObject;
                dropTableObject.deletingTxtandBin(commandArray[2]);
                dropTableObject.deletingFromTables(commandArray[2]);

            }

            else if (command == "DROP INDEX")
            {
                string tableName = commandArray[2];

            }

            else if (command == "DISPLAY TABLE")
            {

                string tabelele[50];
                for (int i = 0; i < 50; i++) tabelele[i] = "nothing";
                int i = 0;

                ifstream fisTabele("tables.txt", ios::in);
                if (fisTabele.is_open()) {
                    while (fisTabele >> tabelele[i]) { i++; }
                    fisTabele.close();
                }
                else {
                    cout << endl << "****** The file is not opened*****";
                }


                string tableName = commandArray[2];
                int ok = 0;

                for (int j = 0; j < i; j++) {
                    if (tabelele[j] == commandArray[2]) {
                        ok = 1;
                        break;
                    }
                }
                // if (ok == 0) cout << "the table to display doesn't exist";
                if (ok == 1) {
                    DISPLAY_TABLE displayObject;
                    displayObject.displayTableFunction(commandArray[2]);
                }
            }
            else if (command == "INSERT INTO")
            {

                if (commandArray.size() != 4)
                {
                    cout << "invalid insert command";
                    return 0;
                }
                string tableName = commandArray[2];

                string firstCommandPart = commandArray[3].substr(0, 6);
                if (firstCommandPart != "VALUES")
                {
                    cout << "Keyword values doesnot exist";
                    return 0;
                }

                insertObject.insertFromSQLCommand(tableName, commandArray[3].substr(6));

            }
            /* else if (command == "SELECT ALL" )
            {
                if (commandArray[2] == "FROM")
                {
                    string tableName = commandArray[3];
                    SELECT selectobject;
                    selectobject.ExecuteALL(tableName);
                    selectobject.binary(tableName);
                }
            }
             else if (commandArray[0] == "SELECT" && commandArray[1]!="ALL")
            {
            string tableName = commandArray[2];}
            */


            else if (command == "DELETE FROM")
            {
                DELETE deleteObject;
                /*  eleteObject.deletingTxtandBin(commandArray[2]);*/
                cout << commandArray[4];

            }

            else
            {
                cout << "The command entered is either invalid or not supported by the current version of the application";
            }

        } while (getline(myfile, line)); //pe asta trebuie neaparat sa o modific !!
        myfile.close();

        return 0;
    }
};

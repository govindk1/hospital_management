#include <bits/stdc++.h>
#include <windows.h>
#include <mysql.h>
#include <cstdlib>
#include <conio.h>


using namespace std;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES * res;
    MYSQL_FIELD *field;

struct data {
int ival;
char cval[10];
float fval;
string sval;
};

void start();

string return_string(int count, string arrvalue[], string modified_arr[], data d[])
{

        stringstream ssinsert;
        for(int i = 0; i < count; i++)
             {
                if(modified_arr[i] == "varchar")
            {
                if(i < count - 1 )
                ssinsert << "'" << d[i].sval << "'" << ", ";

                else
                    ssinsert << "'" << d[i].sval << "'";

            }

            else if(modified_arr[i] == "int")
            {
                if(i < count - 1 )
                ssinsert << d[i].ival << ", ";

                else
                    ssinsert << d[i].ival;
            }

            else if(modified_arr[i] == "char")
            {
                if(i < count - 1 )
                ssinsert << "'" << d[i].cval << "'" << ", ";

                else
                    ssinsert << "'" << d[i].cval << "'";
            }

            else
            {
                if(i < count - 1 )
                ssinsert << "'" << d[i].sval << "'" << ", ";

                else
                    ssinsert << "'" << d[i].sval << "'";

            }
             }

           string q =  ssinsert.str();
           return q;
}


void insert_(string table_name)
{

    data d[10];
    int qstate = 0;
    stringstream ss;
    ss << "DESC " << table_name;

    string q = ss.str();
    const char* q1 = q.c_str();

    qstate = mysql_query(conn, q1);
    res = mysql_use_result(conn);
    int length = mysql_num_fields(res);

    string arrdesc[length];
    string arrvalue[length];
    int kdesc = 0, kvalue = 0;
    int count = 0;

        while(row= mysql_fetch_row(res))
            {
                arrdesc[kdesc] = row[1];  // it will give all the value present in row1
                kdesc++;
                count++;
            }

        qstate = mysql_query(conn, q1);
        res = mysql_use_result(conn);
        while(row= mysql_fetch_row(res))
            {
                arrvalue[kvalue] = row[0];
                kvalue++;

            }





        string modified_arr[count];

        cout << endl;

        for(int i = 0; i < count; i++)
        {
            int j = 0;
            while(isalpha(arrdesc[i][j]))
            {
                j++;
                if(j > 7)
                    break;
            }

            stringstream  ss1;

            for(int k = 0; k < j; k++)
                {
                    ss1 << arrdesc[i][k];
                }
            modified_arr[i] = ss1.str();
        }

        for(int i = 0; i < count; i++)
            cout << modified_arr[i] << " ";

        cout << endl;

       cout << "Fill The Details\n";

        cout << endl;


        for(int i = 0; i < count; i++)
        {
            cout << "enter " << arrvalue[i] <<"  ";
            if(modified_arr[i] == "varchar")
            {
                cin.ignore();
                getline(cin, d[i].sval);

            }

            else if(modified_arr[i] == "int")
            {
                cin >> d[i].ival;

            }

            else if(modified_arr[i] == "char")
            {
                cin >> d[i].cval;
            }

            else
            {
                cin.ignore();
                cin >> d[i].sval;

            }

        }
        string return_string1;
       return_string1 = return_string(count, arrvalue, modified_arr, d);

       stringstream insert_to_mysql;
       insert_to_mysql <<"insert into " << table_name << " values( " << return_string1 << ")";

       string qin = insert_to_mysql.str();
       const char* q2 = qin.c_str();

       qstate = mysql_query(conn, q2);

       if(!qstate)
       {
           cout << "inserted successfully\n";
       }

       else
       {
           cout << mysql_error(conn) << endl;
       }

       _getch();
       system("cls");
       start();
}

MYSQL_RES* select_all(string table_name, int &length, int &qstate, string& return_)
{


    stringstream selectall;
    selectall << "select * from " << table_name;

    return_ = selectall.str();
    const char * q = return_.c_str();

    qstate = mysql_query(conn, q);

    res = mysql_use_result(conn);
    length = mysql_num_fields(res);

    while(field = mysql_fetch_field(res))
    {
        cout << field->name << " ";
    }

    cout << endl;
    return res;


}

MYSQL_RES* specified_column(string table_name, int& count_no_of_columns, int& qstate, string& return_)
{
    cout << "enter  column \n";
    string enter_column;
    cin.ignore();
    getline(cin, enter_column);      // since space  is there

    for(int i = 0; i < enter_column.length(); i++)
    {
        if(enter_column[i] == ' ')
        {
            enter_column[i] = ',';
            count_no_of_columns++;
        }
    }


    stringstream select_specified;
    select_specified << "select " << enter_column << " from " << table_name;


   return_ = select_specified.str();
    const char* q1 = return_.c_str();


    qstate = mysql_query(conn, q1);
    res = mysql_use_result(conn);

    return res;

}


void select_column(string table_name)
{
    printf("enter \"1\" to select all columns of a given table\n");
    printf("enter \"2\" to select specified columns of a given table\n");

    int condition; cin >> condition;
    int length, qstate = 0;
    string return_;


    if(condition == 1)
        {

            string q;
            res = select_all(table_name, length, qstate, return_);

                if(!qstate)
                {
                    while(row = mysql_fetch_row(res))
                    {
                        for(int i = 0; i < length; i++)
                        {
                            if(row[i])
                            cout << row[i] << " ";

                            else
                            cout << "NULL ";
                        }

                        cout << endl;


                    }
                }

                else
                    cout << mysql_error(conn);

        }


        else if(condition == 2)
        {
            int count_no_of_columns = 1;
            res = select_all(table_name, length, qstate, return_);
            mysql_free_result(res);
            res = specified_column(table_name, count_no_of_columns, qstate, return_);

            if(!qstate)
            {
                while(row = mysql_fetch_row(res))
                {
                    for(int i = 0; i < count_no_of_columns; i++)
                    {
                        if(row[i])
                        cout << row[i] << " ";

                        else
                        cout << "NULL ";
                    }

                cout << endl;
            }
            }
        }
        _getch();
       system("cls");
       start();
}


void select_with_condition(string table_name)
{
    printf("enter \"1\" to select all columns \"WITH CONDTION\"\n");
    printf("enter \"2\" to select specified columns \"WITH CONDITION\"\n");

    int condition; cin >> condition;
    int length, qstate = 0;
    string return_;

        if(condition == 1)
        {

           res = select_all(table_name,length,qstate, return_);
           mysql_free_result(res);

           cout << "enter the condtion\n";
           string condition_query;

           cin.ignore();
           getline(cin, condition_query);

           stringstream ss;
           ss << return_ << " where " << condition_query;

           string q = ss.str();
           const char* q1 = q.c_str();

           qstate =  mysql_query(conn, q1);
           res = mysql_use_result(conn);
           length = mysql_num_fields(res);

           if(!qstate)
                {
                    while(row = mysql_fetch_row(res))
                    {
                        for(int i = 0; i < length; i++)
                        {
                            if(row[i])
                            cout << row[i] << " ";

                            else
                            cout << "NULL ";
                        }

                    cout << endl;
                    }
                }

                else
                    cout << mysql_error(conn) << endl;


        }

        else if(condition == 2)
        {
           res = select_all(table_name,length,qstate, return_);
           mysql_free_result(res);

           return_ = " ";

           res = specified_column(table_name, length, qstate, return_);
           mysql_free_result(res);

           cout << "enter the condtion" << endl;
           string condition_query;
           getline(cin, condition_query);

           stringstream ss;
           ss << return_ << " where " << condition_query;

           string q = ss.str();
           const char* q1 = q.c_str();

           qstate =  mysql_query(conn, q1);
           res = mysql_use_result(conn);
           length = mysql_num_fields(res);

           if(!qstate)
                {
                    while(row = mysql_fetch_row(res))
                    {
                        for(int i = 0; i < length; i++)
                        {
                            if(row[i])
                            cout << row[i] << " ";

                            else
                            cout << "NULL ";
                        }

                    cout << endl;
                    }
                }

                else
                    cout << mysql_error(conn) << endl;
        }
        _getch();
       system("cls");
       start();

}

void update(string table_name)
{
    int length, qstate = 0;
    string return_;

    cout << "choose the column from the given column\n";

    res = select_all(table_name,length,qstate, return_);
    mysql_free_result(res);

    cout << endl;

    cout << "enter column name:  ";
    string column_name; cin >> column_name;

    stringstream ss;
    ss << "DESC " << table_name;

    string q = ss.str();
    const char* q1 = q.c_str();

    qstate = mysql_query(conn, q1);
    res = mysql_use_result(conn);

    string arrdesc;

        while(row= mysql_fetch_row(res))
            {
                if(row[0] == column_name)
                arrdesc = row[1];  // it will give all the value present in row1

            }

    cout << "enter value to update:  ";
    string value_update; cin >> value_update;

    cin.ignore();
    cout << "enter condition:  ";
    string condition; getline(cin, condition);



    stringstream ss1;

    if(arrdesc[0] == 'i' || arrdesc[0] == 'I')
        ss1 << "update " << table_name << " set " << column_name<< " = " << value_update << " where " << condition;

    else
        ss1 << "update " << table_name << " set " << column_name<< " = " << "'"+value_update+"'" << " where " << condition;

    string q2 = ss1.str();
    const char* q3= q2.c_str();

    mysql_free_result(res);

    qstate = mysql_query(conn, q3);

    if(!qstate)
    {
        cout << "Inserted successfully\n";
    }

    else
    {
        cout << mysql_error(conn) << endl;
    }

    _getch();
    system("cls");
    start();


}

void delete_(string table_name)
{

    int length, qstate = 0;
    string return_;
    res = select_all(table_name,length,qstate, return_);
    mysql_free_result(res);

    string condition;
    cin.ignore();
    cout << "enter the condition\n";
    getline(cin, condition);

    stringstream ss;
    ss << "delete from " << table_name << " where " << condition ;


    string q1 = ss.str();

    const char* q2 = q1.c_str();

    qstate = mysql_query(conn, q2);

    if(!qstate)
        cout << "Deleted Successfully...\n";

    else
        cout << mysql_error(conn);

    _getch();
    system("cls");
    start();
}

void count_no_of_entries(string table_name)
{
    string return_;
    int length = 0, qstate = 0;
    int count_ = 0;

    res = select_all(table_name, length, qstate, return_);

    while(row = mysql_fetch_row(res))
        count_++;

    cout << count_ << endl;

    _getch();
    system("cls");
    start();
}

void check()
{


    int flag = 0;
    int qstate = 0;
    cout << "enter user name: ";
    string user_name; cin >> user_name;

    string pass ="";
    char ch;
    cout << "Enter pass\n";
    ch = _getch();

   while(ch != 13){//character 13 is enter
        if(ch == 8)
            pass.pop_back();
        else
        pass.push_back(ch);

      ch = _getch();
   }

 qstate = mysql_query(conn, "select * from user");

 res = mysql_use_result(conn);

 while(row = mysql_fetch_row(res))
 {

     if(row[0] == user_name && row[1] == pass)
        {cout << "connected successfully\n";
            flag = 1;}

     else
        cout << "Wrong Password\n";

 }
        _getch();

    if(flag == 0)
        {   system("cls");
            check();
        }

    else
        system("cls");




}


void start()
{
    string table_name;
    cout << "1: insert data\n";
    cout << "2: select column\n";
    cout << "3: select column with condition\n";
    cout << "4: to update\n";
    cout << "5: to delete\n";
    cout << "6: to count no of entries\n";
    cout << "7: to exit\n";

    int input = 0;
	cin >> input;

    if(input == 7)
        exit(0);

    else
    {cout << "enter table name:  "; cin >> table_name;}

	switch(input)
	{

		case 1: system("cls");
                insert_(table_name);
				break;

        case 2: system("cls");
                select_column(table_name);
                break;

        case 3: system("cls");
                select_with_condition(table_name);
                break;

        case 4: system("cls");
                update(table_name);
                break;

        case 5: system("cls");
                delete_(table_name);
                break;

        case 6: system("cls");
                count_no_of_entries(table_name);
                break;
        }



    system("cls");
}

int main()
{

    conn  = mysql_init(0);
    conn = mysql_real_connect(conn,  "host", "user", "password", "database_name", 0, NULL, 0);
    check();

    conn  = mysql_init(0);
    conn = mysql_real_connect(conn,  "host", "user", "password", "database_name", 0, NULL, 0);
    start();

}

#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define HOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "salvadorgroc"
#define DATABASE "dbgame"


std::string sUserInput;

using namespace std;

int main()
{

    //Preguntamos nombre dde usuario
    std::cout << "Inserte nombre de usuario: " << std::endl;
    std::cin >> sUserInput;

    try
    {

        //Conectamos a base de datos.
        sql::Driver* driver = get_driver_instance();
        sql::Connection* con = driver->connect(HOST, USER, PASSWORD);
        con->setSchema(DATABASE);
        sql::Statement* stmt = con->createStatement();

        //Comprobamos si existe el nick del jugador
        sql::ResultSet* res = stmt->executeQuery("SELECT count(*) FROM Jugadores WHERE Nombre = '" + sUserInput + "'");


        if(res->next() && res->getInt(1) == 1) //Existe
        {
            std::cout << "Existe" << std::endl;

        }
        else //No existe
        {
            std::cout << "No existe" << std::endl;

        }
    }

    /*
        try
        {

            sql::Driver* driver = get_driver_instance();
            sql::Connection* con = driver->connect(HOST, USER, PASSWORD);

            con->setSchema(DATABASE);
            sql::Statement* stmt = con->createStatement();
            sql::ResultSet* res = stmt->executeQuery("Select Nombre, Pass from Jugadores");
            std::cout<<"USERNAME     |      USERPASSWORD"<<std::endl;
            while(res->next())
            {
                std::cout<<res->getString("Nombre")<<"      |      "<<res->getString("Pass")<<std::endl;
            }
            delete(res);

            res = stmt->executeQuery("SELECT count(*) FROM Jugadores WHERE Nombre = 'Player1' and Pass = '123' ");

            if(res->next())
            {
                int existe = res->getInt(1);
                if (existe == 1)
                    std::cout << "El usuario existe en la BD. La autenticacion es correcta." <<std::endl;
                else
                    std::cout <<"El usuario NO existe en la BD." <<std::endl;
            }
            delete(res);
            delete(stmt);
            delete(con);
        }
        */
    catch(sql::SQLException &e)
    {
        std::cout << "Se produce el error " << e.getErrorCode()<<std::endl;
    }
    return 0;
}

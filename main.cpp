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

using namespace std;

std::string
sUserNick,
sUserPass,
sUserPassRepeat;

bool
bVerified = false,
bPlayerCreated = false,
bRepeatPassword = false;




int main()
{
    try
    {

        //Conectamos a base de datos.
        sql::Driver* driver = get_driver_instance();
        sql::Connection* con = driver->connect(HOST, USER, PASSWORD);
        con->setSchema(DATABASE);
        sql::Statement* stmt = con->createStatement();

        while(!bVerified)
        {

            //Preguntamos nombre de usuario
            std::cout << "Inserte nombre de usuario: " << std::endl;
            std::cin >> sUserNick;

            //Comprobamos si existe el nick del jugador
            sql::ResultSet* res = stmt->executeQuery("SELECT count(*) FROM Jugadores WHERE Nombre = '" + sUserNick + "'");


            if(res->next() && res->getInt(1) == 1) //Existe usuario
            {
                //Pedimos contraseña
                std::cout << "\nIntroduzca contraseña" << std::endl;
                std::cin >> sUserPass;

                //Comprobamos si existe usuario con dichos datos
                sql::ResultSet* res = stmt->executeQuery("SELECT count(*) FROM Jugadores WHERE Nombre = '" + sUserNick + "' AND Pass = '" + sUserPass + "'");

                if(res->next() && res->getInt(1) == 1) //Existe usuario con contraseña
                {
                    //Se han validado los datos y puede iniciar el juego.
                    bVerified = true;
                }
                else // No existe usuario con contraseña
                {
                    std::cout << "\nNo se ha encontrado ningun usuario con esa contraseña.\n" << std::endl;
                }
            }
            else //No existe usuario
            {
                std::cout << "\nNo existe dicho usuario. Se va a proceder a crear un nuevo usuario.\n" << "\nInserte nombre de usuario" <<std::endl;
                while(!bPlayerCreated)//Mientras no este el jugador creado repetiremos
                {
                    //Pedimos al usuario que indique un nuevo usuario

                    std::cin >> sUserNick;

                    //Comprobamos si el nick esta libre
                    sql::ResultSet* res = stmt->executeQuery("SELECT count(*) FROM Jugadores WHERE Nombre = '" + sUserNick + "'");
                    if(res->next() && res->getInt(1) == 1) //Existe usuario
                    {
                        std::cout << "Usuario ya en uso, inserte otro nombre de usuario." << std::endl;
                    }
                    else  //No existe usuario
                    {
                        std::cout << "Usuario disponible, inserte contraseña.\n";
                        while(!bRepeatPassword)
                        {
                            std::cin >> sUserPass;
                            std::cout << "\nRepita contraseña.\n";
                            std::cin >> sUserPassRepeat;

                            if(sUserPass == sUserPassRepeat)
                            {

                                stmt->execute("INSERT INTO Jugadores (Nombre, Pass) VALUES ('"+ sUserNick +"', '"+ sUserPass +"')");
                                bRepeatPassword = true;
                                bPlayerCreated = true;
                                bVerified = true;

                            }
                            else
                            {
                                std::cout << "\nLas contraseñas no coinciden, introduzca de nuevo la contraseña.\n";

                            }
                        }
                    }
                }
            }
        }
        system("clear");

        std::cout << "Empieza el juego.";

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

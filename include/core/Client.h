#ifndef _FURETTP_CLIENT_H
#define _FURETTP_CLIENT_H

#include "network/tcp/Socket.h"
#include "exception/SystemException.h"

#include "message/answer/ConnectionInitializationAnswer.h"
#include "core/RequestHandler.h"
#include "core/message/request/RequestFactory.h"

namespace FuretTP {

    class FTPServer;

    class Client {

    public:
        Client(FTPServer* server, TCP::Socket& socket);

        void run();

		/// \brief set username of the client
		bool setUsername(const std::string& username);

		/// \brief try to loggin with the previous username.
		bool login(const std::string& password);

		/// \brief reset loggin information
		void resetLogin();

		const std::string& getUsername() const;
		TCP::Socket& getSocket();

    private:
        unsigned int _uid;
        TCP::Socket _socket;
        FTPServer* _server;

        static unsigned int _uidCounter;

		std::string _username;
		std::string _password;
    };
}

#endif

# Conception d'application réparties - Mise en place d'un serveur FTP
#### Nicolas CACHERA - Pierre FALEZ
###### 29/01/2015

#### Introduction

Cette application est un serveur FTP classique permettant d'exécuter les commandes les plus classiques. Le serveur nécessite un compte afin de pouvoir s'y connecter, comptes stockés dans le fichier user.conf. Toutefois, il existe un compte anonyme ayant pour identifiant "anonymous" et pour mot de passe "_".

Voici la liste des commandes disponibles sur le serveur
* CDUP -> Equivalent à CWD ..
* CWD directory -> Permet de changer le répertoire courant du serveur.
* FEAT -> Permet de connaitre d'éventuelles commandes non-habituelles supportées par le serveur.
* LIST -> Affiche la liste des fichiers présent dans le répertoire courant du serveur.
* MKD directory -> Supprime le dossier directory dans le répertoire courant du serveur.
* PASS password -> Permet d'entre le mot de passe utilisateur.
* PASV -> Permet d'activer le mode passif.
* PORT -> Permet d'ouvrir une nouvelle connexion.
* PWD -> Affiche le chemin du répertoire courant du serveur.
* QUIT -> Permet de quitter le serveur.
* RETR filename -> Permet de récuperer un fichier sur le serveur.
* RMD directory -> Crée le dossier directory dans le répertoire courant du serveur.
* STOR filename -> Permet de déposer un fichier sur le serveur.
* SYST -> Demande des informations sur le système du serveur.
* TYPE -> Définis le type de fichier à transférer.
* USER username -> Permet d'entrer le login utilisateur.

#### Architecture

Cette application comprends 4 packages : core, exception, system et network

Le package core contient tout ce qui concerne le serveur en lui même. Il est composé de quelques classes et d'un sous package (message) lui même divisé en deux packages :
* answer : Contient tout ce qui concerne les types de réponse à une requête
* request : Contient tout ce qui concerne les requêtes

Le package exception contient, comme son nom l'indique, tout ce qui concerne les exceptions

Les packages system et network sont des packages contenant toute l'encapsulation de méthode C permettant de manipuler les dossier (system) et les sockets et adresses IP (network).

L'application fonctionne de la sorte :
* Le serveur est lancé, il attends une connexion.
* Lorsqu'une connexion est reçue, le serveur en crée un utilisateur et lui créer un nouveau thread pour pouvoir continuer de recevoir d'autres connexions.
* Une connexion est créée sous la forme de la classe Client, elle est utilisée pour reçevoir toutes les requêtes que l'utilisateur lui enverra.
* Elle garde en mémoire, grâce à la classe User, le nom d'utilisateur et le dossier courant de l'utilisateur.
* Une fois une requête reçu, la classe User la prend et l'envoie à la RequestFactory afin de récupérer la bonne classe correspondant à la requête.
* La RequestFactory va en réalité prendre le premier argument de la requête et le comparer aux commandes qu'elle connait. Si elle trouve la commande, elle crée la requête avec l'aide des arguments suivants si nécessaire. Sinon, elle retourne un pointeur null.
* Enfin, la classe User enverra cette classe au RequestHandler qui s'occupera de distribuer la classe à la bonne méthode afin qu'elle puisse l'executer et renvoyer d'elle même la réponse. Si la classe retournée est un pointeur null, le client enverra une réponse de type AnswerUnimplemented à l'utilisateur.
* Plusieurs classe de réponse sont précréée, chacune liée à un code. Un texte peut y être ajouté en tant qu'argument pour accompagner le code qui sera affiché à l'utilisateur.

Voici les différentes erreurs pouvant être attrapées ou lancées par l'application :
* THROW(UnrecognizedMessageException, "PORT "+raw_address, std::string(token)+" is an incorrect number") par la méthode eval de la classe RequestFactory.
* THROW(UnrecognizedMessageException, "PORT "+raw_address, "too many arguments") par la méthode eval de la classe RequestFactory.
* THROW(UnrecognizedMessageException, "PORT "+raw_address, "not enough argument") par la méthode eval de la classe RequestFactory.
* THROW(UnrecognizedMessageException, "TYPE", "Unrecognized type "+type_char) par la méthode eval de la classe RequestFactory. Lancée lorsque le type de fichier à transferer demander par l'utilisateur est inconnu
* THROW(NoActiveConnectionException, "") par la méthode openDataConnection de la classe Client. Lancée lorsque le serveur tente d'ouvrir une nouvelle connexion active alors qu'aucun port n'a été spécifié.
* try { ... } catch(SystemException& e) { ...	} par la méthode processListConnection de la classe RequestHandler. L'exception est attrapée lorsque le répertoire donné avec la requête liste n'est pas atteignable. Une réponse de type AnswerFileUnavailable sera alors envoyée à l'utilisateur
* THROW(UserNotFoundException, username) par la méthode findUser de la classe UserList. Lancée lorsque l'utilisateur cherché n'a pas été trouvé.
* THROW(FileNotFoundException, pathname) par la méthode process de la classe UserConfigurationReader. Lancée lorsque le fichier de configuration n'a pas été trouvé.
* THROW(IncorrecteFileFormatException, FILE_FORMAT_LINE, line_number, pathname) par la méthode process de la classe UserConfigurationReader. Lancée lorsque la syntaxe du fichier config est incorrecte.
* try { ... }	catch(const Exception& e) { ... } par la méthode main. Attrape l'exception lorsque le serveur à lancé une exception. La méthode envoie alors un message aux utilisateurs indiquant que le serveur a planté.

#### Code Samples

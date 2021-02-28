#include <iostream>
#include <stdio.h>
#include <sqlite3.h>

#include "utils.h"
#include "models.h"
#include "crypt.h"
#include "db.h"
#include "globals.h"

/* Login Prompt */ 
User login(sqlite3 *db) {
	User user;
	struct user_data user_d;
	std::string username, password;
	char *zErrMsg = 0;
	
	getchar(); // Flush stdin

	std::cout << "username : " ;
	std::getline(std::cin, username);
	
	echo(false); // Disable terminal echo
	std::cout << "password : " ;
	std::getline(std::cin, password);
	echo(true); // Enable terminal echo	
	std::cout << std::endl;
	

	user_d = get_user_by_username(username, db); // get user data from database

	// check if user data is not null
	if(user_d.id.compare("") != 0) {
		std::string hashed_password = sha256(password); // calculate sha256 hash of the input password

		// compare hashed input password with hash stored in database
		if(hashed_password.compare(user_d.password) == 0) { 
			user = User(username, password, hashed_password);
			std::cout << "[SUCCESS] Logged in successfully" << std::endl;
		} else {
			std::cerr << "[WARN] Password or username is wrong" << std::endl;
		}
	} else {
		std::cerr << "[WARN] User " << username << " doesnt exist" << std::endl;
	}

	return user;
}

User signup(sqlite3 *db) {
	User user;
	struct user_data user_d ;
	std::string username, password, hashed_password;

	getchar(); // Flush stdin

	std::cout << "username : " ;
	std::getline(std::cin, username);
	user_d = get_user_by_username(username, db); // check if username already exists

	if(user_d.id.compare("") != 0) {
		std::cerr << "[WARN] username " << username << " already exists" << std::endl;
		return user;
	}

	if(username.length() < 6) {
		std::cerr << "[WARN] Minimum length of username is : 6" << std::endl;
		return user;
	}

	echo(false); // Disable terminal echo
	std::cout << "password : " ;
	std::getline(std::cin, password);
	echo(true); // Enable terminal echo	
	std::cout << std::endl;
	
	hashed_password = sha256(password);
	if(create_user(db, username, hashed_password) == 0) {
		user = User(username, password, hashed_password);
		std::cout << "[SUCCESS] New user has been created" << std::endl;
	} 

	return user;
}

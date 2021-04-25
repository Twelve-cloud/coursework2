CREATE DATABASE IF NOT EXISTS BrokerBase;
USE BrokerBase;

CREATE TABLE IF NOT EXISTS Account 
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    AccountLogin 	VARCHAR(50) CONSTRAINT ch_login CHECK (AccountLogin REGEXP '^[:alpha:][[:alnum:]_]{4,14}[:alnum:]$') UNIQUE,
    AccountPassword VARCHAR(50) CONSTRAINT ch_passw CHECK (AccountPassword REGEXP '^[[:alnum:]_]{6,16}$'),
    MobileNumber 	VARCHAR(50) CONSTRAINT ch_mnumb CHECK (MobileNumber REGEXP '^[+][:digit:]{3}[(][:digit:]{2}[)][:digit:]{3}-[:digit:]{2}-[:digit:]{2}$'),
    Email 			VARCHAR(50) CONSTRAINT ch_email CHECK (Email REGEXP '^[:alpha:][[:alnum:]_]{5,15}[@][:alpha:]{2,10}[\.][:alpha:]{2,3}$'),
    Rolename		VARCHAR(50) CONSTRAINT ch_rolen CHECK (Rolename IN ('BROKER', 'CONSULTANT', 'USER'))
);
    
CREATE TABLE IF NOT EXISTS BanList 
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    AccountID INT NOT NULL,
    started DATETIME NOT NULL,
    ended DATETIME NOT NULL,
    CONSTRAINT fkey_account FOREIGN KEY (AccountID) REFERENCES Account(ID) ON DELETE RESTRICT ON UPDATE CASCADE
);

INSERT INTO Account(AccountLogin, AccountPassword, MobileNumber, Email, Rolename) VALUES ('ilyasavin', '26091999', '+375(44)774-41-44', 'ilyasavin@mail.ru', 'USER');
SELECT * FROM Account;

INSERT INTO BanList(AccountID, started, ended) VALUES (1, '2021-04-23', '2026-04-23');
SELECT * FROM BanList;

DROP TABLE IF EXISTS BanList;
DROP TABLE IF EXISTS Account;
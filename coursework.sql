CREATE DATABASE IF NOT EXISTS BrokerBase;
USE BrokerBase;

CREATE TABLE IF NOT EXISTS Account 
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    AccountLogin 	VARCHAR(50) CONSTRAINT ch_login CHECK (AccountLogin REGEXP '^[:alpha:][[:alnum:]_]{4,14}[:alnum:]$') UNIQUE,
    AccountPassword VARCHAR(50) CONSTRAINT ch_passw CHECK (AccountPassword REGEXP '^[[:alnum:]_]{6,16}$'),
    MobileNumber 	VARCHAR(50) CONSTRAINT ch_mnumb CHECK (MobileNumber REGEXP '^[+][:digit:]{3}[(][:digit:]{2}[)][:digit:]{3}-[:digit:]{2}-[:digit:]{2}$') UNIQUE,
    Email 			VARCHAR(50) CONSTRAINT ch_email CHECK (Email REGEXP '^[:alpha:][[:alnum:]_]{5,15}[@][:alpha:]{2,10}[\.][:alpha:]{2,3}$') UNIQUE,
    Rolename		VARCHAR(50) CONSTRAINT ch_rolen CHECK (Rolename IN ('BROKER', 'CONSULTANT', 'USER')) DEFAULT 'USER'
);
    
CREATE TABLE IF NOT EXISTS BanList 
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    AccountID INT NOT NULL UNIQUE,
    started DATE NOT NULL,
    ended DATE NOT NULL,
    CONSTRAINT fkey_account FOREIGN KEY (AccountID) REFERENCES Account(ID) ON DELETE RESTRICT ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Company 
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    CompanyName VARCHAR(50) CONSTRAINT ch_cname CHECK (CompanyName REGEXP '^[[:alnum:] "]{6,16}$') UNIQUE
);

CREATE TABLE IF NOT EXISTS Service
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    ServiceName VARCHAR(50) CONSTRAINT ch_sname CHECK (ServiceName REGEXP '^[[:alnum:] "]{6,32}$'),
    ServicePrice DOUBLE(8, 2) NOT NULL,
    CompanyName VARCHAR(50),
    CONSTRAINT fkey_company FOREIGN KEY (CompanyName) REFERENCES Company(CompanyName) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS Basket
(
	ID INT AUTO_INCREMENT NOT NULL PRIMARY KEY,
    AccountID INT,
    ServiceID INT,
    CONSTRAINT fkey_acc FOREIGN KEY(AccountID) REFERENCES Account(ID) ON DELETE RESTRICT ON UPDATE CASCADE,
    CONSTRAINT fkey_service FOREIGN KEY(ServiceID) REFERENCES Service(ID) ON DELETE CASCADE ON UPDATE CASCADE
);

-- -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

INSERT INTO Account(AccountLogin, AccountPassword, MobileNumber, Email, Rolename) VALUES ('ilyasavin', '26091999', '+375(44)774-41-44', 'ilyasavin@mail.ru', 'USER');
INSERT INTO BanList(AccountID, started, ended) VALUES ((SELECT ID FROM Account WHERE AccountLogin = 'ilyasavin'), '2021-04-23', '2026-04-23');
INSERT INTO Company(CompanyName) VALUES ('Газпром');
INSERT INTO Company(CompanyName) VALUES ('Компания Ильи');
INSERT INTO Service(ServiceName, ServicePrice, CompanyName) VALUES ('Страховка газа', 999.99, 'Газпром');
INSERT INTO Service(ServiceName, ServicePrice, CompanyName) VALUES ('Страховка нефти', 888.88, 'Газпром');
INSERT INTO Service(ServiceName, ServicePrice, CompanyName) VALUES ('Страховка газа', 333.99, 'Компания Ильи');
INSERT INTO Service(ServiceName, ServicePrice, CompanyName) VALUES ('Страховка нефти', 222.88, 'Компания Ильи');
INSERT INTO Basket(AccountID, ServiceID) VALUES ((SELECT ID FROM Account WHERE AccountLogin = 'ilyasavin'), (SELECT ID FROM Service WHERE ServiceName = 'Страховка газа' AND CompanyName = 'Компания Ильи'));

DELETE FROM Service;

SELECT * FROM Account;
SELECT * FROM BanList;
SELECT * FROM Company;
SELECT * FROM Service;
SELECT * FROM Basket;

DROP TABLE IF EXISTS Basket;
DROP TABLE IF EXISTS Service;
DROP TABLE IF EXISTS Company;
DROP TABLE IF EXISTS BanList;
DROP TABLE IF EXISTS Account;
# Webserver

## Table of content

 * [General project information](#general-project-information)
 * [Technologies used](#technologies-used)
 * [Usage](#usage)
 * [Some information about the code](#some-information-about-the-code)
 * [Contact](#contact)

## General project information

This project was carried out as part of the Systems and Networks practical course in semester 7 of the Embedded Systems course under the supervision of our teachers Mr Xavier Redon & Mr Thomas Vantroys.


## Technologies used

This project used several programming languages : 
* C (mainly)
* HTML
* CSS

## Usage

To use our program, you need to : 
 * initialize the server with the command : _"sioux/sioux -p 'port'"_ (8001)
 * open a web page with the following link : _"http://localhost:8001/vote.html"_
 * can use this following command in the terminal : _"GET /vote.html HTTP/1.1"_

## Some information about the code

### sioux

Sioux is the server name used to manage http connections and clients. The server is __multithreaded__, allowing several connections to be managed at the same time. In addition, the server is able to manage a voting system and store votes in a csv file.

### www

_'www'_ is the directory storing the html pages displayed and the csv file storing the number of votes. 
Pictures from drivers are not from me. They are used just for educational purposes.

### libs

_'libs'_ is the directory containing the Network and IPC libraries used by the server. The network library is used to create the socket and the connection listening loop on the server. The IPC library manages memory shared between sioux and ablette.
The Network directory contains the code produced and supplied by Mr Xavier Redon.

### ablette

_'ablette'_ is an independent executable for retrieving packets transiting the server. It transforms these packets into statistics and sends them to the IPC.

## Contact

Created by [@nemocazin] & Antoine Cegarra

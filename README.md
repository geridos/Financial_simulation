# Financial_simulation

AUTHOR:
Bertrand de Montety

Date : 05/01/2021

To compile the client and the server, just execute `make`

In test_scripts dir you will find the scripts to exectute the binaries
with already set parameters.

This app simulates a financial market platform that sends market data to its clients
The client binary takes a string request as parameter. Example : ./client "FB @ 20:50"
There are 2 modes :
    - 'mode 1' : send one market data when the data timestamp matches exactly the client request
    - 'mode 2' : start sending market data at the client request timestamp until there is no more data to send

Server execution example:
export MODE=2; ./server --input market_data.csv
export MODE=1; ./server --input market_data.csv

The server and clients communicate using sockets and the default port 8080

NOTE:
- If the client requests an date before than currentTime, the request will be discard.

IMPROVMENTS / BUGS:
- When 2 clients have the the same request, tasks are created twice and second client could get a delay in it responses.
- Requesting the current time for the calculations is costly to the system. Create a internal clock system.

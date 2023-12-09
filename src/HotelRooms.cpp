#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct HotelRoom {
	int roomNumber;
	bool isBooked;
	string customerName;
	string checkInDate;
	string checkOutDate;
	double roomPrice;
	int numberOfDays;
};

vector<string> split(string line, char c)
{
	string temp;
	stringstream stringstream{ line };
	vector<string> result;

	while (getline(stringstream, temp, c)) {
		result.push_back(temp);
	}

	return result;
}

bool toBoolean(string str)
{
	return str == "true" ? true : false;
}

int main()
{
	string fileName = "HotelRooms.txt";
	ifstream fileReader(fileName);
	ofstream fileWriter;

	vector<HotelRoom> rooms;

	const double baseRoomPrice = 50.0;
	const int roomsPerFloor = 5;

	double totalPrice;

	if ( fileReader.is_open() )
	{
		string strLine;
		while ( getline(fileReader, strLine) )
		{
			vector<string> line = split(strLine, ',');

			HotelRoom room;
			room.roomNumber = stoi(line[0].c_str());
			room.isBooked = toBoolean(line[1]);
			room.customerName = line[2];
			room.checkInDate = line[3];
			room.checkOutDate = line[4];
			room.roomPrice = stod(line[5]);
			room.numberOfDays = stoi(line[6]);

			rooms.push_back(room);
			
		}
		fileReader.close();

		cout << "Welcome to the Hotel Reservation System!" << endl;

		int choice;

		do 
		{
			cout << "\n1. Display available rooms\n"
				<< "2. Book a room\n"
				<< "3. Display customer information\n"
				<< "4. Exit\n"
				<< "Enter your choice: ";

			cin >> choice;

			switch (choice)
			{
			case 1:
				for (auto room : rooms)
				{
					cout << "Room: " << room.roomNumber << " : " << (room.isBooked ? "Booked" : "Available") << endl;
				}
				break;
			case 2:
				while (true)
				{
					int roomNumber;
					bool validRoom = false;

					cout << "Enter the room number you want to book: ";
					cin >> roomNumber;

					for (auto& room : rooms)
					{
						if (room.roomNumber == roomNumber)
						{
							if (!room.isBooked)
							{
								string customerName, checkInDate, checkOutDate;
								double roomPrice;

								cout << "Enter customer name: ";
								cin.ignore();
								getline(cin, customerName);

								cout << "Enter check-in date (YYYY-MM-DD): ";
								cin >> checkInDate;
								checkInDate += " - " + customerName;

								cout << "Enter check-out date (YYYY-MM-DD): ";
								cin >> checkOutDate;

								tm tmCheckIn = {}, tmCheckOut = {};
								istringstream issCheckIn(checkInDate), issCheckOut(checkOutDate);
								issCheckIn >> get_time(&tmCheckIn, "%Y-%m-%d - %*s");
								issCheckOut >> get_time(&tmCheckOut, "%Y-%m-%d");
								time_t timeCheckIn = mktime(&tmCheckIn);
								time_t timeCheckOut = mktime(&tmCheckOut);
								int days = static_cast<int>((timeCheckOut - timeCheckIn) / (60 * 60 * 24));

								roomPrice = days * baseRoomPrice * ((roomNumber - 1) / roomsPerFloor);

								room.isBooked = true;
								room.customerName = customerName;
								room.checkInDate = checkInDate;
								room.checkOutDate = checkOutDate;
								room.numberOfDays = days;
								room.roomPrice = roomPrice;

								cout << "Room " << room.roomNumber << " has been booked successfully!" << endl;
							}
							else
							{
								cout << "Sorry, Room " << room.roomNumber << " is already booked." << endl;
							}

							validRoom = true;
							break;
						}
					}
					if (validRoom)
						break;
					else
						cout << "Invalid room number. Please try again." << endl;
				}
				break;
			case 3:
				cout << "Customer Information:" << endl;
				cout << left << setw(10) << "Room" << setw(20) << "Customer"
					<< setw(15) << "Check-in Date" << setw(15) << "Check-out Date"
					<< setw(15) << "Days" << setw(15) << "Total Price" << endl;

				totalPrice = 0;

				for (auto room : rooms)
				{
					if (room.isBooked)
					{
						string checkInDateOnly = room.checkInDate.substr(0, 10);
						cout << left << setw(10) << room.roomNumber
							<< setw(20) << room.customerName
							<< setw(15) << checkInDateOnly
							<< setw(15) << room.checkOutDate
							<< setw(15) << room.numberOfDays
							<< setw(15) << "PHP" << fixed << setprecision(2) << room.roomPrice
							<< endl;

						totalPrice += room.roomPrice;
					}
				}

				cout << "Total Revenue: PHP" << fixed << setprecision(2) << totalPrice << endl;
				
				break;
			case 4:
				fileWriter.open(fileName, ofstream::trunc);

				for (auto room : rooms)
				{
					fileWriter << room.roomNumber << ","
						<< (room.isBooked ? "true" : "false") << ","
						<< room.customerName << ","
						<< room.checkInDate << ","
						<< room.checkOutDate << ","
						<< room.roomPrice << ","
						<< room.numberOfDays << endl;
				}

				fileWriter.close();
				break;
			default:
				cout << "Invalid input. Please check menu for valid input." << endl;
				break;
			}
		} 
		while (choice != 4);
	}
	else
	{
		cout << "HotelRooms.txt not found!";
	}
	return 0;
}
#include <iostream>
#include "../test_runner.h"
#include "../profile.h"
#include <string>
#include <set>
#include <map>
#include <cstdint>

using namespace std;


struct Book {
    Book(string hotel_, int client_, int room_) 
        :hotel(hotel_),client(client_),room(room_) 
    {}
    string hotel;
    int client=0;
    int room=0;
};

class HotelBooking {
public:
    void BOOK(int64_t time, string hotel_name, int client_id, int room_count) {
        Book cur_book(hotel_name, client_id,room_count);
        time_booked[time].push_back(cur_book);

        hotel_rooms[hotel_name] += room_count;
        

        if (time > current_time) {
            current_time = time;
        }

        auto del_unit = time_booked.lower_bound(current_time - 86399);
        if (del_unit != time_booked.begin()) {
            for (auto it = time_booked.begin();
                it != del_unit;
                it = time_booked.erase(it)) {
                
                auto deleted_elem = *it;
                for (auto b : deleted_elem.second) {
                    hotel_rooms[b.hotel] -= b.room;
                    hotel_clients[b.hotel].erase(b.client);
                }
                
            }
        }
        hotel_clients[hotel_name].insert(client_id);
    }


    int ROOM(const string& hotel_name) const{
        if (hotel_rooms.find(hotel_name) != hotel_rooms.end()) {
             return hotel_rooms.at(hotel_name);
        }
        return 0;
    }

    int CLIENTS(const string& hotel) const{
        if (hotel_clients.find(hotel)!=hotel_clients.end()) {
            return hotel_clients.at(hotel).size();
        }
 
        return 0;
    }
private:
    map<int64_t, vector<Book>> time_booked;
    int64_t current_time = 0;
    map <string, set<int>> hotel_clients;
    map <string, int> hotel_rooms;
   
};


void SpeedTest1() {
    {
        LOG_DURATION("MAX POWER")
            HotelBooking booker;
            for (int i = 0; i < 10000; i++) {
                booker.BOOK(i, to_string(i), i, i);
        }
    }
}

void SpeedTest2() {
    {
        LOG_DURATION("MAX POWER")
            HotelBooking booker;
        
        for (int i = 0; i < 1000; i++) {
            booker.BOOK(i, to_string(i), i, i);
        }
        {LOG_DURATION("Second");

        for (int j = 0; j < 4000; j++) {

            booker.ROOM("42342342");
        }
        }

        {
        LOG_DURATION("Third");
        for (int j = 0; j < 4000; j++) {
            booker.CLIENTS("432423");
        }
        }
    }
}



int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    

    TestRunner tr;
    RUN_TEST(tr, SpeedTest1);
    RUN_TEST(tr, SpeedTest2);

    HotelBooking manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
 
        
        if (query_type == "BOOK") {
            int time, client_id, room_count;
            string hotel_name;
            cin >> time >> hotel_name >> client_id >> room_count;
            manager.BOOK(time, hotel_name, client_id, room_count);
        }
        else if (query_type == "ROOMS") {
            string hotel;
            cin >> hotel;
            cout << manager.ROOM(hotel) << "\n";
        }
        else if (query_type == "CLIENTS") {
            string hotel;
            cin >> hotel;
            cout << manager.CLIENTS(hotel) << "\n";
        }
    }

    return 0;
}


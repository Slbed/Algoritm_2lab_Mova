#pragma once
#include <iostream>

struct Connection {
    int id;          
    int csInId;      
    int csOutId;     
    int diameter;   

    Connection(int pipeId, int inId, int outId, int diam)
        : id(pipeId), csInId(inId), csOutId(outId), diameter(diam) {
    }

    Connection() : id(0), csInId(0), csOutId(0), diameter(0) {} 

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn) {
        os << "Pipe " << conn.id << " (diameter: " << conn.diameter
            << " mm) connects CS " << conn.csInId << " -> CS " << conn.csOutId;
        return os;
    }
};
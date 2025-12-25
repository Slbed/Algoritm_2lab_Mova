#pragma once
#include <iostream>
#include <limits>

struct Connection {
    int id;
    int csInId; 
    int csOutId;
    int diameter;
    double weight;
    double capacity;

    Connection(int pipeId, int inId, int outId, int diam)
        : id(pipeId), csInId(inId), csOutId(outId), diameter(diam),
        weight(std::numeric_limits<double>::max()), capacity(0.0) {
    }

    Connection() : id(0), csInId(0), csOutId(0), diameter(0),
        weight(std::numeric_limits<double>::max()), capacity(0.0) {
    }

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn) {
        os << "Pipe " << conn.id << " (diameter: " << conn.diameter
            << " mm) connects CS " << conn.csInId << " -> CS " << conn.csOutId
            << " [Capacity: " << conn.capacity << " thousand m3/day, Weight: " << conn.weight << "]";
        return os;
    }
};
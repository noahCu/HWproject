#include"lib.h"

void Edge::setEdge( int from, int to, int val ) {
	this->from = from;
	this->to = to;
	this->val = val;
}

Edge::Edge( int from, int to, int val, int edgeID ) {
	this->from = from;
	this->to = to;
	this->val = val;
	this->edgeID = edgeID;
}

Vertex::Vertex ( int x ) {
	ID = x;
}

Edge & Vertex::operator[]( int x ) {
	return e[x];
}

int Vertex::degree() {
	return e.size();
}

int Vertex::push_back( Edge newEdge ) {
	e.push_back( newEdge );
	return e.size() - 1;
}

Vertex & Map::operator[]( int x ) {
	return v[x];
}

int Map::push_back( int x ) {
	v.push_back( Vertex( x ) );
	return v.size() - 1;
}

	




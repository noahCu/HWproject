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
	isCritical = 0;
	isDomin = 0;
}

int Vertex::size() const {
	return e.size();
}

const Edge & Vertex:: operator[] (int x) const {
	return e[x];
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

Map::Map() {
	N = M = -1;
	s.resize(1);
	t.resize(1);
}

int Map::push_back( int x ) {
	v.push_back( Vertex( x ) );
	return v.size() - 1;
}

int Map::push_back( const Vertex & x ) {
	int res = push_back(x.ID);
	v[res].isCritical = x.isCritical;
	return res;
}

const Vertex & Map::operator[](int x) const { return v[x]; }
int Map::size() const {return v.size();}

std::vector<Edge>::iterator Vertex::begin() {return e.begin(); }
std::vector<Edge>::iterator Vertex::end() {return e.end(); }
const std::vector<Edge>::const_iterator Vertex::begin() const {return e.begin(); }
const std::vector<Edge>::const_iterator Vertex::end() const {return e.end(); }

const Edge & BigMap::getOutter( int a, int b ) const { return v[a][b];}
const Edge & BigMap::getInner( int a, int b ) const { return innerEdge[a][b]; }
int BigMap::push_back( int x ) {
	int res = Map::push_back(x);
	innerEdge.push_back( x );
 	return res;
}

int BigMap::push_back( const Vertex & x ) {
	int res = Map::push_back(x);
	innerEdge.push_back( x.ID );
	return res;
}

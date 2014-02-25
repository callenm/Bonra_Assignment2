////////////////////////////////////////////////////////////////////////////
//
// DisjointSet - simple disjoint set class
//  Does searches and unions, and no error checking.
//
// (c) 2010-2013 Borna Noureddin
//      British Columbia Institute of Technology
//      School of Computing and Academic Studies
//      Bachelor of Technology Program
//      Games Development Option
//
////////////////////////////////////////////////////////////////////////////



#ifndef __DISJOINT_SET_H__
#define __DISJOINT_SET_H__

class DisjointSet {

public:
	DisjointSet(int setSize = 256);
	~DisjointSet();

    int Find(int x);
    void UnionSets(int s1, int s2);

private:
	int *setArray;

};

#endif

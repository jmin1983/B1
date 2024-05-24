//
// B1PathGraph.h
//
// Library: B1Path
// Package: B1Path
// Module:  B1Path
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PATH_GRAPH_H
#define _B1PATH_GRAPH_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <list>
#include <map>
#include <set>
#include <vector>

namespace BnD {
    class B1PathGraph {
    public:
        B1PathGraph();
        ~B1PathGraph();
    public:
        enum ADDITIONAL_COST {
            ADDITIONAL_COST_NONE = 0,
            ADDITIONAL_COST_LOW = 500,
            ADDITIONAL_COST_HIGH = 2000,

            ADDITIONAL_COST_DISCRIMINATION = 100,
        };
        struct Edge {
            Edge() : _cost(0), _additionalCost(ADDITIONAL_COST_NONE) {}
            uint32 _cost;
            ADDITIONAL_COST _additionalCost;
        };
        struct Item {
            Item(int32 id, const std::set<int32>& discriminationIDs)
                : _id(id), _additionalCost(ADDITIONAL_COST_NONE), _discriminationIDs(discriminationIDs) {}
            int32 _id;
            ADDITIONAL_COST _additionalCost;
            std::set<int32> _discriminationIDs; //  set<id>. used in getShortestPath() only.
            std::map<Item*, std::vector<Edge> > _adjacencyItems;    //  map<item, vector<edge> >
        };
        struct Path {
            Path() : _totalCost(0) {}
            std::list<int32> _ids;
            uint32 _totalCost;
        };
    protected:
        std::map<int32, Item> _items;   //  map<id, item>
    protected:
        void getAllPathsRecursive(const Item& current, int32 idTo, uint32 pathCost, std::map<int32, bool>* visited, Path* path, std::list<Path>* result) const;
    public:
        void addItem(int32 id);
        void addItem(int32 id, const std::set<int32>& discriminationIDs);
        bool setEdge(int32 idFrom, int32 idTo, uint32 cost);
        bool setEdge(int32 idFrom, int32 idTo, const std::vector<uint32>& costs);
        void setItemAdditionalCost(int32 id, size_t index, ADDITIONAL_COST cost);
        void setEdgeAdditionalCost(int32 idFrom, int32 idTo, size_t index, ADDITIONAL_COST cost);
        bool getAllPaths(int32 idIn, int32 idOut, std::list<Path>* paths) const;
        bool getShortestPath(int32 idFrom, int32 idTo, std::list<int32>* path) const;
    };
}   //  !BnD

#endif  // !_B1PATH_GRAPH_H

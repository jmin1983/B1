//
// B1PathGraph.cpp
//
// Library: B1Path
// Package: B1Path
// Module:  B1Path
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Path.h"
#include "B1PathGraph.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace BnD;

B1PathGraph::B1PathGraph()
    : _items()
{
}

B1PathGraph::~B1PathGraph()
{
}

void B1PathGraph::getAllPathsRecursive(const Item& current, int32 idTo, uint32 pathCost, std::map<int32, bool>* visited, Path* path, std::list<Path>* result) const
{
    visited->at(current._id) = true;
    path->_ids.push_back(current._id);
    path->_totalCost += pathCost;
    if (current._id != idTo) {
        for (const auto& adjacencyItemsPair : current._adjacencyItems) {
            for (const auto& edge : adjacencyItemsPair.second) {
                if (visited->at(adjacencyItemsPair.first->_id) != true) {
                    getAllPathsRecursive(*adjacencyItemsPair.first, idTo, edge._cost + current._additionalCost, visited, path, result);
                }
            }
        }
    }
    else {
        result->push_back(*path);
    }
    path->_ids.pop_back();
    path->_totalCost -= pathCost;
    visited->at(current._id) = false;
}

void B1PathGraph::addItem(int32 id)
{
    addItem(id, std::set<int32>());
}

void B1PathGraph::addItem(int32 id, const std::set<int32>& discriminationIDs)
{
    _items.insert(std::make_pair(id, Item(id, discriminationIDs)));
}

bool B1PathGraph::setEdge(int32 idFrom, int32 idTo, uint32 cost)
{
    return setEdge(idFrom, idTo, std::vector<uint32>(1, cost));
}

bool B1PathGraph::setEdge(int32 idFrom, int32 idTo, const std::vector<uint32>& costs)
{
    auto itrFrom = _items.find(idFrom);
    auto itrTo = _items.find(idTo);
    if (itrFrom == _items.end() ||
        itrTo == _items.end()) {
        return false;
    }
    std::vector<Edge>* edges = &itrFrom->second._adjacencyItems[&itrTo->second];
    edges->resize(costs.size());
    for (size_t i = 0; i < costs.size(); ++i) {
        edges->at(i)._cost = costs[i];
    }
    return true;
}

void B1PathGraph::setItemAdditionalCost(int32 id, size_t index, ADDITIONAL_COST cost)
{
    auto itr = _items.find(id);
    if (itr == _items.end()) {
        B1LOG("item add_cost changed but invalid id: id[%d], cost[%d]", id, cost);
        return;
    }
    B1LOG("item add_cost changed: id[%d], cost[%d]", id, cost);
    itr->second._additionalCost = cost;
}

void B1PathGraph::setEdgeAdditionalCost(int32 idFrom, int32 idTo, size_t index, ADDITIONAL_COST cost)
{
    auto itrFrom = _items.find(idFrom);
    auto itrTo = _items.find(idTo);
    if (itrFrom == _items.end() ||
        itrTo == _items.end()) {
        B1LOG("edge add_cost changed but invalid id: id[%d][%d], index[%d], cost[%d]", idFrom, idTo, index, cost);
        return;
    }
    auto jtr = itrFrom->second._adjacencyItems.find(&itrTo->second);
    if (jtr == itrFrom->second._adjacencyItems.end() ||
        jtr->second.size() <= index) {
        B1LOG("edge add_cost changed but invalid index: id[%d][%d], index[%d], cost[%d]", idFrom, idTo, index, cost);
        return;
    }
    B1LOG("edge add_cost changed: id[%d][%d], index[%d], cost[%d]", idFrom, idTo, index, cost);
    jtr->second[index]._additionalCost = cost;
}

bool B1PathGraph::getAllPaths(int32 idIn, int32 idOut, std::list<Path>* paths) const
{
    auto itr = _items.find(idIn);
    if (itr == _items.end()) {
        return false;
    }
    paths->clear();
    std::map<int32, bool> visited;  //  map<id, is_visited>
    for (const auto& itemsPair : _items) {
        visited[itemsPair.first] = false;
    }
    Path path;
    getAllPathsRecursive(itr->second, idOut, 0, &visited, &path, paths);
    return paths->empty() != true;
}

bool B1PathGraph::getShortestPath(int32 idFrom, int32 idTo, std::list<int32>* path) const
{
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                                  int32,
                                  boost::property<boost::edge_weight_t, uint32> > Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;

    Graph graph;
    VertexDescriptor vertexFrom;
    std::vector<int32> vertexVector;
    vertexVector.reserve(_items.size());
    {
        std::map<int32, VertexDescriptor> zoneVertexMap;
        for (const auto& itemsPair : _items) {
            zoneVertexMap[itemsPair.first] = boost::add_vertex(itemsPair.first, graph);
            vertexVector.push_back(itemsPair.first);
        }
        {
            auto itr = zoneVertexMap.find(idFrom);
            if (itr == zoneVertexMap.end()) {
                return false;
            }
            vertexFrom = itr->second;
        }
        for (const auto& itemsPair : _items) {
            for (const auto& adjacencyItemsPair : itemsPair.second._adjacencyItems) {
                const std::vector<Edge>& edges = adjacencyItemsPair.second;
                for (const auto& edge : edges) {
                    auto u = zoneVertexMap.find(itemsPair.first);
                    if (u == zoneVertexMap.end()) {
                        continue;
                    }
                    auto v = zoneVertexMap.find(adjacencyItemsPair.first->_id);
                    if (v == zoneVertexMap.end()) {
                        continue;
                    }
                    uint32 cost = edge._cost + edge._additionalCost + adjacencyItemsPair.first->_additionalCost;
                    if (itemsPair.second._discriminationIDs.find(adjacencyItemsPair.first->_id) != itemsPair.second._discriminationIDs.end()) {
                        cost += ADDITIONAL_COST_DISCRIMINATION;
                    }
                    boost::add_edge(u->second, v->second, cost, graph);
                }
            }
        }
    }
    std::vector<VertexDescriptor> predecessorVector(vertexVector.size());
    boost::dijkstra_shortest_paths(graph, vertexFrom,
                                   predecessor_map(boost::make_iterator_property_map(predecessorVector.begin(), boost::get(boost::vertex_index, graph))));
    std::map<int32, int32> predecessorMap;
    for (size_t i = 0; i < vertexVector.size(); ++i) {
        predecessorMap[vertexVector[i]] = vertexVector[predecessorVector[i]];
    }
    path->clear();
    path->push_front(idTo);
    while (true) {
        int32 current = path->front();
        if (current == idFrom) {
            break;
        }
        auto itr = predecessorMap.find(current);
        if (itr == predecessorMap.end() ||
            itr->first == itr->second) {
            path->clear();
            return false;
        }
        path->push_front(itr->second);
    }
    return true;
}

//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef NETWORKS2019_LABEL_H
#define NETWORKS2019_LABEL_H

#include <iostream>

#include "goc/goc.h"

#include "vrp_instance.h"

namespace networks2019
{
class Label : public goc::Printable
{
public:
	Label* parent;
	goc::Vertex v; // Last vertex.
	CapacityUnit q; // Label demand.
	ProfitUnit p; // Label profit.
	int length; // Label path length.
	VertexSet S; // Set S of vertices for domination.
	VertexSet U; // S \cup "unreachables".
	goc::PWLFunction duration; // duration(t) = "minimum duration for reaching v at time t".
	goc::Interval rw; // rw = dom(duration).
	double min_cost; // Label minimum cost (min{duration(t)-p : t \in dom(duration)}).
	
	goc::GraphPath Path() const;
	
	virtual void Print(std::ostream& os) const;
};
} // namespace networks2019

#endif //NETWORKS2019_LABEL_H

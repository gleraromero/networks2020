//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "spf.h"

using namespace std;
using namespace goc;

namespace networks2019
{
SPF::SPF(int n) : n(n)
{
	formulation = LPSolver::NewFormulation();
	formulation->Minimize(Expression()); // Minimization.
	formulation->AddConstraint(Expression().EQ(0.0)); // Start depot.
	for (int i = 1; i < n-1; ++i) formulation->AddConstraint(Expression().EQ(1.0)); // Customers.
	formulation->AddConstraint(Expression().EQ(0.0)); // End depot.
	
	// Init structures.
	omega_by_arc = Matrix<vector<int>>(n, n);
	forbidden_arcs = {};
}

SPF::~SPF()
{
	delete formulation;
}

void SPF::AddRoute(const Route& r)
{
	// Add route r to Omega.
	int j = omega.size();
	omega.push_back(r);
	
	// Add variable y_j to the formulation.
	Variable y_j = formulation->AddVariable("y_" + STR(j), VariableDomain::Binary, 0.0, INFTY);
	y.push_back(y_j);
	
	// Set coefficient 1.0 in vertices visited.
	for (int k = 1; k < (int)r.path.size()-1; ++k) formulation->SetConstraintCoefficient(r.path[k], y_j, 1.0);
	
	// Set duration(r) as c_j in the objective function.
	formulation->SetObjectiveCoefficient(y_j, r.duration);
	
	// Add the route to the structure indexed by arcs.
	for (int k = 0; k < (int)r.path.size()-1; ++k) omega_by_arc[r.path[k]][r.path[k+1]].push_back(j);
}

void SPF::SetForbiddenArcs(const vector<Arc>& A)
{
	// Restore previously forbidden arcs.
	for (Arc e: forbidden_arcs)
		for (int j: omega_by_arc[e.tail][e.head])
			formulation->SetVariableBound(y[j], 0.0, INFTY);

	// All routes with forbidden arcs must be set to 0.
	for (Arc e: A)
		for (int j: omega_by_arc[e.tail][e.head])
			formulation->SetVariableBound(y[j], 0.0, 0.0);

	forbidden_arcs = A;
}

const Route& SPF::RouteOf(const Variable& variable) const
{
	return omega[variable.Index()];
}

PricingProblem SPF::InterpretDuals(const vector<double>& duals) const
{
	PricingProblem pp;
	pp.P = duals;
	pp.A = forbidden_arcs;
	return pp;
}

vector<Route> SPF::InterpretSolution(const Valuation& z) const
{
	vector<Route> solution;
	for (auto& y_value: z) solution.push_back(omega[y_value.first.Index()]);
	return solution;
}
} // namespace networks2019
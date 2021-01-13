#pragma once
#include "EquiTrussAttribute.h"
#include "common.h"

void search(EquiTrussAttribute obj, int q_vertex,  FILE* resultFile, FILE* detailFile, int k_value, int attr_count, int selection);
string searchNeo4j(EquiTrussAttribute obj, int q_vertex, int k_value, int attr_count, int selection);
void clearResult(char *path);
/**
 * equitruss
 * @param argv1 graph data file
 * @param argv2 attribute data file
 * @param argv3 query vertices file
 * @param argv4 result path
 * @param argv5 k_value
 * @param argv6 attr_count
 * @param argv7 selection
 */
void algorithm_queries(char *argv1, char *argv2, char *argv3, char *argv4, int argv5, int argv6, int argv7);


/**
 * neo4j
 * @param argv1 graph data file
 * @param argv2 attribute data file
 * @param argv3 query vertex id
 * @param argv4 result path
 * @param argv5 k_value
 * @param argv6 attr_count
 * @param argv7 selection
 */
string algorithm_neo4j(char *argv1, char *argv2, int argv3, char *argv4, int argv5, int argv6, int argv7);
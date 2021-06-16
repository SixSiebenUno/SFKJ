# PFAJ

An implementation of private foreign-key acyclic join algorithm. Our work is based on OPPRF-PSI and ABY framework.

## Required packages:
 - g++ (vection >=8) 
 - libboost-all-dev (version >=1.69) 
 - libgmp-dev 
 - libssl-dev 
 - libntl-dev

## DataSets:

1. We perform on MovieLens-1M dataset. The dataset has 3 tables, which is

   * Ratings (userID, movieID, score, timestamp)
   * User (userID, .....)
   * Movies (movieID, .....)

   So the join tree is a complete binary tree with depth 2. There are two varieties of data processing,

   (1). Filter all IDs under 100 and then join. This will output full data.

   (2). Filter the first 100 tuples and then join. This will output only two tuples with oblivious reveal protocol.

**Experiments**:

Two party setting

* P0 holds Ratings
* P1 holds User and Movies

Multi-party setting (a simplest demo)

* P0 holds Ratings
* P1 holds Movies
* P2 holds User

After join, each party holds the shared data of all its subtree's information. In this demo, P0 holds [RUM], P1 holds [M] and P2 holds [U]



2. We perform another demo on TPCH-1M dataset. The dataset has 3 tables, and we filter some useless attributes in each table, the remaining table is

   * customer (CK, xxx, xxx)
   * orders (OK, CK, xxx)
   * lineitem (OK, LN, xxx)

   And the result table is (OK, LN, xxx, CK, OK, xxx, OK, CK, xxx, CK, xxx, xxx). It is a 3 line join.

**Experiments:**

Two-party setting

* P0 holds customer, lineitem
* P1 holds irders

Three-party setting

* P0 holds lineitem
* P1 holds orders
* P2 holds customer

After the join, P0 holds [LOC], P1 holds [OC] and P2 holds [C]. 


## TODO: 

1. GMW Protocol to update dummy tags and EQtest
2. Linear Regression in 3PC
3. Fix connection waiting time (now 1000 retry * 1s)